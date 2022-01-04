#pragma once
#ifndef FCB_H
#define FCB_H
#include "fs.h"
#include <stdlib.h>
#include <string.h>

struct fcb
{
	char filename[12]; // �ļ�����Ŀ¼����
	int first_block; // �ļ���һ��ţ�Ŀ¼���ڿ�ţ�
	int size;	// �ļ�ռ�ÿ�����Ŀ¼����
	int mode;	// �ļ�ģʽ��0��ʾĿ¼��1��ʾ�ļ�
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

