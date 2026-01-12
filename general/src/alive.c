#define Uses_MEMSET_ZERO_O
#define Uses_alive
#include <general.dep>

void al_init( alive_t * alive )
{
	MEMSET_ZERO_O( alive );
}

LPCSTR al_alive( alive_t * alive , bool read_only )
{
	if ( !read_only )
	{
		alive->alive_indicator[alive->couter] = '.';
		++alive->couter;
		alive->couter %= sizeof( alive->alive_indicator ) - 1;
		alive->alive_indicator[alive->couter] = '*';
	}
	return alive->alive_indicator;
}

