LDLIBS = -lbmp $(shell pkg-config --libs sdl2)
CFLAGS = -std=c11 -lbmp $(shell pkg-config --cflags sdl2)
LDFLAGS = -L../

bmp: main.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@ 
