// lazybee engine
// (C) 2014 Michael Rieder
//

#ifndef BSPLOADER_H
#define BSPLOADER_H

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

#endif // BSPLOADER_H
