%.o: %.c
	$(CC) -c $<

.PHONY: clean

readelf: main.o readelf.o
	$(CC) $^ -o $@

myReadelf: 
	$(CC) -c main.c -m32
	$(CC) -c readelf.c -m32
	$(CC) main.o readelf.o -o $@ -m32 -static -g

hello: hello.c
	$(CC) $^ -o $@ -m32 -static -g

clean:
	rm -f *.o readelf hello
