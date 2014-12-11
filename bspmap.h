// lazybee engine
// (C) 2014 Michael Rieder
//

#ifndef BSPMAP_H
#define BSPMAP_H

typedef struct bspheader_s {
	char		id[4];
	uint32_t	version;
	uint32_t	checksum;
} bspheader_s;

typedef enum lumpdefs_s {
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
} lumpdefs_s;

typedef struct lump_s {
	uint32_t	offset;
	uint32_t	length;
} lump_s;

typedef struct dleaf_s {
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

typedef struct dsurface_s {
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

	vec3		lightmapOrigin;
	vec3		lightmapVecs[3];

	uint32_t	patchWidth;
	uint32_t	patchHeight;

	float		subdivisions;
} dsurface_s;

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
	void lump_loadentities( void );
	void lump_loadleafs( void );
	void lump_loadleafsurfaces( void );
	void lump_loadsurfaces( void );
	// vars
	filestream	*mapfile;
	bspheader_s	header;
	lump_s		lumps[lump_max];
	// mapdata
	char		*entityString;

	dleaf_s		*leafs;
	int		numleafs;
	int		numclusters;
	int		numareas;

	uint32_t	*leafsurfaces;
	int		numleafsurfaces;

	dsurface_s	*surfaces;
	int		numsurfaces;
};

#endif // BSPMAP_H
