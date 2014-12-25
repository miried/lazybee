/*
 * bspmap.cpp - BSP map subsystem
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
#include "bspmap.h"

void count_clusters_areas( dleaf_s *leafs, uint_t numleafs, uint_t *numclusters, uint_t *numareas );

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

void count_clusters_areas( dleaf_s *leafs, uint_t numleafs, uint_t *numclusters, uint_t *numareas )
{
	// count the clusters and areas
	*numclusters = 0;
	*numareas = 0;
	
	for (uint_t i=0;i<numleafs;i++) {
		dleaf_s *leaf = leafs+i;
		if (leaf->cluster >= *numclusters)
			*numclusters = leaf->cluster + 1;
		if (leaf->area >= *numareas)
			*numareas = leaf->area + 1;
	}
}

void bspmap::load_all_lumps( void )
{
	lumpdata_s lumplist[] = {
	/*00*/	{lump_shaders,sizeof(dshader_s), &numshaders,reinterpret_cast<void**>(&shaders)},
	/*01*/	{lump_planes,sizeof(dplane_s), &numplanes,reinterpret_cast<void**>(&planes)},
	/*02*/	{lump_lightmaps,LIGHTMAP_BLOCK_LEN, &numlightmaps,reinterpret_cast<void**>(&lightmapdata)},
	/*03*/	{lump_surfaces,sizeof(dsurface_s), &numsurfaces,reinterpret_cast<void**>(&surfaces)},
	/*04*/	{lump_drawverts,sizeof(drawVert_s), &numdrawverts,reinterpret_cast<void**>(&drawverts)},
	/*05*/	{lump_drawindexes,sizeof(uint32_t), &numdrawindexes,reinterpret_cast<void**>(&drawindexes)},
	/*07*/	{lump_leafsurfaces,sizeof(uint32_t), &numleafsurfaces,reinterpret_cast<void**>(&leafsurfaces)},
	/*08*/	{lump_leafs,sizeof(dleaf_s), &numleafs,reinterpret_cast<void**>(&leafs)},
	/*09*/	{lump_nodes,sizeof(dnode_s), &numnodes,reinterpret_cast<void**>(&nodes)},
	/*14*/	{lump_entities,sizeof(char), &entitystringlen,reinterpret_cast<void**>(&entitystring)}
	};
	int numlumplist = sizeof(lumplist)/sizeof(lumpdata_s);
	
	for (int k=0;k<numlumplist;k++) {
		load_lump( lumplist + k );
	}

	con_printf( "%i shaders\n", numshaders );
	con_printf( "%i planes\n", numplanes );
	con_printf( "%i lightmaps\n", numlightmaps );

	for (uint_t k=0;k<numsurfaces;k++) {
		dsurface_s *surf = surfaces + k;
		switch (surf->surfaceType){
			case MST_BAD:
				con_printf( "bad surface type\n");
				break;
			case MST_PLANAR:
				if (surf->numVerts ==0)
					con_printf( "%i numVerts=0\n",k);
				if (surf->numIndexes ==0)
					con_printf( "%i numIndexes=0\n",k);
				break;
			case MST_PATCH:
				break;
			case MST_TRIANGLE_SOUP:
				break;
			case MST_FLARE:
				break;
			case MST_TERRAIN:
				break;
			default:
				con_printf( "unknown surface type: %i\n",surf->surfaceType);
				break;			
		}
	}

	con_printf( "%i surfaces\n", numsurfaces );
	con_printf( "%i drawverts\n", numdrawverts );
	con_printf( "%i drawindexes\n", numdrawindexes );
	con_printf( "%i leafsurfaces\n", numleafsurfaces );
	count_clusters_areas( leafs,numleafs, &numclusters, &numareas );
	con_printf( "map has %i leafs, %i clusters, %i areas\n",
		numleafs, numclusters, numareas );
	con_printf( "%i nodes\n", numnodes );
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
	void* allocated[] = {
		leafs,
		nodes,
		leafsurfaces,
		surfaces,
		drawverts,
		drawindexes,
		shaders,
		lightmapdata,
		planes,
		entitystring
	};
	int allocatednum = sizeof(allocated)/sizeof(void*);

	for (int k=0;k<allocatednum;k++)
		operator delete(allocated[k]);

	delete mapfile;
}

void copyVertex( float *dest, void *src )
{
	memcpy( dest, src, 5*sizeof(float) );
	dest[0] /= 100.0;
	dest[1] /= 100.0;
	dest[2] /= 100.0;
}

void bspmap::getVertexData( float **ptr, uint_t *num )
{
	// count total number of verts in all surfaces
	uint_t num_verts=0;
	for (uint_t k=0;k<numsurfaces;k++)
		num_verts += surfaces[k].numIndexes;

	float *vertexdata = new float[5*num_verts];

	uint_t vert_counter=0;
	for (uint_t k=0;k<numsurfaces;k++) {
		dsurface_s *surf = surfaces + k;
		for (uint_t l=0;l<surf->numIndexes;l++) {
			uint_t offset = surf->firstVert+drawindexes[surf->firstIndex+l];
			copyVertex( vertexdata+vert_counter*5, drawverts+offset );
			vert_counter++;
		}
	}
	*ptr = vertexdata;
	*num = num_verts;
}
