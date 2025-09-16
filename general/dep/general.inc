#ifdef __GENERAL
	#define IN_GENERAL _EXPORT
#else
	#define IN_GENERAL _IMPORT
#endif

#include "../header/g_types.h"
#include "../header/g_macros.h"
#include "../header/g_consts.h"
#include "../header/g_sys.h" 
#include "../header/g_funcs.h"
#include "../header/g_brk.h"


#ifdef __INC_DGB
	#include <dbg.h>
#endif

#ifdef __INC_struct
	#include <struct.h>
#endif

#ifdef __INC_str
	#include <str.h>
#endif


#ifdef __INC_yaml_h
	#include <yaml.h>
#endif

#ifdef __INC_yaml_private
	#include <yaml_private.h>
#endif

#ifdef __INC_yaml_config
	#include <yaml_config.h>
#endif

#ifdef __INC_json
	#include <json.h>
#endif

#ifdef __INC_cirbuf_packet_sync
	#include <cirbuf_packet_sync.h>
#endif

#ifdef __INC_cirbuf_metric
	#include <cirbuf_metric.h>
#endif

#ifdef __INC_LamportBakeryAlgorithm
	#include <LamportBakeryAlgorithm.h>
#endif

#ifdef __INC_cirbuf_lockfree
	#include <cirbuf_lockfree.h>
#endif

#ifdef __INC_dict_s_s
	#include <dict.h>
#endif

#ifdef __INC_id_dict
	#include <id_dict.h>
#endif

#ifdef __INC_vcbuf_popblk
	#include <vcbuf_popblk.h>
#endif

#ifdef __INC_vcbuf_nonblk
	#include <vcbuf_nonblk.h>
#endif

#ifdef __INC_pub_sub
	#include <pub_sub.h>
#endif

#ifdef __INC_TokenRing_cl
	#include <TokenRing_cl.h>
#endif

#ifdef __INC_TokenRing_p
	#include <TokenRing_p.h>
#endif

#ifdef __INC_dyn_arr
	#include <dyn_arr.h>
#endif

#ifdef __INC_dict_o
	#include <dict_o.h>
#endif

#ifdef __INC_dict_s_i
	#include <dict_s_i.h>
#endif

#ifdef __INC_dict_s_o
	#include <dict_s_o.h>
#endif

#ifdef __INC_cirbuf_infinite
	#include <cirbuf_infinite.h>
#endif

#ifdef __dict_fst
	#include <dict_fst.h>
#endif

