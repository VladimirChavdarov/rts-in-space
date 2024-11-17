#include "precomp.h"
#include "Unit.h"
#include <algorithm>

static Sprite damageParticles(new Surface("assets/rts_assets/smoke3.png"), 5);


Unit::Unit(int2 coordinates, int2 dimensions, int hp, int movementSpeed, int damage, int range, int curve_size, float cooldown) : Object(coordinates, dimensions)
{
	m_hp = hp;
	m_maxHp = hp;
	m_movementSpeed = movementSpeed;
	m_damage = damage;
	m_range = range;
	m_turn_curve_size = curve_size;
	m_cooldown = cooldown;

	m_destination = m_coord;
	float2 a = { 1, 1 };
	m_currentVector = normalize(a);
	m_targetVector = normalize(m_currentVector);
	m_r = { 0, 0 };
	m_obstacleVec = { 0, 0 };
	m_destinationVec = { 0, 0 };

	m_attackConeRange = { static_cast<float>(m_center.x + m_range),
						  static_cast<float>(m_center.y + m_range) };
	m_attackConeRange = normalize(m_attackConeRange);

}


int Unit::GetHp()
{
	return m_hp;
}
void Unit::SetHp(int hp)
{
	m_hp = hp;
}

int Unit::GetMovementSpeed() const
{
	return m_movementSpeed;
}
void Unit::SetMovementSpeed(int movementSpeed)
{
	if (movementSpeed >= 0)
		m_movementSpeed = movementSpeed;
}

int Unit::GetDamage() const
{
	return m_damage;
}
void Unit::SetDamage(int damage)
{
	if (damage >= 0)
		m_damage = damage;
}

int Unit::GetRange() const
{
	return m_range;
}
void Unit::SetRange(int range)
{
	if (range >= 0)
		m_range = range;
}

int Unit::GetCurveSize() const
{
	return m_turn_curve_size;
}
void Unit::SetCurveSize(int curve_size)
{
	if (curve_size > 0)
		m_turn_curve_size = curve_size;
}

void Unit::SetCooldown(float cooldown)
{
	m_cooldown = cooldown;
}

int2 Unit::GetDestination() const
{
	return m_destination;
}
void Unit::SetDestination(int2 destination, int2 moveMap)
{
	m_destination = destination - moveMap;
	
	/*float2 a = { static_cast<float>(m_destination.x - m_center.x),
				 static_cast<float>(m_destination.y - m_center.y) };
	if(a.x != 0 && a.y != 0)
		m_targetVector = normalize(a);*/
}

void Unit::Detect(Tmpl8::Surface* screen, const int2 enemyCoord, const int2 moveMap) //I overcomplicated this by a great margin. There should be a way to optimize it
{
	m_attackConeRange = m_currentVector;
	float2 tempLeftVec = { m_attackConeRange.x + m_attackConeRange.y, m_attackConeRange.y - m_attackConeRange.x };
	float2 tempRightVec = { m_attackConeRange.x - m_attackConeRange.y, m_attackConeRange.y + m_attackConeRange.x };
	float2 enemyVec = { static_cast<float>(enemyCoord.x - m_center.x), static_cast<float>(enemyCoord.y - m_center.y) };
	tempLeftVec = normalize(tempLeftVec);
	tempRightVec = normalize(tempRightVec);
	float2 tempEnemy = normalize(enemyVec);

	static float2 m_fcenter;
	m_fcenter = { static_cast<float>(m_center.x), static_cast<float>(m_center.y) };

	screen->Line(m_fcenter.x, m_fcenter.y, m_fcenter.x + tempLeftVec.x * m_range, m_fcenter.y + tempLeftVec.y * m_range, 0xffffff/*0x0000ff*/);
	screen->Line(m_fcenter.x, m_fcenter.y, m_fcenter.x + tempRightVec.x * m_range, m_fcenter.y + tempRightVec.y * m_range, 0xffffff/*0xff0000*/);

	float i = 0; //degrees of left vector
	float n = 0; //degrees of right vector
	float e = 0; //degrees of enemy vector

	n = ToNormalDegrees(tempRightVec);
	i = ToNormalDegrees(tempLeftVec);

	for (int p = 0; p < 360; p += 45)
	{
		enemyVec = CalcEnemyVector(screen, enemyCoord, p);

		LiftFireFlag(i, n, enemyVec);
		if (fireFlag)
		{
			if (!projectile->isMoving)
			{
				projectile->SetDestination(targetCoord, m_center, moveMap);
				if (projectile->shootTimer.elapsed() >= m_cooldown)
				{
					projectile->isMoving = true;
					projectile->shootTimer.reset();
				}
			}
			break;
		}
	}

	//DrawCone(screen, i, n);
}

