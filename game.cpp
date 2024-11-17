#include "precomp.h"
#include <cstdlib>
#include <ctime>


static Sprite target_attack(new Surface("assets/rts_assets/target_attack.png"), 1);
static Sprite target_move(new Surface("assets/rts_assets/target_move.png"), 1);

static Sprite projectileSprite(new Surface("assets/rts_assets/projectile2.png"), 1);
static Sprite shockSprite(new Surface("assets/rts_assets/shock1.png"), 1);

static Sprite shipSpriteRanged(new Surface("assets/rts_assets/sprite_rotation_ranged_12frames.png"), 12);
static Sprite shipSpriteMelee(new Surface("assets/rts_assets/sprite_rotation_melee_12frames.png"), 12);
static Sprite debrisSprite(new Surface("assets/rts_assets/debris.png"), 1);

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	//************** SETTING TILE'S COORDINATES ************
	tilemap.InitTilemap(moveMap);


	//************** SPAWNING UNITS ************
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < P_UNIT_NUM; i++)
	{
		Projectile* proj = new Projectile({ 1, 1 }, { 16, 16 }, 5, 400, { 0, 0 });
		aUnit->projectile = proj;
		aUnit->SetCoordinates({ 10, (10 + shipSpriteRanged.GetHeight()) * i }, moveMap);
		aUnit->SetDimensions({ shipSpriteRanged.GetWidth(), shipSpriteRanged.GetHeight() });
		aUnit->SetDamage({ 10 });
		aUnit->SetHp({ 50 });
		aUnit->SetMovementSpeed({ 5 });
		aUnit->SetRange({ 300 });
		aUnit->SetCurveSize({ 30 });
		aUnit->SetCooldown({ 1.0f });
		float2 aUnitFlCoord = { static_cast<float>(aUnit->GetCoordinates().x), static_cast<float>(aUnit->GetCoordinates().y) };
		aUnit->projectile->Projectile::SetCoordinates(aUnitFlCoord, moveMap);
		PlayerUnits.push_back(*aUnit);
		PlayerUnits[i].SetDestination({ PlayerUnits[i].GetCoordinates().x + PlayerUnits[i].GetDimensions().x,
			PlayerUnits[i].GetCoordinates().y + PlayerUnits[i].GetDimensions().y}, moveMap);
	}
	for (int i = 0; i < E_UNIT_NUM; i++)
	{
		Projectile* proj = new Projectile({ 1, 1 }, { 24, 24 }, 5, 400, { 0, 0 });
		aUnit->projectile = proj;
		aUnit->SetCoordinates({ MAP_WIDTH - 256 - shipSpriteMelee.GetWidth(),
			(50 + shipSpriteMelee.GetHeight()) * i }, moveMap);
		aUnit->SetDimensions({ shipSpriteMelee.GetWidth(), shipSpriteMelee.GetHeight() });
		aUnit->SetDamage({ 5 });
		aUnit->SetHp({ 60 });
		aUnit->SetMovementSpeed({ 10 });
		aUnit->SetRange({ 150 });
		aUnit->SetCurveSize({ 15 });
		aUnit->SetCooldown({ 0.0f });
		float2 aUnitFlCoord = { static_cast<float>(aUnit->GetCoordinates().x), static_cast<float>(aUnit->GetCoordinates().y) };
		aUnit->projectile->Projectile::SetCoordinates(aUnitFlCoord, moveMap);
		BotUnits.push_back(*aUnit);

		int random = rand() % P_UNIT_NUM;
		botTargetIndex.push_back(random);
		BotUnits[i].SetDestination(PlayerUnits[botTargetIndex[i]].m_center, moveMap);
	}
}

void Game::KeyDown(int key)
{
	p_oneToNine = key - 48;
	switch (key)
	{
	case GLFW_KEY_LEFT:
	{
		p_Left = MAP_SCROLL_SPEED;
		break;
	}
	case GLFW_KEY_RIGHT:
	{
		p_Right = MAP_SCROLL_SPEED;
		break;
	}
	case GLFW_KEY_UP:
	{
		p_Up = MAP_SCROLL_SPEED;
		break;
	}
	case GLFW_KEY_DOWN:
	{
		p_Down = MAP_SCROLL_SPEED;
		break;
	}
	case GLFW_KEY_LEFT_CONTROL:
	{
		p_Ctrl = true;
		break;
	}
	default:
		break;
	}
}

