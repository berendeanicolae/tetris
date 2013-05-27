tetris
======

Feel free to modify these files as you wish. If you find any bug, please contact<br/>
me (I did my best to find all, but I may have missed some) by submitting an<br/>
issue at https://github.com/berendeanicolae/tetris/issues . Thank you!<br/><br/>
In order to work, the program needs the files located in resources. The<br/>
executable must be in the same directory as resources. The files have very<br>
suggestive names so you can replace them with anything you want.<br/><br/>
To compile run the command g++ -o game game.cpp main.cpp game.h tetromino.cpp<br/>
tetromino.h functions.h functions.cpp -Wall -lSDL -lSDLmain -lSDL_image<br/>
-lSDL_ttf -lSDL_mixer . If you're trying to compile on windows, you need<br/>
another flag -lmingw32 .
