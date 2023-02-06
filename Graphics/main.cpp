// Includes
#include <time.h>
#include <math.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <queue>
#include "glut.h"
#include "Node.h"
#include "Room.h"
#include "NodesComparator.h"
#include "NPC.h"
#include "Shooter.h"
#include "Courier.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Settings.h"

using namespace std;

// Fields
bool gameOver = false, restart = false, foundRoad = false;
int map[MAP_DIMENSION][MAP_DIMENSION] = { 0 }, mapCopy[MAP_DIMENSION][MAP_DIMENSION] = { 0 };
double securityMap[MAP_DIMENSION][MAP_DIMENSION] = { 0 };
Room rooms[MAX_ROOMS];
Shooter* shooters[2 * MAX_SHOOTERS_IN_TEAM];
Courier* couriers[2 * MAX_COURIERS_IN_TEAM];
vector<Position*> supplies;
vector<Bullet*> bullets;
vector<Grenade*> grenades;
vector<Node*> visitedNodes; // vector to be used by both teams

// Methods

/// <summary>
/// Calculates Manhattan distance between 2 coordinates.
/// </summary>
/// <param name="row1">The row offset of coordinate 1</param>
/// <param name="column1">The column offset of coordinate 1</param>
/// <param name="row2">The row offset of coordinate 2</param>
/// <param name="column2">The column offset of coordinate 2</param>
/// <returns>The Manhattan distance between the coordinates</returns>
double ManhattanDistance(int row1, int column1, int row2, int column2)
{
	return fabs(row1 - row2) + fabs(column1 - column2);
}

/// <summary>
/// Calculates Manhattan distance between 2 positions.
/// </summary>
/// <param name="p1">The first position</param>
/// <param name="p2">The second position</param>
/// <returns>The Manhattan distance between the position</returns>
double ManhattanDistance(Position p1, Position p2)
{
	return ManhattanDistance(p1.GetRow(), p1.GetColumn(), p2.GetRow(), p2.GetColumn());
}

/// <summary>
/// Finds the shooter nearest to a courier.
/// </summary>
/// <param name="courier">The courier that searches a teammate shooter</param>
/// <returns>The nearest shooter of the courier's team</returns>
Position FindNearestShooter(Courier* courier)
{
	int nearestOffset = 2;
	double currentDistance, nearestDistance = (double)MAP_DIMENSION * MAP_DIMENSION;

	for (int i = 0; i < MAX_SHOOTERS_IN_TEAM; i++)
	{
		// Validation
		if (shooters[i] == nullptr)
			continue;

		currentDistance = ManhattanDistance(courier->GetPosition(), shooters[i]->GetPosition());
		if (currentDistance < nearestDistance)
		{
			nearestOffset = i;
			nearestDistance = currentDistance;
		}
	}

	return shooters[nearestOffset]->GetPosition();
}

/// <summary>
/// Finds the enemy nearest to a shooter.
/// </summary>
/// <param name="shooter">The shooter that searches an enemy</param>
/// <returns>The nearest enemy to the shooter</returns>
Position FindNearestEnemy(Shooter* shooter)
{
	int nearestOffset = 2;
	double currentDistance, nearestDistance = (double)MAP_DIMENSION * MAP_DIMENSION;

	for (int i = 0; i < 2 * MAX_SHOOTERS_IN_TEAM; i++)
	{
		// Validation
		if ((shooters[i] == nullptr) || (shooters[i]->GetTeam() == shooter->GetTeam()))
			continue;

		currentDistance = ManhattanDistance(shooter->GetPosition(), shooters[i]->GetPosition());
		if (currentDistance < nearestDistance)
		{
			nearestOffset = i;
			nearestDistance = currentDistance;
		}
	}

	return shooters[nearestOffset]->GetPosition();
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
	for (int i = 0; i < visitedNodes.size(); i++)
		if (visitedNodes[i]->GetF() < minimalF)
		{
			minimalF = visitedNodes[i]->GetF();
			eraseOffset = i;
		}

	// Removes retrieved Node from vector
	minimalNodePtr = visitedNodes[eraseOffset];
	visitedNodes.erase(visitedNodes.begin() + eraseOffset);

	return minimalNodePtr;
}

