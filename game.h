#include "tetromino.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

class Game{
    //Variables
    private:
        const SDL_VideoInfo *vinfo;
        Mix_Chunk *pop;
        SDL_Surface *screen,*background;
        TTF_Font *font;
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
        void update_score();
        bool Tetromino_move(Tetromino *fig[3], bool **grid, key keyboard[4], tetromino_list **tetrominos, int l); //returns true if tetromino can not enter
        void Tetromino_delete(Tetromino *fig);
        bool Tetromino_down_end(Tetromino *fig[3], bool **grid, tetromino_list **tetrominos, int l);
        void replace_with(SDL_Surface *from, short x, short y);
};

#endif // GAME_H_INCLUDED
