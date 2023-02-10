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
#include "Bullet.h"
#include "Courier.h"
#include "Grenade.h"
#include "NPC.h"
#include "Node.h"
#include "NodesComparator.h"
#include "Room.h"
#include "Settings.h"

using namespace std;

// Fields
bool gameOver = false, restart = false, securityMapVisible = false;
int map[MAP_DIMENSION][MAP_DIMENSION] = { 0 }, mapCopy[MAP_DIMENSION][MAP_DIMENSION] = { 0 };
double securityMap[MAP_DIMENSION][MAP_DIMENSION] = { 0 };

Room rooms[MAX_ROOMS];
Attacker attackers[2 * ATTACKERS_IN_TEAM];
Courier couriers[2 * COURIERS_IN_TEAM];

vector<Position*> supplies;
vector<Bullet*> bullets;
vector<Grenade*> grenades;
vector<Node*> nodes;

// Methods

/// <summary>
/// Calculates Manhattan distance between 2 locations.
/// </summary>
/// <param name="p1">The first location</param>
/// <param name="p2">The second location</param>
/// <returns>The Manhattan distance between the location</returns>
double ManhattanDistance(Position p1, Position p2)
{
	int row1 = p1.GetRow(), column1 = p1.GetColumn(), row2 = p2.GetRow(), column2 = p2.GetColumn();

	return fabs(row1 - row2) + fabs(column1 - column2);
}

/// <summary>
/// Finds the attackers by their team.
/// </summary>
/// <param name="teamColor">The attackers' team</param>
/// <returns>The team's attackers</returns>
Attacker** FindTeamAttackers(int teamColor)
{
	Attacker* teamAttackers[ATTACKERS_IN_TEAM];

	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
		if ((attackers[i].GetHealth() > 0) && (attackers[i].GetTeam() == teamColor))
			teamAttackers[i / ATTACKERS_IN_TEAM] = &attackers[i];

	return teamAttackers;
}

/// <summary>
/// Finds a courier by its team.
/// </summary>
/// <param name="teamColor">The courier's team</param>
/// <returns>The team's courier</returns>
Courier* FindTeamCourier(int teamColor)
{
	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
		if ((couriers[i].GetHealth() > 0) && (couriers[i].GetTeam() == teamColor))
			return &couriers[i];

	return nullptr;
}

/// <summary>
/// Finds a team by its color
/// </summary>
/// <param name="teamColor">The team's color</param>
/// <returns>The team</returns>
NPC** FindTeam(int teamColor)
{
	Attacker** teamAttackers = FindTeamAttackers(teamColor);
	Courier* teamCourier = FindTeamCourier(teamColor);
	NPC* team[TEAM_SIZE] = { teamAttackers[0], teamAttackers[1], teamCourier };

	return team;
}

/// <summary>
/// Finds the enemy nearest to a Attacker.
/// </summary>
/// <param name="attacker">The attacker that searches an enemy</param>
/// <returns>The nearest enemy to the attacker</returns>
NPC* FindNearestEnemy(Attacker attacker)
{
	int enemyTeamColor = attacker.GetTeam() == RED_TEAM ? BLUE_TEAM : RED_TEAM, nearestOffset = -1;
	double currentDistance, nearestDistance = (double)MAP_DIMENSION * MAP_DIMENSION;
	NPC** enemyTeam = FindTeam(enemyTeamColor);

	// Finds the enemy (attacker / courier) nearest to the attacker
	for (int i = 0; i < TEAM_SIZE; i++)
	{
		// Validation
		if (enemyTeam[i]->GetHealth() <= 0)
			continue;

		currentDistance = ManhattanDistance(attackers[i].GetLocation(), attacker.GetLocation());
		if (currentDistance < nearestDistance)
		{
			nearestOffset = i;
			nearestDistance = currentDistance;
		}
	}

	return enemyTeam[nearestOffset];
}

