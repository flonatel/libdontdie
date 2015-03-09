CC=gcc
CFLAGS+=-fpic -Wall -Wextra -O3 ${CPPFLAGS}
LDFLAGS+=-shared -Wl,-soname,libdontdie.so.0

all: libdontdie.so libdontdie.so.0

testapps: test/Cperformance/perf test/java/EchoClient.class \
	test/Cblackbox/tcp_server test/Cblackbox/tcp_client


libdontdie.so.0:
	ln -s libdontdie.so libdontdie.so.0

libdontdie.so: src/libdontdie.c
	${CC} ${CFLAGS} -o $@ $< ${LDFLAGS} -ldl

test/Cblackbox/tcp_server: test/Cblackbox/tcp_server.c
	${CC} ${CFLAGS} -o $@ $<

test/Cblackbox/tcp_client: test/Cblackbox/tcp_client.c
	${CC} ${CFLAGS} -o $@ $<

test/Cperformance/perf: test/Cperformance/perf.c
	${CC} ${CFLAGS} -o $@ $<

test/java/EchoClient.class: test/java/EchoClient.java
	(cd test/java; javac EchoClient.java)

.PHONY: clean
clean:
	rm -f libdontdie.so* test/Cperformance/perf test/java/EchoClient.class \
	   test/Cblackbox/tcp_server test/Cblackbox/tcp_client
