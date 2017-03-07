#pragma once
#include "commonUtils.h"

namespace sc
{
	Uint64 GetHash64( const char* text )
	{
		const Uint64 FNV_prime = 0x100000001b3;
		
		Uint64 hash = 0xcbf29ce484222325;
		const char* c = text;
		while ( *c )
		{
			hash ^= *c;
			hash *= FNV_prime;
			++c;
		}

		return hash;
	}


	Uint32 GetHash32( const char* text )
	{
		const Uint32 FNV_prime = 0x1000193;

		Uint32 hash = 0x811C9DC5;

		const char* c = text;
		while ( *c )
		{
			hash ^= *c;
			hash *= FNV_prime;
			++c;
		}

		return hash;
	}
}