/// <summary>
/// Finds the teammate attacker nearest to a courier.
/// </summary>
/// <param name="courier">The courier that searches a teammate attacker</param>
/// <returns>The nearest Attacker of the courier's team</returns>
Attacker* FindNearestAlly(Courier courier)
{
	int nearestOffset = -1;
	double currentDistance, nearestDistance = (double)MAP_DIMENSION * MAP_DIMENSION;
	Attacker** teamAttackers = FindTeamAttackers(courier.GetTeam());

	for (int i = 0; i < ATTACKERS_IN_TEAM; i++)
	{
		// Validation
		if (attackers[i].GetHealth() <= 0)
			continue;

		currentDistance = ManhattanDistance(courier.GetLocation(), attackers[i].GetLocation());
		if (currentDistance < nearestDistance)
		{
			nearestOffset = i;
			nearestDistance = currentDistance;
		}
	}

	return teamAttackers[nearestOffset];
}

/// <summary>
/// Checks whether a bullet hit an enemy.
/// </summary>
/// <param name="bullet">The bullet to handle</param>
void CheckBulletHit(Bullet* bullet)
{
	NPC** enemyTeam = FindTeam(bullet->GetTeam() == RED_TEAM ? BLUE_TEAM : RED_TEAM);
	vector<Bullet*>::iterator bulletsIterator;

	// Checks if any enemy is hit by the bullet
	for (int i = 0; i < TEAM_SIZE; i++)
		if (enemyTeam[i]->GetLocation() == bullet->GetLocation())
		{
			enemyTeam[i]->TakeDamage(1);
			bulletsIterator = find(bullets.begin(), bullets.end(), bullet);
			if (bulletsIterator != bullets.end())
				bullets.erase(bullets.begin() + (bulletsIterator - bullets.begin()));
		}
}

/// <summary>
/// Explodes a grenade.
/// </summary>
/// <param name="grenade">The grenade to explode</param>
void ExplodeGrenade(Grenade* grenade)
{
	Bullet** grenadeShards = grenade->GetShards();
	vector<Grenade*>::iterator grenadesIterator = find(grenades.begin(), grenades.end(), grenade);

	// Explodes the grenade and turns to manage its shards
	for (int shardOffset = 0; shardOffset < SHARDS_IN_GRENADE; shardOffset++)
		bullets.push_back(grenadeShards[shardOffset]);

	// Removes the grenade from the grenades vector
	if (grenadesIterator != grenades.end())
		grenades.erase(grenades.begin() + (grenadesIterator - grenades.begin()));
}

/// <summary>
/// Checks whether a grenade hit an enemy.
/// </summary>
/// <param name="grenade">The grenade to handle</param>
void CheckGrenadeHit(Grenade* grenade)
{
	NPC** enemyTeam = FindTeam(grenade->GetTeam() == RED_TEAM ? BLUE_TEAM : RED_TEAM);

	// Checks if any enemy is hit by the grenade
	for (int i = 0; i < TEAM_SIZE; i++)
		if (enemyTeam[i]->GetLocation() | grenade->GetLocation())
		{
			enemyTeam[i]->TakeDamage(MAX_HEALTH); // Instant death
			ExplodeGrenade(grenade);
			break;
		}
}

/// <summary>
/// Finds the ID of the room which contains a given NPC.
/// </summary>
/// <param name="npc">The NPC to check</param>
/// <returns>The ID of the room the NPC is in, -1 otherwise (the NPC is inside a tunnel)</returns>
int FindRoom(NPC* npc)
{
	for (int i = 0; i < MAX_ROOMS; i++)
		if (rooms[i].IsNPCInside(npc))
			return i;

	return -1;
}

