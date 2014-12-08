// lazybee engine
// (C) 2014 Michael Rieder
//

#include "main.h"

void filestream::open( const char *fname )
{
	if (fname != NULL)
		fp = fopen( fname, "rb" );
	if (fp == NULL)
		con_printf( "error opening %s\n", fname );
}

void filestream::close( void )
{
	if (fp)
		fclose( fp );
}

size_t filestream::read( void *buffer, size_t size )
{
	size_t bytes_read;
	if (fp)
		bytes_read = fread( buffer, 1, size, fp );
	else
		bytes_read = 0;
	return bytes_read;
}
