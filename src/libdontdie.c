#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <dlfcn.h>

#define LOG(msg...)                                                            \
  do {                                                                         \
    if (debug > 0) {                                                           \
      syslog(LOG_USER | LOG_INFO, "libdontdie: " msg);                         \
    }                                                                          \
  } while (0)

int (*socket_call_clib)(int domain, int type, int protocol);

int debug = 0;
int eval_environment_once = 0;
int tcp_keepalive_time = -1;
int tcp_keepalive_intvl = -1;
int tcp_keepalive_probes = -1;

static eval_environment() {
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
  if (str_eval_environment_once == NULL || *str_eval_environment_once == '1') {
    eval_environment_once = 1;
    LOG("Evaluate environment only once");
  }

#define EVAL_ENV(ltype, strtype)                                               \
  char const *const str_tcp_keepalive_##ltype =                                \
      getenv("DD_TCP_KEEPALIVE_" strtype);                                     \
  if (str_tcp_keepalive_##ltype != NULL) {                                     \
    tcp_keepalive_##ltype = atoi(str_tcp_keepalive_##ltype);                   \
    LOG("Set TCP_KEEPALIVE_" strtype "= [%d]", tcp_keepalive_##ltype);         \
  }

  EVAL_ENV(time, "TIME");
  EVAL_ENV(intvl, "INTVL");
  EVAL_ENV(probes, "PROBES");

#undef EVAL_ENV
}

__attribute__((constructor)) void libdontdie_init() {
  eval_environment();

  socket_call_clib = dlsym(RTLD_NEXT, "socket");
}
