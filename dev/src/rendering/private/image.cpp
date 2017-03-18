#include "build.h"
#include "image.h"

#pragma warning( push )
#pragma warning( disable: 4996 )

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#pragma warning( pop )

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gf
{
	namespace helper
	{
		void GrayscaleToRGB( const Uint8* src, Uint8* dst, const Uint32 srcSize )
		{
			for ( Uint32 i = 0; i < srcSize; ++i )
			{
				dst[ i * 3 ] = src[ i ];
				dst[ i * 3 + 1 ] = src[ i ];
				dst[ i * 3 + 2 ] = src[ i ];
			}
		}
	}
	
	Image::Image()
		: m_width( 0 )
		, m_height( 0 )
		, m_data( nullptr )
		, m_size( 0 )
		, m_channels( 0 )
	{
	}

	Image::Image( const Image& other, Bool forceRGB )
	{
		SetImageData( other, forceRGB );
	}

	Image::~Image()
	{
		delete[] m_data;
		m_data = nullptr;
	}

	void Image::Load( const char * fileName )
	{
		Int32 w, h, comp;

		Uint8* result = stbi_load( fileName, &w, &h, &comp, STBI_rgb );
		SC_ASSERT( result, "No image loaded" );
		SC_ASSERT( comp == STBI_rgb, "Only 3 channels save/load is supported" );

		m_channels = comp;
		m_data = result;
		m_width = w;
		m_height = h;
		m_size = w * h;
	}

	void Image::Save( const char* fileName )
	{
		stbi_write_png( fileName, m_width, m_height, STBI_rgb, m_data, 0 );
	}


	void Image::SetData( void* data, Uint32 size )
	{
		SC_ASSERT( GetDataSize() == size, "Inconsistent data size" );
		memcpy( m_data, data, size );
	}

	void Image::SetImageData( const Image& src, Bool forceRGB )
	{
		m_channels = src.m_channels;
		m_width = src.m_width;
		m_height = src.m_height;

		m_size = src.m_size;

		if ( forceRGB && m_channels != 3 )
		{
			SC_ASSERT( m_channels == 1, "There is only from greyscale convertion" );
			SC_ASSERT( src.GetSize() == src.GetDataSize(), "Number of bytes must match number of elements." );

			m_channels = 3;

			// we assume that source image has 1 channel
			m_data = static_cast< Uint8* >( realloc( m_data, m_size * m_channels ) );
			helper::GrayscaleToRGB( static_cast< Uint8* >( src.GetData() ), m_data, m_size );
		}
		else
		{
			// create simple copy
			void* srcPtr = src.GetData();
			const Uint32 size = src.GetDataSize();

			delete m_data;
			m_data = new Uint8[ size ];
			memcpy( m_data, srcPtr, size );
		}
	}

	Image* Image::GenerateGreyscale() const
	{
		Image* result = new Image();
		result->m_data = new Uint8[ m_size ];
		for ( Uint32 i = 0; i < m_size; ++i )
		{
			result->m_data[ i ] = ( m_data[ i * 3 ] + m_data[ i * 3 + 1 ] + m_data[ i * 3 + 2 ] ) / 3;
		}

		result->m_size = m_size;
		result->m_channels = 1;
		result->m_width = m_width;
		result->m_height = m_height;
		return result;
	}
}