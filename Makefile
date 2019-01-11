build:
	gcc -o nltest *.c -lmnl -Wall -Wextra
clean:
	rm nltest *.o
