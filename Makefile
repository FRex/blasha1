all: mysha1sum.exe native.exe

mysha1sum.exe: blasha1.h mysha1sum.c
	gcc -std=c89 -Wall -Wextra -O2 mysha1sum.c -o mysha1sum.exe

native.exe: blasha1.h mysha1sum.c
	gcc -std=c89 -Wall -Wextra -O2 -march=native mysha1sum.c -o native.exe

clean:
	rm -f mysha1sum.exe native.exe
