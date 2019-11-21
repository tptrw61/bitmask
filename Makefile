
CFLAGS+= -Wall

test: test.o libbitmask.a
	gcc $(CFLAGS) $< -o $@ -L. -lbitmask

test.o: test.c
	gcc -c $(CFLAGS) $< -o $@

libbitmask.a: bitmask.o
	ar rcs $@ $^

bitmask.o: bitmask.c bitmask.h
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o test

