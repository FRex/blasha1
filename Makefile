all: b.exe a.exe mysha1sum.exe

b.exe: b.c blasha1.h blasha1.c other.h
	gcc -std=c89 -Wall -Wextra -O2 b.c blasha1.c -o b.exe

a.exe: b.exe
	cp b.exe a.exe

mysha1sum.exe: blasha1.h blasha1.c mysha1sum.c
	gcc -std=c89 -Wall -Wextra -O2 mysha1sum.c blasha1.c -o mysha1sum.exe

clean:
	rm -f b.exe a.exe mysha1sum.exe
