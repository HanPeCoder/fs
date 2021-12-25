#pragma once
#ifndef FS_H
#define FS_H

#include "fcb.h"
#include <stdio.h>
#include <stdlib.h>
// vdisk.c

struct i_block
{
	char data[508];
	int next;
};

void init_disk();
void write_disk(struct fcb* fb, const char* buffer, const int size);

// bitmap.c

#define LOW 10
#define COL 10

struct bitmap
{
	int free_size;
	int map[LOW][COL];
};
struct bitmap* create_bitmap();
void save_bitmap(struct bitmap* bm);
void recycle_block(const int b);
struct bitmap* read_bitmap();
int* get_free_block(const int n);
struct i_block* read_disk(struct fcb* fb);
// dir.c
void add(struct fcb* fb);

#endif // !FS_H
