#pragma once
#include <vector>

namespace gf
{
	class Image;
	class IProcessingPass;

	class ImageProcessor
	{
	public:
		typedef Uint8( *ValueFunc )( Uint8 );
		typedef Color3( *ColorFunc )( Color3 );

		ImageProcessor();
		~ImageProcessor();

		void SetImage( const Image* img );

		void ProcessImage( Bool outputIsSource = false );

		virtual void OnProcessingEnd()
		{ }

		Image* GetOutput() const { return m_output; }

		void AddPass( IProcessingPass* pass ) { m_passes.push_back( pass ); }

	private:
		template< typename PixelType >
		inline void IterateImage( const PixelType* src, PixelType* dst, Uint32 size, IProcessingPass* pass )
		{
			if ( src == dst )
			{
				for ( Uint32 i = 0; i < size; ++i, ++dst )
					*dst = pass->Process( *dst );
			}
			else
			{
				for ( Uint32 i = 0; i < size; ++i, ++src, ++dst )
					*dst = pass->Process( *dst );
			}
		}

	protected:
		const Image*					m_src;
		Image*							m_output;

		std::vector< IProcessingPass* >	m_passes;
		Uint32							m_currentPass;
	};
}