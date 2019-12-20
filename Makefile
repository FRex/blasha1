all: b.exe

b.exe: b.c blasha1.h blasha1.c other.h
	gcc -O2 b.c blasha1.c -o b.exe

clean:
	rm -f b.exe
