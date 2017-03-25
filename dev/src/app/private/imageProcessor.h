#pragma once
#include <vector>
#include <bitset>

#include "../../utils/public/point.h"

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
		void FilterImage( Matrix33 filter );

		virtual void OnProcessingEnd()
		{ }

		Image* GetOutput() const { return m_output; }

		void AddPass( IProcessingPass* pass );
		Matrix33 SampleSurrounding( Uint32 x, Uint32 y ) const;

	private:
		template< typename PixelType >
		inline void IterateImage( const PixelType* src, PixelType* dst, Uint32 size, IProcessingPass* pass )
		{
			m_currentPos.x = 0;
			m_currentPos.y = 0;

			if ( src == dst )
			{
				for ( Uint32 i = 0; i < size; ++i, ++dst )
				{
					*dst = pass->Process( *dst );
					++m_currentPos.x;
					if ( m_currentPos.x == m_src->GetWidth() )
					{
						m_currentPos.x = 0;
						++m_currentPos.y;
					}
				}
			}
			else
			{
				for ( Uint32 i = 0; i < size; ++i, ++src, ++dst )
				{
					*dst = pass->Process( *dst );
					++m_currentPos.x;
					if ( m_currentPos.x == m_src->GetWidth() )
					{
						m_currentPos.x = 0;
						++m_currentPos.y;
					}
				}
			}
		}

	protected:
		const Image*						m_src;
		Image*								m_output;

		Point2								m_currentPos;

		std::vector< IProcessingPass* >		m_passes;
		Uint32								m_currentPass;
	};
}