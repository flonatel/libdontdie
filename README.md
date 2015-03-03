# libdontdie
TCP lib that sets KEEP ALIVE parameters when socket(3) is called

## Introduction
This is a complete rewrite of the <a
href="http://libkeepalive.sourceforge.net">libkeepalive</a>.

The main reason for the rewrite was the license of libkeepalive (GPL -
without exceptions) which is always something clowdy for a library.

The other reason was, that the libkeepalive does not work with Java -
because of a bug.

So instead of messing around with source code that comes with a
strange license, I started over.

## Usage

```bash
DD_DEBUG=1 DD_TCP_KEEPALIVE_TIME=4 DD_TCP_KEEPALIVE_INTVL=5 DD_TCP_KEEPALIVE_PROBES=6 LD_PRELOAD=/usr/lib/libdontdie.so java EchoClient 127.0.0.1 22
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

## Example
In the java directory there is a simple echo client.  To compile it run

```bash
javac EchoClient.java
```

This connects to localhost port 22:

```bash
DD_DEBUG=1 DD_TCP_KEEPALIVE_TIME=4 DD_TCP_KEEPALIVE_INTVL=5 DD_TCP_KEEPALIVE_PROBES=6 LD_PRELOAD=/usr/lib/libdontdie.so java EchoClient 127.0.0.1 22
```

Log output:
```
Mar  4 00:50:00 thynias java: libdontdie: Initialization
Mar  4 00:50:00 thynias java: libdontdie: Evaluate environment only once
Mar  4 00:50:00 thynias java: libdontdie: TCP keepalive is switched on
Mar  4 00:50:00 thynias java: libdontdie: Set TCP_KEEPALIVE_TIME= [4]
Mar  4 00:50:00 thynias java: libdontdie: Set TCP_KEEPALIVE_INTVL= [5]
Mar  4 00:50:00 thynias java: libdontdie: Set TCP_KEEPALIVE_PROBES= [6]
Mar  4 00:50:00 thynias java: libdontdie: socket() called
Mar  4 00:50:00 thynias java: libdontdie: domain [AF_INET6]
Mar  4 00:50:00 thynias java: libdontdie: type [SOCK_STREAM]
Mar  4 00:50:00 thynias java: libdontdie: type [SOCK_SEQPACKET]
Mar  4 00:50:00 thynias java: libdontdie: type [SOCK_RAW]
Mar  4 00:50:00 thynias java: libdontdie: protocol [0]
Mar  4 00:50:00 thynias java: libdontdie: socket() call returned fd [12]
Mar  4 00:50:00 thynias java: libdontdie: Parameters check passed
Mar  4 00:50:00 thynias java: libdontdie: Setting KEEPALIVE for socket
Mar  4 00:50:00 thynias java: libdontdie: Seting TIME [4]
Mar  4 00:50:00 thynias java: libdontdie: Seting INTVL [5]
Mar  4 00:50:00 thynias java: libdontdie: Seting PROBES [6]
Mar  4 00:50:00 thynias java: libdontdie: Finished; returning to caller [12]
Mar  4 00:50:00 thynias java: libdontdie: socket() called
Mar  4 00:50:00 thynias java: libdontdie: domain [AF_INET6]
Mar  4 00:50:00 thynias java: libdontdie: type [SOCK_STREAM]
Mar  4 00:50:00 thynias java: libdontdie: type [SOCK_SEQPACKET]
Mar  4 00:50:00 thynias java: libdontdie: type [SOCK_RAW]
Mar  4 00:50:00 thynias java: libdontdie: protocol [0]
Mar  4 00:50:00 thynias java: libdontdie: socket() call returned fd [13]
Mar  4 00:50:00 thynias java: libdontdie: Parameters check passed
Mar  4 00:50:00 thynias java: libdontdie: Setting KEEPALIVE for socket
Mar  4 00:50:00 thynias java: libdontdie: Seting TIME [4]
Mar  4 00:50:00 thynias java: libdontdie: Seting INTVL [5]
Mar  4 00:50:00 thynias java: libdontdie: Seting PROBES [6]
Mar  4 00:50:00 thynias java: libdontdie: Finished; returning to caller [13]
```

TCP Dump output with TCP Keepalives:

```
00:50:00.943286 IP 10.0.0.25.39128 > 10.0.0.25.22: Flags [S], seq 1357126392, win 43690, options [mss 65495,sackOK,TS val 110288247 ecr 0,nop,wscale 7], length 0
00:50:00.943301 IP 10.0.0.25.22 > 10.0.0.25.39128: Flags [S.], seq 2231596575, ack 1357126393, win 43690, options [mss 65495,sackOK,TS val 110288247 ecr 110288247,nop,wscale 7], length 0
00:50:00.943312 IP 10.0.0.25.39128 > 10.0.0.25.22: Flags [.], ack 1, win 342, options [nop,nop,TS val 110288247 ecr 110288247], length 0
00:50:00.948144 IP 10.0.0.25.22 > 10.0.0.25.39128: Flags [P.], seq 1:33, ack 1, win 342, options [nop,nop,TS val 110288248 ecr 110288247], length 32
00:50:00.948154 IP 10.0.0.25.39128 > 10.0.0.25.22: Flags [.], ack 33, win 342, options [nop,nop,TS val 110288248 ecr 110288248], length 0
00:50:04.945350 IP 10.0.0.25.39128 > 10.0.0.25.22: Flags [.], ack 33, win 342, options [nop,nop,TS val 110289248 ecr 110288248], length 0
00:50:04.945368 IP 10.0.0.25.22 > 10.0.0.25.39128: Flags [.], ack 1, win 342, options [nop,nop,TS val 110289248 ecr 110288248], length 0
00:50:09.953337 IP 10.0.0.25.39128 > 10.0.0.25.22: Flags [.], ack 33, win 342, options [nop,nop,TS val 110290500 ecr 110289248], length 0
00:50:09.953367 IP 10.0.0.25.22 > 10.0.0.25.39128: Flags [.], ack 1, win 342, options [nop,nop,TS val 110290500 ecr 110288248], length 0
00:50:14.961346 IP 10.0.0.25.39128 > 10.0.0.25.22: Flags [.], ack 33, win 342, options [nop,nop,TS val 110291752 ecr 110290500], length 0
00:50:14.961359 IP 10.0.0.25.22 > 10.0.0.25.39128: Flags [.], ack 1, win 342, options [nop,nop,TS val 110291752 ecr 110288248], length 0
00:50:19.969355 IP 10.0.0.25.39128 > 10.0.0.25.22: Flags [.], ack 33, win 342, options [nop,nop,TS val 110293004 ecr 110291752], length 0
00:50:19.969385 IP 10.0.0.25.22 > 10.0.0.25.39128: Flags [.], ack 1, win 342, options [nop,nop,TS val 110293004 ecr 110288248], length 0

```