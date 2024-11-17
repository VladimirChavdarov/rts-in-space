#include "precomp.h"
#include "Projectile.h"

static Sprite attackEffect(new Surface("assets/rts_assets/attack_effect.png"), 7);

Projectile::Projectile(int2 coordinates, int2 dimensions, int speed, int maxRange, float2 direction) : Object(coordinates, dimensions)
{
	m_coord = coordinates;
	m_fcoord.x = static_cast<float>(m_coord.x);
	m_fcoord.y = static_cast<float>(m_coord.y);
	m_dimensions = dimensions;
	m_speed = speed;
	m_maxRange = maxRange;
	m_direction = direction;
	m_startPos = m_coord;
	m_destination = m_coord;
}

float2 Projectile::GetFloatCoordinates() const
{
	return m_fcoord;
}
void Projectile::SetCoordinates(float2 coordinates, const int2 moveMap)
{
	m_fcoord.x = static_cast<float>(coordinates.x - moveMap.x);
	m_fcoord.y = static_cast<float>(coordinates.y - moveMap.y);
	m_coord.x = static_cast<int>(m_fcoord.x);
	m_coord.y = static_cast<int>(m_fcoord.y);

	m_center.x = m_coord.x + m_dimensions.x / 2;
	m_center.y = m_coord.y + m_dimensions.y / 2;
}

int Projectile::GetSpeed() const
{
	return m_speed;
}
void Projectile::SetSpeed(int speed)
{
	m_speed = speed;
}

int Projectile::GetMaxRange() const
{
	return m_maxRange;
}
void Projectile::SetMaxRange(int maxRange)
{
	m_maxRange = maxRange;
}

float2 Projectile::GetDirection() const
{
	return m_direction;
}
void Projectile::SetDirection(float2 direction)
{
	m_direction = direction;
}

int2 Projectile::GetDestination() const
{
	return m_destination;
}
void Projectile::SetDestination(int2 destination, int2 startPos, const int2 moveMap)
{
	m_startPos = startPos - moveMap;
	m_destination = destination - moveMap;
}

void Projectile::CollidesWithWall(int* tileIndex, const vector<Tile*>& map)
{
	for (int j = 0; j < sizeof(tileIndex) + 1; j++)
	{
		if (tileIndex[j] < TILEMAP_WIDTH * TILEMAP_HEIGHT && tileIndex[j] >= 0)
		{
			if (map[tileIndex[j]]->GetEffect() == 2) //make the effects into an enum later pls
			{
				isMoving = false;
			}
		}
	}
}

int* Projectile::GetOccupiedTiles(const int2 newCoord, const int2 offset, const vector<Tile*>& map, Tmpl8::Surface* screen)
{
	static int2 worldspaceShipCoord;
	worldspaceShipCoord = { newCoord.x + offset.x, newCoord.y + offset.y };
	static int dimx;
	dimx = map[0]->GetDimensions().x;
	static int dimy;
	dimy = map[0]->GetDimensions().y;

	int topLeftIndex = (worldspaceShipCoord.x / dimx) + (worldspaceShipCoord.y / dimy) * TILEMAP_WIDTH;
	int bottomRightIndex = ((worldspaceShipCoord.x + m_dimensions.x) / dimx) + ((worldspaceShipCoord.y + m_dimensions.y) / dimy) * TILEMAP_WIDTH;
	int topRightIndex = ((worldspaceShipCoord.x + m_dimensions.x) / dimx) + (worldspaceShipCoord.y / dimy) * TILEMAP_WIDTH;
	int bottomLeftIndex = (worldspaceShipCoord.x / dimx) + ((worldspaceShipCoord.y + m_dimensions.y) / dimy) * TILEMAP_WIDTH;
	int2 shipTileLength = { topRightIndex - topLeftIndex + 1, (bottomLeftIndex - topLeftIndex) / TILEMAP_WIDTH + 1 };

	static int tileIndex[9];
	int x = 0, y = 0;
	for (int j = 0; j < shipTileLength.x * shipTileLength.y; j++)
	{
		tileIndex[j] = (worldspaceShipCoord.x / dimx) + x + ((worldspaceShipCoord.y / dimy) + y) * TILEMAP_WIDTH;
		x++;
		if (x >= shipTileLength.x)
		{
			y++;
			x = 0;
		}
	}

	return tileIndex;
}

void Projectile::Move(Tmpl8::Surface* screen, int2 shipCoord, int2 moveMap)
{
	m_startPos -= moveMap;
	m_destination -= moveMap;
	m_direction = { static_cast<float>(m_destination.x - m_startPos.x), static_cast<float>(m_destination.y - m_startPos.y) };
	if(!(m_direction.x == 0 && m_direction.y == 0))
		m_direction = normalize(m_direction);
	
	m_fcoord.x += m_direction.x * m_speed;
	m_fcoord.y += m_direction.y * m_speed;


	float2 a = { static_cast<float>(m_fcoord.x - m_startPos.x), static_cast<float>(m_fcoord.y - m_startPos.y) };

	if (length(a) >= length(m_direction) * m_maxRange)
	{
		isMoving = false;
	}
	if (!isMoving)
	{
		m_fcoord.x = static_cast<float>(shipCoord.x);
		m_fcoord.y = static_cast<float>(shipCoord.y);
	}

	m_coord.x = static_cast<int>(m_fcoord.x);
	m_coord.y = static_cast<int>(m_fcoord.y);
}

void Projectile::RunImpactAnimation(Tmpl8::Surface* screen, int2 moveMap)
{
	if (playImpactAnim)
	{
		float a = impactAnimTimer.elapsed() * 10;
		if (a >= 8.0f)
		{
			impactAnimTimer.reset();
			playImpactAnim = false;
		}
		if (a < 7.0f)
			attackEffect.SetFrame(static_cast<int>(a));
		impactCoord.x -= moveMap.x;
		impactCoord.y -= moveMap.y;
		attackEffect.Draw(screen, impactCoord.x - 24, impactCoord.y - 24);
	}
}