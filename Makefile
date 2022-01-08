.PHONY: all install clean

all: crypto

install: all

clean:
	del /Q main.o md5.o sha1.o sha2.o

crypto : main.o md5.o sha1.o sha2.o  
	gcc -o $@ $^  

main.o: main.c md5.h sha1.h sha2.h
	gcc -c main.c -W -Wall -O3 -Wno-incompatible-pointer-types 

md5.o: md5.c md5.h
	gcc -c md5.c -W -Wall -O3 -Wno-incompatible-pointer-types 

sha1.o: sha1.c sha1.h
	gcc -c sha1.c -W -Wall -O3 -Wno-incompatible-pointer-types 

sha2.o: sha2.c sha2.h
	gcc -c sha2.c -W -Wall -O3 -Wno-incompatible-pointer-types 