void Game::KeyUp(int key)
{
	p_oneToNine = 0 - 48;
	switch (key)
	{
	case GLFW_KEY_LEFT:
	{
		p_Left = 0;
		break;
	}
	case GLFW_KEY_RIGHT:
	{
		p_Right = 0;
		break;
	}
	case GLFW_KEY_UP:
	{
		p_Up = 0;
		break;
	}
	case GLFW_KEY_DOWN:
	{
		p_Down = 0;
		break;
	}
	case GLFW_KEY_LEFT_CONTROL:
	{
		p_Ctrl = false;
		break;
	}
	default:
		break;
	}
}

void Game::MouseDown(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		mouseLeft = true;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		mouseRight = true;
		break;
	}
	}
}

void Game::MouseUp(int button)
{
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		mouseLeft = false;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		mouseRight = false;
		break;
	}
	}
}

// -----------------------------------------------------------
// Main application tick function - Executed once per frame
// -----------------------------------------------------------
void Game::Tick( float deltaTime )
{
	screen->Clear( 0 );

	//************** REMOVE SHIPS AND CHECK WIN CONDITIONS ************
	if (!botWin && !playerWin)
	{
		playerWin = HandleDestroyedUnits(BotUnits);
		botWin = HandleDestroyedUnits(PlayerUnits);
	}

	if (playerWin)
	{
		screen->Print("Player Won!", SCRWIDTH / 2, SCRHEIGHT / 2, 0x00ff00);
		//winScreen.CopyTo(screen, 0, 0);
	}
	if (botWin)
	{
		screen->Print("Computer Won!", SCRWIDTH / 2, SCRHEIGHT / 2, 0xff0000);
		//loseScreen.CopyTo(screen, 0, 0);
	}

	if (!playerWin && !botWin)
	{
		//************** CAMERA OFFSET + BOUNDS ************
		int horizontalMove = p_Right - p_Left;
		int verticalMove = p_Down - p_Up;
		CalculateCameraMove(mapOffset, moveMap, horizontalMove, verticalMove);

		//************** OFFSETTING EVERY OBJECT BY THE CAMERA'S OFFSET ************
		OffsetObjects(moveMap);

		tilemap.DrawMap(screen);

		DrawDebris();

		//************** GIVING ORDERS ************
		select_mouseStartPos -= moveMap;
		SetupSelectBox();
		SelectUnits();

		HandlePlayerUnits();

		HandleBotUnits();
	}

	Sleep( 10 ); // otherwise it will be too fast!
}

void Game::Shutdown()
{
	tilemap.~Tilemap();
	delete SelectBox;
	delete aUnit;
}

void Game::CalculateCameraMove(int2& mapOffset, int2& moveMap, int horizontalMove, int verticalMove)
{
	int2 nMoveMap = { 0,0 };
	nMoveMap.x = horizontalMove;
	if (mapOffset.x + nMoveMap.x >= 0 && mapOffset.x + nMoveMap.x + SCRWIDTH <= MAP_WIDTH)
	{
		mapOffset.x += nMoveMap.x;
		moveMap.x = nMoveMap.x;
	}
	else
		moveMap.x = 0;

	nMoveMap.y = verticalMove;
	if (mapOffset.y + nMoveMap.y >= 0 && mapOffset.y + nMoveMap.y + SCRHEIGHT <= MAP_HEIGHT)
	{
		mapOffset.y += nMoveMap.y;
		moveMap.y = nMoveMap.y;
	}
	else
		moveMap.y = 0;
}

void Game::OffsetObjects(const int2 moveMap)
{
	for (int i = 0; i < size(tilemap.map); i++)
	{
		tilemap.map[i]->SetCoordinates({ tilemap.map[i]->GetCoordinates().x, tilemap.map[i]->GetCoordinates().y }, moveMap);
	}

	for (int i = 0; i < PlayerUnits.size(); i++)
	{
		PlayerUnits[i].SetCoordinates({ PlayerUnits[i].GetCoordinates().x, PlayerUnits[i].GetCoordinates().y }, moveMap);
		PlayerUnits[i].SetDestination({ PlayerUnits[i].GetDestination().x, PlayerUnits[i].GetDestination().y }, moveMap);
		PlayerUnits[i].projectile->SetCoordinates({ PlayerUnits[i].projectile->GetFloatCoordinates().x, PlayerUnits[i].projectile->GetFloatCoordinates().y }, moveMap);

		PlayerUnits[i].isColliding = false;
	}
	for (int i = 0; i < BotUnits.size(); i++)
	{
		BotUnits[i].SetCoordinates({ BotUnits[i].GetCoordinates().x, BotUnits[i].GetCoordinates().y }, moveMap);
		BotUnits[i].SetDestination({ BotUnits[i].GetDestination().x, BotUnits[i].GetDestination().y }, moveMap);
		BotUnits[i].projectile->SetCoordinates({ BotUnits[i].projectile->GetFloatCoordinates().x, BotUnits[i].projectile->GetFloatCoordinates().y }, moveMap);
		
		BotUnits[i].isColliding = false;
	}
	for (int i = 0; i < debris.size(); i++)
	{
		debris[i].x -= moveMap.x;
		debris[i].y -= moveMap.y;
	}
}

