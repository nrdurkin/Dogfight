CPPFILES = main.cpp  Player.cpp Enemy.cpp Bullet.cpp Controller.cpp Display.cpp
HFILES = $(CPPFILES:.cpp=.h)

OFILES= $(CPPFILES:.cpp=.o)

CC = g++
LIBPATH =

CFLAGS = -std=c++11 -Wall -g

all: Dogfight

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

Dogfight: $(OFILES)
	$(CC) -o Dogfight $(OFILES) $(CFLAGS)  -lallegro -lallegro_font -lallegro_ttf -lallegro_primitives -lm

clean: 
	rm *.o
	rm Dogfight