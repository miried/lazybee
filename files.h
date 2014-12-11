// lazybee engine
// (C) 2014 Michael Rieder
//

#ifndef FILES_H
#define FILES_H

class filestream
{
public:
	size_t read( void *buffer, size_t size );
	void seek( long int offset, bool relative=false );
	filestream( const char* fname = NULL, size_t size = BUFSIZ )
	{
		if (fname!=NULL) open(fname);
	}
	~filestream()
	{
		if (fp) close();
	}
protected:
	void open( const char* fname );
	void close( void );
	// vars
	FILE *fp = NULL;
	void *int_buffer;
};

#endif // FILES_H
