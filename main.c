#include <SDL2/SDL.h>
#undef main
#include <stdio.h>
#include <signal.h>
#include "square.h"
#define MAP_FILE "map.txt"
#define SIDEX 10
#define SIDEY 10
#define BARLEN 70
#define BOXLEN 150
#define BOXWID BOXLEN
#define BOXNUM 6
#define SCRLEN (BOXNUM*BOXLEN+2*SIDEX)
#define SCRWID (BOXNUM*BOXWID+2*SIDEY+BARLEN)
#define OUTPUT(x,...) printf(x,##__VA_ARGS__)

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Texture *bg = NULL, *num[10];
SDL_Event ev;
bool move = true;
bool pos = 1;
int playerx=0, playery=0;
square_game_t *game = NULL;

void quitall(int a){
	if(ren) SDL_DestroyRenderer(ren);
	if(win) SDL_DestroyWindow(win);
	if(num[0])
		for(int i=0; i<10; i++){
			SDL_DestroyTexture(num[i]);
		}
	if(bg) SDL_DestroyTexture(bg);
	del_square_game(game);
	fclose(stdin);
	puts("quit.");
	SDL_Quit();
}

void disp_number(int x){
	int i=0;
	do{
		SDL_RenderCopy(ren, num[x%10], NULL, (SDL_Rect[]){SCRLEN-50*i-40,SCRWID-60, 30, 50});
	}while(x/=10);
}

static inline void draw_screen(){
	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderCopy(ren, bg, NULL, (SDL_Rect[]){0,0,SCRLEN,SCRWID});
//	SDL_RenderCopy(ren, bg, NULL, NULL); 
	SDL_SetRenderDrawColor(ren, game->turn?255:0, game->turn?0:255, 0, 255);
	SDL_RenderFillRect(ren, (SDL_Rect[]){playerx*BOXLEN+BOXLEN/3+SIDEX, playery*BOXWID+BOXWID/3+SIDEY, BOXLEN/5, BOXWID/5});
	disp_number(game->score[game->turn?0:1]);
	SDL_RenderCopy(ren, num[move?0:1], NULL, (SDL_Rect[]){10,SCRWID-60, 30, 50});

	SDL_RenderPresent(ren);
}

void click(int x, int y){
	bool l=x<SCRLEN/3, r=x>SCRLEN*2/3, u=y<SCRLEN/3, d=y>SCRLEN*2/3;
	if((l||r)&&(u||d))move = !move;
	else if(move){
		if(l) playerx = ((playerx+BOXNUM-1)%BOXNUM);
		else if(r) playerx = (playerx+1)%BOXNUM;
		else if(u) playery = ((playery+BOXNUM-1)%BOXNUM);
		else if(d) playery = (playery+1)%BOXNUM;
	} else {
		SDL_SetRenderTarget(ren, bg);
		if(l) {
			Squ_step(game, playerx, playery+1, true);
			SDL_RenderFillRect(ren, (SDL_Rect[]){playerx*BOXLEN-10+SIDEX, playery*BOXWID-10+SIDEY, 20, BOXWID+20});
		}
		else if(r) {
			Squ_step(game, playerx+1, playery+1, true);
			SDL_RenderFillRect(ren, (SDL_Rect[]){playerx*BOXLEN+BOXLEN-10+SIDEX, playery*BOXWID-10+SIDEY, 20, BOXWID+20});
		}
		else if(u) {
			Squ_step(game, playerx+1, playery, false);
			SDL_RenderFillRect(ren, (SDL_Rect[]){playerx*BOXLEN-10+SIDEX, playery*BOXWID-10+SIDEY, BOXLEN+20, 20});
		}
		else if(d) {
			Squ_step(game, playerx+1, playery+1, false);
			SDL_RenderFillRect(ren, (SDL_Rect[]){playerx*BOXLEN-10+SIDEX, playery*BOXWID+BOXLEN-10+SIDEY, BOXLEN+20, 20});
		}
	}
	draw_screen();
	OUTPUT("%d %d %d %d %d\n", x, y, playerx, playery, move);
}

