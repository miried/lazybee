// lazybee engine
// (C) 2014 Michael Rieder
//

#ifndef FILES_H
#define FILES_H

class filestream
{
public:
	void open( const char* fname );
	void close( void );
	size_t read( void *buffer, size_t size );
	filestream( const char* fname = NULL, size_t size = BUFSIZ )
	{
		if (fname!=NULL)
			open(fname);
	}
	~filestream()
	{
		if (fp)
			close();
	}
protected:
	FILE *fp = NULL;
	void *int_buffer;
};

#endif // FILES_H
