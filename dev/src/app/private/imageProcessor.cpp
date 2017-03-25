#include "build.h"
#include "imageProcessor.h"
#include "processingPass.h"
#include "../../rendering/public/image.h"

namespace gf
{
	namespace helper
	{
		Int32 Combine( const ColorMatrix& a, const FilterMatrix& b )
		{
			Int32 result = 0;
			for ( Uint8 i = 0; i < 9; ++i )
			{
				result += a.arr[ i ] * b.arr[ i ];
			}

			if ( result < 0 )
				return 0;
			if ( result > 255 )
				return 255;

			return result;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ImageProcessor
	//////////////////////////////////////////////////////////////////////////
	ImageProcessor::ImageProcessor()
		: m_src( nullptr )
		, m_output( nullptr )
		, m_currentPass( 0 )
	{ }

	ImageProcessor::~ImageProcessor()
	{
		delete m_output;
		m_output = nullptr;
	}

	void ImageProcessor::SetImage( const Image* img )
	{
		if ( m_output != nullptr )
		{
			delete m_output;
			m_output = nullptr;
		}

		m_src = img;
		m_output = new Image( *m_src, false );
	}

	void ImageProcessor::ProcessImage( Bool outputIsSource )
	{
		SC_ASSERT( m_src, "No source image provided" );

		for ( IProcessingPass* pass : m_passes )
		{
			if ( !pass->OnStarted( this ) )
			{
				pass->OnFinished( this );
				++m_currentPass;
				continue;
			}

			if ( m_src->GetChannels() == 1 )
			{
				if ( outputIsSource )
				{
					Uint8* dataPtr = static_cast< Uint8* >( m_output->GetData() );
					IterateImage< Uint8 >( dataPtr, dataPtr, m_output->GetSize(), m_passes[ m_currentPass ] );
				}
				else
				{
					Uint8* srcPtr = static_cast< Uint8* >( m_src->GetData() );
					Uint8* outPtr = static_cast< Uint8* >( m_output->GetData() );

					// for every pixel
					IterateImage< Uint8 >( srcPtr, outPtr, m_src->GetSize(), m_passes[ m_currentPass ] );
				}
			}
			else
			{
				SC_ASSERT( m_src->GetChannels() == 3, "Only RGB and greyscale." );

				if ( outputIsSource )
				{
					Color3* dataPtr = static_cast< Color3* >( m_output->GetData() );
					IterateImage< Color3 >( dataPtr, dataPtr, m_src->GetSize(), m_passes[ m_currentPass ] );
				}
				else
				{
					Color3* srcPtr = static_cast< Color3* >( m_src->GetData() );
					Color3* outPtr = static_cast< Color3* >( m_output->GetData() );

					// for every pixel
					IterateImage< Color3 >( srcPtr, outPtr, m_src->GetSize(), m_passes[ m_currentPass ] );
				}
			}

			++m_currentPass;

			if ( !pass->OnFinished( this ) )
				break;
		}

		m_currentPass = 0;
	}

	void ImageProcessor::FilterImage( const FilterMatrix filter )
	{
		const Uint32 w = m_src->GetWidth() - 1;
		const Uint32 h = m_src->GetHeight() - 1;

		Uint8 filterSum = 0;
		for ( Uint8 i = 0; i < 9; ++i )
			filterSum += filter.arr[ i ];

		if ( filterSum == 0 )
			filterSum = 1;

		ColorMatrix currentPixels;
		Uint32 pixelValue = 0;
		for ( Uint32 i = 1; i < h; ++i )
		{
			for ( Uint32 j = 1; j < w; ++j )
			{
				currentPixels = SampleSurrounding( j, i );
				pixelValue = helper::Combine( currentPixels, filter ) / filterSum;
				m_output->SetValue< Uint8 >( ( Uint8 )pixelValue, j, i );
			}
		}
	}

	void ImageProcessor::AddPass( IProcessingPass* pass ) 
	{
		pass->OnEffectRegistered( this );
		m_passes.push_back( pass ); 
	}

	ColorMatrix ImageProcessor::SampleSurrounding( Uint32 x, Uint32 y ) const
	{
		ColorMatrix result;
		--y;
		result.arr[ 0 ] = m_src->GetValue< Uint8 >( x - 1, y );
		result.arr[ 1 ] = m_src->GetValue< Uint8 >( x, y );
		result.arr[ 2 ] = m_src->GetValue< Uint8 >( x + 1, y );

		++y;
		result.arr[ 3 ] = m_src->GetValue< Uint8 >( x - 1, y );
		result.arr[ 4 ] = m_src->GetValue< Uint8 >( x, y );
		result.arr[ 5 ] = m_src->GetValue< Uint8 >( x + 1, y );

		++y;
		result.arr[ 6 ] = m_src->GetValue< Uint8 >( x - 1, y );
		result.arr[ 7 ] = m_src->GetValue< Uint8 >( x, y );
		result.arr[ 8 ] = m_src->GetValue< Uint8 >( x + 1, y );
		return result;
	}
}
