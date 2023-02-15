// Includes
#include <time.h>
#include <math.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <queue>
#include <string>
#include "glut.h"
#include "Attacker.h"
#include "Weapon.h"
#include "Courier.h"
#include "NPC.h"
#include "Node.h"
#include "NodesComparator.h"
#include "Room.h"
#include "Settings.h"

using namespace std;

// Fields
bool gameOver = false, restart = false, securityMapVisible = false;
int map[MAP_DIMENSION][MAP_DIMENSION] = { 0 };
double securityMap[MAP_DIMENSION][MAP_DIMENSION] = { 0 };

Room rooms[MAX_ROOMS];
Attacker attackers[2 * ATTACKERS_IN_TEAM];
Courier couriers[2 * COURIERS_IN_TEAM];

vector<Position*> supplies;
vector<Weapon*> weapons;
vector<Node*> greyNodes, blackNodes;

// Methods

/// <summary>
/// Calculates Manhattan distance between 2 locations.
/// </summary>
/// <param name="p1">The first location</param>
/// <param name="p2">The second location</param>
/// <returns>The Manhattan distance between the location</returns>
double ManhattanDistance(Position p1, Position p2)
{
	double row1 = p1.GetRow(), column1 = p1.GetColumn(), row2 = p2.GetRow(), column2 = p2.GetColumn();

	return fabs(row1 - row2) + fabs(column1 - column2);
}

/// <summary>
/// Given a position in the board - figure out what to do with it
/// </summary>
/// <param name="row">Row in board</param>
/// <param name="column">Column in board</param>
/// <param name="currentNode">The node that called it (parent)</param>
/// <param name="pathFound">The flag indicating whether a path was found</param>
void CheckNeighbor(int row, int column, Node* currentNode, bool* pathFound)
{
	Position destination = currentNode->GetDestination();
	Node* neighborNode;

	if (currentNode->GetLocation() == destination)
	{
		*pathFound = true;
		return;
	}

	neighborNode = new Node(new Position(row, column), &destination, currentNode, securityMap[row][column] + currentNode->GetG());
	for (vector<Node*>::iterator blackNodesIterator = blackNodes.begin(); blackNodesIterator != blackNodes.end(); blackNodesIterator++)
		if ((*blackNodesIterator)->GetLocation() == neighborNode->GetLocation())
			return;

	for (vector<Node*>::iterator greyNodesIterator = greyNodes.begin(); greyNodesIterator != greyNodes.end(); greyNodesIterator++)
		if ((*greyNodesIterator)->GetLocation() == neighborNode->GetLocation())
		{
			if ((*greyNodesIterator)->GetF() > neighborNode->GetF())
			{
				greyNodes.erase(greyNodesIterator);
				greyNodes.push_back(neighborNode);
			}

			return;
		}

	greyNodes.push_back(neighborNode); // insert the new one
}

/// <summary>
/// Finds the Node with minimal F value (for A*).
/// </summary>
/// <returns>The Node with minimal F value</returns>
Node* FindMinimalFNode()
{
	Node* minimalNode;
	int eraseOffset = 0;
	float minimalF = MAX_ITERATIONS;

	// Retrieves Node with minimal f value
	for (int i = 0; i < greyNodes.size(); i++)
		if (greyNodes[i]->GetF() < minimalF)
		{
			minimalF = greyNodes[i]->GetF();
			eraseOffset = i;
		}

	// Removes retrieved Node from vector
	minimalNode = greyNodes[eraseOffset];
	greyNodes.erase(greyNodes.begin() + eraseOffset);
	blackNodes.push_back(minimalNode);

	return minimalNode;
}

/// <summary>
/// Finds the next location a given NPC should move to.
/// </summary>
/// <param name="npc">The NPC to find the next location of</param>
/// <returns>The NPC's next location</returns>
Position FindNextLocation(NPC* npc)
{
	Position source = npc->GetLocation(), destination = npc->GetDestination(), currentLocation;
	Node* currentNode = new Node(&source, &destination, nullptr, 0);
	int nodeRow, nodeColumn;
	bool pathFound = false;
	int counter = 0;

	blackNodes.clear();
	greyNodes.clear();
	greyNodes.push_back(currentNode);

	while (!pathFound)
	{
		// Validation
		if (greyNodes.empty())
			return source;

		counter++;

		currentNode = FindMinimalFNode();
		currentLocation = currentNode->GetLocation();
		nodeRow = (int)currentLocation.GetRow();
		nodeColumn = (int)currentLocation.GetColumn();

		if ((!pathFound) && (map[nodeRow - 1][nodeColumn] != WALL))
			CheckNeighbor(nodeRow - 1, nodeColumn, currentNode, &pathFound);
		if ((!pathFound) && (map[nodeRow + 1][nodeColumn] != WALL))
			CheckNeighbor(nodeRow + 1, nodeColumn, currentNode, &pathFound);
		if ((!pathFound) && (map[nodeRow][nodeColumn - 1] != WALL))
			CheckNeighbor(nodeRow, nodeColumn - 1, currentNode, &pathFound);
		if ((!pathFound) && (map[nodeRow][nodeColumn + 1] != WALL))
			CheckNeighbor(nodeRow, nodeColumn + 1, currentNode, &pathFound);

		if (counter >= MAX_ITERATIONS)
			break;
	}

	if (counter >= MAX_ITERATIONS)
		return source;

	// Restores the path
	while ((currentNode->GetParent() != nullptr) && (currentNode->GetParent()->GetParent() != nullptr))
		currentNode = currentNode->GetParent();

	return currentNode->GetLocation();
}

