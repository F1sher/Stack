all: main

main: main.o
		gcc -o main.o main.c stack.c unity.c

clean:
		rm main.o
