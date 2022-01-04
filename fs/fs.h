#pragma once
#ifndef FS_H
#define FS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// vdisk.c

#define DISK_SIZE 100*512
#define BLOCK_SIZE 512
#define BLOCK_NUM DISK_SIZE/BLOCK_SIZE // 100
#define DATA_SIZE 508

struct i_block
{
	char data[508];
	int next;
};

void init_disk();
void mount();
void write_file(struct fcb* fb, const char* buffer, const int len);
void update(struct fcb* fb, const char* buffer, const int len);
void write_dir(struct fcb* fb);
char* read_file(struct fcb* fb);
struct i_block* get_block(const int bid);

//buffer.c
char* ibuf;
void init_ibuf();
void write_buffer(const char* buffer);
char* read_buffer();
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
int* get_used_block();

// dir.c
struct fcb* root;
struct fcb* current;
struct fcb* pre;
struct fcb* mkdir(const char* filename, int mode);
int dup_name(const char* name);
int add(struct fcb* fb);
struct fcb* sys_open(const char* filename);
int sys_close(const char* filename);
struct fcb* find_entry(const char* path, struct fcb* fb);
char** ls();
int rm_file(const char* path);
int share(const char* path1, const char* name);
int rename_file(const char* name, const char* newname);
int change_loc(const char* path, const char* name);
struct fcb* test();
#endif // !FS_H
