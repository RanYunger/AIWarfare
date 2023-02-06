#pragma once

// Constants
const double PI = 3.1415;

// Window related
const int WINDOW_HEIGHT = 50, WINDOW_WIDTH = 400;

// Map Related
const int MAP_DIMENSION = 130;
const int WALL = 1, SPACE = 2, BLOCK_TO_CHECK = 3, RESTORED_PATH = 4;
const int WALL_WEIGHT = 5, SPACE_WEIGHT = 1;
const int MAX_SHOOTERS_IN_TEAM = 2, MAX_COURIERS_IN_TEAM = 1;
const int MAX_ARMS_IN_ROOM = 3, MAX_MEDS_IN_ROOM = 3, MAX_OBSTACLES_IN_ROOM = 3, MAX_ROOMS = 5;

// Game related
const int RED_TEAM = 7, BLUE_TEAM = 8, ARMS = 9, MEDS = 10;
const int MAX_HEALTH = 100, MAX_ARMS = 5;

// Weaponry related
const double BULLET_STEP = 0.2, BULLET_SIZE = 0.001;
const int SHARDS_IN_GRENADE = 20;