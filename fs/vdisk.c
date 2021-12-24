/*
* 
* 
*/

#include "fs.h"

#define DISK_SIZE 100*512
#define BLOCK_SIZE 512
#define BLOCK_NUM DISK_SIZE/BLOCK_SIZE // 100
#define DATA_SIZE 508


//∑÷≈‰¥≈≈Ãø’º‰
char* allot_disk()
{
	char* vdisk = (char*)malloc(DISK_SIZE*sizeof(char));
	return vdisk;
}

// Õ∑≈¥≈≈Ãø’º‰
void relese_disk(char* vdisk)
{
	free(vdisk);
}

char* disk_out()
{
	char* buffer = allot_disk();
	FILE* fp;
	fp = fopen("data.dat", "rb");
	if (fp) {
		fread(buffer, DISK_SIZE, 1, fp);
		fclose(fp);
	}
	return buffer;
}

void disk_in(const char* buffer)
{
	FILE* fp;
	fp = fopen("data.dat", "wb");
	if (fp) {
		fwrite(buffer, DISK_SIZE, 1, fp);
		fclose(fp);
	}
}

struct i_block* get_block(const int bid)
{
	char* vdisk = disk_out();
	struct i_block* b = (struct i_block*)vdisk;
	return &b[bid];
}

void init_disk()
{
	char* buffer = allot_disk();
	struct i_block* i_blk = (struct i_block*)(buffer);
	FILE* fp = NULL;
	for (size_t i = 0; i < 100; i++) {
		i_blk[i].next = -1;
	}
	save_bitmap(create_bitmap());
	disk_in(buffer);
	relese_disk(buffer);
}

struct i_block* request_block(const int n)
{
	struct i_block* f_blk = (struct i_block*)malloc(n * sizeof(struct i_block));
	if (f_blk) {
		for (int i = 0; i < n; i++) {
			//int bid = get_free_block();
			//f_blk[i] = get_block(bid);
		}
	}
	return f_blk;
}

void release_block(const int b)
{
	recycle_block(b);
}

void write_disk(fcb fb, const char* buffer, const int size)
{
	int n = size / DATA_SIZE + ((size%DATA_SIZE != 0) ? 1 : 0);
	int* bid = get_free_block(n);
	if (bid != NULL) {
		fb.first_block = bid[0];
		struct i_block* current = get_block(bid[0]);
		for (int i = 0; i < DATA_SIZE; i++) {
			current->data[i] = buffer[i];
		}
		for (int i = 1; i < n; i++) {
			current->next = bid[i];
			current = get_block(bid[i]);
			for (int j = 0; j < DATA_SIZE; j++) {
				current->data[i] = buffer[i + DATA_SIZE];
			}
		}
	}
}

