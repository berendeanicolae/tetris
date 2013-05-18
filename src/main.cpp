/*  To be compiled with the following flags: -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer -static-libgcc -static-libstdc++
*/
#include "game.h"

int main(int argc, char **argv)
{
    Game game;
    game.run();
    return 0;
}