/// <summary>
/// Finds an attacker by its location.
/// </summary>
/// <param name="attackerLocation">The requested attacker's location</param>
/// <returns>The Attacker in the given location (if there's one), Nullptr otherwise</returns>
Attacker* FindAttackerByLocation(Position attackerLocation)
{
	Position currentLocation;

	for (int i = 0; i < ATTACKERS_IN_TEAM; i++)
		if (attackers[i].GetLocation() == attackerLocation)
			return &attackers[i];

	return nullptr;
}

/// <summary>
/// Finds the attackers by their team.
/// </summary>
/// <param name="teamColor">The attackers' team</param>
/// <returns>The team's attackers</returns>
Attacker** FindAttackersByColor(int teamColor)
{
	Attacker* teamAttackers[ATTACKERS_IN_TEAM];

	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
		if (attackers[i].GetTeam() == teamColor)
			teamAttackers[i % ATTACKERS_IN_TEAM] = &attackers[i];

	return teamAttackers;
}

/// <summary>
/// Finds a courier by its team.
/// </summary>
/// <param name="teamColor">The courier's team</param>
/// <returns>The team's courier</returns>
Courier* FindCourierByColor(int teamColor)
{
	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
		if (couriers[i].GetTeam() == teamColor)
			return &couriers[i];

	return nullptr;
}

/// <summary>
/// Finds a team by its color
/// </summary>
/// <param name="teamColor">The team's color</param>
/// <returns>The team</returns>
NPC** FindTeamByColor(int teamColor)
{
	Attacker** teamAttackers = FindAttackersByColor(teamColor);
	Attacker* currentAttacker = *teamAttackers;
	Courier* teamCourier = FindCourierByColor(teamColor);
	NPC* team[TEAM_SIZE];

	// Validation
	if ((currentAttacker == nullptr) || (teamCourier == nullptr))
		return nullptr;

	team[0] = teamCourier;
	for (int i = 1; i < TEAM_SIZE; i++)
		team[i] = currentAttacker++;

	return team;
}

/// <summary>
/// Finds the enemy nearest to a Attacker.
/// </summary>
/// <param name="attacker">The attacker that searches an enemy</param>
/// <returns>The nearest enemy to the attacker</returns>
NPC* FindNearestEnemy(Attacker attacker)
{
	int enemyTeamColor = attacker.GetTeam() == RED_TEAM ? BLUE_TEAM : RED_TEAM;
	double currentDistance, nearestDistance = (double)MAX_ITERATIONS;
	NPC** enemyTeam = FindTeamByColor(enemyTeamColor);
	NPC* currentEnemy = *enemyTeam, * nearestEnemy = nullptr;

	// Validation
	if (currentEnemy == nullptr)
		return nullptr;

	// Finds the enemy (attacker / courier) nearest to the attacker
	for (int i = 0; i < TEAM_SIZE; i++, currentEnemy++)
	{
		// Validation
		if (currentEnemy->GetHealth() <= 0)
			continue;

		currentDistance = ManhattanDistance(currentEnemy->GetLocation(), attacker.GetLocation());
		if (currentDistance < nearestDistance)
		{
			nearestEnemy = currentEnemy;
			nearestDistance = currentDistance;
		}
	}

	return nearestEnemy;
}

/// <summary>
/// Finds the teammate attacker nearest to a courier.
/// </summary>
/// <param name="courier">The courier that searches a teammate attacker</param>
/// <returns>The nearest Attacker of the courier's team</returns>
Attacker* FindNearestAlly(Courier courier)
{
	double currentDistance, nearestDistance = (double)MAX_ITERATIONS;
	Attacker** teamAttackers = FindAttackersByColor(courier.GetTeam());
	Attacker* currentAttacker = *teamAttackers, * nearestAttacker = nullptr;

	// Validation
	if (currentAttacker == nullptr)
		return nullptr;

	for (int i = 0; i < ATTACKERS_IN_TEAM; i++, currentAttacker++)
	{
		// Validation
		if (currentAttacker->GetHealth() <= 0)
			continue;

		currentDistance = ManhattanDistance(courier.GetLocation(), attackers[i].GetLocation());
		if (currentDistance < nearestDistance)
		{
			nearestAttacker = currentAttacker;
			nearestDistance = currentDistance;
		}
	}

	return nearestAttacker;
}

/// <summary>
/// Finds the ID of the room which contains a given NPC.
/// </summary>
/// <param name="npc">The NPC to check</param>
/// <returns>The ID of the room the NPC is in, -1 otherwise (the NPC is inside a tunnel)</returns>
int FindContainerRoom(NPC* npc)
{
	for (int i = 0; i < MAX_ROOMS; i++)
		if (rooms[i].IsNPCInside(npc))
			return i;

	return -1; // The NPC is in a corridor
}

/// <summary>
/// Finds the supply nearest to a given courier.
/// </summary>
/// <param name="courier">The courier that searches for supplies</param>
/// <returns>The location of the nearest supply</returns>
Position FindNearestSupply(Courier courier)
{
	int nearestOffset = -1;
	double currentDistance, nearestDistance = (double)MAX_ITERATIONS;

	for (int i = 0; i < supplies.size(); i++)
	{
		currentDistance = ManhattanDistance(courier.GetLocation(), *supplies[i]);
		if (currentDistance < nearestDistance)
		{
			nearestOffset = i;
			nearestDistance = currentDistance;
		}
	}

	return *supplies[nearestOffset];
}

