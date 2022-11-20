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
	int32_t len, wid;	// start from 1 and added 1
						// number of dots
	int32_t score[2];
	bool turn;		// one's turn
} square_game_t;

// number of cells
square_game_t *new_square_game(int32_t len, int32_t wid);	

void del_square_game(square_game_t *buf);
void set_square_value(square_game_t *buf, 
		int32_t x, int32_t y, int8_t value);
int8_t get_square_value(square_game_t *buf, 
		int32_t x, int32_t y);
void set_square(square_game_t *buf, 
		int32_t x, int32_t y, bool right);
bool get_square(square_game_t *buf, 
		int32_t x, int32_t y, bool right);
bool check_square_cell(square_game_t *buf,
		int32_t x, int32_t y);

void Squ_step(square_game_t *buf, 
		int32_t x, int32_t y, bool right);
