CFLAGS+=$(shell ncursesw5-config --cflags)
LIBS=$(shell ncursesw5-config --libs)

all:
	g++ main.cpp ants.cpp draw.cpp $(CFLAGS) $(LIBS) -o anthill
