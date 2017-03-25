#pragma once

typedef long long Int64;
typedef unsigned long long Uint64;

typedef int Int32;
typedef unsigned int Uint32;

typedef short Int16;
typedef unsigned short Uint16;

typedef char Int8;
typedef unsigned char Uint8;

typedef char AnsiChar;

typedef float Float;
typedef double Double;

typedef bool Bool;

struct Color3
{
	union
	{
		struct
		{
			Uint8 r, g, b;
		};

		Uint8 arr[ 3 ];
	};
};

struct ColorMatrix
{
	union
	{
		struct
		{
			Uint8 m00, m10, m20;
			Uint8 m01, m11, m21;
			Uint8 m02, m12, m22;
		};

		Uint8 arr[ 9 ];
	};
};

struct FilterMatrix
{
	union
	{
		struct
		{
			Int8 m00, m10, m20;
			Int8 m01, m11, m21;
			Int8 m02, m12, m22;
		};

		Int8 arr[ 9 ];
	};

};