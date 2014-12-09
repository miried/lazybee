// lazybee engine
// (C) 2014 Michael Rieder
//

#include <cstdarg>
#include "main.h"
#include "bspmap.h"

void con_printf( const char *string, ... )
{
	va_list vl;
	va_start(vl,string);
	vprintf(string,vl);
	va_end(vl);
}

int main( int argc, char *argv[] )
{
	bspmap *map = new bspmap("mohdm2.bsp");

	return EXIT_SUCCESS;
}
