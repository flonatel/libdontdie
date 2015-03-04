CC=gcc
CFLAGS=-fpic -Wall -Wextra -O3
LDFLAGS=-shared -Wl,-soname,libdontdie.so 

all: libdontdie.so
tests: test/Cperformance/perf test/java/EchoClient.class

libdontdie.so: src/libdontdie.c
	${CC} ${CFLAGS} -o libdontdie.so src/libdontdie.c ${LDFLAGS} -ldl

test/Cperformance/perf: test/Cperformance/perf.c
	${CC} ${CFLAGS} -o test/Cperformance/perf test/Cperformance/perf.c

test/java/EchoClient.class: test/java/EchoClient.java
	(cd test/java; javac EchoClient.java)

.PHONY: clean
clean:
	rm -f libdontdie.so test/Cperformance/perf test/java/EchoClient.class
