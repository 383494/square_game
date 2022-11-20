#include "square.h"

square_game_t *new_square_game(int32_t len, int32_t wid){	// len++, wid++
	square_game_t *buf = (square_game_t *)calloc(sizeof(square_game_t *), 1);
	buf->len = ++len;
	buf->wid = ++wid;
	buf->map = (square_cell_t *)malloc(sizeof(square_cell_t *)* len);
	for(int i=0; i<len; ++i){
		buf->map[i] = (square_cell_t)calloc(sizeof(square_cell_t), wid);
	}
	return buf;
}

void del_square_game(square_game_t *buf){
	for(int i=0; i<=len; ++i){

	}
}
