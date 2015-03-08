CC=gcc
CFLAGS=-fpic -Wall -Wextra -O3
LDFLAGS=-shared -Wl,-soname,libdontdie.so 

all: libdontdie.so 

testapps: test/Cperformance/perf test/java/EchoClient.class \
	test/Cblackbox/tcp_server test/Cblackbox/tcp_client

libdontdie.so: src/libdontdie.c
	${CC} ${CFLAGS} -o libdontdie.so src/libdontdie.c ${LDFLAGS} -ldl

test/Cblackbox/tcp_server: test/Cblackbox/tcp_server.c
	${CC} ${CFLAGS} -o test/Cblackbox/tcp_server test/Cblackbox/tcp_server.c

test/Cblackbox/tcp_client: test/Cblackbox/tcp_client.c
	${CC} ${CFLAGS} -o test/Cblackbox/tcp_client test/Cblackbox/tcp_client.c

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