/// <summary>
/// Finds the shelter position nearest to an NPC.
/// </summary>
/// <param name="npc">The NPC that seeks shelter</param>
/// <returns>The location of the nearest shelter</returns>
Position FindNearestShelter(NPC* npc)
{
	Position shelter;
	int shelterRow = -1, shelterColumn = -1;
	double minimalSecurityFactor = (double)MAP_DIMENSION * MAP_DIMENSION;

	for (int row = 0; row <= MAP_DIMENSION; row++)
		for (int column = 0; column <= MAP_DIMENSION; column++)
		{
			if (map[row][column] == WALL)
				continue;
			else if (securityMap[row][column] < minimalSecurityFactor)
			{
				minimalSecurityFactor = securityMap[row][column];
				shelterRow = row;
				shelterColumn = column;
			}
		}

	shelter.SetRow(shelterRow);
	shelter.SetColumn(shelterColumn);

	return shelter;
}

/// <summary>
/// Explodes a combustable weapon (grenade).
/// </summary>
/// <param name="grenade">The grenade to explode</param>
void ExplodeGrenade(Weapon* grenade)
{
	double alpha = 0, teta = (360.0 / SHARDS_IN_GRENADE) * PI / 180;
	double dRow, dColumn;
	Position shardLocation;

	// Creates shards at the grenade's proximity
	for (int i = 0; i < SHARDS_IN_GRENADE; i++, alpha += teta)
	{
		shardLocation = grenade->GetLocation();
		dRow = (rand() % 2 == 0 ? 1 : -1) * WEAPON_STEP;
		dColumn = (rand() % 2 == 0 ? 1 : -1) * WEAPON_STEP;

		shardLocation.SetRow(shardLocation.GetRow() + dRow);
		shardLocation.SetColumn(shardLocation.GetColumn() + dColumn);

		weapons.push_back(new Weapon(shardLocation, alpha, grenade->GetTeam(), false));
	}
}

/// <summary>
/// Indicates whether a weaponry (bullet / grenade) is still active.
/// </summary>
/// <param name="weaponLocation">The weapon's location</param>
/// <returns>True if the weapon is still active, False otherwise</returns>
bool IsWeaponActive(Weapon* weapon)
{
	Position weaponLocation = weapon->GetLocation(), enemyLocation;
	NPC** enemyTeam;
	NPC* currentEnemy;

	// Checks if the weapon hits a wall
	if (map[(int)(weaponLocation.GetRow() - WEAPON_SIZE)][(int)weaponLocation.GetColumn()] == WALL)
		return false;
	if (map[(int)(weaponLocation.GetRow() + WEAPON_SIZE)][(int)weaponLocation.GetColumn()] == WALL)
		return false;
	if (map[(int)weaponLocation.GetRow()][(int)(weaponLocation.GetColumn() - WEAPON_SIZE)] == WALL)
		return false;
	if (map[(int)weaponLocation.GetRow()][(int)(weaponLocation.GetColumn() + WEAPON_SIZE)] == WALL)
		return false;

	// Checks if the weapon hits an enemy
	enemyTeam = FindTeamByColor(weapon->GetTeam() == RED_TEAM ? BLUE_TEAM : RED_TEAM);
	currentEnemy = *enemyTeam;

	for (int i = 0; i < TEAM_SIZE; i++, currentEnemy++)
	{
		// Validation
		if (currentEnemy == nullptr)
		{
			cout << "Error in 'IsWeaponActive', line 409\n";
			return false;
		}
		if (currentEnemy->GetHealth() <= 0)
			continue;

		enemyLocation = currentEnemy->GetLocation();
		if (enemyLocation == EMPTY_POSITION)
		{
			cout << "Error in 'IsWeaponActive', line 418\n";
			return false;
		}

		if ((fabs(weaponLocation.GetRow() - enemyLocation.GetRow()) <= WEAPON_SIZE)
			&& (fabs(weaponLocation.GetColumn() - enemyLocation.GetColumn()) <= WEAPON_SIZE))
		{
			currentEnemy->TakeDamage(weapon->GetDamage()); // Weapon damage calculated by distance

			return false; // Weapon hit and should dissappear.
		}
	}

	return true;
}

/// <summary>
/// Checks whether a weapon hits a wall or an enemy.
/// </summary>
/// <param name="weapon">The weapon to check hit</param>
void CheckWeaponHit(Weapon* weapon)
{
	if (!IsWeaponActive(weapon))
	{
		if (weapon->IsCombustable())
			ExplodeGrenade(weapon);

		// Removes the weapon from the weapons vector
		for (vector<Weapon*>::iterator weaponsIterator = weapons.begin(); weaponsIterator != weapons.end(); weaponsIterator++)
			if ((*weaponsIterator)->GetLocation() == weapon->GetLocation())
			{
				weapons.erase(weaponsIterator);
				break;
			}
	}
}

// Map

/// <summary>
/// Indicates whether an NPC can spawn in a given location.
/// </summary>
/// <param name="spawnPosition">The spawn point to check</param>
/// <param name="teamColor">The NPC's team</param>
/// <returns>True if the NPC can spawn in the given location, False otherwise</returns>
bool IsValidNPCSpawn(Position spawnPosition, int teamColor)
{
	// Checks whether any teammate is already in the designated spawn location
	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
		if (attackers[i].GetLocation() == spawnPosition)
			return false;

	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
		if (couriers[i].GetLocation() == spawnPosition)
			return false;

	return true;
}

