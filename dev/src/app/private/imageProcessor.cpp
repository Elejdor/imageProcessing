#include "build.h"
#include "imageProcessor.h"
#include "processingPass.h"
#include "../../rendering/public/image.h"

namespace gf
{
	ImageProcessor::ImageProcessor()
		: m_src( nullptr )
		, m_output( nullptr )
		, m_currentPass( 0 )
	{ }

	ImageProcessor::~ImageProcessor()
	{
		delete m_output;
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
					IterateImage< Uint8 >( dataPtr, dataPtr, m_src->GetSize(), m_passes[ m_currentPass ] );
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

	const SurroundingInfo& ImageProcessor::GetCurrentSurrounding()
	{
		Point2 pos = m_currentPos - Point2( 1, 1 );

		const Uint8* dataPtr = static_cast< Uint8* >( m_src->GetData() );

		Uint8 row = 0;
		for ( Uint8 i = 0; i < 3; ++i )
		{
			if ( ( Uint32 )pos.x >= 0 && ( Uint32 )pos.x < m_src->GetWidth() )
			{
				if ( ( Uint32 )pos.y >= 0 && ( Uint32 )pos.x < m_src->GetHeight() )
				{
					const Uint8* const dataPtr = static_cast< Uint8* >( m_src->GetData() );
					m_surrounding.values.arr[ i + row * 3 ] = dataPtr[ pos.x + pos.y * m_src->GetWidth() ];
					m_surrounding.mask[ i + row * 3 ] = 1;
				}
				else
				{
					m_surrounding.mask[ i + row * 3 ] = 0;
				}
			}

			++pos.x;
			if ( i == 2 )
			{
				if ( row == 2 )
					break;

				++pos.y;
				++row;
			}
		}

		return m_surrounding;
	}

	////////////////////////////////////////////////////////////////////////////
	//// BrightnessProcessor
	////////////////////////////////////////////////////////////////////////////
	//Uint8 BrightnessProcessor::ProcessValues( Uint8 input )
	//{
	//	static const int inc = 50;
	//	if ( 255 - input > inc )
	//	{
	//		return input + inc;
	//	}
	//	else
	//	{
	//		return 255;
	//	}
	//}
}
