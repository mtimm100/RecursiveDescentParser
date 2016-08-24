CFLAGS = -g -Wall
CC = gcc

objects = Lexan.o Parser.o RecursiveDescent.o 

test : $(objects)
	  $(CC) -o test $(objects) 

Lexan.o : Lexan.c Lexan.h 
	  gcc -c -Wall Lexan.c
Parser.o : Parser.c Parser.h Lexan.h
	  gcc -c -Wall Parser.c
RecursiveDescent.o : RecursiveDescent.c Parser.h 
	  gcc -c -Wall RecursiveDescent.c

.PHONY : clean
clean : 
	  rm test $(objects)
