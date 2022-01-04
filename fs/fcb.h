#pragma once
#ifndef FCB_H
#define FCB_H
#include "fs.h"
#include <stdlib.h>
#include <string.h>

struct fcb
{
	char filename[12]; // 文件名（目录名）
	int first_block; // 文件下一块号（目录所在块号）
	int size;	// 文件占用块数（目录数）
	int mode;	// 文件模式：0表示目录，1表示文件
};

struct fcb* currentfile;

inline struct fcb* get_empty_fcb()
{
	return (struct fcb*)malloc(sizeof(struct fcb));
}

inline struct fcb* create(const char* name, int mode)
{
	struct fcb* fb = (struct fcb*)malloc(sizeof(struct fcb));
	int len = strlen(name);
	for (int i = 0; i < len; i++) {
		fb->filename[i] = name[i];
	}
	if (len < 12) {
		fb->filename[len] = '\0';
	}
	fb->mode = mode;
	return fb;
}


inline char* read()
{
	char* buffer = (char*)malloc(10);
	buffer = read_file(currentfile);
	return buffer;
}

inline int open(const char* filename)
{
	currentfile = sys_open(filename);
	if (currentfile != NULL) {
		return 1;
	}
	return 0;
}

inline void save()
{
	update(currentfile, ibuf, strlen(ibuf));
}


inline int close()
{
	return 1;
}

#endif // !FCB_H

