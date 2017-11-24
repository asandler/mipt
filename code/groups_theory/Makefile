GXX=g++ -O3 -Wall -Wextra -pedantic -g
O_FILES=permutation.o factorial.o bitarray.o

all: main.cpp permutation factorial bitarray
	$(GXX) main.cpp $(O_FILES) -o task
	rm -f $(O_FILES)

permutation: permutation.cpp permutation.h
	$(GXX) -c permutation.cpp -o permutation.o

factorial: factorial.cpp factorial.h
	$(GXX) -c factorial.cpp -o factorial.o

bitarray: bitarray.cpp bitarray.h
	$(GXX) -c bitarray.cpp -o bitarray.o

clean:
	rm -f task $(O_FILES)
