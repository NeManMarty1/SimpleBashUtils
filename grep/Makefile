CC = gcc
CFLAGS = -Wall -Werror -Wextra
s21_grep:
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c
rebuild:
	rm s21_grep
	$(CC) $(CFLAGS) -o s21_grep s21_grep.c
test:
	sh test.sh

clean:
	rm -rf *.o