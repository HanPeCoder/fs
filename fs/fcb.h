#pragma once
#ifndef FCB_H
#define FCB_H
#include "fs.h"
#include <stdlib.h>
#include <string.h>

struct fcb
{
	char* filename; // �ļ�����Ŀ¼����
	int first_block; // �ļ���һ��ţ�Ŀ¼���ڿ�ţ�
	int size;	// �ļ�ռ�ÿ�����Ŀ¼����
	int mode;	// �ļ�ģʽ��0��ʾĿ¼��1��ʾ�ļ�
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
	//����Ŀ¼
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