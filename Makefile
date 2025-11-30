all: bin2hex example

bin2hex:
	gcc -o bin2hex ./bin2hex.c

example:
	gcc -o simple microui.c Example/simple.c murl.c -I./Includes/ -lraylib 
