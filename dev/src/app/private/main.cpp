#include "build.h"

#include "../../rendering/public/rendering.h"
#include "../../rendering/public/gfViewport.h"
#include "../../rendering/public/image.h"

#include <stdio.h>
#include "imageProcessor.h"
#include "simpleImageEffects.h"
#include "gui.h"

gf::GuiDrawer gui;

typedef std::unique_ptr< gf::Image > ImageUniquePtr;
typedef std::shared_ptr< gf::Image > ImagePtr;

rendering::Renderer g_renderer;

void SetMainImage( gf::Image* img )
{
	g_renderer.SetImage( img );
}

int main( int, char** )
{
	g_renderer.Init();
	
	ImageUniquePtr iss( new gf::Image() );

	if ( iss->GetData() == nullptr )
		iss->Load( "iss.jpeg" );

	g_renderer.SetImage( iss->GenerateGreyscale() );

	ImagePtr grey( iss->GenerateGreyscale() );

	gui.SetImage( grey.get() );
	g_renderer.SetGui( &gui );

	while ( g_renderer.Render() );

	return 0;
}