/// <summary>
/// Randomizes a valid spawn point in a given room.
/// </summary>
/// <param name="Room">The room to spawn in</param>
/// <param name="teamColor">The NPC's team</param>
/// <returns>A valid spawn point in the Room</returns>
Position RandomizeNPCSpawn(Room* room, int teamColor)
{
	Position roomCenter = room->GetCenterPosition(), spawnLocation;
	int roomRowStart = (int)fabs(roomCenter.GetRow() - room->GetHeight() / 2);
	int roomColumnStart = (int)fabs(roomCenter.GetColumn() - room->GetWidth() / 2);

	do
	{
		spawnLocation.SetRow((double)roomRowStart + rand() % room->GetHeight());
		spawnLocation.SetColumn((double)roomColumnStart + rand() % room->GetWidth());
	} while ((map[(int)spawnLocation.GetRow()][(int)spawnLocation.GetColumn()] == WALL)
		|| (!IsValidNPCSpawn(spawnLocation, teamColor)));

	return spawnLocation;
}

/// <summary>
/// Given a situation in which the gray Node in the priorityQueue isn't better than the Node we discovered,
/// We will have to update the priorityQueue to include the new details
/// </summary>
/// <param name="neighborPtr">The pointer to the neighborNode we are replacing</param>
/// <param name="priorityQueue">The priority queue</param>
void UpdateNeighbor(Node* neighborPtr, priority_queue <Node*, vector<Node*>, NodesComparator>& priorityQueue)
{
	vector<Node*> tempVector;
	Node* updatedNodePtr;

	// Pours the priority queue into temp	
	while ((!priorityQueue.empty()) && (!(*priorityQueue.top() == *neighborPtr)))
	{
		tempVector.push_back(priorityQueue.top());
		priorityQueue.pop();
	}

	// Validation
	if (priorityQueue.empty())
		exit(1);

	// Updates the node and places it in the right place in the priority queue
	updatedNodePtr = priorityQueue.top();
	priorityQueue.pop();

	updatedNodePtr->SetG(neighborPtr->GetG());
	updatedNodePtr->SetF(updatedNodePtr->GetG() + updatedNodePtr->GetH());
	priorityQueue.push(updatedNodePtr);

	while (!tempVector.empty())
	{
		priorityQueue.push(tempVector.back());
		tempVector.pop_back();
	}
}

/// <summary>
/// Adds a neighbor node to the priority queue and node vectors.
/// </summary>
/// <param name="source">The neighbor's source location</param>
/// <param name="destination">The neighbor's destination location</param>
/// <param name="parent">The neighbor's parent</param>
/// <param name="priorityQueue">The priority queue managing the greyNodes</param>
/// <param name="grays">The vector of in-process greyNodes</param>
/// <param name="blacks">The vectors of processed greyNodes</param>
void AddNeighbor(Position* source, Position* destination, Node* parent,
	priority_queue <Node*, vector<Node*>, NodesComparator>& priorityQueue, vector<Node>& graysVector, vector<Node>& blacksVector)
{
	double weight = map[(int)source->GetRow()][(int)source->GetColumn()] == WALL ? WALL_WEIGHT : SPACE_WEIGHT;
	Node* neighborPtr = new Node(source, destination, parent, parent->GetG() + weight);
	vector<Node>::iterator graysIterator, blacksIterator;

	// Checks if the blacks and grays already contain the node
	graysIterator = find(graysVector.begin(), graysVector.end(), *neighborPtr);
	blacksIterator = find(blacksVector.begin(), blacksVector.end(), *neighborPtr);
	if ((graysIterator == graysVector.end()) && (blacksIterator == blacksVector.end())) // WHITE square
	{
		priorityQueue.push(neighborPtr);
		graysVector.push_back(*neighborPtr);
	}
	else if (graysIterator != graysVector.end())
		if (graysIterator->GetF() > neighborPtr->GetF()) // We found a better route to the node, so we replace it.
		{
			graysIterator->SetG(neighborPtr->GetG());
			graysIterator->SetF(graysIterator->GetG() + graysIterator->GetH());
			UpdateNeighbor(neighborPtr, priorityQueue);
		}
}

/// <summary>
/// Given an Node, paints the path in the maze as a Room
/// </summary>
/// <param name="currentNode">Pointer to the last Node in the path</param>
void RestoreTunnelPath(Node* currentNode)
{
	Position location;

	while (currentNode->GetParent() != nullptr)
	{
		location = currentNode->GetLocation();
		map[(int)location.GetRow()][(int)location.GetColumn()] = SPACE;

		currentNode = currentNode->GetParent();
	}
}

/// <summary>
/// Digs a tunnel between two existing rooms.
/// </summary>
/// <param name="room1ID">The first room's ID</param>
/// <param name="room2ID">The second room's ID</param>
void DigTunnel(int room1ID, int room2ID)
{
	priority_queue <Node*, vector<Node*>, NodesComparator> priorityQueue;
	vector<Node> graysVector, blacksVector;
	Position source = rooms[room1ID].GetCenterPosition(), destination = rooms[room2ID].GetCenterPosition();
	Node* currentNode = new Node(&source, &destination, nullptr, 0);

	priorityQueue.push(currentNode);
	graysVector.push_back(*currentNode);

	while (!priorityQueue.empty())
	{
		currentNode = priorityQueue.top();
		priorityQueue.pop();

		if (currentNode->GetH() <= 0.1) // Destination has been reached
		{
			RestoreTunnelPath(currentNode);
			return;
		}

		source = currentNode->GetLocation();
		destination = currentNode->GetDestination();

		if (source.GetRow() > 0)
			AddNeighbor(new Position(source.GetRow() - 1, source.GetColumn()), &destination, currentNode, priorityQueue,
				graysVector, blacksVector);
		if (source.GetRow() < (double)MAP_DIMENSION - 1)
			AddNeighbor(new Position(source.GetRow() + 1, source.GetColumn()), &destination, currentNode, priorityQueue,
				graysVector, blacksVector);
		if (source.GetColumn() > 0)
			AddNeighbor(new Position(source.GetRow(), source.GetColumn() - 1), &destination, currentNode, priorityQueue,
				graysVector, blacksVector);
		if (source.GetColumn() < (double)MAP_DIMENSION - 1)
			AddNeighbor(new Position(source.GetRow(), source.GetColumn() + 1), &destination, currentNode, priorityQueue,
				graysVector, blacksVector);
	}
}

