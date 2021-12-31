#include "fs.h"

struct bitmap* create_bitmap()
{
	struct bitmap* bm = (struct bitmap*)malloc(sizeof(struct bitmap));
	if (bm) {
		bm->free_size = LOW*COL;
		for (int i = 0; i < LOW; i++) {
			for (int j = 0; j < COL; j++) {
				bm->map[i][j] = 0;
			}
		}
	} else {
		printf("Mem request fail");
	}
	return bm;
}

void save_bitmap(struct bitmap* bm)
{
	int length = LOW * COL + 1;
	char* buffer = (char*)malloc(length * sizeof(char));
	if (buffer != NULL) {
		buffer[0] = bm->free_size;
		for (int i = 0; i < length; i++) {
			buffer[i+1] = (char)bm->map[i / 10][i % 10];
		}
		FILE* fp = NULL;
		fp = fopen("bitmap.dat", "wb");
		if (fp) {
			fwrite(buffer, length, 1, fp);
			fclose(fp);
		}
	}
	free(buffer);
}

struct bitmap* read_bitmap()
{
	int length = LOW * COL + 1;
	char* buffer = (char*)malloc(length * sizeof(char));
	struct bitmap* bm = create_bitmap();
	if (buffer != NULL) {
		FILE* fp = NULL;
		fp = fopen("bitmap.dat", "rb");
		if (fp) {
			fread(buffer, length, 1, fp);
			fclose(fp);
		}
		bm->free_size = (int)buffer[0];
		for (int i = 0; i < length; i++) {
			bm->map[i / 10][i % 10] = (int)buffer[i+1];
		}
	}
	free(buffer);
	return bm;
}

int* get_free_block(const int n)
{
	int* b = NULL;
	struct bitmap* bm = read_bitmap();
	if (n <= bm->free_size) {
		bm->free_size -= n;
		b = (int*)malloc(n * sizeof(int));
		int k = 0;
			for (int i = 0; i < LOW; i++) {
				for (int j = 0; j < COL; j++) {
					if (bm->map[i][j] == 0 && k < n) {
						b[k++] = i * COL + j;
						bm->map[i][j] = 1;
					}
				}
			}
		save_bitmap(bm);
		return b;
	}
	return b;
}

int* get_used_block()
{
	int* b = NULL;
	struct bitmap* bm = read_bitmap();
	int n = BLOCK_NUM - bm->free_size;
	if (n <= bm->free_size) {
		b = (int*)malloc(n * sizeof(int));
		int k = 0;
		for (int i = 0; i < LOW; i++) {
			for (int j = 0; j < COL; j++) {
				if (bm->map[i][j] == 1 && k < n) {
					b[k++] = i * COL + j;
				}
			}
		}
		return b;
	}
	return b;
}

void recycle_block(const int b)
{
	int i = b / COL;
	int j = b % LOW;
	struct bitmap* bm = read_bitmap();
	bm->map[i][j] = 0;
	bm->free_size++;
	save_bitmap(bm);
}