/// <summary>
/// Finds the supply nearest to a given courier.
/// </summary>
/// <param name="courier">The courier that searches for supplies</param>
/// <returns>The location of the nearest supply</returns>
Position FindNearestSupply(Courier courier)
{
	int nearestOffset = -1;
	double currentDistance, nearestDistance = (double)MAP_DIMENSION * MAP_DIMENSION;

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
/// Finds the Node with minimal F value (for A*).
/// </summary>
/// <returns>The Node with minimal F value</returns>
Node* FindMinimalFNode()
{
	Node* minimalNodePtr;
	int eraseOffset = 0;
	float minimalF = MAP_DIMENSION * MAP_DIMENSION;

	// Retrieves Node with minimal f value
	for (int i = 0; i < nodes.size(); i++)
		if (nodes[i]->GetF() < minimalF)
		{
			minimalF = nodes[i]->GetF();
			eraseOffset = i;
		}

	// Removes retrieved Node from vector
	minimalNodePtr = nodes[eraseOffset];
	nodes.erase(nodes.begin() + eraseOffset);

	return minimalNodePtr;
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
	{
		// Validation
		if ((attackers[i].GetTeam() != teamColor) || (attackers[i].GetHealth() <= 0))
			continue;

		if (attackers[i].GetLocation() == spawnPosition)
			return false;
	}

	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
	{
		// Validation
		if ((couriers[i].GetTeam() != teamColor) || (couriers[i].GetHealth() <= 0))
			continue;

		if (couriers[i].GetLocation() == spawnPosition)
			return false;
	}

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
	Position centerPosition = room->GetCenterPosition(), spawnLocation;
	int roomRowStart = (int)fabs(centerPosition.GetRow() - room->GetHeight() / 2);
	int roomColumnStart = (int)fabs(centerPosition.GetColumn() - room->GetWidth() / 2);

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
/// <param name="priorityQueue">The priority queue managing the nodes</param>
/// <param name="grays">The vector of in-process nodes</param>
/// <param name="blacks">The vectors of processed nodes</param>
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
		location = currentNode->GetSourcePosition();
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

		source = currentNode->GetSourcePosition();
		destination = currentNode->GetDestinationPosition();

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
	int height, width, row, column;

	// Fills room with obstacles
	for (int i = 0; i < obstaclesCount; i++)
	{
		height = orientation == 0 ? 1 + rand() % 2 : 0;
		width = orientation == 1 ? 1 + rand() % 2 : 0;

		do
		{
			row = (roomRowStart + 5) + rand() % (roomRowEnd - (height + 5));
			column = (roomColumnStart + 5) + rand() % (roomColumnEnd - (width + 5));
		} while (map[row][column] != SPACE);

		map[row][column] = WALL;

		if (height > 0)
			for (int h = 0; h < height; h++)
				map[row + h][column] = WALL;
		else if (width > 0)
			for (int w = 0; w < width; w++)
				map[row][column + w] = WALL;
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

	// Fills the room with ammo
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
			centerRow = 15 + rand() % (MAP_DIMENSION - 35);
			centerColumn = 15 + rand() % (MAP_DIMENSION - 35);
			height = 15 + rand() % 25;
			width = 15 + rand() % 25;
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
			securityMap[row][column] = map[row][column] == WALL ? (MAP_DIMENSION * MAP_DIMENSION) : 0;

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
			securityFactor = 1 - securityMap[row][column];

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

	supplies.clear();
	bullets.clear();
	grenades.clear();
	nodes.clear();

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

	// Moves color-wise
	map[oldRow][oldColumn] = attacker->GetSteppedOnSupply();
	attacker->SetSteppedOnSupply(map[newRow][newColumn]);
	map[newRow][newColumn] = attacker->GetTeam();

	// Moves location-wise
	attacker->SetLocation(newLocation);
	attacker->SetRoom(FindRoom(attacker));
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

	// Collects supply (if there's one)
	if ((map[newRow][newColumn] == ARMS) || (map[newRow][newColumn] == MEDS))
	{
		courier->PickSupply(map[newRow][newColumn]);

		// Removes the supply from vector
		for (int i = 0; i < (int)supplies.size(); i++)
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
	courier->SetRoom(FindRoom(courier));
}

/// <summary>
/// Finds the next location a given NPC should move to.
/// </summary>
/// <param name="npc">The NPC to find the next location of</param>
/// <returns>The NPC's next location</returns>
Position FindNextLocation(NPC* npc)
{
	Position source = npc->GetLocation(), destination = npc->GetDestination();

	// TODO: COMPLETE

	return source;
}

/// <summary>
/// Activates an attacker by its state.
/// </summary>
/// <param name="attacker">The attacker to activate</param>
void IterateAttacker(Attacker* attacker)
{
	Courier* teamCourier = FindTeamCourier(attacker->GetTeam());
	NPC* nearestEnemy = FindNearestEnemy(*attacker);
	Bullet* bullet = attacker->GetBullet();
	Grenade* grenade = attacker->GetGrenade();
	vector<Bullet*>::iterator bulletsIterator = find(bullets.begin(), bullets.end(), bullet);
	vector<Grenade*>::iterator grenadesIterator = find(grenades.begin(), grenades.end(), grenade);

	// Manages the attacker's states
	if (attacker->IsSearchingEnemy())
	{
		attacker->SetDestination(nearestEnemy->GetLocation());

		if ((attacker->GetRoom() == nearestEnemy->GetRoom()) && (attacker->HasLineOfSight(*nearestEnemy, map)))
			attacker->GetActiveState()->Transform(attacker); // SearchEnemyState -> AttackState
	}
	else if (attacker->IsAttacking())
	{
		attacker->Attack(attacker->GetDestination(), map, securityMap);
		attacker->GetActiveState()->Transform(attacker); // AttackState -> SearchShelterState
	}
	else if (attacker->IsSearchingShelter())
	{
		attacker->SetDestination(teamCourier->GetLocation());
		if (attacker->GetRoom() == teamCourier->GetRoom())
			attacker->GetActiveState()->Transform(attacker); // SearchShelterState -> SearchEnemyState
	}

	// Manages the attacker's weaponry (if there is any)
	if (bulletsIterator == bullets.end())
		bullets.push_back(bullet);
	if (grenadesIterator == grenades.end())
		grenades.push_back(grenade);

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

	// Manages the courier's states
	if (courier->IsSearchingSupply())
	{
		courier->SetDestination(FindNearestSupply(*courier));
		if (courier->IsCalled())
			courier->GetActiveState()->Transform(courier); // SearchSupplyState -> SearchAllyState
	}
	else if (courier->IsSearchingAlly())
	{
		courier->SetDestination(nearestAlly->GetLocation());
		if (courier->GetLocation() | nearestAlly->GetLocation())
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
	string winningTeam;
	int deadTeammates = 0;

	// Iterates team members
	for (int i = 0; i < 2 * ATTACKERS_IN_TEAM; i++)
	{
		// Validation
		if (attackers[i].GetTeam() != teamColor)
			continue;

		if (attackers[i].GetHealth() == 0)
			deadTeammates++;
		else
			IterateAttacker(&attackers[i]);
	}

	for (int i = 0; i < 2 * COURIERS_IN_TEAM; i++)
	{
		// Validation
		if (couriers[i].GetTeam() != teamColor)
			continue;

		if (couriers[i].GetHealth() == 0)
			deadTeammates++;
		else
			IterateCourier(&couriers[i]);
	}

	// Checks whether the entire team is dead (and the game is over)
	if (deadTeammates == TEAM_SIZE)
	{
		gameOver = true;
		winningTeam = teamColor == RED_TEAM ? "Blue" : "Red";
		cout << "Game Over. " << winningTeam << " team wins.\n";
	}
}

/// <summary>
/// Iterates the game.
/// </summary>
void IterateGame()
{
	//if (!gameOver)
	//	IterateTeam(RED_TEAM);
	//if (!gameOver)
	//	IterateTeam(BLUE_TEAM);

	Sleep(100);
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

	for (int i = 0; i < bullets.size(); i++)
	{
		if (!bullets[i]->Move(map, securityMap))
			bullets.erase(bullets.begin() + i);
		else
		{
			bullets[i]->Draw();
			CheckBulletHit(bullets[i]);
		}
	}

	for (int i = 0; i < grenades.size(); i++)
	{
		if (!grenades[i]->Move(map, securityMap))
			ExplodeGrenade(grenades[i]);
		else
		{
			grenades[i]->Draw();
			CheckGrenadeHit(grenades[i]);
		}
	}

	glutSwapBuffers(); // show all
}
void idle()
{
	if (restart)
		Restart();
	else if (gameOver)
		return;
	else
		IterateGame();

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