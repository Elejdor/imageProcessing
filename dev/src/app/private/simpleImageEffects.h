#pragma once
#include "processingPass.h"

namespace gf
{
	class ImageProcessor;

	namespace effects
	{
		class CalculateAvg : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( gf::ImageProcessor* proc ) override;
			virtual Bool OnFinished( gf::ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			Uint32 GetResult() const { return m_result; }

		private:
			Uint32	m_result;
			Bool	m_calculated;
			Bool	m_bake;
		};

		class ChangeBrightness : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void SetBrightness( Int32 brightness ) { m_brightness = brightness; }

		private:
			Uint8  m_valueLUT[ 256 ];
			Int32  m_brightness;
		};
	}
}