TARGET = sdl1 point free hello

all:$(TARGET)

CFLAGS = -Wall -g

sdl1:sdl1.c
	ccache gcc $(CFLAGS) -o $@ $< `sdl-config --cflags` `sdl-config --libs`

point:point.c
	gcc $(CFLAGS) -o $@ $< `sdl-config --cflags` `sdl-config --libs`

free:free.c
	gcc $(CFLAGS) -o $@ $< `freetype-config --libs --cflags` `sdl-config --cflags` `sdl-config --libs`

hello:hello.c
	gcc $(CFLAGS) -o $@ $< `sdl-config --cflags` `sdl-config --libs`

clean:
	rm -rf $(TARGET)

