all: mysha1sum.exe native.exe tester.exe tester-native.exe

mysha1sum.exe: blasha1.h mysha1sum.c
	gcc -std=c89 -Wall -Wextra -Werror -Wno-long-long -O2 mysha1sum.c -o mysha1sum.exe

native.exe: blasha1.h mysha1sum.c
	gcc -std=c89 -Wall -Wextra -Werror -Wno-long-long -O2 mysha1sum.c -march=native -o native.exe

tester.exe: blasha1.h tester.c
	gcc -std=c89 -Wall -Wextra -Werror -Wno-long-long -O2 tester.c -o tester.exe

tester-native.exe: blasha1.h tester.c
	gcc -std=c89 -Wall -Wextra -Werror -Wno-long-long -O2 tester.c -march=native -o tester-native.exe

clean:
	rm -f mysha1sum.exe native.exe tester.exe tester-native.exe
