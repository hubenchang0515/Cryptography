CFLAGS := -W -Wall -O3 -Wno-incompatible-pointer-types

crypto : main.o md5.o sha1.o sha2.o
	gcc -o crypto main.o md5.o sha1.o sha2.o $(CFLAGS)

main.o : main.c md5.h sha1.h sha2.h
	gcc -c main.c $(CFLAGS)

md5.o : md5.c md5.h
	gcc -c md5.c $(CFLAGS)

sha1.o : sha1.c sha1.h
	gcc -c sha1.c $(CFLAGS)

sha2.o : sha2.c sha2.h
	gcc -c sha2.c $(CFLAGS)

clean :
	rm -f main.o md5.o sha1.o sha2.o