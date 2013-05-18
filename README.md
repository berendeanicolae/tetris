tetris
======

Feel free to modify these files as you wish. <br/>If you find any bug, please contact<br/>
me (I did my best to find all, but I may have missed some) by submitting an<br/>
issue at https://github.com/berendeanicolae/tetris/issues . Thank you!<br/>

In order to work, the program needs some files you can download from:<br/>
  https://docs.google.com/file/d/0B9bS4AoQCn4IYmxIQU16dnMyUU0/edit?usp=sharing or<br/>
  https://www.dropbox.com/s/wqcewww028skama/resources.zip
These files must be located in a folder named resources and this folder and the<br/>
executable must be in the same directory. You can use other files if you want to<br/>
(but in that folder you can find what you can replace).<br/><br/>

To compile run the command g++ -o game game.cpp main.cpp game.h tetromino.cpp<br/>
tetromino.h functions.h functions.cpp -Wall -lSDL -lSDLmain -lSDL_image -lSDL_ttf -lSDL_mixer .<br/>
If you're trying to compile on windows, you need another flag -lmingw32 .
