TARGET = sdl1

all:$(TARGET)

CFLAGS = -Wall -g -lm

sdl1:sdl1.c
	ccache gcc $(CFLAGS) -o $@ $< `sdl-config --cflags` `sdl-config --libs`


clean:
	rm -rf $(TARGET)