void Unit::CollidesWithWall(int* tileIndex, const vector<Tile*>& map)
{
	for (int j = 0; j < sizeof(tileIndex) + 1; j++)
	{
		if (tileIndex[j] < TILEMAP_WIDTH * TILEMAP_HEIGHT && tileIndex[j] >= 0)
		{
			if (map[tileIndex[j]]->GetEffect() == 2) //make the effects into an enum later pls
			{
				isColliding = true;
			}
		}
	}
}

int* Unit::GetOccupiedTiles(const int2 newCoord, const int2 offset, const vector<Tile*>& map, Tmpl8::Surface* screen)
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
		tileIndex[j] = (worldspaceShipCoord.x / 64) + x + ((worldspaceShipCoord.y / 64) + y) * TILEMAP_WIDTH;
		x++;
		if (x >= shipTileLength.x)
		{
			y++;
			x = 0;
		}
	}

	return tileIndex;
}

int2 Unit::CheckDesiredPos()
{
	int2 nCoord = m_coord;
	nCoord.x += static_cast<int>(m_currentVector.x * m_movementSpeed);
	nCoord.y += static_cast<int>(m_currentVector.y * m_movementSpeed);
	return nCoord;
}

void Unit::CalculateTargetVector(vector<int2>& obstacleCenters)
{
	//--------- DEVELOPING COLLISION AVOIDANCE ALGORITHM --------------------
	m_destinationVec = { static_cast<float>(m_destination.x - m_center.x), static_cast<float>(m_destination.y - m_center.y) };

	static int dirVectorsSize = sizeof(dirVectors) / sizeof(dirVectors[0]);
	dirVectors[0] = m_destinationVec;
	dirVectors[0] = normalize(dirVectors[0]);
	for (int i = 1; i < dirVectorsSize; i++)
	{
		static float2 r;
		r = { dirVectors[i - 1].y / 3.5f, -dirVectors[i - 1].x / 3.5f };

		dirVectors[i] = dirVectors[i - 1] + r;
		dirVectors[i] = normalize(dirVectors[i]);
	}


	float maxDotProduct = -3;
	for (int i = 0; i < dirVectorsSize; i++)
	{

		float obstacleDotSum = 0;
		for (int j = 0; j < obstacleCenters.size(); j++)
		{
			m_obstacleVec = { static_cast<float>(obstacleCenters[j].x - m_center.x),
				static_cast<float>(obstacleCenters[j].y - m_center.y) };


			float obstacleDot;
			if (lightLength(m_destinationVec) < lightLength(m_obstacleVec) || lightLength(m_obstacleVec) > pow(CA_RANGE, 2.0))
				obstacleDot = 0;
			else
				obstacleDot = -dot(normalize(m_obstacleVec), dirVectors[i]);

			obstacleDotSum += obstacleDot;
		}
		float destinationDot = dot(normalize(m_destinationVec), dirVectors[i]);
		float result = (destinationDot + obstacleDotSum) / (obstacleCenters.size() + 1) + 1;

		dirVectors[i] = { dirVectors[i].x * result, dirVectors[i].y * result };
	}

	float maxLength = -1;
	int desiredVecIndex = 0;
	for (int i = 0; i < dirVectorsSize; i++)
	{
		if (length(dirVectors[i]) > maxLength)
		{
			maxLength = length(dirVectors[i]);
			desiredVecIndex = i;
		}
	}
	m_targetVector = dirVectors[desiredVecIndex];

	m_targetVector = normalize(m_targetVector);
	//---------------------------------------------
}

void Unit::Move(Tmpl8::Surface* screen)
{

	//Setting up the direction of "offset" vector (m_r)
	if (!(m_destination.x - m_center.x == 0 && m_destination.y - m_center.y == 0))
	{
		m_currentVector = normalize(m_currentVector);
	}

	m_r = { m_currentVector.y / m_turn_curve_size, -m_currentVector.x / m_turn_curve_size };
	if (dot(m_r, m_targetVector) < 0)
	{
		m_r = -m_r;
	}
	else if (dot(m_r, m_targetVector) == 0)
	{
		m_r = { 0, 0 };
	}

	MoveCurrentVector();

	m_coord = SetNewPosition();
}