void Game::SetupSelectBox()
{
	//Drawing the selectBox correctly so it always starts at the top left corner and has positive dimensions
	if (select_mouseStartPos.x < mousePos.x && select_mouseStartPos.y < mousePos.y)
	{
		SelectBox->SetCoordinates({ select_mouseStartPos.x, select_mouseStartPos.y }, moveMap);
		SelectBox->SetDimensions({ mousePos.x - select_mouseStartPos.x, mousePos.y - select_mouseStartPos.y });
	}
	if (select_mouseStartPos.x > mousePos.x && select_mouseStartPos.y < mousePos.y)
	{
		SelectBox->SetCoordinates({ select_mouseStartPos.x - abs(mousePos.x - select_mouseStartPos.x), select_mouseStartPos.y }, moveMap);
		SelectBox->SetDimensions({ abs(mousePos.x - select_mouseStartPos.x), abs(mousePos.y - select_mouseStartPos.y) });
	}
	if (select_mouseStartPos.x < mousePos.x && select_mouseStartPos.y > mousePos.y)
	{
		SelectBox->SetCoordinates({ select_mouseStartPos.x, select_mouseStartPos.y - abs(mousePos.y - select_mouseStartPos.y) }, moveMap);
		SelectBox->SetDimensions({ abs(mousePos.x - select_mouseStartPos.x), abs(mousePos.y - select_mouseStartPos.y) });
	}
	if (select_mouseStartPos.x > mousePos.x && select_mouseStartPos.y > mousePos.y)
	{
		SelectBox->SetCoordinates({ select_mouseStartPos.x - abs(mousePos.x - select_mouseStartPos.x),
			select_mouseStartPos.y - abs(mousePos.y - select_mouseStartPos.y) }, moveMap);
		SelectBox->SetDimensions({ abs(mousePos.x - select_mouseStartPos.x), abs(mousePos.y - select_mouseStartPos.y) });
	}
}

void Game::SelectUnits()
{
	//selecting via select box
	if (!mouseLeft)
		select_mouseStartPos = mousePos;
	if (mouseLeft)
	{
		for (int i = 0; i < PlayerUnits.size(); i++)
			PlayerUnits[i].isSelected = false;
		screen->Box(SelectBox->GetCoordinates().x, SelectBox->GetCoordinates().y,
			SelectBox->GetCoordinates().x + SelectBox->GetDimensions().x, SelectBox->GetCoordinates().y + SelectBox->GetDimensions().y, 0x33e806);
		for (int i = 0; i < PlayerUnits.size(); i++)
		{
			if (SelectBox->Collides(PlayerUnits[i]))
			{
				PlayerUnits[i].isSelected = true;
			}
		}
	}

	//selecting via control groups
	if (p_oneToNine >= 1 && p_oneToNine <= 9 && !p_Ctrl)
	{
		for (int i = 0; i < PlayerUnits.size(); i++)
		{
			if (PlayerUnits[i].controlGroup == p_oneToNine)
				PlayerUnits[i].isSelected = true;
			else
				PlayerUnits[i].isSelected = false;
		}
	}
}

