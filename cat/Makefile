all: build

build: s21_cat.o
	gcc -Wall -Werror -Wextra s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	gcc -Wall -Werror -Wextra -c s21_cat.c


clean:
	rm -rf *.o