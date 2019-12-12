crypto : main.o md5.o sha1.o sha2.o
	gcc -o crypto main.o md5.o sha1.o sha2.o -W -Wall -O3 

main.o : main.c md5.h sha1.h sha2.h
	gcc -c main.c -W -Wall -O3 -Wno-incompatible-pointer-types

md5.o : md5.c md5.h
	gcc -c md5.c -W -Wall -O3 

sha1.o : sha1.c sha1.h
	gcc -c sha1.c -W -Wall -O3 

sha2.o : sha2.c sha2.h
	gcc -c sha2.c -W -Wall -O3 

clean :
	rm -f main.o md5.o sha1.o sha2.o