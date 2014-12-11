// lazybee engine
// (C) 2014 Michael Rieder
//

#include "main.h"
#include "bspmap.h"

void bspmap::lump_loadentities( void )
{
	// seek the lump
	uint32_t offset, length;

	offset = lumps[lump_entities].offset;
	length = lumps[lump_entities].length;

	mapfile->seek( offset );

	// the string should be 0-terminated but we'll make sure
	entityString = new char[length+1];
	entityString[length] = 0;
	
	// read the lump
	mapfile->read( entityString, length );
}

void bspmap::lump_loadleafs( void )
{
	// seek the lump
	uint32_t offset, length;

	offset = lumps[lump_leafs].offset;
	length = lumps[lump_leafs].length;

	// sanity check
	if (length % sizeof(dleaf_s)) {
		con_printf( "weird leaf lump size!\n" );
		return;
	}

	mapfile->seek( offset );

	// read the data
	numleafs = length / sizeof(dleaf_s);
	leafs = new dleaf_s[numleafs];

	mapfile->read( leafs, length );

	// count the clusters and areas
	numclusters = 0;
	numareas = 0;
	
	for (int i=0;i<numleafs;i++) {
		dleaf_s *leaf = leafs+i;
		if (leaf->cluster >= numclusters)
			numclusters = leaf->cluster + 1;
		if (leaf->area >= numareas)
			numareas = leaf->area + 1;
	}

	con_printf( "map has %i leafs, %i clusters, %i areas\n",
		numleafs, numclusters, numareas );
}

void bspmap::lump_loadleafsurfaces( void )
{
	// seek the lump
	uint32_t offset, length;

	offset = lumps[lump_leafsurfaces].offset;
	length = lumps[lump_leafsurfaces].length;

	// sanity check
	if (length % sizeof(uint32_t)) {
		con_printf( "weird leafsurfaces lump size!\n" );
		return;
	}

	numleafsurfaces = length / sizeof(uint32_t);
	leafsurfaces = new uint32_t[numleafsurfaces];
	mapfile->seek( offset );
	mapfile->read( leafsurfaces, length );
	con_printf( "%i leafsurfaces\n", numleafsurfaces );
}

void bspmap::lump_loadsurfaces( void )
{
	// seek the lump
	uint32_t offset, length;

	offset = lumps[lump_surfaces].offset;
	length = lumps[lump_surfaces].length;

	// sanity check
	if (length % sizeof(dsurface_s)) {
		con_printf( "weird surfaces lump size!\n" );
		return;
	}

	numsurfaces = length / sizeof(dsurface_s);
	surfaces = new dsurface_s[numsurfaces];
	mapfile->seek( offset );
	mapfile->read( surfaces, length );
	con_printf( "%i surfaces\n", numsurfaces );
}

void bspmap::open( const char* mname )
{
	mapfile = new filestream( mname );
	mapfile->read( &header, sizeof(header) );

	if (memcmp(header.id,"2015",4)==0)
		con_printf( "loading '2015' map\n" );
	else {
		con_printf( "unknown map format %c%c%c%c\n",
			header.id[0],header.id[1],header.id[2],header.id[3] );
	}
	con_printf( "map is BSP v%.2i\n", header.version );
	
	if (header.version == 19)
		con_printf( "format is MOHAA v1.12, checksum %#.8x\n",
				header.checksum );
	
	mapfile->read( lumps, sizeof(lumps) );
	
	lump_loadentities();
	lump_loadleafs();
	lump_loadleafsurfaces();
	lump_loadsurfaces();
}

void bspmap::close( void )
{
	delete[] entityString;
	delete[] leafs;
	delete[] leafsurfaces;
	delete[] surfaces;
	delete mapfile;
}
