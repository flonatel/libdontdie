CC=gcc
CFLAGS=-fpic -Wall -Wextra
LDFLAGS=-shared -Wl,-soname,libdontdie.so -ggdb 

libdontdie.so: libdontdie.c
	${CC} ${CFLAGS} -o libdontdie.so libdontdie.c ${LDFLAGS} -ldl

.PHONY: clean
clean:
	rm libdontdie.so
