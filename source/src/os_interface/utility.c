//
// utility.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./utility.h"


inline int GetStringKey(const char* key)
{
	int hash = 0;
	while(*key)
	{
		hash = (*key) + (hash << 6) + (hash << 16) - hash;
		key++;
	}
	return hash;
}