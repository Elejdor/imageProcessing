#pragma once
#include "macros.h"
#include "types.h"

struct Point2
{
	Int32 x, y;

	Point2()
		: x( 0 )
		, y( 0 )
	{ }

	Point2( Int32 x, Int32 y )
		: x( x )
		, y( y )
	{ }

	Point2 operator-( Point2 other )
	{
		Point2 res( x - other.x, y - other.y );
		return res;
	}

	Point2 operator+( Point2 other )
	{
		Point2 res( x + other.x, y + other.y );
		return res;
	}

	void operator-=( Point2 other )
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