#pragma once

// Includes
#include "Position.h"

// Constants
const int GIVE = 0, TAKE = 1;																			 // Transaction identifier for Courier - DO NOT TOUCH THIS LINE
const double PI = 3.1415;																				 // The PI constant (for angle calculations) - DO NOT TOUCH THIS LINE
const Position EMPTY_POSITION;																			 // A default Position: (-1, -1) - DO NOT TOUCH THIS LINE

// Window related
const int WINDOW_HEIGHT = 50, WINDOW_WIDTH = 400;														 // GLUT window dimenstions

// Map Related
const int MAP_DIMENSION = 100;																			 // Game map dimenstion
const int WALL = 1, SPACE = 2;																			 // Designation in map - DO NOT TOUCH THIS LINE
const int SPACE_WEIGHT = 1, WALL_WEIGHT = 3;															 // For tunnel configurations
const int MAX_ITERATIONS = (MAP_DIMENSION * MAP_DIMENSION) / 6;											 // To limit potential infinite loops

// Room Related
const int EDGE_OFFSET = 15;																				 // Starting offset to place room
const int MAX_ROOMS = 7, MAX_OBSTACLES_IN_ROOM = 5, MAX_ARMS_IN_ROOM = 7, MAX_MEDS_IN_ROOM = 7;			 // For map configurations
const int ROOM_HEIGHT = 7, ROOM_WIDTH = 7;																 // Room dimensions 

// Game related
const int RED_TEAM = 7, BLUE_TEAM = 8, ARMS = 9, MEDS = 10;												 // Designation in map - DO NOT TOUCH THIS LINE
const int ATTACKERS_IN_TEAM = 2, COURIERS_IN_TEAM = 1, TEAM_SIZE = ATTACKERS_IN_TEAM + COURIERS_IN_TEAM; // For team configuarions
const int HEALTH_BOOST = 20, MAX_HEALTH = 100;															 // Health values
const int INIT_ATTACKER_ARMS = 2, INIT_ATTACKER_MEDS = 0, INIT_COURIER_ARMS = 3, INIT_COURIER_MEDS = 3;	 // Initial equipment values
const int MAX_ARMS = 10, MAX_MEDS = 10;																	 // Max equipment per character

// Weaponry related
const int SHARDS_IN_GRENADE = 16;																		 // Number of bullets the grenade breaks into
const double WEAPON_SIZE = 0.5, WEAPON_STEP = 1.5;														 // The size of the bullet drawn, and the step it takes per iteration.
const double BULLET_SECURITY_WEIGHT = 0.1, GRENADE_SECURITY_WEIGHT = 0.3;								 // The Secrity map values. Keep below 1
const int GRENADE_DAMAGE = 3, BULLET_DAMAGE = 45;														 // Grenade explodes when DMG reaches 0. Bullet DMG shrinks as it moves.
const int BULLET_STEP_DAMAGE_REDUCTION = 1;																 // How much the damage is reduced per iteration