CFLAGS = -std=c11

bmp: main.o bmp.o
	$(CC) -o bmp $^ -lSDL2

.PHONY: clean

clean:
	rm *.o bmp
