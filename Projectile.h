#pragma once
#include "Object.h"
#include "Tile.h"

class Projectile : public Object
{
public:
	Projectile(int2 coordinates, int2 dimensions, int speed, int maxRange, float2 direction);

	float2 GetFloatCoordinates() const;
	void SetCoordinates(float2 coordinates, const int2 moveMap);

	int GetSpeed() const;
	void SetSpeed(int speed);

	int GetMaxRange() const;
	void SetMaxRange(int maxRange);

	float2 GetDirection() const;
	void SetDirection(float2 direction);

	int2 GetDestination() const;
	void SetDestination(int2 destination, int2 startPos, const int2 moveMap);

	void Move(Tmpl8::Surface* screen, int2 shipCoord, int2 moveMap);
	void CollidesWithWall(int* tileIndex, const vector<Tile*>& map);
	int* Projectile::GetOccupiedTiles(const int2 newCoord, const int2 offset, const vector<Tile*>& map, Tmpl8::Surface* screen);

	void RunImpactAnimation(Tmpl8::Surface* screen, int2 moveMap);

	bool isMoving = false;
	Timer shootTimer;

	Timer impactAnimTimer;
	bool playImpactAnim = false;
	int2 impactCoord;

private:
	float2 m_fcoord;
	int m_speed;
	int m_maxRange;
	float2 m_direction;
	int2 m_destination;
	int2 m_startPos;
};

