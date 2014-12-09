// lazybee engine
// (C) 2014 Michael Rieder
//

#ifndef BSPMAP_H
#define BSPMAP_H

typedef struct {
	char		id[4];
	uint32_t	version;
	uint32_t	checksum;
} bspheader_t;

typedef struct {
	uint32_t	offset;
	uint32_t	length;
} lump_t;

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
} lumpdefs_t;

class bspmap
{
public:
	void open( const char* mname );
	bspmap( const char* mname = NULL )
	{
		if (mname!=NULL)
			open(mname);
	}
	~bspmap()
	{

	}
protected:
	int test;
};

#endif // BSPMAP_H
