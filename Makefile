CXX = g++
CXX_FLAGS = -Wall

LIBS = -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer

APP_NAME = tetris

SRC_FILES = src/main.cpp src/game.cpp src/tetromino.cpp src/functions.cpp

executable:
	$(CXX) $(CXX_FLAGS) -O3 -o $(APP_NAME) $(SRC_FILES) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf *.o ./$(APP_NAME)