int main(int argc, char *argv[]) {
	if(freopen(MAP_FILE , "r", stdin) == NULL){
		fprintf(stderr,"error redirecting stdin.\n");
		return 1;
	}

	if(SDL_Init(SDL_INIT_VIDEO)){
		printf("Error to init: %s",SDL_GetError());
		return 1;
	}

	signal(SIGINT, quitall);

	win = SDL_CreateWindow("square"
		, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
		, SCRLEN, SCRWID, SDL_WINDOW_SHOWN);
	if(!win){
		printf("Error to create window: %s",SDL_GetError());
		quitall(0);
		return 1;
	}

	ren = SDL_CreateRenderer(win, -1, 0);
	if (!ren) {
		printf("Error to create renderer: %s", SDL_GetError());
		quitall(0);
		return 1;
	}

	bg = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_TARGET, SCRLEN, SCRWID);
	for(int i=0; i<10; i++){
		num[i] = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_TARGET, 3, 5);
		SDL_SetRenderTarget(ren, num[i]);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderDrawPoint(ren, 1, 1);
		SDL_RenderDrawPoint(ren, 1, 3);
	}
	//SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_SetRenderTarget(ren, num[0]);
	SDL_RenderDrawPoint(ren, 1, 2);
	SDL_SetRenderTarget(ren, num[1]);
	SDL_RenderFillRect(ren, (SDL_Rect[]){0, 0, 2, 5});
	SDL_SetRenderTarget(ren, num[2]);
	SDL_RenderDrawPoint(ren, 0, 1);
	SDL_RenderDrawPoint(ren, 2, 3);
	SDL_SetRenderTarget(ren, num[3]);
	SDL_RenderDrawPoint(ren, 0, 1);
	SDL_RenderDrawPoint(ren, 0, 3);
	SDL_SetRenderTarget(ren, num[4]);
	SDL_RenderDrawPoint(ren, 1, 0);
	SDL_RenderDrawPoint(ren, 0, 3);
	SDL_RenderDrawPoint(ren, 0, 4);
	SDL_RenderDrawPoint(ren, 1, 4);
	SDL_SetRenderTarget(ren, num[5]);
	SDL_RenderDrawPoint(ren, 2, 1);
	SDL_RenderDrawPoint(ren, 0, 3);
	SDL_SetRenderTarget(ren, num[6]);
	SDL_RenderDrawPoint(ren, 2, 1);
	SDL_SetRenderTarget(ren, num[7]);
	SDL_RenderFillRect(ren, (SDL_Rect[]){0, 1, 2, 4});
	SDL_SetRenderTarget(ren, num[9]);
	SDL_RenderDrawPoint(ren, 0, 3);

	SDL_SetRenderTarget(ren, NULL);
	SDL_RenderClear(ren);

	SDL_SetRenderTarget(ren, bg);
	SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
	SDL_RenderClear(ren);
	game = new_square_game(BOXNUM,BOXNUM);
	for(int i=1; i<=BOXNUM; ++i){
	for(int j=1; j<=BOXNUM; ++j){
		scanf("%hhd", &game->map[i][j].value);
		SDL_RenderCopy(ren, num[game->map[i][j].value], NULL, (SDL_Rect[]){i*BOXLEN-BOXLEN*2/3+SIDEX, j*BOXWID-BOXLEN*2/3+SIDEY, BOXLEN/3, BOXLEN/3});
	}
	}
	draw_screen();

	for(;;){
		while(SDL_PollEvent(&ev)){
			switch(ev.type){
				case SDL_QUIT:
					quitall(0);
					return 0;
				case SDL_MOUSEBUTTONDOWN:
					click(ev.button.x, ev.button.y);
					break;
//				case SDL_FINGERUP:
//					click((int)ev.tfinger.x*SCRLEN, (int)ev.tfinger.y*SCRWID);
//					break;
			}
		}
	}

	printf("\nquiting.\n");

}