SRC=main.cpp sdl_help.cpp menu.cpp game.cpp player.cpp ball.cpp
OBJ=$(SRC:.cpp=.o)

CXX=g++
CXX_FLAGS=-std=c++11 -O3 -ggdb
INC=-I../lib
LIB=-lSDL2 -lSDL2_ttf -L../lib -lpugixml

all: pong.x

pong.x: $(OBJ)
	$(CXX) $(CXX_FLAGS) -o $@ $^ $(LIB)

%.o: %.cpp 
	$(CXX) $(CXX_FLAGS) -c $< $(INC)
val:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./pong.x
clean:
	-rm *.o *.x
