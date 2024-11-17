#pragma once
#include "Tile.h"

class Tilemap
{
public:
	Tilemap()/* = default*/;
	~Tilemap();
	void InitTilemap(int2 moveMap);
	void DrawMap(Surface* screen);

	//space tile
	Tile* O = new Tile({ 0, 0 }, { 64, 64 }, { 0 });
	//red space tile?
	Tile* X = new Tile({ 0, 0 }, { 64, 64 }, { 1 });
	//asteroid tile
	Tile* V = new Tile({ 0, 0 }, { 64, 64 }, { 2 });

	vector<int> wallTileIndex;

	vector<Tile*> map = { //make it into vector<Tile*> map
		//Tile* map[TILEMAP_WIDTH * TILEMAP_HEIGHT] = {
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, X, X, X, O, O, O, V, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, X,
				O, O, O, X, X, X, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, X,
				O, O, O, O, O, X, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, X,
				O, O, O, O, O, X, O, O, O, O, O, O, O, O, O, O, O, O, V, O, O, O, X, O, V, V, O, O, O, O, O, O, O, O, O, O, O, O, O, X,
				O, O, O, O, O, X, O, O, O, O, O, O, O, O, O, O, O, V, V, O, O, O, X, O, V, V, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, X, O, O, O, O, O, O, O, O, X, O, X, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, X, O, O, O, O, O, O, O, O, X, O, X, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, X, O, O, O, O, O, O, O, O, X, O, X, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, X, X, X, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, V, V, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, V, V, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, V, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, V, V, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
				O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O
	};

private:
};

