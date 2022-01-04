#include "fs.h"
#include "fcb.h"


void init_ibuf()
{
	ibuf = (char*)malloc(1024);
}

void write_buffer(const char* buffer)
{
	strcpy(ibuf, buffer);
}

char* read_buffer()
{
	char* buffer = (char*)malloc(1);
	strcpy(buffer, ibuf);
}