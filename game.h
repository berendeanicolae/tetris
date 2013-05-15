#include "tetromino.h"
#include <SDL/SDL_mixer.h>
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

class Game{
    //Variables
    private:
        const SDL_VideoInfo *vinfo;
        SDL_Surface *screen,*background;
        bool quit;
        Sint16 boxh,board_x,board_y;
        int level,to_next_lv,points;
        enum{
            MENU,
            PLAY
        }state;
    public:
        Game();
        ~Game();
        void run();
    private:
        void draw_menu();
        void draw_play();
        void menu();
        void play();
        void Tetromino_move(Tetromino *fig[3], bool **grid, key keyboard[4], tetromino_list **tetrominos, int l);
        void Tetromino_delete(Tetromino *fig);
        void Tetromino_down_end(Tetromino *fig[3], bool **grid, tetromino_list **tetrominos, int l);
        void replace_with(SDL_Surface *from, short x, short y);
        Mix_Chunk *pop;
};

#endif // GAME_H_INCLUDED
