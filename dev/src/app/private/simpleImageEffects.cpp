#include "build.h"
#include "simpleImageEffects.h"
#include "imageProcessor.h"
#include "../../rendering/public/image.h"

namespace gf
{
	namespace effects
	{
		//////////////////////////////////////////////////////////////////////////
		// CalculateAvg
		//////////////////////////////////////////////////////////////////////////
		Bool CalculateAvg::OnStarted( ImageProcessor* processor )
		{
			if ( m_bake && m_calculated )
				return false;

			m_result = 0;
			return true;
		}

		Bool CalculateAvg::OnFinished( ImageProcessor* proc )
		{
			m_result /= proc->GetOutput()->GetDataSize();
			m_calculated = true;
			return true;
		}

		Uint8 CalculateAvg::Process( Uint8 input )
		{
			m_result += input;
			return input;
		}

		Color3 CalculateAvg::Process( Color3 input )
		{
			m_result += ( input.r + input.g + input.b );
			return input;
		}

		//////////////////////////////////////////////////////////////////////////
		// ChangeBrightness
		//////////////////////////////////////////////////////////////////////////
		Bool ChangeBrightness::OnStarted( ImageProcessor * proc )
		{
			Uint8 val = 0;
			if ( m_brightness > 0 )
			{
				for ( Uint16 i = 0; i < 256; ++i )
				{
					val = ( Uint8 )i;
					if ( ( 255 - val ) < m_brightness )
						m_valueLUT[ i ] = 255;
					else
						m_valueLUT[ i ] = val + m_brightness;
				}
			}
			else if ( m_brightness < 0 )
			{
				for ( Uint16 i = 0; i < 256; ++i )
				{
					val = ( Uint8 )i;
					if ( val < ( -m_brightness ) )
						m_valueLUT[ i ] = 0;
					else
						m_valueLUT[ i ] = val + m_brightness;
				}
			}
			else
			{
				for ( Uint16 i = 0; i < 256; ++i )
					m_valueLUT[ i ] = ( Uint8 )i;
			}
			
			return true;
		}

		Uint8 ChangeBrightness::Process( Uint8 input )
		{
			return m_valueLUT[ input ];
		}

		Color3 ChangeBrightness::Process( Color3 input )
		{
			return Color3();
		}
	}
}