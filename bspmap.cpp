// lazybee engine
// (C) 2014 Michael Rieder
//

#include "main.h"
#include "bspmap.h"


void bspmap::open( const char* mname )
{
	filestream *mapfile;
	bspheader_t header;
	lump_t	lumps[lump_max];

	mapfile = new filestream( mname );
	mapfile->read( &header, sizeof(header) );

	if (memcmp(header.id,"2015",4)==0)
		con_printf( "loading 2015 map\n" );
	else {
		con_printf( "unknown map format %c%c%c%c\n",
			header.id[0],header.id[1],header.id[2],header.id[3] );
	}
	
	mapfile->read( lumps, sizeof(lumps) );
	
	

	delete mapfile;
}
