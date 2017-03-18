#include "build.h"

#include "../../rendering/public/rendering.h"
#include "../../rendering/public/gfViewport.h"
#include "../../rendering/public/image.h"

#include <stdio.h>

struct Pixel
{
	union 
	{
		Uint8 arr[ 3 ];
		struct
		{
			Uint8 r, g, b;
		};
	};

};

void IterateImage( void( *func )( Pixel& pixel ), Image& img )
{
	const Uint32 len = img.GetSize();
	for ( Uint32 i = 0; i < len; ++i )
	{
		void* dataPtr = img.GetData() + sizeof( Pixel ) * i;
		Pixel* px = static_cast< Pixel* >( dataPtr );
		func( *px );
	}
}

void IncreaseBrightness( Pixel& px )
{
	const Uint8 amount = 80;
	for ( Uint8 i = 0; i < 3; ++i )
	{
		if ( 255 - px.arr[ i ] > amount )
		{
			px.arr[ i ] += amount;
		}
		else
		{
			px.arr[ i ] = 255;
		}
	}
}

int main( int, char** )
{
	rendering::Renderer renderer;
	renderer.Init();

	Image iss;
	if ( iss.GetData() == nullptr )
		iss.Load( "iss.jpeg" );

	IterateImage( IncreaseBrightness, iss );

	renderer.SetImage( &iss );

	// Main loop
	while ( renderer.Render() );

	return 0;
}