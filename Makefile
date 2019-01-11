build:
	gcc -o nltest *.c -lmnl
clean:
	rm nltest *.o
