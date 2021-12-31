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

#endif // !FCB_H

inline struct fcb* get_empty_fcb()
{
	return (struct fcb*)malloc(sizeof(struct fcb));
}

inline struct fcb* create(const char* name,int mode)
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


inline void save(struct fcb* fb, char* buffer, int size)
{
	write_file(fb, buffer, size);
	add(fb);
}

inline void close(char* name)
{
	
}