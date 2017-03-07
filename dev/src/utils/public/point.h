#pragma once
#include "macros.h"
#include "types.h"

struct Point2
{
	int x, y;

	void operator+( Point2 other )
	{
		x += other.x;
		y += other.y;
	}

	void operator-( Point2 other )
	{
		x -= other.x;
		y -= other.y;
	}

	void operator+=( Point2 other )
	{
		x += other.x;
		y += other.y;
	}

	SC_INLINE Bool operator==( Point2 other )
	{
		return ( other.x == x ) && ( other.y == y );
	}

	SC_INLINE Bool operator!=( Point2 other )
	{
		return !( other == *this );
	}

};