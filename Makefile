TARGET = sdl1 point free hello border

all:$(TARGET)

CFLAGS = -Wall -g

SDLCFLAGS = -I/home/shane/pro/libsdl/SDL-1.2.14/include/
SDLLIBS = -L/home/shane/pro/libsdl/SDL-1.2.14 -lSDL

sdl1:sdl1.c
	ccache gcc $(CFLAGS) -o $@ $< `sdl-config --cflags` `sdl-config --libs`

point:point.c
	gcc $(CFLAGS) -o $@ $< `sdl-config --cflags` `sdl-config --libs`

free:free.c
	gcc $(CFLAGS) -o $@ $< `freetype-config --libs --cflags` `sdl-config --cflags` `sdl-config --libs`

hello:hello.c
#	gcc $(CFLAGS) -o $@ $< `sdl-config --cflags` `sdl-config --libs`
	droid-gcc -o hello hello.c $(SDLCFLAGS) $(SDLLIBS) -lSDL -L/home/LinDoremi/telecow/out/target/product/leo/system/lib -lts -g

border:border.cpp
	g++ $(CFLAGS) -o $@ $< `sdl-config --cflags --libs` `freetype-config --libs --cflags`

up:hello
	adb push hello /system/bin

clean:
	rm -rf $(TARGET)