Position* CheckNeighborAStar(Position* source, Position* destination, Node* currentNode)
{
	int sourceRow, sourceColumn, destinationRow, destinationColumn;
	Position position;

	if (source == destination)
	{
		foundRoad = true;

		if (currentNode->GetParent() == nullptr)
		{
			position = currentNode->GetSourcePosition();

			return &position;
		}

		while (currentNode->GetParent()->GetParent() != nullptr)
			currentNode = currentNode->GetParent();

		position = currentNode->GetSourcePosition();

		return &position;
	}

	destinationRow = currentNode->GetSourcePosition().GetRow();
	destinationColumn = currentNode->GetSourcePosition().GetColumn();

	visitedNodes.push_back(new Node(source, destination, currentNode, currentNode->GetG() + securityMap[destinationRow][destinationColumn]));

	return nullptr;
}

/// <summary>
/// Retrieves the next position an NPC needs to step into.
/// </summary>
/// <param name="npc">The npc</param>
/// <returns>The NPC's next position</returns>
Position* GetNPCNextStep(NPC* npc)
{
	Position sourcePosition = npc->GetPosition(), destinationPosition = npc->GetDestination();
	Position* stepPtr = nullptr;

	Node* currentNode = new Node(&sourcePosition, &destinationPosition, nullptr, 0);
	double defaultValue = (double)MAP_DIMENSION * MAP_DIMENSION;

	int row, column;

	foundRoad = false;
	visitedNodes.clear();
	visitedNodes.push_back(currentNode);

	while (!foundRoad)
	{
		if (visitedNodes.empty())
		{
			foundRoad = true; // to stop iterating
			cout << "CRITICAL ERROR: NPC couldn't find ANY road to destination???\n";

			return nullptr;
		}
		else
		{
			currentNode = FindMinimalFNode();
			sourcePosition = currentNode->GetSourcePosition();
			destinationPosition = currentNode->GetDestinationPosition();
			row = (int)sourcePosition.GetRow();
			column = (int)sourcePosition.GetColumn();


			if ((!foundRoad) && (securityMap[row - 1][column] < defaultValue))
			{
				stepPtr = CheckNeighborAStar(new Position((double)row - 1, column), &destinationPosition, currentNode);
				if (stepPtr != nullptr)
					return stepPtr;
			}

			if ((!foundRoad) && (securityMap[row + 1][column] < defaultValue))
			{
				stepPtr = CheckNeighborAStar(new Position((double)row + 1, column), &destinationPosition, currentNode);
				if (stepPtr != nullptr)
					return stepPtr;
			}

			if ((!foundRoad) && (securityMap[row][column - 1] < defaultValue))
			{
				stepPtr = CheckNeighborAStar(new Position(row, (double)column - 1), &destinationPosition, currentNode);
				if (stepPtr != nullptr)
					return stepPtr;
			}

			if ((!foundRoad) && (securityMap[row][column + 1] < defaultValue))
			{
				stepPtr = CheckNeighborAStar(new Position(row, (double)column + 1), &destinationPosition, currentNode);
				if (stepPtr != nullptr)
					return stepPtr;
			}
		}
	}

	return stepPtr;
}

// Map

