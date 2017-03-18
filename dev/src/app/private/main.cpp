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


	gf::ImageProcessor imageEffect;
	imageEffect.SetImage( grey );

	// brightness pass
	gf::effects::ChangeBrightness pass0;
	pass0.SetBrightness( 0 );
	imageEffect.AddPass( &pass0 );

	// contrast pass
	gf::effects::CalculateAvg avgPass;
	gf::effects::ChangeContrast contrastPass;
	contrastPass.SetAvg( &avgPass );
	contrastPass.SetContrast( 10 );
	//imageEffect.AddPass( &avgPass );
	//imageEffect.AddPass( &contrastPass );

	// negate
	gf::effects::Negate negatePass;
	//imageEffect.AddPass( &negatePass );

	// normalize
	gf::effects::Normalize normalizePass;
	gf::effects::CalcRange range;
	normalizePass.SetRange( &range );
	normalizePass.SetNewRange( 0, 100 );

	imageEffect.AddPass( &range );
	imageEffect.AddPass( &normalizePass );

	imageEffect.ProcessImage( true );
	
	gf::Image renderImg;
	renderImg.SetImageData( *imageEffect.GetOutput(), true );
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