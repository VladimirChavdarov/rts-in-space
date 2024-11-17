#include "precomp.h"
#include "Tile.h"

Tile::Tile(int2 coordinates, int2 dimensions/* = {32, 32}*/, int effect) : Object(coordinates, dimensions)
{
	m_effect = effect;
}

static Surface* spaceSurface = new Surface("assets/rts_assets/bg56.png");
static Surface* redSurface = new Surface("assets/rts_assets/bg47.png");
static Surface* asteroidSurface = new Surface("assets/rts_assets/meteorite3.png");

int Tile::GetEffect() const
{
	return m_effect;
}

void Tile::DrawObject(Tmpl8::Surface* screen)
{
	switch (m_effect)
	{
	case 0:
	{
		spaceSurface->CopyTo(screen, m_coord.x, m_coord.y);
		//spaceSprite.Draw(screen, m_coord.x, m_coord.y);
		break;
	}
	case 1:
	{
		redSurface->CopyTo(screen, m_coord.x, m_coord.y);
		//redSprite.Draw(screen, m_coord.x, m_coord.y);
		break;
	}
	case 2:
	{
		asteroidSurface->CopyTo(screen, m_coord.x, m_coord.y);
		//asteroidSprite.Draw(screen, m_coord.x, m_coord.y);
		//screen->Box(m_coord.x, m_coord.y, m_coord.x + m_dimensions.x, m_coord.y + m_dimensions.y, 0xe80606);

		break;
	}
	default:
		break;
	}
}