/// <summary>
/// Indicates whether a designated room's placement overlaps with an existing room.
/// </summary>
/// <param name="centerRow">The designated room's center row</param>
/// <param name="centerColumn">The designated room's center column</param>
/// <param name="height">The designated room's height</param>
/// <param name="width">The designated room's width</param>
/// <returns>True if the designated room's placement overlaps with an existing room, False otherwise</returns>
bool IsRoomOverlapping(int centerRow, int centerColumn, int height, int width)
{
	Position currentPosition;
	int dx, dy;

	for (int i = 0; i < MAX_ROOMS; i++)
	{
		currentPosition = rooms[i].GetCenterPosition();
		dx = fabs(centerColumn - currentPosition.GetColumn());
		dy = fabs(centerRow - currentPosition.GetRow());

		if ((dx < ((width + rooms[i].GetWidth()) / 2 + 4)) && (dy < ((height + rooms[i].GetHeight()) / 2 + 4)))
			return true;
	}

	return false;
}

/// <summary>
/// Places obstacles in a given room.
/// </summary>
/// <param name="room">The room to place obstacles in</param>
void PlaceObstacles(Room* room)
{
	int roomRowStart = (int)fabs(room->GetCenterPosition().GetRow() - room->GetHeight() / 2) + 3;
	int roomRowEnd = ((int)room->GetCenterPosition().GetRow() + room->GetHeight() / 2) - 3;
	int roomColumnStart = (int)fabs(room->GetCenterPosition().GetColumn() - room->GetWidth() / 2) + 3;
	int roomColumnEnd = ((int)room->GetCenterPosition().GetColumn() + room->GetWidth() / 2) - 3;
	int obstaclesCount = 2 + rand() % (MAX_OBSTACLES_IN_ROOM - 2), orientation = rand() % 2;
	int row, column;

	// Fills room with obstacles
	for (int i = 0; i < obstaclesCount; i++)
	{
		do
		{
			row = (roomRowStart + 5) + rand() % (roomRowEnd - (roomRowStart + 5));
			column = (roomColumnStart + 5) + rand() % (roomColumnEnd - (roomColumnStart + 5));
		} while (map[row][column] != SPACE);

		map[row][column] = WALL;
	}
}

/// <summary>
/// Indicates whether a supply can be placed at a specific location.
/// </summary>
/// <param name="row">The location's row offset</param>
/// <param name="column">The location's column offset</param>
/// <returns>True if the supply can be placed, False otherwise</returns>
bool CanPlaceSupply(int row, int column)
{
	if (map[row][column] != SPACE)
		return false;
	if ((map[row - 1][column] != SPACE) || (map[row + 1][column] != SPACE) || (map[row][column - 1] != SPACE) || (map[row][column + 1] != SPACE))
		return false;

	return true;
}

/// <summary>
/// Places ammo and meds in a given room.
/// </summary>
/// <param name="room">The room to place supplies in</param>
void PlaceSupplies(Room* room)
{
	int roomRowStart = (int)fabs(room->GetCenterPosition().GetRow() - room->GetHeight() / 2);
	int roomRowEnd = (int)room->GetCenterPosition().GetRow() + room->GetHeight() / 2;
	int roomColumnStart = (int)fabs(room->GetCenterPosition().GetColumn() - room->GetWidth() / 2);
	int roomColumnEnd = (int)room->GetCenterPosition().GetColumn() + room->GetWidth() / 2;
	int ammosCount = rand() % ARMS, medsCount = rand() % MAX_MEDS_IN_ROOM;
	int row, column;

	// Fills the room with arms
	for (int i = 0; i < ammosCount; i++)
	{
		do
		{
			row = roomRowStart + rand() % (roomRowEnd + 1);
			column = roomColumnStart + rand() % (roomColumnEnd + 1);
		} while (!CanPlaceSupply(row, column));

		map[row][column] = ARMS;
		supplies.push_back(new Position(row, column));
	}

	// Fills the room with meds
	for (int i = 0; i < medsCount; i++)
	{
		do
		{
			row = roomRowStart + rand() % (roomRowEnd + 1);
			column = roomColumnStart + rand() % (roomColumnEnd + 1);
		} while (!CanPlaceSupply(row, column));

		map[row][column] = MEDS;
		supplies.push_back(new Position(row, column));
	}
}

/// <summary>
/// Places a given room on the map matrix.
/// </summary>
/// <param name="room">The room to place on the map</param>
void PlaceRoom(Room* room)
{
	int roomRowStart = (int)fabs(room->GetCenterPosition().GetRow() - room->GetHeight() / 2);
	int roomRowEnd = (int)room->GetCenterPosition().GetRow() + room->GetHeight() / 2;
	int roomColumnStart = (int)fabs(room->GetCenterPosition().GetColumn() - room->GetWidth() / 2);
	int roomColumnEnd = (int)room->GetCenterPosition().GetColumn() + room->GetWidth() / 2;

	for (int row = roomRowStart; row <= roomRowEnd; row++)
		for (int column = roomColumnStart; column <= roomColumnEnd; column++)
			map[row][column] = SPACE;
}

