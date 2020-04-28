all: mysha1sum.exe

mysha1sum.exe: blasha1.h mysha1sum.c
	gcc -std=c89 -Wall -Wextra -O2 mysha1sum.c -o mysha1sum.exe

clean:
	rm -f mysha1sum.exe
