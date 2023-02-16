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
bool gameOver = false, restart = false, securityMapVisible = false, pathFound = false;
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
void CheckNeighbor(int row, int column, Node* currentNode)
{
	Position destination = currentNode->GetDestination();
	Node* neighborNode;

	if (currentNode->GetLocation() == destination)
	{
		pathFound = true;
		return;
	}

	neighborNode = new Node(new Position(row, column), &destination, currentNode, securityMap[row][column] + currentNode->GetG());

	if (!blackNodes.empty())
		for (vector<Node*>::iterator blackNodesIterator = blackNodes.begin(); blackNodesIterator != blackNodes.end(); blackNodesIterator++)
			if ((*blackNodesIterator)->GetLocation() == neighborNode->GetLocation()) // If the node is in the blacks already
				return; // We don't need it.

	if (!greyNodes.empty())
		for (vector<Node*>::iterator greyNodesIterator = greyNodes.begin(); greyNodesIterator != greyNodes.end(); greyNodesIterator++)
			if ((*greyNodesIterator)->GetLocation() == neighborNode->GetLocation()) // The node is in grays already
			{
				if ((*greyNodesIterator)->GetF() > neighborNode->GetF()) // If we found a better node
				{
					greyNodes.erase(greyNodesIterator); // Erase the previous one
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

	int counter = 0;

	blackNodes.clear();
	greyNodes.clear();
	greyNodes.push_back(currentNode);

	pathFound = false;
	while (!pathFound)
	{
		// Validation
		if (greyNodes.empty())
		{
			pathFound = false;
			/* DEBNUGGING PURPOSES: ENABLE LINES TO SEE WHEN A CHARACTER CANNOT REACH HIS DESTINATION */
			cout << "\nDesitnation:" << destination.GetRow() << "," << destination.GetColumn() << ":\n";
			cout << "Exit because grey nodes is empty when it shouldn't be. In function 'FindNextLocation'\n";
			Sleep(5000);
			return source;
		}

		counter++;
		currentNode = FindMinimalFNode();
		currentLocation = currentNode->GetLocation();
		nodeRow = (int)currentLocation.GetRow();
		nodeColumn = (int)currentLocation.GetColumn();

		if ((!pathFound) && (map[nodeRow - 1][nodeColumn] != WALL))
			CheckNeighbor(nodeRow - 1, nodeColumn, currentNode);
		if ((!pathFound) && (map[nodeRow + 1][nodeColumn] != WALL))
			CheckNeighbor(nodeRow + 1, nodeColumn, currentNode);
		if ((!pathFound) && (map[nodeRow][nodeColumn - 1] != WALL))
			CheckNeighbor(nodeRow, nodeColumn - 1, currentNode);
		if ((!pathFound) && (map[nodeRow][nodeColumn + 1] != WALL))
			CheckNeighbor(nodeRow, nodeColumn + 1, currentNode);

		if (counter >= MAX_ITERATIONS)
			break;
	}

	if (counter >= MAX_ITERATIONS)
	{
		cout << "Exit because too many iterations\n";
		Sleep(1500);
		return source;
	}

	// Restores the path
	while ((currentNode->GetParent() != nullptr) && (currentNode->GetParent()->GetLocation() != source))
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
/// Finds the enemy nearest to a Attacker.
/// </summary>
/// <param name="attacker">The attacker that searches an enemy</param>
/// <returns>The nearest enemy to the attacker</returns>
NPC* FindNearestEnemy(Attacker attacker)
{
	int enemyTeamColor = attacker.GetTeam() == RED_TEAM ? BLUE_TEAM : RED_TEAM;
	NPC* nearestEnemy = nullptr;
	double currentDistance, nearestDistance = (double)MAP_DIMENSION * MAP_DIMENSION;

	// Finds the enemy (attacker / courier) nearest to the attacker
	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
	{
		// Validation
		if ((attackers[i].GetTeam() == attacker.GetTeam()) || (attackers[i].GetHealth() <= 0))
			continue;

		currentDistance = ManhattanDistance(attackers[i].GetLocation(), attacker.GetLocation());
		if (currentDistance < nearestDistance)
		{
			nearestDistance = currentDistance;
			nearestEnemy = (NPC*)&attackers[i];
		}
	}

	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
	{
		// Validation
		if ((couriers[i].GetTeam() != attacker.GetTeam()) || (couriers[i].GetHealth() <= 0))
			continue;

		currentDistance = ManhattanDistance(couriers[i].GetLocation(), attacker.GetLocation());
		if (currentDistance < nearestDistance)
		{
			nearestDistance = currentDistance;
			nearestEnemy = (NPC*)&attackers[i];
		}
	}

	if (nearestEnemy == nullptr)
		cout << "ERROR IN FINDING AN ENEMY IN 'FindNearestEnemy'\n";

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
	Attacker* nearestAttacker = nullptr;

	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
	{
		// Validation
		if (attackers[i].GetHealth() <= 0)
			continue;

		currentDistance = ManhattanDistance(courier.GetLocation(), attackers[i].GetLocation());
		if (currentDistance < nearestDistance)
		{
			nearestAttacker = &attackers[i];
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

	return -1; // The NPC is in a tunnel
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

	// Validation
	if (supplies.size() == 0)
	{
		cout << "No supplies around to collect\n";
		return courier.GetLocation();
	}

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
	Room room = rooms[npc->GetRoom()];
	Position shelter;
	int shelterRow = (int)npc->GetLocation().GetRow(), shelterColumn = (int)npc->GetLocation().GetColumn();
	double minimalSecurityFactor = securityMap[shelterRow][shelterColumn];
	int roomRowStart = (int)fabs(room.GetCenterPosition().GetRow() - room.GetHeight() / 2);
	int roomRowEnd = (int)room.GetCenterPosition().GetRow() + room.GetHeight() / 2;
	int roomColumnStart = (int)fabs(room.GetCenterPosition().GetColumn() - room.GetWidth() / 2);
	int roomColumnEnd = (int)room.GetCenterPosition().GetColumn() + room.GetWidth() / 2;

	for (int row = roomRowStart; row < roomRowEnd; row++)
		for (int column = roomColumnStart; column < roomColumnEnd; column++)
		{
			if (securityMap[row][column] < minimalSecurityFactor)
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
/// Indicates whether a weaponry (bullet / grenade) is still active.
/// </summary>
/// <param name="weaponLocation">The weapon's location</param>
/// <returns>True if the weapon is still active, False otherwise</returns>
bool IsWeaponActive(Weapon* weapon)
{
	Position weaponLocation = weapon->GetLocation(), enemyLocation;

	// Checks if the weapon can no longer cause damage
	if (weapon->GetDamage() <= 0)
		return false;

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
	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
	{
		// Validation
		if ((attackers[i].GetTeam() == weapon->GetTeam()) || (attackers[i].GetHealth() <= 0))
			continue;

		enemyLocation = attackers[i].GetLocation();
		if (enemyLocation == EMPTY_POSITION)
		{
			cout << "Error in 'IsWeaponActive', line " <<__LINE__ <<"\n";
			return false;
		}

		if ((fabs(weaponLocation.GetRow() - enemyLocation.GetRow()) <= WEAPON_SIZE)
			&& (fabs(weaponLocation.GetColumn() - enemyLocation.GetColumn()) <= WEAPON_SIZE))
		{
			attackers[i].TakeDamage(weapon->GetDamage()); // Weapon damage calculated by distance

			return false; // Weapon hit and should dissappear.
		}
	}

	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
	{
		// Validation
		if ((couriers[i].GetTeam() == weapon->GetTeam()) || (couriers[i].GetHealth() <= 0))
			continue;

		enemyLocation = couriers[i].GetLocation();
		if (enemyLocation == EMPTY_POSITION)
		{
			cout << "Error in 'IsWeaponActive', line " << __LINE__ << "\n";
			return false;
		}

		if ((fabs(weaponLocation.GetRow() - enemyLocation.GetRow()) <= WEAPON_SIZE)
			&& (fabs(weaponLocation.GetColumn() - enemyLocation.GetColumn()) <= WEAPON_SIZE))
		{
			couriers[i].TakeDamage(weapon->GetDamage()); // Weapon damage calculated by distance

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
	double alpha, teta;

	if (!IsWeaponActive(weapon))
	{
		// Explodes a grenade and spawns its shards
		if (weapon->IsCombustable())
		{
			alpha = 0;
			teta = (360.0 / SHARDS_IN_GRENADE) * PI / 180;

			for (int i = 0; i < SHARDS_IN_GRENADE; i++, alpha += teta)
				weapons.push_back(new Weapon(weapon->GetLocation(), alpha, weapon->GetTeam(), false));
		}

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
/// <param name="spawnLocation">The spawn point to check</param>
/// <param name="teamColor">The NPC's team</param>
/// <returns>True if the NPC can spawn in the given location, False otherwise</returns>
bool IsValidNPCSpawn(Position spawnLocation, int teamColor)
{
	// Checks whether any teammate is already in the designated spawn location
	if (map[(int)spawnLocation.GetRow()][(int)spawnLocation.GetColumn()] == WALL)
		return false;
	if (map[(int)spawnLocation.GetRow()][(int)spawnLocation.GetColumn()] == ARMS)
		return false;
	if (map[(int)spawnLocation.GetRow()][(int)spawnLocation.GetColumn()] == MEDS)
		return false;

	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
		if (attackers[i].GetLocation() == spawnLocation)
			return false;

	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
		if (couriers[i].GetLocation() == spawnLocation)
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
	} while (!IsValidNPCSpawn(spawnLocation, teamColor));

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
	pathFound = false;

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
	Courier* teamCourier = nullptr;
	NPC* nearestEnemy = nullptr;
	Position nearestShelter;

	// Manages the attacker's states
	//if (attacker->GetLocation() == attacker->GetDestination())
	//{
	//	attacker->GetActiveState()->Transform(attacker);
	//	return;
	//}

	if (attacker->IsSearchingEnemy())
	{
		nearestEnemy = FindNearestEnemy(*attacker);
		if (nearestEnemy == nullptr)
		{
			cout << "Attacker couldn't find an enemy in 'Iterate IterateAttacker' in SearchingEnemy.\n";
			return;
		}
		attacker->SetDestination(nearestEnemy->GetLocation());

		if ((attacker->IsInSameRoom(*nearestEnemy)) && (attacker->HasLineOfSight(*nearestEnemy, map)))
			attacker->GetActiveState()->Transform(attacker); // SearchEnemyState -> AttackState
	}
	else if (attacker->IsAttacking())
	{
		nearestEnemy = FindNearestEnemy(*attacker);
		if (nearestEnemy == nullptr)
		{
			cout << "Attacker couldn't find an enemy in 'Iterate IterateAttacker' in Attacking\n";
			return;
		}
		if ((nearestEnemy->GetLocation().GetRow() < 0) || (nearestEnemy->GetLocation().GetRow() > MAP_DIMENSION))
			cout << "Ran you idiot its this line (1088)\n";
		if ((nearestEnemy->GetLocation().GetColumn() < 0) || (nearestEnemy->GetLocation().GetColumn() > MAP_DIMENSION))
			cout << "Ran you idiot its this line (1090)\n";

		if (attacker->GetArms() > 0)
			weapons.push_back(attacker->Attack(nearestEnemy->GetLocation()));

		attacker->GetActiveState()->Transform(attacker); // AttackState -> SearchShelterState
	}
	else if (attacker->IsSearchingShelter())
	{
		nearestEnemy = FindNearestEnemy(*attacker);
		if (nearestEnemy == nullptr)
		{
			cout << "Attacker couldn't find an enemy in 'Iterate IterateAttacker' in shelter-seeking\n";
			return;
		}
		if ((nearestEnemy->GetLocation().GetRow() < 0) || (nearestEnemy->GetLocation().GetRow() > MAP_DIMENSION))
			cout << "Ran you idiot its this line\n";
		if ((nearestEnemy->GetLocation().GetColumn() < 0) || (nearestEnemy->GetLocation().GetColumn() > MAP_DIMENSION))
			cout << "Ran you idiot its this line\n";
		nearestShelter = FindNearestShelter(attacker);

		attacker->SetDestination(nearestShelter);

		if (attacker->GetRoom() != nearestEnemy->GetRoom())
			attacker->GetActiveState()->Transform(attacker); // SearchShelterState -> SearchEnemyState
	}

	// Checks whether the team courier is called 
	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
		if (couriers[i].GetTeam() == attacker->GetTeam())
		{
			teamCourier = &couriers[i];
			break;
		}

	if (attacker->GetArms() == 0)
	{
		attacker->CallCourier(teamCourier, ARMS, TAKE);
		//cout << "Attacker has called courier to TAKE arms\n";
	}
	else if (attacker->IsAllyRequiresSupply(attackers, ARMS))
	{
		//cout << "Attacker has called courier to GIVE arms\n";
		attacker->CallCourier(teamCourier, ARMS, GIVE);
	}

	if (attacker->GetMeds() == 0)
	{
		//cout << "Attacker has called courier to TAKE meds\n";
		attacker->CallCourier(teamCourier, MEDS, TAKE);
	}
	else if (attacker->IsAllyRequiresSupply(attackers, MEDS))
	{
		//cout << "Attacker has called courier to GIVE meds\n";
		attacker->CallCourier(teamCourier, MEDS, GIVE);
	}

	MoveAttacker(attacker, FindNextLocation(attacker));
}

/// <summary>
/// Activates a courier by its state.
/// </summary>
/// <param name="courier">The courier to activate</param>
void IterateCourier(Courier* courier)
{
	Attacker* nearestAlly;
	Position nearestSupply;

	// Manages the courier's states
	if (courier->IsSearchingSupply())
	{
		nearestSupply = FindNearestSupply(*courier);
		courier->SetDestination(nearestSupply);

		if (courier->IsCalled()) // If the courier is being called for action
		{
			if (courier->GetTransaction() == TAKE) // If an ally wants the courier to pass supplies to another ally
			{
				//cout << "Courier has been called to TAKE supplies\n";
				courier->GetActiveState()->Transform(courier);
			}
			else // If an ally needs a supply the courier has
			{
				if ((courier->GetSupply() == ARMS) && (courier->GetArms() > 0))
				{
					//cout << "Courier has been called to GIVE Meds\n";
					courier->GetActiveState()->Transform(courier);
				}
				else if ((courier->GetSupply() == MEDS) && (courier->GetMeds() > 0))
				{
					//cout << "Courier has been called to GIVE MEDS\n";
					courier->GetActiveState()->Transform(courier);
				}
			}
		}
	}
	else if (courier->IsSearchingAlly())
	{
		nearestAlly = FindNearestAlly(*courier);
		courier->SetDestination(nearestAlly->GetLocation());

		if (courier->GetLocation() | nearestAlly->GetLocation())
			courier->GetActiveState()->Transform(courier); // SearchAllyState -> ShareSupplyState
	}
	else if (courier->IsSharingSupply())
	{
		nearestAlly = FindNearestAlly(*courier);
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
	int deadTeammates = 0;

	// Iterates team members
	cout << currentTeam;

	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
	{
		// Validation
		if (attackers[i].GetTeam() != teamColor)
			continue;

		if (attackers[i].GetHealth() <= 0)
		{
			teammateLocation = attackers[i].GetLocation();
			map[(int)teammateLocation.GetRow()][(int)teammateLocation.GetColumn()] = attackers[i].GetSteppedOn();
			cout << "Attacker [" << (i + 1) << "] is dead\n";

			deadTeammates++;
		}
		else
		{
			cout << "Attacker [" << (i + 1) << "]: Hp:" << attackers[i].GetHealth() << " Arms: " << attackers[i].GetArms() << " Meds: " << attackers[i].GetMeds();
			cout << " State: " << attackers[i].GetStateName() << "\n";
			IterateAttacker(&attackers[i]);
		}
	}

	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
	{
		// Validation
		if (couriers[i].GetTeam() != teamColor)
			continue;

		if (couriers[i].GetHealth() <= 0)
		{
			teammateLocation = couriers[i].GetLocation();
			map[(int)teammateLocation.GetRow()][(int)teammateLocation.GetColumn()] = SPACE;
			cout << "Courier is dead\n";

			deadTeammates++;
		}
		else
		{
			cout << "Courier: Hp:" << couriers[i].GetHealth() << " Arms: " << couriers[i].GetArms() << " Meds: " << couriers[i].GetMeds();
			cout << " State: " << couriers[i].GetStateName() << "\n";
			IterateCourier(&couriers[i]);
		}
	}

	// Checks whether the entire team is dead (and the game is over)
	if (deadTeammates == TEAM_SIZE)
	{
		gameOver = true;

		winningTeam = teamColor == RED_TEAM ? "Blue" : "Red";
		cout << "Game Over. " << winningTeam << " team wins.\n";
		Sleep(3000);

		exit(0);
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

	// Iterates both teams

	system("CLS"); // Clears the console
	if (!gameOver)
		IterateTeam(RED_TEAM);
	if (!gameOver)
		IterateTeam(BLUE_TEAM);

	Sleep(100);

	// Iterates spawned weaponry
	for (int i = 0; i < weapons.size(); i++)
	{
		weapons[i]->Move();

		weaponLocation = weapons[i]->GetLocation();
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