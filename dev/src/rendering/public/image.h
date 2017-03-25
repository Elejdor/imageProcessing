#pragma once

namespace gf
{
	class Image
	{
	public:
		Image();
		Image( const Image& other, Bool forceRGB = true );

		~Image();

		void Load( const char* fileName );
		void Save( const char* fileName );

		void SetData( void* data, Uint32 size );
		void SetImageData( const Image& src, Bool forceRGB = true );
		void* GetData() const { return m_data; }

		Uint32 GetDataSize() const { return m_channels * m_size; }
		Uint32 GetSize() const { return m_size; }

		template< typename ValueT >
		inline ValueT GetValue( Uint32 x, Uint32 y ) const;

		template< typename ValueT >
		inline void SetValue( ValueT val, Uint32 x, Uint32 y ) const;

		Uint32 GetWidth() const { return m_width; }
		Uint32 GetHeight() const { return m_height; }

		Image* GenerateGreyscale() const;
		Uint32 GetChannels() const { return m_channels; }

	private:
		Uint8* m_data;

		Uint32 m_width;
		Uint32 m_height;
		Uint32 m_size;
		Uint32 m_channels;
	};

	template< typename ValueT >
	inline ValueT Image::GetValue( Uint32 x, Uint32 y ) const
	{
		SC_ASSERT( m_channels == sizeof( ValueT ), "Channels number missmatch" );
		return static_cast< ValueT* >( m_data )[ x + y * m_width ];
	}

	template< typename ValueT >
	inline void Image::SetValue( ValueT val, Uint32 x, Uint32 y ) const
	{
		SC_ASSERT( m_channels == sizeof( ValueT ), "Channels number missmatch" );
		ValueT* data = static_cast< ValueT* >( m_data ) + ( x + y * m_width );
		*data = val;
	}
}