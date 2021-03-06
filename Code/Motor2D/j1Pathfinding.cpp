#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Pathfinding.h"
#include "p2List.h"
#include "Brofiler/Brofiler.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Map.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

bool j1PathFinding::Start()
{
	debug_tex = App->tex->Load("textures/pathfinding.png");
	return true;
}

bool j1PathFinding::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Pathfinding.cpp", Profiler::Color::Salmon)
	return true;
}

bool j1PathFinding::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_Pathfinding.cpp", Profiler::Color::MediumSlateBlue)
	if (debug)
	{
		// debug pathfing ------------------
		static iPoint origin;
		static bool origin_selected = false;

		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (origin_selected == true)
			{
				
				origin_selected = false;
			
				if (CreatePath(origin, p,WALKING) != -1)
				{
					createdDebugPath = true;
				}
				
			}
			else
			{
				origin = p;
				origin_selected = true;
				createdDebugPath = false;
				debugPath.Clear();
				
			}
		}

		

		if (createdDebugPath)
		{
			uint debugPathSize = debugPath.Count();
			if (debugPathSize==0)
			{
				const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
				uint sizeArray = path->Count();
				for (uint i = 0; i < sizeArray; ++i)
				{
					debugPath.PushBack(*path->At(i));
				}
			}
			else
			{
				for (uint i = 0; i < debugPathSize; ++i)
				{
					iPoint pos = App->map->MapToWorld(debugPath.At(i)->x, debugPath.At(i)->y);
					App->render->Blit(debug_tex, pos.x, pos.y);
				}
			}
			
		}
		
	}
	
	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}


// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	

	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : numSteps(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : numSteps(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : numSteps(node.numSteps), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	BROFILER_CATEGORY("FindWalkableAdjacents.cpp", Profiler::Color::Coral)
	iPoint cell(pos.x, pos.y + 1);

	// south
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// north
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.Count();
}

uint PathNode::FindWalkableAdjacentsWalking(PathList & list_to_fill) const
{
	BROFILER_CATEGORY("FindWalkableAdjacentsWalking.cpp", Profiler::Color::Coral)
	iPoint cell;

	// south
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
	{
			list_to_fill.list.add(PathNode(-1, -1, cell, this));
	}
	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable({ pos.x + 1, pos.y }) 
		&& App->pathfinding->IsWalkable({ pos.x, pos.y + 1 }) == false)
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell)
		&& App->pathfinding->IsWalkable({ pos.x, pos.y + 1 }) == false)
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.Count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return numSteps + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	numSteps = parent->numSteps + 1;
	
	h = pos.DistanceManhattan(destination);

	return numSteps + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination, pathTypes type)
{
	BROFILER_CATEGORY("CreatePath", Profiler::Color::Violet)
	if(!IsWalkable(origin) || !IsWalkable(destination))
		return -1;
	
	PathList openList, closeList;
	
	openList.list.add({ 0, origin.DistanceTo(destination), origin, nullptr });

	bool findDestination = false;

	while(openList.list.Count()>0 && !findDestination)
	{
		p2List_item<PathNode>* lowerNode = openList.GetNodeLowestScore();
		p2List_item<PathNode>* currNode = closeList.list.add(lowerNode->data);
		openList.list.del(lowerNode);

		if (currNode->data.pos == destination)
		{
			findDestination = true;
			break;
		}

		PathList neighbours;
		if(type == FLYING)
			currNode->data.FindWalkableAdjacents(neighbours);
		else if(type == WALKING)
			currNode->data.FindWalkableAdjacentsWalking(neighbours);
		
		
		for (p2List_item<PathNode>* nodeNeigh= neighbours.list.start; nodeNeigh; nodeNeigh= nodeNeigh->next)
		{
			 p2List_item<PathNode>* closeItem = (p2List_item<PathNode>*) closeList.Find(nodeNeigh->data.pos);
			 p2List_item<PathNode>* openItem = (p2List_item<PathNode>*) openList.Find(nodeNeigh->data.pos);

			 if (closeItem != NULL)
				 continue;

			 nodeNeigh->data.CalculateF(destination);
			if (openItem == NULL)
				openList.list.add(nodeNeigh->data);
			
			else if (nodeNeigh->data.numSteps < openItem->data.numSteps)
				{
					openItem->data.parent = &currNode->data;
					
				}

			
		}

		
	}
		if (findDestination)
		{
			last_path.Clear();
			const p2List_item<PathNode>* pathNode = closeList.list.end;
			while (pathNode != NULL)
			{
				last_path.PushBack(pathNode->data.pos);
				if (pathNode->data.parent != nullptr)
					pathNode = closeList.Find(pathNode->data.parent->pos);
				else
					pathNode = NULL;
			}

			last_path.Flip();
			return 1;
		}
		
	
	return -1;
}

