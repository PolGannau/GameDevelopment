#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1CollisionManager.h"


struct BgImages
{
	SDL_Texture* texture = nullptr;
	float parallax_speed = 1.F;
	SDL_Rect* rect = nullptr;
	int posx = 0;
	int posy = 0;
};
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;
	bool Contains(int id) const
	{
		return (id >= firstgid && id <= firstgid + tilecount-1);
	}

	p2SString			name;
	uint					firstgid=0u;
	uint					tilecount = 0u;
	uint					margin = 0u;
	uint					spacing = 0u;
	uint					tile_width = 0u;
	uint					tile_height = 0u;
	SDL_Texture*		texture = nullptr;
	uint					tex_width = 0u;
	uint					tex_height = 0u;
	uint					num_tiles_width = 0u;
	uint					num_tiles_height = 0u;
	uint					offset_x = 0u;
	uint					offset_y = 0u;
};

struct MapLayer{
	p2SString name = "";
	uint width = 0u;
	uint height = 0u;
	uint* data = nullptr;
	~MapLayer() {
		if (data != nullptr)
		{
			delete[] data;
		}
	}

	inline uint Get(int x, int y) const {
		return x + (width * y);
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	p2List<fPoint>		groundEnemies_Position, flyingEnemies_Position;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<Collider*>	colliders;
	p2List<BgImages*>	bg_images;
	fPoint				player_position, camera_position, win_position;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	bool WalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();

	//loads tilesets attributes
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	//loads tilesets textures
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	//loads the layer information
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	//loads and create all map colliders
	bool LoadColliders(pugi::xml_node& node);
	//loads player camera and endgame positions
	bool LoadPositionLayer(pugi::xml_node& node);
	//loads background
	bool LoadImageLayer(pugi::xml_node& node, BgImages* bg_image);
	//loads the enemies on its position
	bool LoadEnemiesPosition(pugi::xml_node& node);
	
	//gets the tileset that contains that specific tile id
	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData				data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__