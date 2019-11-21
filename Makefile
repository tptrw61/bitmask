
CFLAGS+= -Wall

libbitmask.a: bitmask.o
	ar rcs $@ $^

bitmask.o: bitmask.c bitmask.h
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f bitmask.o test

