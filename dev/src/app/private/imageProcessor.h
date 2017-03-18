#pragma once
#include <vector>
#include <bitset>

#include "../../utils/public/point.h"

namespace gf
{
	struct Matrix33
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

	struct SurroundingInfo
	{
		Matrix33			values;
		std::bitset< 9 >	mask;
	};

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

		const SurroundingInfo& GetCurrentSurrounding();

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
		SurroundingInfo					m_surrounding;
		const Image*					m_src;
		Image*							m_output;

		Point2							m_currentPos;

		std::vector< IProcessingPass* >	m_passes;
		Uint32							m_currentPass;
	};
}