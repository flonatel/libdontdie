CC=gcc
CFLAGS=-fpic
LDFLAGS=-shared -Wl,-soname,libdontdie.so

libdontdie.so: libdontdie.c
	${CC} ${CFLAGS} -o libdontdie.so libdontdie.c ${LDFLAGS}

.PHONY: clean
clean:
	rm libdontdie.so
