GCCG = gcc
 #FLAGS= -Wall -Werror -Wextra -std=c11 -g # <== flag

all: clean build

build:
	${GCCG} ${FLAGS} s21_sscanf.c s21_sscanf.h -o main
	./main

clean:
	rm -rf test.c test main

test: clean make_tests
	${GCCG} ${FLAGS} test.c -lcheck -lm -o test
	./test

make_tests:
	checkmk clean_mode=1 *.check > test.c
