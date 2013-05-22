#include <SDL/SDL.h>
#ifndef TETROMINO_H_INCLUDED
#define TETROMINO_H_INCLUDED

#define frame 10
#define height 20
#define width 10

struct key{
    bool pressed;
    int downtime;
};

class tetromino_node{
    public:
        //Constructors & Destructors
        tetromino_node(): w(0), h(0), shape(NULL), look(NULL), next(NULL) {}
        tetromino_node(int w_, int h_, char *fill, int color, int boxh);
        ~tetromino_node();
        //Variables
        Uint16 w,h;
        bool **shape;
        SDL_Surface *look;
        tetromino_node *next;
};

class tetromino_list{
    public:
        //Constructors & Destructors
        tetromino_list(): first(NULL) {}
        tetromino_list(int w, int h, char *fill, int color, int turns, int boxh);
        ~tetromino_list();
        void rotate(char *fill, int w, int h);
        //Variables
        tetromino_node *first;
};

class Tetromino{
    private:
    public:
        //Constructors & Destructors
        Tetromino(): node(NULL), update(0), x(-1), y(-1) {}
        Tetromino(tetromino_node *first, int level): node(first), update(0), rotated(0), ox((width-node->w)>>1), oy(node->h), x(ox), y(oy), time(1000/level) {}
        ~Tetromino() {}
        //Variables
        tetromino_node *node;
        bool update,rotated;
        Sint16 ox,oy,x,y,time;
        //Methods
        bool can_enter(bool **grid);
        bool move_left(bool **grid);
        bool move_down(bool **grid);
        bool move_right(bool **grid);
        bool rotate(bool **grid);
};

#endif // TETROMINO_H_INCLUDED
