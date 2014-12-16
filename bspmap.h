// lazybee engine
// (C) 2014 Michael Rieder
//

#ifndef BSPMAP_H
#define BSPMAP_H

#define	LIGHTMAP_SIZE		128
#define LIGHTMAP_BLOCK_LEN	(LIGHTMAP_SIZE*LIGHTMAP_SIZE*3)

typedef struct {
	char		id[4];
	uint32_t	version;
	uint32_t	checksum;
} bspheader_s;

typedef enum {
	lump_shaders = 0,
	lump_planes = 1,
	lump_lightmaps = 2,
	lump_surfaces = 3,
	lump_drawverts = 4,
	lump_drawindexes = 5,
	lump_leafbrushes = 6,
	lump_leafsurfaces = 7,
	lump_leafs = 8,
	lump_nodes = 9,
	lump_sideequations = 10,
	lump_brushsides = 11,
	lump_brushes = 12,
	lump_models = 13,
	lump_entities = 14,
	lump_visibility = 15,
	lump_lightgridpalette = 16,
	lump_lightgridoffsets = 17,
	lump_lightgriddata = 18,
	lump_spherelights = 19,
	lump_spherelightvis = 20,
	lump_lightdefs = 21,
	lump_terrain = 22,
	lump_terrainindexes = 23,
	lump_staticmodeldata = 24,
	lump_staticmodeldef = 25,
	lump_staticmodelindexes = 26,
	lump_unknown = 27,
	lump_max = 28
} lumpdefs_e;

typedef struct {
	uint32_t	offset;
	uint32_t	length;
} lump_s;

typedef struct {
	char		shader[64];
	uint32_t	surfaceFlags;
	uint32_t	contentFlags;
	uint32_t	subdivisions;
	char		fenceMaskImage[64];
} dshader_s;

typedef struct {
	float		normal[3];
	float		dist;
} dplane_s;

typedef struct {
	uint32_t	cluster;
	uint32_t	area;

	uint32_t	mins[3];
	uint32_t	maxs[3];

	uint32_t	firstLeafSurface;
	uint32_t	numLeafSurfaces;

	uint32_t	firstLeafBrush;
	uint32_t	numLeafBrushes;
		
	uint32_t	firstTerraPatch;
	uint32_t	numTerraPatches;

	uint32_t	firstStaticModel;
	uint32_t	numStaticModels;
} dleaf_s;

typedef struct {
	uint32_t	shaderNum;
	uint32_t	fogNum;
	uint32_t	surfaceType;

	uint32_t	firstVert;
	uint32_t	numVerts;

	uint32_t	firstIndex;
	uint32_t	numIndexes;

	uint32_t	lightmapNum;
	uint32_t	lightmapX, lightmapY;
	uint32_t	lightmapWidth, lightmapHeight;

	float		lightmapOrigin[3];
	float		lightmapVecs[3][3];

	uint32_t	patchWidth;
	uint32_t	patchHeight;

	float		subdivisions;
} dsurface_s;

typedef struct {
	// int
	lumpdefs_e	lumptype;
	uint_t		blocksize;
	// out
	uint_t		*numblocks;
	void		**ptr;
} lumpdata_s;

class bspmap
{
public:
	bspmap( const char* mname )
	{
		if (mname!=NULL) open(mname);
	}
	~bspmap()
	{
		close();
	}
protected:
	void open( const char* mname );
	void close( void );
	
	void load_lump( lumpdata_s *lump );
	void load_all_lumps( void );
	// vars
	filestream	*mapfile;
	bspheader_s	header;
	lump_s		lumps[lump_max];
	// allocated
	char		*entitystring;
	dleaf_s		*leafs;
	uint32_t	*leafsurfaces;
	dsurface_s	*surfaces;
	dshader_s	*shaders;
	uint8_t		*lightmapdata;
	dplane_s	*planes;
	// counters
	uint_t		numshaders;
	uint_t		entitystringlen;
	uint_t		numleafs;
	uint_t		numclusters;
	uint_t		numareas;
	uint_t		numleafsurfaces;
	uint_t		numsurfaces;
	uint_t		numlightmaps;
	uint_t		numplanes;
};

#endif // BSPMAP_H
