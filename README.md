# libdontdie
TCP lib that sets KEEP ALIVE parameters when socket(3) is called

## Introduction
This is a complete rewrite of the <a
href="http://libkeepalive.sourceforge.net">libkeepalive</a>.

The main reason for the rewrite was the license (GPL - without
exceptions) which is always something clowdy for a library.

The other reason was, that the libkeepalive does not work with Java -
because of a bug.

So instead of messing around with source code that comes with a
strange license, I started over.

## Usage

```bash
LD_PRELOAD=/usr/lib/libdontdie.so DD_DEBUG=1 DD_TCP_KEEPALIVE_TIME=60 \
   DD_TCP_KEEPALIVE_INTVL=30 DD_TCP_KEEPALIVE_PROBES=45 \
   java EchoServer
```

* DD_DEBUG: if set to 1, it print each call of socket - including the
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

