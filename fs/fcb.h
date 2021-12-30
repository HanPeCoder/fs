#pragma once
#ifndef FCB_H
#define FCB_H
#include "fs.h"
#include <stdlib.h>
#include <string.h>

struct fcb
{
	char* filename; // 文件名（目录名）
	int first_block; // 文件下一块号（目录所在块号）
	int size;	// 文件占用块数（目录数）
	int mode;	// 文件模式：0表示目录，1表示文件
};

#endif // !FCB_H

inline struct fcb* create(char* name)
{
	struct fcb* fb = (struct fcb*)malloc(sizeof(struct fcb));
	strcpy(fb->filename, name);
	return fb;
}

inline struct fcb* open(char* name)
{
	//检索目录
	// find file
	return (struct fcb*)malloc(sizeof(struct fcb));
}


inline void save(struct fcb* fb, char* buffer, int size)
{
	//write_disk(fb, buffer, size);
	
}

inline void close(char* name)
{

}