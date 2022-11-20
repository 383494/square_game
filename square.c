#include <stdlib.h>
#include "square.h"

square_game_t *new_square_game(int32_t len, int32_t wid){	// len++, wid++
	square_game_t *buf = (square_game_t *)calloc(sizeof(square_game_t *), 1);
	buf->len = ++len;
	buf->wid = ++wid;
	buf->map = (square_cell_t **)malloc(sizeof(square_cell_t **)* len);
	for(int i=0; i<len; ++i){
		buf->map[i] = (square_cell_t *)calloc(sizeof(square_cell_t), wid);
	}
	return buf;
}

void del_square_game(square_game_t *buf){
	for(int i=0; i<buf->len; ++i){
		free(buf->map[i]);
	}
	free(buf->map);
	free(buf);
	return;
}

inline void set_square_value(square_game_t *buf, 
		int32_t x, int32_t y, int8_t value){
	buf->map[x][y].value = value;
	return;
}

inline int8_t get_square_value(square_game_t *buf, 
		int32_t x, int32_t y){
	return buf->map[x][y].value;
}

void set_square(square_game_t *buf, 
		int32_t x, int32_t y, bool right){
	if(right) 
		buf->map[x][y].right = true;
	else 
		buf->map[x][y].down = true;

	return;
}

inline bool get_square(square_game_t *buf, 
		int32_t x, int32_t y, bool right){
	return right ? 
		buf->map[x][y].right : 
		buf->map[x][y].down;
}

inline bool check_square_cell(square_game_t *buf,
		int32_t x, int32_t y){
	if(x==0 || y==0)
		return false;

	return buf->map[x][y].right && buf->map[x][y].down && 
		buf->map[x-1][y].right && buf->map[x][y-1].down;
}

void Squ_step(square_game_t *buf, 
		int32_t x, int32_t y, bool right){
	if(get_square(buf, x, y, right)) 
		return;
	set_square(buf, x, y, right);
	bool change_turn = true;
	bool next = false;
	do{
		if(check_square_cell(buf, x, y)){
			buf->score[buf->turn?0:1] += buf->map[x][y].value;
			buf->map[x][y].value = 0;	// set mark
			change_turn = false;
		}
		if(right) ++x;
		else ++y;
		if(x>buf->len || y>buf->wid) break;
		next = !next;
	}while(next == false);

	if(change_turn)
		buf->turn = !buf->turn;
	return;
}
