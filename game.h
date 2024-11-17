#pragma once
#include "Object.h"
#include "Unit.h"
#include "RangedShip.h"
#include "Tile.h"
#include "Tilemap.h"

#include "Projectile.h"

namespace Tmpl8
{

class Game : public TheApp
{
public:
	// game flow methods
	void Init();
	void Tick( float deltaTime );
	void Shutdown();// { /* implement if you want to do something on exit */ }
	// input handling
	void MouseUp(int button);
	void MouseDown(int button);
	void MouseMove( int x, int y ) { mousePos.x = x, mousePos.y = y; }
	void MouseWheel( float y ) { /* implement if you want to handle the mouse wheel */ }
	void KeyUp(int key);
	void KeyDown(int key);

	// data members
	int2 mousePos;
	int p_Up, p_Down, p_Left, p_Right;
	int p_oneToNine;
	bool p_Ctrl;
	bool mouseLeft = 0, mouseRight = 0;
	bool playerWin = false;
	bool botWin = false;

	Unit* aUnit = new Unit({ 10, 10 }, { 128, 128 }, { 50 }, { 5 }, { 15 }, { 300 }, { 25 }, {0.0f});
	vector<Unit> PlayerUnits; //make into a list
	const int P_UNIT_NUM = 6;
	vector<Unit> BotUnits;
	const int E_UNIT_NUM = 6;
	vector<int> botTargetIndex;

	vector<int2> debris;

	int2 select_mouseStartPos;
	Object* SelectBox = new Object({0, 0}, {10, 10});
	int currentCG = 9;

	const int MAP_SCROLL_SPEED = 20;

	int2 mapOffset = 0;
	int2 moveMap = 0;

	Tilemap tilemap;

	vector<int2> obstacleCenters;

private:
	//---------------------- LOCAL FUNCTIONS ---------------------
	void CalculateCameraMove(int2& mapOffset, int2& moveMap, int horizontalMove, int verticalMove);

	void OffsetObjects(const int2 moveMap);

	void SetupSelectBox();

	void SelectUnits();

	void DrawEffects();

	void HandlePlayerUnits();
	void HandleBotUnits();

	void SetDestinationToSelected(Unit& unit);
	void CollisionWithArmy(vector<Unit>& other_army, Unit& cur_unit, int cur_unit_index = -1);
	void AttackSequence(vector<Unit>& other_army, Unit& cur_unit, Tmpl8::Sprite* sprite);

	void SetDestinationToBots(Unit& unit, int index);

	void DrawDebris();

	bool HandleDestroyedUnits(vector<Unit>& army);
	//------------------ END OF LOCAL FUNCTIONS ------------------
};

} // namespace Tmpl8