/// <summary>
/// Indicates whether an NPC can spawn in a given position.
/// </summary>
/// <param name="spawnPosition">The spawn point to check</param>
/// <param name="teamColor">The NPC's team</param>
/// <returns>True if the NPC can spawn in the given position, False otherwise</returns>
bool IsValidNPCSpawn(Position spawnPosition, int teamColor)
{
	Position npcPosition;

	// Checks whether any teammate is already in the designated spawn position
	for (int i = 0; i < 2 * MAX_SHOOTERS_IN_TEAM; i++)
		if ((shooters[i] != nullptr) && (shooters[i]->GetTeam() == teamColor))
		{
			npcPosition = shooters[i]->GetPosition();
			if (spawnPosition == npcPosition)
				return false;
		}

	for (int i = 0; i < 2 * MAX_COURIERS_IN_TEAM; i++)
		if ((couriers[i] != nullptr) && (couriers[i]->GetTeam() == teamColor))
		{
			npcPosition = couriers[i]->GetPosition();
			if (spawnPosition == npcPosition)
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
	Position spawnPosition;
	int roomRowStart = fabs(room->GetCenterPosition().GetRow() - room->GetHeight() / 2);
	int roomColumnStart = fabs(room->GetCenterPosition().GetColumn() - room->GetWidth() / 2);

	do
	{
		spawnPosition.SetRow((double)roomRowStart + rand() % room->GetHeight());
		spawnPosition.SetColumn((double)roomColumnStart + rand() % room->GetWidth());
	} while ((map[(int)spawnPosition.GetRow()][(int)spawnPosition.GetColumn()] == WALL)
		|| (!IsValidNPCSpawn(spawnPosition, teamColor)));

	return spawnPosition;
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
/// <param name="source">The neighbor's source position</param>
/// <param name="destination">The neighbor's destination position</param>
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
/// <param name="currentNodePtr">Pointer to the last Node in the path</param>
void RestoreTunnelPath(Node* currentNodePtr)
{
	Position position;

	while (currentNodePtr->GetParent() != nullptr)
	{
		position = currentNodePtr->GetSourcePosition();
		map[(int)position.GetRow()][(int)position.GetColumn()] = SPACE;

		currentNodePtr = currentNodePtr->GetParent();
	}
}

/// <summary>
/// Digs a tunnel between 2 existing rooms.
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
	int roomRowStart = fabs(room->GetCenterPosition().GetRow() - room->GetHeight() / 2);
	int roomRowEnd = room->GetCenterPosition().GetRow() + room->GetHeight() / 2;
	int roomColumnStart = fabs(room->GetCenterPosition().GetColumn() - room->GetWidth() / 2);
	int roomColumnEnd = room->GetCenterPosition().GetColumn() + room->GetWidth() / 2;
	int obstaclesCount = 2 + rand() % (MAX_OBSTACLES_IN_ROOM - 2), orientation = rand() % 2;
	int height, width, row, column;

	// Fills room with obstacles
	for (int i = 0; i < obstaclesCount; i++)
	{
		height = orientation == 0 ? 1 + rand() % 3 : 0;
		width = orientation == 1 ? 1 + rand() % 3 : 0;

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
/// Places ammo and meds in a given room.
/// </summary>
/// <param name="room">The room to place supplies in</param>
void PlaceSupplies(Room* room)
{
	int roomRowStart = fabs(room->GetCenterPosition().GetRow() - room->GetHeight() / 2);
	int roomRowEnd = room->GetCenterPosition().GetRow() + room->GetHeight() / 2;
	int roomColumnStart = fabs(room->GetCenterPosition().GetColumn() - room->GetWidth() / 2);
	int roomColumnEnd = room->GetCenterPosition().GetColumn() + room->GetWidth() / 2;
	int ammosCount = rand() % MAX_ARMS, medsCount = rand() % MAX_MEDS_IN_ROOM;
	int row, column;

	// Fills Room with ammo
	for (int i = 0; i < ammosCount; i++)
	{
		do
		{
			row = roomRowStart + rand() % (roomRowEnd + 1);
			column = roomColumnStart + rand() % (roomColumnEnd + 1);
		} while (map[row][column] != SPACE);

		map[row][column] = ARMS;
		supplies.push_back(new Position(row, column));
	}

	// Fills Room with meds
	for (int i = 0; i < medsCount; i++)
	{
		do
		{
			row = roomRowStart + rand() % (roomRowEnd + 1);
			column = roomColumnStart + rand() % (roomColumnEnd + 1);
		} while (map[row][column] != SPACE);

		map[row][column] = MEDS;
	}
}

/// <summary>
/// Places a given room on the map matrix.
/// </summary>
/// <param name="room">The room to place on the map</param>
void PlaceRoom(Room* room)
{
	int roomRowStart = fabs(room->GetCenterPosition().GetRow() - room->GetHeight() / 2);
	int roomRowEnd = room->GetCenterPosition().GetRow() + room->GetHeight() / 2;
	int roomColumnStart = fabs(room->GetCenterPosition().GetColumn() - room->GetWidth() / 2);
	int roomColumnEnd = room->GetCenterPosition().GetColumn() + room->GetWidth() / 2;

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
	Position spawnPosition;
	int redTeamSpawnRoom, blueTeamSpawnRoom;

	// Chooses rooms to spawn both teams
	do
	{
		redTeamSpawnRoom = rand() % MAX_ROOMS;
		blueTeamSpawnRoom = rand() % MAX_ROOMS;
	} while (redTeamSpawnRoom == blueTeamSpawnRoom);

	// Initiates 4 shooters and 2 couriers (2 +  on each team
	for (int i = 0; i < MAX_SHOOTERS_IN_TEAM * 2; i++)
	{
		spawnPosition = RandomizeNPCSpawn(i < MAX_SHOOTERS_IN_TEAM ? &rooms[redTeamSpawnRoom] : &rooms[blueTeamSpawnRoom],
			i < MAX_SHOOTERS_IN_TEAM ? RED_TEAM : BLUE_TEAM);
		shooters[i] = new Shooter(spawnPosition, i < MAX_SHOOTERS_IN_TEAM ? RED_TEAM : BLUE_TEAM,
			i < MAX_SHOOTERS_IN_TEAM ? redTeamSpawnRoom : blueTeamSpawnRoom);

		map[(int)spawnPosition.GetRow()][(int)spawnPosition.GetColumn()] = shooters[i]->GetTeam();
	}

	for (int i = 0; i < MAX_COURIERS_IN_TEAM * 2; i++)
	{
		spawnPosition = RandomizeNPCSpawn(i < MAX_COURIERS_IN_TEAM ? &rooms[redTeamSpawnRoom] : &rooms[blueTeamSpawnRoom],
			i < MAX_COURIERS_IN_TEAM ? RED_TEAM : BLUE_TEAM);
		couriers[i] = new Courier(spawnPosition, i < MAX_COURIERS_IN_TEAM ? RED_TEAM : BLUE_TEAM,
			i < MAX_COURIERS_IN_TEAM ? redTeamSpawnRoom : blueTeamSpawnRoom);

		map[(int)spawnPosition.GetRow()][(int)spawnPosition.GetColumn()] = couriers[i]->GetTeam();
	}
}

/// <summary>
/// Initiates the security map.
/// </summary>
void InitSecurityMap()
{
	for (int row = 0; row < MAP_DIMENSION; row++)
		for (int column = 0; column < MAP_DIMENSION; column++)
			securityMap[row][column] = map[row][column] == WALL ? (MAP_DIMENSION * MAP_DIMENSION) : 0;
}

/// <summary>
/// Initiates the map matrix.
/// </summary>
void InitMap()
{
	cout << "Initiating map...";

	// Fills the map with Wall (empty map)
	for (int row = 0; row < MAP_DIMENSION; row++)
		for (int column = 0; column < MAP_DIMENSION; column++)
			map[row][column] = WALL;

	// Initiates the rooms of the map
	InitRooms();

	// Initiates the security map
	InitSecurityMap();

	cout << "complete.\n";
}

/// <summary>
/// Fills the map matrix with corresponding colors.
/// </summary>
void DrawMap()
{
	for (int row = 0; row < MAP_DIMENSION; row++)
		for (int column = 0; column < MAP_DIMENSION; column++)
		{
			switch (map[row][column])
			{
			case WALL:
				glColor3d(0, 0, 0);			// Black
				break;
			case SPACE:
				glColor3d(0.3, 0.3, 0.3);	// Gray
				break;
			case RED_TEAM:
				glColor3d(1, 0, 0);			// Red
				break;
			case BLUE_TEAM:
				glColor3d(0, 0, 1);			// Blue
				break;
			case ARMS:
				glColor3d(1, 0.64, 0);		// Orange
				break;
			case MEDS:
				glColor3d(0, 1, 0);			// Green
				break;
			}

			// draw square
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

	visitedNodes.clear();

	InitMap();
	InitGame();

	restart = false;
}

/// <summary>
/// Grants a courier a supply.
/// </summary>
/// <param name="courier">The courier that takes the supply</param>
/// <param name="supplyRow">The supply's row offset</param>
/// <param name="supplyColumn">The supply's column offset</param>
void TakeSupply(Courier* courier, int supplyRow, int supplyColumn)
{
	Position* supplyPosition = new Position(supplyRow, supplyColumn);
	int eraseOffset = -1;

	// Takes the supply
	if (map[supplyRow][supplyColumn] == ARMS)
		courier->TakeArms();
	else if (map[supplyRow][supplyColumn] == MEDS)
		courier->TakeMeds();

	// Removes the supply from vector
	for (int i = 0; i < supplies.size(); i++)
		if (supplies[i] == supplyPosition)
		{
			eraseOffset = i;
			break;
		}

	supplies.erase(supplies.begin() + eraseOffset);
}

/// <summary>
/// Activates a shooter.
/// </summary>
/// <param name="shooter">The shooter to activate</param>
/// <param name="newPosition">The shooter's new position</param>
void MoveShooter(Shooter* shooter, Position newPosition)
{
	Position oldPosition = shooter->GetPosition();
	int oldRow = (int)oldPosition.GetRow(), oldColumn = (int)oldPosition.GetColumn();
	int newRow = (int)newPosition.GetRow(), newColumn = (int)newPosition.GetColumn();

	// Moves color-wise
	map[oldRow][oldColumn] = shooter->GetPreviousCellContent(); // The cell shooter leaves returns to its original color
	shooter->SetPreviousCellContent(map[newRow][newColumn]); // shooter remembers the color of the new cell
	map[newRow][newColumn] = shooter->GetTeam(); // shooter occupies palastine

	// Moves position-wise
	shooter->SetPosition(newPosition);
}

/// <summary>
/// Activates a courier.
/// </summary>
/// <param name="courier">The courier to activate</param>
/// <param name="newPosition">The courier's new position</param>
void MoveCourier(Courier* courier, Position newPosition)
{
	Position oldPosition = courier->GetPosition();
	int oldRow = (int)oldPosition.GetRow(), oldColumn = (int)oldPosition.GetColumn();
	int newRow = (int)newPosition.GetRow(), newColumn = (int)newPosition.GetColumn();

	// Collects supplies
	if ((map[newRow][newColumn] == ARMS) || (map[newRow][newColumn] == MEDS))
		TakeSupply(courier, newRow, newColumn);

	// Moves color-wise
	map[oldRow][oldColumn] = SPACE; // the space courier leaves has to be a space.
	map[newRow][newColumn] = courier->GetTeam();

	// Moves position-wise
	courier->SetPosition(newPosition);
}

/// <summary>
/// Finds the ID of the room which contains a given NPC.
/// </summary>
/// <param name="npc">The NPC to check</param>
/// <returns>The ID of the room the NPC is in, -1 otherwise (the NPC is inside a tunnel)</returns>
int FindNPCContainer(NPC* npc)
{
	for (int i = 0; i < MAX_ROOMS; i++)
		if (rooms[i].IsNPCInside(npc))
			return i;

	return -1;
}

/// <summary>
/// Indicates whether there's a line of sight between two positions.
/// </summary>
/// <param name="p1">The first position</param>
/// <param name="p2">The second position</param>
/// <returns>True if there's a line of sight between the two positions, False otherwise</returns>
bool HasLineOfSight(Position p1, Position p2)
{
	int rowDiff = p2.GetRow() - p1.GetRow();
	int colDiff = p2.GetColumn() - p1.GetColumn();
	double length = sqrt(rowDiff * rowDiff + colDiff * colDiff);
	double dRow = rowDiff / length, dColumn = colDiff / length;

	for (double row = p1.GetRow(), column = p1.GetColumn(); ((int)row != p2.GetRow()) && ((int)column != p2.GetColumn());
		row += dRow, column += dColumn)
		if (map[(int)round(row)][(int)round(column)] == WALL)
			return false;

	return true;
}

/// <summary>
/// Activates a shooter by its state.
/// </summary>
/// <param name="shooter">The shooter to activate</param>
void ActivateShooter(Shooter* shooter)
{
	int row = (int)shooter->GetPosition().GetRow();
	int column = (int)shooter->GetPosition().GetColumn();
	Position attackPosition, enemyPosition;
	Position* newPosition = GetNPCNextStep((NPC*)shooter);

	if (shooter->IsSearchingCourier())
	{
		for (int i = 0; i < 2 * MAX_COURIERS_IN_TEAM; i++)
			if ((couriers[i] != nullptr) && (couriers[i]->GetTeam() == shooter->GetTeam()))
			{
				shooter->SetDestination(couriers[i]->GetPosition());
				break;
			}
	}
	else if (shooter->IsAttacking())
	{
		for (int i = 0; i < 2 * MAX_SHOOTERS_IN_TEAM; i++)
		{
			if ((shooters[i] != nullptr) && (shooters[i]->GetTeam() != shooter->GetTeam()))
			{
				attackPosition = shooters[i]->GetPosition();

				// Attacks the enemy if there's a line of sight
				if (HasLineOfSight(shooter->GetPosition(), attackPosition))
					shooter->Attack(attackPosition);
				else
					shooter->GetState()->Transform(shooter);
			}
		}

		for (int i = 0; i < 2 * MAX_COURIERS_IN_TEAM; i++)
		{
			if ((couriers[i] != nullptr) && (couriers[i]->GetTeam() != shooter->GetTeam()))
			{
				attackPosition = couriers[i]->GetPosition();

				// Attacks the enemy if there's a line of sight
				if (HasLineOfSight(shooter->GetPosition(), attackPosition))
					shooter->Attack(attackPosition);
				else
					shooter->GetState()->Transform(shooter);
			}
		}
	}
	else if (shooter->IsSearchingEnemy())
	{
		enemyPosition = FindNearestEnemy(shooter);
		if (HasLineOfSight(shooter->GetPosition(), enemyPosition))
			shooter->GetState()->Transform(shooter);
		else
			shooter->SetDestination(enemyPosition);
	}
	else
	{
		cout << "SEVERE ERROR OCCURRED\nA shooter has no state. in function 'ActivateShooter'\n";
	}

	MoveShooter(shooter, *newPosition);

	shooter->SetRoom(FindNPCContainer(shooter));
}

/// <summary>
/// Activates a courier by its state.
/// </summary>
/// <param name="courier">The courier to activate</param>
void ActivateCourier(Courier* courier)
{
	int row = (int)courier->GetPosition().GetRow();
	int column = (int)courier->GetPosition().GetColumn();
	Position* newPosition = GetNPCNextStep((NPC*)courier);

	MoveCourier(courier, *newPosition);

	courier->SetRoom(FindNPCContainer(courier));
}

/// <summary>
/// Iterates a team.
/// </summary>
/// <param name="teamColor">The team's color</param>
void IterateTeam(int teamColor)
{
	int row, column;

	// Checks whether any shooter has died
	for (int i = 0; i < 2 * MAX_SHOOTERS_IN_TEAM; i++)
	{
		if (shooters[i] == nullptr)
			continue;

		if (shooters[i]->GetTeam() == teamColor)
		{
			if (shooters[i]->GetHealth() <= 0)
			{
				shooters[i]->~Shooter();
				shooters[i] = nullptr;
			}
			//else
			//	ActivateShooter(shooters[i]);
		}
	}

	// Checks whether any of the couriers has died
	for (int i = 0; i < 2 * MAX_COURIERS_IN_TEAM; i++)
	{
		if (couriers[i] == nullptr)
			continue;

		if (couriers[i]->GetTeam() == teamColor)
		{
			if (couriers[i]->GetHealth() <= 0)
			{
				couriers[i]->~Courier();
				couriers[i] = nullptr;
			}
			//else
			//	ActivateCourier(couriers[i]);
		}
	}
}

/// <summary>
/// Iterates the game.
/// </summary>
void IterateGame()
{
	if (!gameOver)
	{
		IterateTeam(RED_TEAM);
		IterateTeam(BLUE_TEAM);
	}

	Sleep(100);
}

// Default Methods

void CreateMenu(int choice)
{
	switch (choice)
	{
	case -1: exit(0); break;
	case 0: restart = true; break;
	case 1:
		//createSecuriteMap();
		break;
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

	glBegin(GL_POLYGON);
	glEnd();

	glutSwapBuffers(); // show all
}
void idle()
{
	if (restart)
		Restart();

	else if (gameOver)
	{
		return;
	}
	else
		IterateGame();

	glutPostRedisplay(); // indirect call to display
}

void mouse(int button, int state, int x, int y)
{
	//if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	//{
	//	pb = new Bullet(x * MAPDIMENSION / (double)WINDOWWIDTH, y * MAPDIMENSION / (double)WINDOWHEIGHT, (rand() % 360) * PI / 180.0);
	//}
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
	glutMouseFunc(mouse);

	// Create menu to ask user which algorithm they'd like to activate
	glutCreateMenu(CreateMenu);

	glutAddMenuEntry("Show Security Map", 1);
	glutAddMenuEntry("Restart", 0);
	glutAddMenuEntry("Exit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}