/*
 * files.cpp - file io subsystem
 *
 * Copyright (C) 2014 Michael Rieder
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

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

void filestream::seek( long int offset, bool relative )
{
	if (fp==NULL)
		return;
	if (relative==false)
		fseek( fp, offset, SEEK_SET );
	else
		fseek( fp, offset, SEEK_CUR );
}