void Unit::Attack(Tmpl8::Surface* screen, vector<Unit>& enemyUnit, const int2 moveMap, const int2 mapOffset, vector<Tile*>& map)
{
	//each projectile goes through the whole opposing army each frame. I cringe at the thought of how many checks that accummulates.
	for (int i = 0; i < enemyUnit.size(); i++)
	{
		if (projectile->Collides(enemyUnit[i]))
		{
			enemyUnit[i].SetHp(enemyUnit[i].GetHp() - m_damage);
			projectile->isMoving = false;

			projectile->impactAnimTimer.reset();
			projectile->playImpactAnim = true;
			projectile->impactCoord = projectile->GetCoordinates();
		}
	}

	projectile->CollidesWithWall(projectile->GetOccupiedTiles(projectile->GetCoordinates(), mapOffset, map, screen), map);

	projectile->Move(screen, m_center, moveMap);
}


void Unit::DrawObject(Tmpl8::Surface* screen, Tmpl8::Sprite* sprite)
{	
	SetupFrames(sprite);

	sprite->Draw(screen, m_coord.x, m_coord.y);

	if (isSelected)
	{
		screen->Box(m_coord.x, m_coord.y ,
			m_coord.x + m_dimensions.x, m_coord.y + m_dimensions.y, 0x33e806);

		int green = 250;
		int red = green - m_hp * 5;
		green -= red;
		screen->Bar(m_coord.x + 14, m_coord.y - 20, m_coord.x + m_hp * 2, m_coord.y - 10, (red * 256 * 256) + (green * 256));
	}

	if (m_hp <= m_maxHp / 2)
	{
		float a = smoketTimer.elapsed() * 10;
		if (smoketTimer.elapsed() * 10 >= 0.5f * 10)
		{
			smoketTimer.reset();
		}
		if(a < 5.0f)
			damageParticles.SetFrame(static_cast<int>(a));
		damageParticles.Draw(screen, m_center.x + 16 - damageParticles.GetWidth() / 2, m_center.y - 32 - damageParticles.GetHeight() / 2);
	}
}


//---------------------- LOCAL FUNCTIONS ---------------------
float Unit::ToNormalDegrees(float2 vector)
{
	float a;
	if (static_cast<int>(360.0f + atan2(vector.y, vector.x) * 180.0f / PI) <= 360)
		a = 360.0f + atan2(vector.y, vector.x) * 180.0f / PI;
	else
		a = atan2(vector.y, vector.x) * 180.0f / PI;

	return a;
}

bool Unit::CompareFloat(float a, float b, float epsilon)
{
	if (fabs(a - b) < epsilon)
		return true;
	else
		return false;
}

void Unit::DrawCone(Tmpl8::Surface* screen, float i, float n)
{
	if (i > n)
	{
		DrawCircle(screen, i, 360.0f);
		
		DrawCircle(screen, 0.0f, n);
	}
	else
	{
		DrawCircle(screen, i, n);
	}
}

void Unit::DrawCircle(Tmpl8::Surface* screen, float i, float n)
{
	float angle;
	float tx, ty;
	uint color = 0xffffff;
	for (i; i < n; i++)
	{
		angle = (float)i * (PI / 180.0f);
		tx = m_center.x + cos(angle) * m_range;
		ty = m_center.y + sin(angle) * m_range;

		screen->Plot(static_cast<int>(tx), static_cast<int>(ty), color);
	}
}

float2 Unit::CalcEnemyVector(Tmpl8::Surface* screen, int2 enemyCoord, int deg)
{
	float pointAngle;
	float2 currentPoint;
	pointAngle = (float)deg * (PI / 180.0f);
	currentPoint.x = enemyCoord.x + cos(pointAngle) * m_dimensions.x / 2; //half the size of the sprite
	currentPoint.y = enemyCoord.y + sin(pointAngle) * m_dimensions.y / 2;
	targetCoord = { static_cast<int>(currentPoint.x), static_cast<int>(currentPoint.y)};

	return { static_cast<float>(currentPoint.x - m_center.x), static_cast<float>(currentPoint.y - m_center.y) };
}

