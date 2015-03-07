# libdontdie
a library that can be pre-loaded and that sets the TCP KEEP-ALIVE flag
whenever `socket(2)' is called

[![Build
Status](https://secure.travis-ci.org/flonatel/libdontdie.png)](http://travis-ci.org/flonatel/pipexec)

## Introduction
This is a complete rewrite of the <a
href="http://libkeepalive.sourceforge.net">libkeepalive</a>.

The main reason for the rewrite was the license of libkeepalive (GPL -
without exceptions) which is always a grey area for a library.

The other reason was, that the libkeepalive does not work with Java -
because of a bug.

## Usage
The library is developed for Linux.  The commands given must be
entered in a shell.

```bash
DD_DEBUG=1 DD_TCP_KEEPALIVE_TIME=4 DD_TCP_KEEPALIVE_INTVL=5 \
  DD_TCP_KEEPALIVE_PROBES=6 LD_PRELOAD=/usr/lib/libdontdie.so \
  java EchoClient 127.0.0.1 22
```

* DD_DEBUG: if set to 1, it prints each call of socket - including the
  provided parameters to syslog.  Also logged are the decisions if the
  keep alive should be set and which parameters are used.

* DD_TCP_KEEPALIVE_TIME, DD_TCP_KEEPALIVE_INTVL,
  DD_TCP_KEEPALIVE_PROBES:
  These values are related to the appropriate TCP options.  There a
  <a
  href="http://tldp.org/HOWTO/TCP-Keepalive-HOWTO/usingkeepalive.html">good
  descriptions around</a>.

* DD_EVAL_ENVIRONMENT_ONCE: if set to 1 or if the environment variable
  is not set, all the environment variables are evaluated just once
  and not during each socket call.

* DD_USE_KEEPALIVE: if set to 1 or if the environment variable
  is not set, tcp keepalive will be switched on.  All other values
  that are not specified are used from the system.

## Compile and Install
Download the tarball from the release or checkout from git.
Only make and some up to data C compiler is needed.

Run:
```bash
make
```

The library will be created in the current directory.  You might want
to copy it into a global folder (like /usr/lib) or copy it locally
to your user or project directory.

```bash
sudo cp libdontdie.so /usr/lib
```

## Example
In the 'test/java' directory there is a simple echo client.  Compile:

```bash
javac EchoClient.java
```

This connects to localhost port 22:

```bash
socat echo TCP-LISTEN:7777,fork &
DD_DEBUG=1 DD_TCP_KEEPALIVE_TIME=4 DD_TCP_KEEPALIVE_INTVL=5 DD_TCP_KEEPALIVE_PROBES=6 LD_PRELOAD=/usr/lib/libdontdie.so java EchoClient 127.0.0.1 7777
```

Log output:
```
Mar  4 12:40:20 rs3 java: libdontdie: Initialization
Mar  4 12:40:20 rs3 java: libdontdie: Evaluate environment only once
Mar  4 12:40:20 rs3 java: libdontdie: TCP keepalive is switched on
Mar  4 12:40:20 rs3 java: libdontdie: set TCP_KEEPALIVE_TIME [4]
Mar  4 12:40:20 rs3 java: libdontdie: set TCP_KEEPALIVE_INTVL [5]
Mar  4 12:40:20 rs3 java: libdontdie: set TCP_KEEPALIVE_PROBES [6]
Mar  4 12:40:20 rs3 java: libdontdie: socket() called
Mar  4 12:40:20 rs3 java: libdontdie: domain [AF_INET6]
Mar  4 12:40:20 rs3 java: libdontdie: type [SOCK_STREAM]
Mar  4 12:40:20 rs3 java: libdontdie: type [SOCK_SEQPACKET]
Mar  4 12:40:20 rs3 java: libdontdie: type [SOCK_RAW]
Mar  4 12:40:20 rs3 java: libdontdie: protocol [0]
Mar  4 12:40:20 rs3 java: libdontdie: socket() call returned fd [12]
Mar  4 12:40:20 rs3 java: libdontdie: Parameters check passed
Mar  4 12:40:20 rs3 java: libdontdie: Setting KEEPALIVE for socket
Mar  4 12:40:20 rs3 java: libdontdie: Seting TIME [4]
Mar  4 12:40:20 rs3 java: libdontdie: Seting INTVL [5]
Mar  4 12:40:20 rs3 java: libdontdie: Seting PROBES [6]
Mar  4 12:40:20 rs3 java: libdontdie: Finished; returning to caller [12]
Mar  4 12:40:20 rs3 java: libdontdie: socket() called
Mar  4 12:40:20 rs3 java: libdontdie: domain [AF_INET6]
Mar  4 12:40:20 rs3 java: libdontdie: type [SOCK_STREAM]
Mar  4 12:40:20 rs3 java: libdontdie: type [SOCK_SEQPACKET]
Mar  4 12:40:20 rs3 java: libdontdie: type [SOCK_RAW]
Mar  4 12:40:20 rs3 java: libdontdie: protocol [0]
Mar  4 12:40:20 rs3 java: libdontdie: socket() call returned fd [13]
Mar  4 12:40:20 rs3 java: libdontdie: Parameters check passed
Mar  4 12:40:20 rs3 java: libdontdie: Setting KEEPALIVE for socket
Mar  4 12:40:20 rs3 java: libdontdie: Seting TIME [4]
Mar  4 12:40:20 rs3 java: libdontdie: Seting INTVL [5]
Mar  4 12:40:20 rs3 java: libdontdie: Seting PROBES [6]
Mar  4 12:40:20 rs3 java: libdontdie: Finished; returning to caller [13]
```

TCP Dump output with TCP Keepalives:

```
12:40:20.168088 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [S], seq 2760789742, win 43690, options [mss 65495,sackOK,TS val 1204785362 ecr 0,nop,wscale 7], length 0
12:40:20.168113 IP 127.0.0.1.7777 > 127.0.0.1.40298: Flags [S.], seq 207201762, ack 2760789743, win 43690, options [mss 65495,sackOK,TS val 1204785362 ecr 1204785362,nop,wscale 7], length 0
12:40:20.168135 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204785362 ecr 1204785362], length 0
12:40:24.173260 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204786364 ecr 1204785362], length 0
12:40:24.173348 IP 127.0.0.1.7777 > 127.0.0.1.40298: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204786364 ecr 1204785362], length 0
12:40:29.181281 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204787616 ecr 1204786364], length 0
12:40:34.189248 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204788868 ecr 1204786364], length 0
12:40:39.197282 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204790120 ecr 1204786364], length 0
12:40:44.205280 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204791372 ecr 1204786364], length 0
12:40:49.213271 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204792624 ecr 1204786364], length 0
12:40:54.221254 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [.], ack 1, win 342, options [nop,nop,TS val 1204793876 ecr 1204786364], length 0
12:40:59.229282 IP 127.0.0.1.40298 > 127.0.0.1.7777: Flags [R.], seq 1, ack 1, win 342, options [nop,nop,TS val 1204795128 ecr 1204786364], length 0
```

Shortly after 12:40:24 the firewall was used to drop all packets to port 7777:

```bash
iptables -A INPUT -p tcp --destination-port 7777 -j DROP
```

Every five seconds a keep-alive is sent out and after 6 retries (see
DD_TCP_KEEPALIVE_PROBES=6), the client connection gets into a failed
state:

```
Socket exception java.net.SocketException: Connection timed out
```

## Performance
Because the most costly operations are only done once - and not during
each socket() call - libdontdie is faster than the libkeepalive.

The next table compares runtimes of libdontdie against libkeepalive
and when none of them are used.

The times are given are the real times as given by the 'time'
command.  Percents are computed based on the reference 'raw socket' is
100%.  Performance test was done using Debian 8 system, gcc 4.9.2,
Intel i5-3570 CPU, 3.40GHz and 16GByte RAM.  100000000 calls of
'socket()' were executed.

raw socket | libdontdie | libkeepalive
--- | --- | ---
1:51 | 2:31 | 3:16
100% | 136% | 177%

