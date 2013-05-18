tetris
======

Feel free to modify these files as you wish. <br/>If you find any bug, please contact
me (I did my best to find all, but I may have missed some) by submitting an
issue at https://github.com/berendeanicolae/tetris/issues . Thank you!

In order to work, the program needs some files you can download from:
  https://docs.google.com/file/d/0B9bS4AoQCn4IYmxIQU16dnMyUU0/edit?usp=sharing or
  https://www.dropbox.com/s/wqcewww028skama/resources.zip
These files must be located in a folder named resources and this folder and the 
executable must be in the same directory. You can use other files if you want to 
(but in that folder you can find what you can replace).


I am sorry for not documenting the files. I started this as a project for school
and it got bigger than I expected.

To compile run the command g++ -o game game.cpp main.cpp game.h tetromino.cpp 
tetromino.h functions.h functions.cpp -Wall -lSDL -lSDLmain -lSDL_image -lSDL_ttf -lSDL_mixer .
If you're trying to compile on windows, you need another flag -lmingw32 .
