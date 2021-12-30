#pragma once
#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// vdisk.c

struct i_block
{
	char data[508];
	int next;
};

void init_disk();
void write_file(struct fcb* fb, const char* buffer, const int size);
char* read_file(struct fcb* fb);
struct i_block* get_block(const int bid);
// bitmap.c

#define LOW 10
#define COL 10

struct bitmap
{
	int free_size;
	int map[LOW][COL];
};
struct bitmap* bitm;
struct bitmap* create_bitmap();
void save_bitmap(struct bitmap* bm);
void recycle_block(const int b);
struct bitmap* read_bitmap();
int* get_free_block(const int n);

// dir.c
struct fcb* root;
struct fcb* current;
struct fcb* mkdir(const char* filename, int mode);

#endif // !FS_H
