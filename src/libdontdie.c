#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>

#define LOG(msg...)                                                            \
  do {                                                                         \
    if (debug > 0) {                                                           \
      syslog(LOG_USER | LOG_INFO, "libdontdie: " msg);                         \
    }                                                                          \
  } while (0)

int (*socket_call_clib)(int domain, int type, int protocol);

int debug = 0;
int use_keepalive = 1;
int eval_environment_once = 0;
int tcp_keepalive_time = -1;
int tcp_keepalive_intvl = -1;
int tcp_keepalive_probes = -1;

static void eval_environment() {
  if (eval_environment_once) {
    return;
  }

  char const *const str_debug = getenv("DD_DEBUG");
  if (str_debug != NULL && str_debug[0] == '1') {
    debug = 1;
  }

  LOG("Initialization");

  char const *const str_eval_environment_once =
      getenv("DD_EVAL_ENVIRONMENT_ONCE");
  if (str_eval_environment_once != NULL && *str_eval_environment_once == '0') {
    LOG("Evaluate environment for every socket() call");
  } else {
    eval_environment_once = 1;
    LOG("Evaluate environment only once");
  }

  char const *const str_use_keepalive = getenv("DD_USE_KEEPALIVE");
  if (str_use_keepalive != NULL && *str_use_keepalive == '0') {
    use_keepalive = 0;
    LOG("TCP keepalive is switched off");
  } else {
    LOG("TCP keepalive is switched on");
  }

#define EVAL_ENV(ltype, strtype)                                               \
  char const *const str_tcp_keepalive_##ltype =                                \
      getenv("DD_TCP_KEEPALIVE_" strtype);                                     \
  if (str_tcp_keepalive_##ltype != NULL) {                                     \
    tcp_keepalive_##ltype = atoi(str_tcp_keepalive_##ltype);                   \
    LOG("set TCP_KEEPALIVE_" strtype " [%d]", tcp_keepalive_##ltype);         \
  }

  EVAL_ENV(time, "TIME");
  EVAL_ENV(intvl, "INTVL");
  EVAL_ENV(probes, "PROBES");

#undef EVAL_ENV
}

__attribute__((constructor)) void libdontdie_init() {
  eval_environment();

  // Cast it: according to the man page this construct must be used:
  *(void **)(&socket_call_clib) = dlsym(RTLD_NEXT, "socket");
  if (socket_call_clib == NULL) {
    LOG("No dynamic symbol with name 'socket' found [%m]");
    abort();
  }
}

static void log_parameter_domain(int const domain) {
  switch (domain) {
  case AF_UNIX:
    LOG("domain [AF_UNIX]");
    break;
  case AF_INET:
    LOG("domain [AF_INET]");
    break;
  case AF_INET6:
    LOG("domain [AF_INET6]");
    break;
  case AF_IPX:
    LOG("domain [AF_IPX]");
    break;
  case AF_NETLINK:
    LOG("domain [AF_NETLINK]");
    break;
  case AF_X25:
    LOG("domain [AF_X25]");
    break;
  case AF_AX25:
    LOG("domain [AF_AX25]");
    break;
  case AF_ATMPVC:
    LOG("domain [AF_ATMPVC]");
    break;
  case AF_APPLETALK:
    LOG("domain [AF_APPLETALK]");
    break;
  case AF_PACKET:
    LOG("domain [AF_PACKET]");
    break;
  }
}

static void log_parameter_type(int const type) {
  if (type & SOCK_STREAM) {
    LOG("type [SOCK_STREAM]");
  }
  if (type & SOCK_DGRAM) {
    LOG("type [SOCK_DGRAM]");
  }
  if (type & SOCK_SEQPACKET) {
    LOG("type [SOCK_SEQPACKET]");
  }
  if (type & SOCK_RAW) {
    LOG("type [SOCK_RAW]");
  }
  if (type & SOCK_RDM) {
    LOG("type [SOCK_RDM]");
  }
  if (type & SOCK_PACKET) {
    LOG("type [SOCK_PACKET]");
  }
  if (type & SOCK_NONBLOCK) {
    LOG("type [NONBLOCK]");
  }
  if (type & SOCK_CLOEXEC) {
    LOG("type [CLOEXEC]");
  }
}

static void log_parameters(int domain, int type, int protocol) {
  log_parameter_domain(domain);
  log_parameter_type(type);
  LOG("protocol [%d]", protocol);
}

int socket(int domain, int type, int protocol) {
  eval_environment();
  if (debug) {
    LOG("socket() called");
    log_parameters(domain, type, protocol);
  }

  int const socket_fd = (*socket_call_clib)(domain, type, protocol);
  if (socket_fd == -1) {
    LOG("Error socket() call [%m]");
    return socket_fd;
  }
  LOG("socket() call returned fd [%d]", socket_fd);

  // Check some parameters
  if (domain != AF_INET && domain != AF_INET6) {
    LOG("Ignoring TCP KEEPALIVE on a non INET4/6 socket");
    return socket_fd;
  }
  if ((type & SOCK_STREAM) == 0) {
    LOG("Ignoring TCP KEEPALIVE on a non stream socket");
    return socket_fd;
  }

  LOG("Parameters check passed");

  if (use_keepalive == 0) {
    LOG("Keepalive switched off by configuration");
    return socket_fd;
  }

  LOG("Setting KEEPALIVE for socket");

  int const ssopt_ka = setsockopt(socket_fd, SOL_SOCKET, SO_KEEPALIVE,
                                  &use_keepalive, sizeof(use_keepalive));
  if (ssopt_ka == -1) {
    LOG("Setting KEEPALIVE returned error [%m]");
    return socket_fd;
  }

#define SET_SOCK_OPT(ltype, strtype, tcp_type)                                 \
  if (tcp_keepalive_##ltype >= 0) {                                            \
    LOG("Seting " strtype " [%d]", tcp_keepalive_##ltype);                     \
    int const ssopt =                                                          \
        setsockopt(socket_fd, SOL_TCP, tcp_type, &tcp_keepalive_##ltype,       \
                   sizeof(tcp_keepalive_##ltype));                             \
    if (ssopt == -1) {                                                         \
      LOG("Setting " strtype " returned error [%m]");                          \
    }                                                                          \
  }

  SET_SOCK_OPT(time, "TIME", TCP_KEEPIDLE);
  SET_SOCK_OPT(intvl, "INTVL", TCP_KEEPINTVL);
  SET_SOCK_OPT(probes, "PROBES", TCP_KEEPCNT);

#undef SET_SOCK_OPT

  LOG("Finished; returning to caller [%d]", socket_fd);

  return socket_fd;
}
