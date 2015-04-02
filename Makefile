GCC=g++
CFLAGS=-DLINUX
LDFLAGS=-lncurses
OUT=battleships

OBJ = main.o ships.o shipset.o screen.o positioning.o

all: $(OBJ)
	$(GCC) -o $(OUT) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(GCC) -c -o $@ $< $(CFLAGS)

clean:
	rm -rf *.o $(OUT)
