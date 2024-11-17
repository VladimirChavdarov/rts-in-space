#pragma once
#include "Object.h"

class Tile : public Object
{
public:
	Tile(int2 coordinates, int2 dimensions, int effect);
	void DrawObject(Tmpl8::Surface* screen);

	int GetEffect() const;

private:
	//int2 m_asset_pos;
	int m_effect;
};