void Game::HandlePlayerUnits()
{
	//filling the vector with all the obstacles
	//asteroid tiles
	for (int i = 0; i < tilemap.wallTileIndex.size(); i++)
	{
		obstacleCenters.push_back(tilemap.map[tilemap.wallTileIndex[i]]->m_center);
	}
	//enemy ships
	for (int i = 0; i < BotUnits.size(); i++)
	{
		obstacleCenters.push_back(BotUnits[i].m_center);
	}

	for (int i = 0; i < PlayerUnits.size(); i++)
	{
		if (PlayerUnits[i].isSelected)
		{
			SetDestinationToSelected(PlayerUnits[i]);
		}

		//filling the obstacle vector with other ally ships
		int cnt = 0;
		for (int v = 0; v < PlayerUnits.size(); v++)
		{
			if (i != v)
			{
				obstacleCenters.push_back(PlayerUnits[v].m_center);
				cnt++;
			}
		}
		PlayerUnits[i].CalculateTargetVector(obstacleCenters);

		CollisionWithArmy(PlayerUnits, PlayerUnits[i], i);

		CollisionWithArmy(BotUnits, PlayerUnits[i]);

		PlayerUnits[i].CollidesWithWall(PlayerUnits[i].GetOccupiedTiles(PlayerUnits[i].CheckDesiredPos(), mapOffset, tilemap.map, screen), tilemap.map);

		AttackSequence(BotUnits, PlayerUnits[i], &projectileSprite);
		
		////current vector of unit (yellow)
		//screen->Line(PlayerUnits[i].GetCoordinates().x, PlayerUnits[i].GetCoordinates().y,
		//	PlayerUnits[i].GetCoordinates().x + PlayerUnits[i].m_currentVector.x * 100,
		//	PlayerUnits[i].GetCoordinates().y + PlayerUnits[i].m_currentVector.y * 100, 0xfff00f);
		////target vector of unit (blue)
		//screen->Line(PlayerUnits[i].GetCoordinates().x, PlayerUnits[i].GetCoordinates().y,
		//	PlayerUnits[i].GetCoordinates().x + PlayerUnits[i].m_targetVector.x * 100,
		//	PlayerUnits[i].GetCoordinates().y + PlayerUnits[i].m_targetVector.y * 100, 0x0000ff);
		////offset vector (green)
		//screen->Line(PlayerUnits[i].GetCoordinates().x + PlayerUnits[i].m_currentVector.x * 100,
		//	PlayerUnits[i].GetCoordinates().y + PlayerUnits[i].m_currentVector.y * 100,
		//	PlayerUnits[i].GetCoordinates().x + PlayerUnits[i].m_currentVector.x * 100 + PlayerUnits[i].m_r.x * 1000,
		//	PlayerUnits[i].GetCoordinates().y + PlayerUnits[i].m_currentVector.y * 100 + PlayerUnits[i].m_r.y * 1000, 0x00ff00);
		
		PlayerUnits[i].Move(screen);
		//popping out allies out of the "obstacles" so the vector is ready for handling the next ship in queue
		while (cnt != 0)
		{
			obstacleCenters.pop_back();
			cnt--;
		}

		PlayerUnits[i].DrawObject(screen, &shipSpriteRanged);
	}

	obstacleCenters.clear();
}

void Game::SetDestinationToSelected(Unit& unit)
{
	if (p_oneToNine >= 1 && p_oneToNine <= 9 && p_Ctrl)
	{
		unit.controlGroup = p_oneToNine;
	}

	if (mouseRight)
	{
		target_move.Draw(screen, mousePos.x - target_move.GetWidth() / 2, mousePos.y - target_move.GetHeight() / 2);
		unit.SetDestination({ mousePos.x, mousePos.y}, moveMap);
	}
	target_move.Draw(screen, unit.GetDestination().x - target_move.GetWidth() / 2,
		unit.GetDestination().y - target_move.GetHeight() / 2);
}

void Game::CollisionWithArmy(vector<Unit>& other_army, Unit& cur_unit, int cur_unit_index)
{
	for (int j = 0; j < other_army.size(); j++)
	{
		if (j != cur_unit_index)
		{
			int2 changePos;
			changePos.x = cur_unit.CheckDesiredPos().x - cur_unit.GetCoordinates().x;
			changePos.y = cur_unit.CheckDesiredPos().y - cur_unit.GetCoordinates().y;
			if (cur_unit.Collides(other_army[j], changePos))
			{
				cur_unit.isColliding = true;
				/*screen->Box(cur_unit.GetCoordinates().x, cur_unit.GetCoordinates().y,
					cur_unit.GetCoordinates().x + cur_unit.GetDimensions().x,
					cur_unit.GetCoordinates().y + cur_unit.GetDimensions().y, 0xe80606);*/
			}
		}
	}
}


