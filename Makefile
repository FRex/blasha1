all: b.exe

b.exe: b.c bsha1.h bsha1.c other.h
	gcc -O2 b.c bsha1.c -o b.exe

clean:
	rm -f b.exe
