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

# dh_make leaves some strange relicts that are deleted here:
.PHONY: dh_make_clean
dh_make_clean:
	rm -f debian/*.ex debian/README.Debian debian/README.source \
	   debian/rules.dh7 debian/*.EX 

.PHONY: clean
clean:
	rm -f libdontdie.so test/Cperformance/perf test/java/EchoClient.class
