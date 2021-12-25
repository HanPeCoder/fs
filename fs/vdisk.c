/*
* 
* 
*/

#include "fs.h"

#define DISK_SIZE 100*512
#define BLOCK_SIZE 512
#define BLOCK_NUM DISK_SIZE/BLOCK_SIZE // 100
#define DATA_SIZE 508


//·ÖÅä´ÅÅÌ¿Õ¼ä
char* allot_disk()
{
	char* vdisk = (char*)malloc(DISK_SIZE*sizeof(char));
	return vdisk;
}

//ÊÍ·Å´ÅÅÌ¿Õ¼ä
void relese_disk(char* vdisk)
{
	free(vdisk);
}

char* disk_out()
{
	char* buffer = allot_disk();
	FILE* fp;
	fp = fopen("disk.dat", "rb");
	if (fp) {
		fread(buffer, DISK_SIZE, 1, fp);
		fclose(fp);
	}
	return buffer;
}

void disk_in(const char* buffer)
{
	FILE* fp;
	fp = fopen("disk.dat", "wb");
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

void put_block(struct i_block* b, int bid)
{
	char* vdisk = disk_out();
	struct i_block* i_blk = (struct i_block*)vdisk;
	i_blk[bid] = (*b);
	printf("%d->", i_blk[bid].next);
	disk_in(vdisk);
}

void init_disk()
{
	char* buffer = allot_disk();
	struct i_block* i_blk = (struct i_block*)(buffer);
	FILE* fp = NULL;
	for (size_t i = 0; i < 100; i++) {
		i_blk[i].next = -1;
	}
	save_bitmap(read_bitmap());
	disk_in(buffer);
	relese_disk(buffer);
}

void release_block(const int b)
{
	recycle_block(b);
}

void write_disk(struct fcb* fb, const char* buffer, const int size)
{
	int n = size / DATA_SIZE + ((size%DATA_SIZE != 0) ? 1 : 0);
	int* bid = get_free_block(n);
	if (bid == NULL) {
		printf("Insufficient disk space");
		return;
	}
	struct i_block* b = (struct i_block*)malloc(sizeof(struct i_block));
	struct i_block* current = b;
	if (bid != NULL) {
		fb->first_block = bid[0];
		fb->size = n;
		for (int i = 0; i < DATA_SIZE; i++) {
			current->data[i] = buffer[i];
		}
		for (int i = 1; i < n; i++) {
			current->next = bid[i];
			put_block(current, bid[i-1]);
			current = b;
			for (int j = 0; j < DATA_SIZE; j++) {
				current->data[j] = buffer[i + DATA_SIZE];
			}
		}
	}
}

struct i_block* read_disk(struct fcb* fb)
{
	int n = fb->size;
	struct i_block* i_blk = (struct i_block*)malloc(n*sizeof(struct i_block));
	int current = fb->first_block;
	if (i_blk != NULL) {
		for (int i = 0; i < n; i++) {
			i_blk[i] = *get_block(current);
			current = i_blk[i].next;
		}
	}
	return i_blk;
}