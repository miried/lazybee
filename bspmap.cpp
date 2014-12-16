// lazybee engine
// (C) 2014 Michael Rieder
//

#include "main.h"
#include "bspmap.h"


void bspmap::load_lump( lumpdata_s *lump )
{
	// seek the lump
	uint32_t offset, length;

	offset = lumps[lump->lumptype].offset;
	length = lumps[lump->lumptype].length;

	// sanity check
	if (length % lump->blocksize) {
		con_printf( "weird size in lump %i!\n", lump->lumptype );
		con_printf( "blocksize %i for length %i\n", lump->blocksize, length );
		*lump->numblocks = 0;
		*lump->ptr = NULL;
		return;
	}

	*lump->numblocks = length / lump->blocksize;
	*lump->ptr = operator new(length);
	mapfile->seek( offset );
	mapfile->read( *lump->ptr, length );
}

void bspmap::load_all_lumps( void )
{
	lumpdata_s lump;

	// 0 - shaders
	lump.lumptype = lump_shaders;
	lump.blocksize = sizeof(dshader_s);
	lump.numblocks = &numshaders;
	lump.ptr = (void**)&shaders;
	load_lump( &lump );
	con_printf( "%i shaders\n", numshaders );

	// 1 - planes
	lump.lumptype = lump_planes;
	lump.blocksize = sizeof(dplane_s);
	lump.numblocks = &numplanes;
	lump.ptr = (void**)&planes;
	load_lump( &lump );
	con_printf( "%i surfaces\n", numplanes );

	// 2 - lightmaps
	lump.lumptype = lump_lightmaps;
	lump.blocksize = LIGHTMAP_BLOCK_LEN;
	lump.numblocks = &numlightmaps;
	lump.ptr = (void**)&lightmapdata;
	load_lump( &lump );
	con_printf( "%i lightmaps\n", numlightmaps );

	// 3 - surfaces
	lump.lumptype = lump_surfaces;
	lump.blocksize = sizeof(dsurface_s);
	lump.numblocks = &numsurfaces;
	lump.ptr = (void**)&surfaces;
	load_lump( &lump );
	con_printf( "%i surfaces\n", numsurfaces );

	// 8 - leafsurfaces
	lump.lumptype = lump_leafsurfaces;
	lump.blocksize = sizeof(uint32_t);
	lump.numblocks = &numleafsurfaces;
	lump.ptr = (void**)&leafsurfaces;
	load_lump( &lump );
	con_printf( "%i leafsurfaces\n", numleafsurfaces );

	// 8 - leafs
	lump.lumptype = lump_leafs;
	lump.blocksize = sizeof(dleaf_s);
	lump.numblocks = &numleafs;
	lump.ptr = (void**)&leafs;
	load_lump( &lump );

	// 14 - entities
	lump.lumptype = lump_entities;
	lump.blocksize = 1;
	lump.numblocks = &entitystringlen;
	lump.ptr = (void**)&entitystring;
	load_lump( &lump );

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
	
	load_all_lumps();
}

void bspmap::close( void )
{
	//operator delete();
	operator delete(shaders);
	operator delete(entitystring);
	operator delete(leafs);
	operator delete(leafsurfaces);
	operator delete(surfaces);
	delete mapfile;
}