/// <summary>
/// Initiates the map rooms.
/// </summary>
void InitRooms()
{
	Position centerPosition;
	int centerRow, centerColumn, height, width;

	// Randomizes rooms
	for (int roomID = 0; roomID < MAX_ROOMS; roomID++)
	{
		do
		{
			centerRow = EDGE_OFFSET + rand() % (MAP_DIMENSION - (4 * EDGE_OFFSET));
			centerColumn = EDGE_OFFSET + rand() % (MAP_DIMENSION - (4 * EDGE_OFFSET));
			height = EDGE_OFFSET + rand() % ROOM_HEIGHT;
			width = EDGE_OFFSET + rand() % ROOM_WIDTH;
		} while (IsRoomOverlapping(centerRow, centerColumn, height, width));

		centerPosition.SetRow(centerRow);
		centerPosition.SetColumn(centerColumn);

		rooms[roomID].SetCenterPosition(&centerPosition);
		rooms[roomID].SetID(roomID);
		rooms[roomID].SetHeight(height);
		rooms[roomID].SetWidth(width);

		// Places the Room in the map matrix
		PlaceRoom(&rooms[roomID]);
	}

	// Connects each two rooms with a tunnel
	for (int sourceRoomID = 0; sourceRoomID < MAX_ROOMS; sourceRoomID++)
		for (int destinationRoomID = sourceRoomID + 1; destinationRoomID < MAX_ROOMS; destinationRoomID++)
			DigTunnel(sourceRoomID, destinationRoomID);

	// Fills each room with obstacles and supplies
	for (int i = 0; i < MAX_ROOMS; i++)
	{
		PlaceObstacles(&rooms[i]);
		PlaceSupplies(&rooms[i]);
	}
}

/// <summary>
/// Position pacman in the center of the map as a new instance of a "Node".
/// Position the Ghosts and instantiate them
/// </summary>
void InitGame()
{
	Position spawnLocation;
	int redTeamSpawnRoom, blueTeamSpawnRoom;

	// Chooses rooms to spawn both teams
	do
	{
		redTeamSpawnRoom = rand() % MAX_ROOMS;
		blueTeamSpawnRoom = rand() % MAX_ROOMS;
	} while (redTeamSpawnRoom == blueTeamSpawnRoom);

	// Initiates 4 Attackers (2 on each team)
	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
	{
		spawnLocation = RandomizeNPCSpawn(i < ATTACKERS_IN_TEAM ? &rooms[redTeamSpawnRoom] : &rooms[blueTeamSpawnRoom],
			i < ATTACKERS_IN_TEAM ? RED_TEAM : BLUE_TEAM);

		attackers[i].SetLocation(spawnLocation);
		attackers[i].SetTeam(i < ATTACKERS_IN_TEAM ? RED_TEAM : BLUE_TEAM);
		attackers[i].SetRoom(i < ATTACKERS_IN_TEAM ? redTeamSpawnRoom : blueTeamSpawnRoom);

		map[(int)spawnLocation.GetRow()][(int)spawnLocation.GetColumn()] = attackers[i].GetTeam();
	}

	// Initiates 2 couriers (1 on each team)
	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
	{
		spawnLocation = RandomizeNPCSpawn(i < COURIERS_IN_TEAM ? &rooms[redTeamSpawnRoom] : &rooms[blueTeamSpawnRoom],
			i < COURIERS_IN_TEAM ? RED_TEAM : BLUE_TEAM);

		couriers[i].SetLocation(spawnLocation);
		couriers[i].SetTeam(i < COURIERS_IN_TEAM ? RED_TEAM : BLUE_TEAM);
		couriers[i].SetRoom(i < COURIERS_IN_TEAM ? redTeamSpawnRoom : blueTeamSpawnRoom);

		map[(int)spawnLocation.GetRow()][(int)spawnLocation.GetColumn()] = couriers[i].GetTeam();
	}
}

/// <summary>
/// Initiates the map matrix.
/// </summary>
void InitMap()
{
	cout << "Initiating map...";

	// Fills the map with WALL (empty map)
	for (int row = 0; row < MAP_DIMENSION; row++)
		for (int column = 0; column < MAP_DIMENSION; column++)
			map[row][column] = WALL;

	// Initiates the rooms of the map
	InitRooms();

	// Initiates the security map
	for (int row = 0; row < MAP_DIMENSION; row++)
		for (int column = 0; column < MAP_DIMENSION; column++)
			securityMap[row][column] = map[row][column] == WALL ? MAX_ITERATIONS : 1;

	cout << "complete.\n";
}

/// <summary>
/// Fills the map matrix with corresponding colors.
/// </summary>
void DrawMap()
{
	double securityFactor;

	for (int row = 0; row < MAP_DIMENSION; row++)
		for (int column = 0; column < MAP_DIMENSION; column++)
		{
			securityFactor = (MAX_ITERATIONS - securityMap[row][column] - 1) / (double)MAX_ITERATIONS;

			switch (map[row][column])
			{
			case WALL:
				glColor3d(0, 0, 0);				// Black
				break;
			case SPACE:
				if (securityMapVisible)
					glColor3d(securityFactor, securityFactor, securityFactor);
				else
					glColor3d(0.3, 0.3, 0.3);	// Gray
				break;
			case RED_TEAM:
				glColor3d(1, 0, 0);				// Red
				break;
			case BLUE_TEAM:
				glColor3d(0, 0, 1);				// Blue
				break;
			case ARMS:
				glColor3d(1, 0.64, 0);			// Orange
				break;
			case MEDS:
				glColor3d(0, 1, 0);				// Green
				break;
			}

			// Draws the matrix cell as a square
			glBegin(GL_POLYGON);
			glVertex2d((double)column, (double)row);
			glVertex2d((double)column + 1, (double)row);
			glVertex2d((double)column + 1, (double)row + 1);
			glVertex2d((double)column, (double)row + 1);
			glEnd();
		}
}

