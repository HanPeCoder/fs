#pragma once
#ifndef FCB_H
#define FCB_H

#include <stdlib.h>

struct fcb
{
	char* filename;
	int first_block;
	int size;
};

#endif // !FCB_H

//struct fcb* create_file(char* name)
//{
//	struct fcb* fb = (struct fcb*)malloc(sizeof(struct fcb));
//	fb->filename = name;
//	//add(fb);
//	return fb;
//}

//struct fcb* save(const char* buffer)
//{
//	return;
//}