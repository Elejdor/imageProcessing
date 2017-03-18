#include "build.h"

#include "../../rendering/public/rendering.h"
#include "../../rendering/public/gfViewport.h"
#include "../../rendering/public/image.h"

#include <stdio.h>
#include "imageProcessor.h"
#include "simpleImageEffects.h"


void Test1( rendering::Renderer* renderer )
{
	gf::Image* iss = new gf::Image();
	if ( iss->GetData() == nullptr )
		iss->Load( "iss.jpeg" );

	gf::Image* grey = iss->GenerateGreyscale();

	gf::ImageProcessor brightener;
	brightener.SetImage( grey );

	gf::effects::ChangeBrightness pass0;
	pass0.SetBrightness( -100 );
	brightener.AddPass( &pass0 );
	brightener.ProcessImage( true );
	
	gf::Image renderImg;
	renderImg.SetImageData( *brightener.GetOutput(), true );
	renderer->SetImage( &renderImg );

	delete grey;
	delete iss;
}

int main( int, char** )
{
	rendering::Renderer renderer;
	renderer.Init();

	Test1( &renderer );

	// Main loop
	while ( renderer.Render() );

	return 0;
}