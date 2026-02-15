#pragma once

#if defined Uses_issuesMaster_handle || !defined __COMPILING

typedef struct s_issue_handle
{
	char * issue_msg;
	time_t arrive_time;
	uint16 timeout_sec;
	bool retired; // every body must hand offs him

} issue_h;

typedef struct s_issuesMaster_handle
{
	dyn_mms_arr issues_ar;
	pthread_mutex_t pr_lock;
	uint16 default_timeout_sec;

} issues_h;

status issue_init( issues_h * hissues , size_t default_timeout_sec , size_t max_issues_cnt , Brief_Err * imortalErrStr );
void issue_cleanup( issues_h * hissues );

void issue_add( issues_h * hissues , LPCSTR issue_msg ); // i intentionally make return void to sealed any more return from this obj

void issue_retire( issues_h * hissues );
void issue_garbage_collector( issues_h * hissues );



#endif
