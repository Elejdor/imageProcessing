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
}