#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "Brofiler/Brofiler.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Draw", Profiler::Color::FloralWhite);
	if (map_loaded == false)
		return;

	p2List_item  <MapLayer*>* layer = data.layers.start;
	p2List_item<TileSet*>* tileset = data.tilesets.start;
	p2List_item<BgImages*>* images = data.bg_images.start;
	while (images != nullptr)
	{
		App->render->Blit(images->data->texture, images->data->posx, images->data->posy, images->data->rect, images->data->parallax_speed);
		images = images->next;
	}

	while (layer != nullptr)
	{
		for (uint i = 0; i < layer->data->width; i++)
		{
			for (uint j = 0; j < layer->data->height; j++)
			{
				iPoint positionInWorld = MapToWorld(i, j);
				if (layer->data->data[layer->data->Get(i, j)] == 0)
				{
					continue;
				}
				while (tileset != nullptr) {
					if (tileset->data->Contains(layer->data->data[layer->data->Get(i, j)])) break;
					tileset = tileset->next;
				}
				if (tileset != nullptr)
				{
					SDL_Rect rect = tileset->data->GetTileRect(layer->data->data[layer->data->Get(i, j)]);
				if (App->render->IsInCameraBounds({ positionInWorld.x, positionInWorld.y, rect.w, rect.h }))App->render->Blit(tileset->data->texture, positionInWorld.x, positionInWorld.y, &rect);
				}

				tileset = data.tilesets.start;
			}
		}
		layer = layer->next;
	}
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	// Tileset based on a tile id

	p2List_item<TileSet*>* tileset = data.tilesets.start;
	while (tileset != NULL)
	{
		if (tileset->next && id < tileset->next->data->firstgid) return tileset->data;
		tileset = tileset->next;
	}
	return data.tilesets.end->data;

}



iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret;

	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers

	p2List_item<MapLayer*>* layer;
	layer = data.layers.start;

	while (layer != NULL)
	{
		RELEASE(layer->data);
		layer = layer->next;
	}
	data.layers.clear();

	p2List_item<Collider*>* collider;
	collider = data.colliders.start;
	while (collider != nullptr)
	{
		collider->data->to_delete = true;
		collider = collider->next;
	}
	data.colliders.clear();

	//clean all images
	p2List_item<BgImages*>* images;
	images = data.bg_images.start;

	while ( images != NULL)
	{
		App->tex->UnLoad(images->data->texture);
		RELEASE(images->data);
		images = images->next;
	}
	data.bg_images.clear();
	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay);
	}

	pugi::xml_node colliders;
	
	for (colliders = map_file.child("map").child("objectgroup"); colliders && ret; colliders = colliders.next_sibling("objectgroup"))
	{
		LoadColliders(colliders);
	}

	pugi::xml_node imageLayer;
	for (imageLayer = map_file.child("map").child("imagelayer"); imageLayer&&ret; imageLayer = imageLayer.next_sibling("imagelayer"))
	{
		BgImages* img = new BgImages();

		ret = LoadImageLayer(imageLayer, img);

		if (ret)
			data.bg_images.add(img);
	}

	pugi::xml_node PlayerPosition;

	PlayerPosition = map_file.child("map").find_child_by_attribute("name", "Position_Layer");

	LoadPositionLayer(PlayerPosition);

	pugi::xml_node position_enemy;

	position_enemy = map_file.child("map").find_child_by_attribute("name", "Enemies_position_Layer");

	LoadEnemiesPosition(position_enemy);

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
		p2List_item<BgImages*>* item_image = data.bg_images.start;
		while (item_image != NULL)
		{
			BgImages* b = item_image->data;
			LOG("item image ----");
			LOG("");
			LOG("");
			item_image = item_image->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_uint();
	set->tilecount = tileset_node.attribute("tilecount").as_uint();
	set->tile_width = tileset_node.attribute("tilewidth").as_uint();
	set->tile_height = tileset_node.attribute("tileheight").as_uint();
	set->margin = tileset_node.attribute("margin").as_uint();
	set->spacing = tileset_node.attribute("spacing").as_uint();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_uint();
		set->offset_y = offset.attribute("y").as_uint();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

bool j1Map::LoadColliders(pugi::xml_node & node)
{
	bool ret = true;
	int i = 0;
	for (pugi::xml_node col = node.child("object"); col; col = col.next_sibling("object")) {
		SDL_Rect r;
		r.x = col.attribute("x").as_int();
		r.y = col.attribute("y").as_int();
		r.w = col.attribute("width").as_int();
		r.h = col.attribute("height").as_int();

		if (node.child("properties").find_child_by_attribute("name", "type").attribute("value").as_int() == COLLIDER_GROUND) {
			data.colliders.add(App->collision->AddCollider(r, COLLIDER_TYPE::COLLIDER_GROUND));
		}
		else if (node.child("properties").find_child_by_attribute("name", "type").attribute("value").as_int() == COLLIDER_LIMIT) {
			data.colliders.add(App->collision->AddCollider(r, COLLIDER_TYPE::COLLIDER_LIMIT));
		}
		else if (node.child("properties").find_child_by_attribute("name", "type").attribute("value").as_int() == COLLIDER_PLATFORM) {
			data.colliders.add(App->collision->AddCollider(r, COLLIDER_TYPE::COLLIDER_PLATFORM));
		}
	}
	return ret;
}

bool j1Map::LoadPositionLayer(pugi::xml_node & node)
{
	data.player_position.x = node.find_child_by_attribute("name", "PlayerPosition").attribute("x").as_float();
	data.player_position.y = node.find_child_by_attribute("name", "PlayerPosition").attribute("y").as_float();

	data.camera_position.x = node.find_child_by_attribute("name", "CameraPosition").attribute("x").as_float();
	data.camera_position.y = -node.find_child_by_attribute("name", "CameraPosition").attribute("y").as_float();

	data.win_position.x = node.find_child_by_attribute("name", "WinPosition").attribute("x").as_float();
	data.win_position.y = node.find_child_by_attribute("name", "WinPosition").attribute("y").as_float();

	return true;
}

bool j1Map::LoadImageLayer(pugi::xml_node& node, BgImages* bg_image)
{
	bool ret = true;
	bg_image->posx = node.attribute("offsetx").as_int();
	bg_image->posy = node.attribute("offsety").as_int();
	bg_image->rect = new SDL_Rect({ 0,
		0,
		node.child("image").attribute("width").as_int(),
		node.child("image").attribute("height").as_int() });
	bg_image->parallax_speed = node.child("properties").find_child_by_attribute("name", "parallax_speed").attribute("value").as_float();
	bg_image->texture = App->tex->Load(PATH(folder.GetString(), node.child("image").attribute("source").as_string()));
	return ret;
}

bool j1Map::LoadEnemiesPosition(pugi::xml_node & node)
{
	int i = 0;
	for (pugi::xml_node enemy = node.child("object"); enemy; enemy.next_sibling("object"))
	{
		p2SString name = enemy.attribute("name").as_string();
		if (name == "ground_enemy") {
			data.groundEnemies_Position.add({ enemy.attribute("x").as_float(), enemy.attribute("y").as_float() });
		}
		else if (name == "flying_enemy") {
			data.flyingEnemies_Position.add({ enemy.attribute("x").as_float(), enemy.attribute("y").as_float() });
		}
	}
	return true;
}
