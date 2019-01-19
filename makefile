LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

all: a.out

a.out: main.c
	gcc -std=c99 main.c $(LIBS)

clean:
	rm *.os
