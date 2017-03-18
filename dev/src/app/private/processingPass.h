#pragma once

namespace gf
{
	class ImageProcessor;

	class IProcessingPass
	{
	public:
		// return value tells whether to apply this pass
		virtual Bool OnStarted( ImageProcessor* proc ) { return true; }

		// return value tells whether to proceed to the next pass
		virtual Bool OnFinished( ImageProcessor* proc ) { return true; }

		virtual Uint8 Process( Uint8 input ) = 0;
		virtual Color3 Process( Color3 input ) = 0;
	};
}