void Unit::LiftFireFlag(float leftVecDeg, float rightVecDeg, float2 enemyVec)
{
	float e = ToNormalDegrees(normalize(enemyVec));

	if (leftVecDeg > rightVecDeg)
	{
		if (e >= leftVecDeg && e <= 360 && length(enemyVec) <= length(m_attackConeRange * static_cast<float>(m_range)))
			fireFlag = true;

		if (e >= 0 && e <= rightVecDeg && length(enemyVec) <= length(m_attackConeRange * static_cast<float>(m_range)))
			fireFlag = true;
	}
	else
	{
		if (e >= leftVecDeg && e <= rightVecDeg && length(enemyVec) <= length(m_attackConeRange * static_cast<float>(m_range)))
			fireFlag = true;
	}
}

int2 Unit::SetNewPosition()
{
	int2 nCoord = m_coord;

	float2 a = { static_cast<float>(m_destination.x - m_center.x), static_cast<float>(m_destination.y - m_center.y) };

	if (!isColliding && (length(a) >= TURN_IN_PLACE_RANGE || (CompareFloat(m_currentVector.x, m_targetVector.x) && CompareFloat(m_currentVector.y, m_targetVector.y))))
	{
		if (a.x != 0 || a.y != 0)
		{
			if ((m_center.x < m_destination.x + PRECISION && m_center.x + m_currentVector.x * m_movementSpeed > m_destination.x - PRECISION))
			{
				nCoord.x = m_destination.x - m_dimensions.x / 2;
			}
			else
				nCoord.x += static_cast<int>(m_currentVector.x * m_movementSpeed);

			if ((m_center.y < m_destination.y + PRECISION && m_center.y + m_currentVector.y * m_movementSpeed > m_destination.y - PRECISION))
			{
				nCoord.y = m_destination.y - m_dimensions.y / 2;
			}
			else
				nCoord.y += static_cast<int>(m_currentVector.y * m_movementSpeed);
		}
	}

	return nCoord;
}

void Unit::MoveCurrentVector()
{
	if (!CompareFloat(m_currentVector.x, m_targetVector.x))
	{
		if (m_currentVector.x < m_targetVector.x && m_currentVector.x + m_r.x > m_targetVector.x)
		{
			m_currentVector.x = m_targetVector.x;
		}
		else
		{
			m_currentVector.x = m_currentVector.x + m_r.x;
		}
	}

	if (!CompareFloat(m_currentVector.y, m_targetVector.y))
	{
		if (m_currentVector.y < m_targetVector.y && m_currentVector.y + m_r.y > m_targetVector.y)
		{
			m_currentVector.y = m_targetVector.y;
		}
		else
		{
			m_currentVector.y = m_currentVector.y + m_r.y;
		}
	}
}

void Unit::SetupFrames(Tmpl8::Sprite* sprite)
{
	float currentVecDeg = ToNormalDegrees(m_currentVector);
	if ((currentVecDeg > 345.0f && currentVecDeg <= 360.0f) || (currentVecDeg >= 0.0f && currentVecDeg <= 15.0f))
		sprite->SetFrame(0);
	else if (currentVecDeg > 15.0f && currentVecDeg <= 45.0f)
		sprite->SetFrame(1);
	else if (currentVecDeg > 45.0f && currentVecDeg <= 75.0f)
		sprite->SetFrame(2);
	else if (currentVecDeg > 75.0f && currentVecDeg <= 105.0f)
		sprite->SetFrame(3);
	else if (currentVecDeg > 105.0f && currentVecDeg <= 135.0f)
		sprite->SetFrame(4);
	else if (currentVecDeg > 135.0f && currentVecDeg <= 165.0f)
		sprite->SetFrame(5);
	else if (currentVecDeg > 165.0f && currentVecDeg <= 195.0f)
		sprite->SetFrame(6);
	else if (currentVecDeg > 195.0f && currentVecDeg <= 225.0f)
		sprite->SetFrame(7);
	else if (currentVecDeg > 225.0f && currentVecDeg <= 255.0f)
		sprite->SetFrame(8);
	else if (currentVecDeg > 255.0f && currentVecDeg <= 285.0f)
		sprite->SetFrame(9);
	else if (currentVecDeg > 285.0f && currentVecDeg <= 315.0f)
		sprite->SetFrame(10);
	else if (currentVecDeg > 315.0f && currentVecDeg <= 345.0f)
		sprite->SetFrame(11);
}