void Game::AttackSequence(vector<Unit>& other_army, Unit& cur_unit, Tmpl8::Sprite* sprite)
{
	for (int j = 0; j < other_army.size(); j++)
	{
		cur_unit.fireFlag = false;
		int2 enemyCoord;
		enemyCoord.x = other_army[j].GetCoordinates().x + other_army[j].GetDimensions().x / 2;
		enemyCoord.y = other_army[j].GetCoordinates().y + other_army[j].GetDimensions().y / 2;
		cur_unit.Detect(screen, enemyCoord, moveMap);
		if (cur_unit.fireFlag)
		{
			break;
		}
	}
	if (!cur_unit.fireFlag && !cur_unit.projectile->isMoving)
	{
		float2 curUnitFlCoord = { static_cast<float>(cur_unit.m_center.x), static_cast<float>(cur_unit.m_center.y) };
		cur_unit.projectile->SetCoordinates(curUnitFlCoord, moveMap);
		cur_unit.projectile->isMoving = false;
	}
	else
	{
		cur_unit.Attack(screen, other_army, moveMap, mapOffset, tilemap.map);
	}
	cur_unit.projectile->RunImpactAnimation(screen, moveMap);

	sprite->Draw(screen, cur_unit.projectile->GetCoordinates().x, cur_unit.projectile->GetCoordinates().y);
}

void Game::HandleBotUnits()
{
	//filling the vector with all the obstacles
	//asteroid tiles
	for (int i = 0; i < tilemap.wallTileIndex.size(); i++)
	{
		obstacleCenters.push_back(tilemap.map[tilemap.wallTileIndex[i]]->m_center);
	}
	//enemy ships
	for (int i = 0; i < PlayerUnits.size(); i++)
	{
		obstacleCenters.push_back(PlayerUnits[i].m_center);
	}

	for (int i = 0; i < BotUnits.size(); i++)
	{
		SetDestinationToBots(BotUnits[i], i);

		//filling the obstacle vector with other ally ships
		int cnt = 0;
		for (int v = 0; v < BotUnits.size(); v++)
		{
			if (i != v)
			{
				obstacleCenters.push_back(BotUnits[v].m_center);
				cnt++;
			}
		}
		BotUnits[i].CalculateTargetVector(obstacleCenters);

		CollisionWithArmy(BotUnits, BotUnits[i], i);

		CollisionWithArmy(PlayerUnits, BotUnits[i]);

		BotUnits[i].CollidesWithWall(BotUnits[i].GetOccupiedTiles(BotUnits[i].CheckDesiredPos(), mapOffset, tilemap.map, screen), tilemap.map);

		AttackSequence(PlayerUnits, BotUnits[i], &shockSprite);

		BotUnits[i].Move(screen);
		//popping out allies out of the "obstacles" so the vector is ready for handling the next ship in queue
		while (cnt != 0)
		{
			obstacleCenters.pop_back();
			cnt--;
		}

		BotUnits[i].DrawObject(screen, &shipSpriteMelee);

		/*screen->Box(BotUnits[i].GetCoordinates().x, BotUnits[i].GetCoordinates().y,
			BotUnits[i].GetCoordinates().x + BotUnits[i].GetDimensions().x,
			BotUnits[i].GetCoordinates().y + BotUnits[i].GetDimensions().y, 0x00ffff);*/
	}

	obstacleCenters.clear();
}

void Game::SetDestinationToBots(Unit& unit, int index)
{
	if (botTargetIndex[index] > PlayerUnits.size() - 1)
	{
		botTargetIndex[index] = rand() % PlayerUnits.size();
	}
	unit.SetDestination(PlayerUnits[botTargetIndex[index]].m_center, moveMap);
}

void Game::DrawDebris()
{
	if (!debris.empty())
	{
		for (int i = 0; i < debris.size(); i++)
		{
			debrisSprite.Draw(screen, debris[i].x, debris[i].y);
		}
	}
}

bool Game::HandleDestroyedUnits(vector<Unit>& army)
{
	//if all units in the army are above 0 hp, this function returns false.
	for (int i = 0; i < army.size(); i++)
	{
		if (army[i].GetHp() <= 0)
		{
			debris.push_back(army[i].GetCoordinates());
			army.erase(army.begin() + i);
			if (army.empty())
			{
				break;
			}
		}
	}

	if (army.empty())
		return true;
	else
		return false;
}