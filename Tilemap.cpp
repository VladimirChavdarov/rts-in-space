#include "precomp.h"
#include "Tilemap.h"

Tilemap::Tilemap()
{
	map.reserve(TILEMAP_WIDTH * TILEMAP_HEIGHT);
}

Tilemap::~Tilemap()
{
	delete O, X, V;
}

void Tilemap::InitTilemap(int2 moveMap)
{
	Tile* aTile = new Tile({ 0, 0 }, { 64, 64 }, { 0 });
	map[0] = aTile;
	for (int i = 1; i < map.size(); i++)
	{
		map[i] = new Tile({ 0, 0 }, { 64, 64 }, { map[i]->GetEffect() });
		map[i]->SetCoordinates({ map[i - 1]->GetCoordinates().x + map[i - 1]->GetDimensions().x, map[i - 1]->GetCoordinates().y }, moveMap);
		if (i % TILEMAP_WIDTH == 0)
		{
			map[i]->SetCoordinates({ 0, map[i - 1]->GetCoordinates().y + map[i - 1]->GetDimensions().y }, moveMap);
		}
		//cout << map[i].GetCoordinates().x << " " << map[i].GetCoordinates().y << endl;

		if (map[i]->GetEffect() == 2)
			wallTileIndex.push_back(i);
	}
}

void Tilemap::DrawMap(Surface* screen)
{
	for (int i = 0; i < size(map); i++)
	{
		map[i]->DrawObject(screen);
	}
}
