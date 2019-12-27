all: b.exe a.exe

b.exe: b.c blasha1.h blasha1.c other.h
	gcc -O2 b.c blasha1.c -o b.exe

a.exe: b.exe
	cp b.exe a.exe

clean:
	rm -f b.exe a.exe
