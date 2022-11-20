#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct square_cell {
	int8_t value; 
	bool right, down;
} square_cell_t; 

typedef struct square_game {
	square_cell_t **map;
	int32_t len, wid;	// start from 1
	int32_t score[2];
	bool turn;		// one's turn
} square_game_t;
