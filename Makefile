build:
	gcc -o nltest *.c -lmnl -Wall -Wextra -Werror
clean:
	rm nltest *.o