/// <summary>
/// Clear all game assets and return to initial point
/// </summary>
void Restart()
{
	gameOver = false;
	securityMapVisible = false;

	supplies.clear();
	weapons.clear();
	greyNodes.clear();
	blackNodes.clear();

	InitMap();
	InitGame();

	restart = false;
}

/// <summary>
/// Moves an attacker on the map.
/// </summary>
/// <param name="attacker">The attacker to move</param>
/// <param name="newLocation">The attacker's new location</param>
void MoveAttacker(Attacker* attacker, Position newLocation)
{
	Position oldLocation = attacker->GetLocation();
	int oldRow = (int)oldLocation.GetRow(), oldColumn = (int)oldLocation.GetColumn();
	int newRow = (int)newLocation.GetRow(), newColumn = (int)newLocation.GetColumn();

	//Attacker* otherAttacker;
	// Validation (TODO: FIX)
	//if ((map[newRow][newColumn] == RED_TEAM) || (map[newRow][newColumn] == BLUE_TEAM))
	//{
	//	otherAttacker = FindAttackerByLocation(newLocation);
	//	if (otherAttacker == nullptr)
	//	{
	//		cout << "Error in 'MoveAttacker' function (line 956)\n";
	//		exit(1);
	//	}
	//	else
	//		attacker->SetSteppedOn(otherAttacker->GetSteppedOn());
	//}

	// Moves color-wise
	map[oldRow][oldColumn] = attacker->GetSteppedOn();
	if ((map[newRow][newColumn] != RED_TEAM) && (map[newRow][newColumn] != BLUE_TEAM))
		attacker->SetSteppedOn(map[newRow][newColumn]);
	map[newRow][newColumn] = attacker->GetTeam();

	// Moves location-wise
	attacker->SetLocation(newLocation);
	attacker->SetRoom(FindContainerRoom(attacker));
}

/// <summary>
/// Moves a courier on the map.
/// </summary>
/// <param name="courier">The courier to activate</param>
/// <param name="newLocation">The courier's new location</param>
void MoveCourier(Courier* courier, Position newLocation)
{
	Position oldLocation = courier->GetLocation();
	int oldRow = (int)oldLocation.GetRow(), oldColumn = (int)oldLocation.GetColumn();
	int newRow = (int)newLocation.GetRow(), newColumn = (int)newLocation.GetColumn();

	// Validation
	if ((map[newRow][newColumn] == RED_TEAM) || (map[newRow][newColumn] == BLUE_TEAM))
		return;

	// Collects supply (if there's one)
	if ((map[newRow][newColumn] == ARMS) || (map[newRow][newColumn] == MEDS))
	{
		courier->PickSupply(map[newRow][newColumn]);

		// Removes the supply from vector
		for (int i = 0; i < supplies.size(); i++)
			if ((supplies[i]->GetRow() == newRow) && (supplies[i]->GetColumn() == newColumn))
			{
				supplies.erase(supplies.begin() + i);
				break;
			}
	}

	// Moves color-wise
	map[oldRow][oldColumn] = SPACE;
	map[newRow][newColumn] = courier->GetTeam();

	// Moves location-wise
	courier->SetLocation(newLocation);
	courier->SetRoom(FindContainerRoom(courier));
}

/// <summary>
/// Activates an attacker by its state.
/// </summary>
/// <param name="attacker">The attacker to activate</param>
void IterateAttacker(Attacker* attacker)
{
	Courier* teamCourier = FindCourierByColor(attacker->GetTeam());
	NPC* nearestEnemy = FindNearestEnemy(*attacker);
	Weapon* shotWeapon = nullptr;

	// Validation
	if ((teamCourier == nullptr) || (nearestEnemy == nullptr))
		return;

	// Manages the attacker's states
	if (attacker->IsSearchingEnemy())
	{
		attacker->SetDestination(nearestEnemy->GetLocation());

		if ((attacker->IsInSameRoom(*nearestEnemy)) && (attacker->HasLineOfSight(*nearestEnemy, map)))
			attacker->GetActiveState()->Transform(attacker); // SearchEnemyState -> AttackState
	}
	else if (attacker->IsAttacking())
	{
		if (attacker->GetArms() > 0)
			weapons.push_back(attacker->Attack(nearestEnemy->GetLocation()));

		attacker->GetActiveState()->Transform(attacker); // AttackState -> SearchShelterState (if dying) / SearchEnemyState (if not dying)
	}
	else if (attacker->IsSearchingShelter())
	{
		attacker->SetDestination(FindNearestShelter(attacker));

		if (attacker->GetRoom() != nearestEnemy->GetRoom())
			attacker->GetActiveState()->Transform(attacker); // SearchShelterState -> SearchEnemyState
	}

	// Checks whether the team courier is called 
	if (attacker->GetArms() == 0)
		attacker->CallCourier(teamCourier, ARMS, TAKE);
	else if (attacker->IsAllyRequiresSupply(attackers, ARMS))
		attacker->CallCourier(teamCourier, ARMS, GIVE);

	if (attacker->GetMeds() == 0)
		attacker->CallCourier(teamCourier, MEDS, TAKE);
	else if (attacker->IsAllyRequiresSupply(attackers, MEDS))
		attacker->CallCourier(teamCourier, MEDS, GIVE);

	MoveAttacker(attacker, FindNextLocation(attacker));
}

