CFLAGS = -std=c11

all: libbmp.a example

libbmp.a: bmp.o
	ar r $@ $^

.PHONY: clean

example: libbmp.a
	make -C ./example

clean:
	rm *.o *.a
