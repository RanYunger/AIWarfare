#pragma once

// Includes
#include "Position.h"

// Constants
const int GIVE = 0, TAKE = 1;
const double PI = 3.1415;
const Position EMPTY_POSITION;

// Window related
const int WINDOW_HEIGHT = 50, WINDOW_WIDTH = 400;

// Map Related
const int MAP_DIMENSION = 100;
const int WALL = 1, SPACE = 2;
const int SPACE_WEIGHT = 1, WALL_WEIGHT = 5;
const int MAX_ITERATIONS = MAP_DIMENSION * MAP_DIMENSION;

// Room Related
const int EDGE_OFFSET = 15;
const int MAX_ROOMS = 3, MAX_OBSTACLES_IN_ROOM = 5, MAX_ARMS_IN_ROOM = 7, MAX_MEDS_IN_ROOM = 7;
const int ROOM_HEIGHT = 7, ROOM_WIDTH = 7;

// Game related
const int RED_TEAM = 7, BLUE_TEAM = 8, ARMS = 9, MEDS = 10;
const int ATTACKERS_IN_TEAM = 1, COURIERS_IN_TEAM = 1, TEAM_SIZE = ATTACKERS_IN_TEAM + COURIERS_IN_TEAM;
const int HEALTH_BOOST = 20, MAX_HEALTH = 100, MAX_ARMS = 3, MAX_MEDS = 3;

const int SHARDS_IN_GRENADE = 16;
const double WEAPON_STEP = 0.5, WEAPON_SIZE = 0.5;
const double BULLET_SECURITY_WEIGHT = 1;
const double GRENADE_SECURITY_WEIGHT = 3;
const double SHARD_SECURITY_WEIGHT = 1;