/// <summary>
/// Activates a courier by its state.
/// </summary>
/// <param name="courier">The courier to activate</param>
void IterateCourier(Courier* courier)
{
	Attacker* nearestAlly = FindNearestAlly(*courier);

	// Validation
	if (nearestAlly == nullptr)
		return;

	// Manages the courier's states
	if (courier->IsSearchingSupply())
	{
		courier->SetDestination(FindNearestSupply(*courier));

		if (courier->IsCalled()) // If the courier is being called for action
		{
			if (courier->GetTransaction() == TAKE) // If an ally wants the courier to pass supplies to another ally
				courier->GetActiveState()->Transform(courier);
			else // If an ally needs a supply the courier has
			{
				if ((courier->GetSupply() == ARMS) && (courier->GetArms() > 0))
					courier->GetActiveState()->Transform(courier);
				else if ((courier->GetSupply() == MEDS) && (courier->GetMeds() > 0))
					courier->GetActiveState()->Transform(courier);
			}
		}
	}
	else if (courier->IsSearchingAlly())
	{
		courier->SetDestination(nearestAlly->GetLocation());

		if ((courier->GetLocation() == nearestAlly->GetLocation()) || (courier->GetLocation() | nearestAlly->GetLocation()))
			courier->GetActiveState()->Transform(courier); // SearchAllyState -> ShareSupplyState
	}
	else if (courier->IsSharingSupply())
	{
		courier->TradeSupply(nearestAlly);

		courier->GetActiveState()->Transform(courier); // ShareSupplyState -> SearchAllyState (if called) / SearchSupplyState (if not called)
	}

	MoveCourier(courier, FindNextLocation(courier));
}

/// <summary>
/// Iterates a team.
/// </summary>
/// <param name="teamColor">The team's color</param>
void IterateTeam(int teamColor)
{
	string currentTeam = teamColor == RED_TEAM ? "RED TEAM:\n" : "BLUE TEAM:\n", winningTeam;
	Position teammateLocation;
	Attacker** teamAttackers = FindAttackersByColor(teamColor);
	Attacker* currentAttacker = *teamAttackers;
	Courier* teamCourier = FindCourierByColor(teamColor);
	int deadTeammates = 0;

	// Validation
	if ((currentAttacker == nullptr) || (teamCourier == nullptr))
		return;

	// Iterates team members
	cout << currentTeam;
	for (int i = 0; i < ATTACKERS_IN_TEAM; i++, currentAttacker++)
	{
		if (currentAttacker->GetHealth() <= 0)
		{
			teammateLocation = currentAttacker->GetLocation();
			map[(int)teammateLocation.GetRow()][(int)teammateLocation.GetColumn()] = currentAttacker->GetSteppedOn();
			cout << "Attacker [" << (i + 1) << "] is dead\n";

			deadTeammates++;
		}
		else
		{
			cout << "Attacker [" << (i + 1) << "]: Hp:" << currentAttacker->GetHealth() << " Arms: " << currentAttacker->GetArms() << " Meds: " << currentAttacker->GetMeds();
			cout << " State: " << currentAttacker->GetStateName() << "\n";
			IterateAttacker(currentAttacker);
		}
	}
	if (teamCourier->GetHealth() <= 0)
	{
		teammateLocation = teamCourier->GetLocation();
		map[(int)teammateLocation.GetRow()][(int)teammateLocation.GetColumn()] = SPACE;
		cout << "Courier is dead\n";

		deadTeammates++;
	}
	else
	{
		cout << "Courier: Hp:" << teamCourier->GetHealth() << " Arms: " << teamCourier->GetArms() << " Meds: " << teamCourier->GetMeds();
		cout << " State: " << teamCourier->GetStateName() << "\n";
		IterateCourier(teamCourier);
	}

	// Checks whether the entire team is dead (and the game is over)
	if (deadTeammates == TEAM_SIZE)
	{
		gameOver = true;
		winningTeam = teamColor == RED_TEAM ? "Blue" : "Red";
		cout << "Game Over. " << winningTeam << " team wins.\n";
	}
}

// Default Methods

void CreateMenu(int choice)
{
	switch (choice)
	{
	case -1: exit(0); break;
	case 0: restart = true; break;
	case 1: securityMapVisible = !securityMapVisible; break;
	}
}
void init()
{
	srand(time(0));
	glClearColor(0.9, 0.9, 0.9, 1);						// color of window background
	glOrtho(0, MAP_DIMENSION, MAP_DIMENSION, 0, 0, 1);	// set the coordinates system

	Restart();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMap();

	for (int i = 0; i < weapons.size(); i++)
		weapons[i]->Draw();

	glutSwapBuffers(); // show all
}
void idle()
{
	Position weaponLocation;

	if (restart)
		Restart();
	else if (gameOver)
	{
		system("CLS");
		cout << "GAME OVER!";
		Sleep(3000);

		exit(0);
	}

	system("CLS"); // Clears the console

	// Iterates both teams
	if (!gameOver)
		IterateTeam(RED_TEAM);

	if (!gameOver)
		IterateTeam(BLUE_TEAM);

	Sleep(100);

	// Iterates spawned weaponry
	for (int i = 0; i < weapons.size(); i++)
	{
		weaponLocation = weapons[i]->GetLocation();

		weapons[i]->Move();
		securityMap[(int)weaponLocation.GetRow()][(int)weaponLocation.GetColumn()] += weapons[i]->IsCombustable() ?
			GRENADE_SECURITY_WEIGHT : BULLET_SECURITY_WEIGHT;

		CheckWeaponHit(weapons[i]);
	}

	glutPostRedisplay(); // indirect call to display
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Red VS Blue");
	glutDisplayFunc(display); // display is the refresh function
	glutIdleFunc(idle);

	// Creates menu to ask user for operation
	glutCreateMenu(CreateMenu);

	glutAddMenuEntry("Toggle Security Map", 1);
	glutAddMenuEntry("Restart", 0);
	glutAddMenuEntry("Exit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}