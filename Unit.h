#pragma once
#include "Object.h"
#include "Projectile.h"
#include "Tile.h"

const int PRECISION = 0;
//const int TURN_CURVE_SIZE = 25;
const int TURN_IN_PLACE_RANGE = 200;
const int CA_RANGE = 250;

class Unit : public Object
{
public:
	Unit(int2 coordinates, int2 dimensions, int hp, int movementSpeed, int damage, int range, int curve_size, float cooldown);

	int GetHp();
	void SetHp(int hp);

	int GetMovementSpeed() const;
	void SetMovementSpeed(int movementSpeed);

	int GetDamage() const;
	void SetDamage(int damage);

	int GetRange() const;
	void SetRange(int range);

	int GetCurveSize() const;
	void SetCurveSize(int curve_size);

	void SetCooldown(float cooldown);

	int2 GetDestination() const;
	void SetDestination(int2 destination, int2 moveMap);

	void CalculateTargetVector(vector<int2>& obstacleCenters);

	int2 CheckDesiredPos();
	void Move(Tmpl8::Surface* screen);

	void CollidesWithWall(int* tileIndex, const vector<Tile*>& map);

	int* GetOccupiedTiles(const int2 newCoord, const int2 offset, const vector<Tile*>& map, Tmpl8::Surface* screen);

	void Detect(Tmpl8::Surface* screen, const int2 enemyCoord, const int2 moveMap);

	void Attack(Tmpl8::Surface* screen, vector<Unit>& enemyUnit, const int2 moveMap, const int2 mapOffset, vector<Tile*>& map);

	void DrawObject(Tmpl8::Surface* screen, Tmpl8::Sprite* sprite);


	//can be made protected but will need getters and setters
	bool isSelected = false;
	bool isColliding = false;
	int controlGroup = 0;

	bool fireFlag = false;
	int2 targetCoord = {0, 0};
	Projectile* projectile = new Projectile({ m_coord }, { 12, 12 }, 15, 300, { 0, 0 });


protected:
	int m_maxHp;
	int m_hp;
	int m_movementSpeed;
	int m_damage;
	int m_range;
	int m_turn_curve_size = 25;
	float m_cooldown;

	int2 m_destination;

	float2 m_targetVector; //the unit aims to align its currentVector with the targetVector
	float2 m_currentVector;
	float2 m_r;
	float2 m_attackConeRange; //Deprecated, BUT I'll leave it for now for the sake of clarity

	float2 dirVectors[24] = { {0,0} };
	float2 m_destinationVec;
	float2 m_obstacleVec;

	Timer smoketTimer;

private:
	//---------------------- LOCAL FUNCTIONS ---------------------
	float ToNormalDegrees(float2 vector);
	bool CompareFloat(float a, float b, float epsilon = 0.01f);

	void DrawCone(Tmpl8::Surface* screen, float i, float n);
	void DrawCircle(Tmpl8::Surface* screen, float i, float n);
	void SetupFrames(Tmpl8::Sprite* sprite);

	float2 CalcEnemyVector(Tmpl8::Surface* screen, int2 enemyCoord, int deg);
	
	void Unit::LiftFireFlag(float leftVecDeg, float rightVecDeg, float2 enemyVec);

	int2 SetNewPosition();

	void MoveCurrentVector();
	//------------------ END OF LOCAL FUNCTIONS ------------------
};

