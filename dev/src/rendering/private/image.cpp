#include "build.h"
#include "image.h"

#pragma warning( push )
#pragma warning( disable: 4996 )

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma warning( pop )

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image()
	: m_width( 0 )
	, m_height( 0 )
	, m_data( nullptr )
	, m_size( 0 )
{
}

Image::~Image()
{
	delete m_data;
	m_data = nullptr;
}

void Image::Load( const char * fileName )
{
	Int32 w, h, comp;

	Uint8* result = stbi_load( fileName, &w, &h, &comp, STBI_rgb );
	SC_ASSERT( result, "No image loaded" );
	SC_ASSERT( comp == STBI_rgb, "Only 3 channels save/load is supported" );

	m_data = result;
	m_width = w;
	m_height = h;
	m_size = w * h;
}

void Image::Save( const char * fileName )
{
	stbi_write_png( fileName, m_width, m_height, STBI_rgb, m_data, 0 );
}
