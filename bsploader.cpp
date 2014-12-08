// lazybee engine
// (C) 2014 Michael Rieder
//

#include "main.h"
#include "bsploader.h"

void bspmap::open( const char* mname )
{
	size_t n;
	char buffer[16];
	filestream *mapfile;

	memset(buffer, 'x', 16);
	mapfile = new filestream( mname );
	n = mapfile->read( buffer, sizeof(buffer) );
	buffer[4] = '\n';
	buffer[5] = 0;
	
	con_printf( buffer );
con_printf( "%is\n",BUFSIZ);
	delete mapfile;
}
