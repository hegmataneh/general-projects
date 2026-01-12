#pragma once

#if defined Uses_alive || !defined __COMPILING

typedef struct simple_alive_indicator /*8 byte aligned*/
{
	uint8 couter;
	char alive_indicator[7];

} alive_t;

void al_init( alive_t * alive );
LPCSTR al_alive( alive_t * alive , bool read_only );

#endif
