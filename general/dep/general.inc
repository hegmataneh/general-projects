#ifdef __GENERAL
	#define IN_GENERAL _EXPORT
#else
	#define IN_GENERAL _IMPORT
#endif

#ifdef __INC_stddef
	#include <stddef.h>
#endif

#ifdef __INC_time
	#include <time.h>
#endif


#include "../header/g_types.h"
#include "../header/g_macros.h"
#include "../header/g_consts.h"
#include "../header/g_mlk.h"
#include "../header/g_brk.h"

#include "../header/g_sys.h"

#include "../header/g_funcs.h"

#if defined __INC_LOG_H || !defined __COMPILING
	#include "../header/g_log.h"
#endif

#ifdef __INC_sys_types
	#include <sys/types.h>
#endif

#ifdef __INC_stdarg
	#include <stdarg.h>
#endif


#ifdef __INC_stdint
	#include <stdint.h>
#endif

#ifdef __INC_cstdio_
	#include <stdio.h>
#endif

#ifdef __INC_errno
	#include <errno.h>
#endif

#ifdef __INC_stdio
	#include <stdio.h>
#endif


#ifdef __INC_yaml_h
	#include <yaml.h>
#endif

#ifdef __INC_assert_h
	#include <assert.h>
#endif

#ifdef __INC_yaml_private
	#include <yaml_private.h>
#endif

#ifdef __INC_yaml_config
	#include <yaml_config.h>
#endif

#ifdef __INC_string
	#include <string.h>
#endif

#ifdef __INC_stdlib
	#include <stdlib.h>
#endif

#ifdef __INC_stdbool
	#include <stdbool.h>
#endif

#ifdef __INC_pthread
	#include <pthread.h>
#endif


#ifdef __INC_select
	#include <sys/select.h>
#endif

#ifdef __INC_json
	#include <json.h>
#endif


#ifdef __INC_unistd
	#include <unistd.h>
#endif

#ifdef __INC_netinet
	#include <arpa/inet.h>
	#include <netinet/in.h>
#endif

#ifdef __INC_sys_socket
	#include <sys/socket.h>
#endif


