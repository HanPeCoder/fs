#include "fs.h"
#include "fcb.h"

#define DISK_SIZE 100*512
#define BLOCK_SIZE 512
#define BLOCK_NUM DISK_SIZE/BLOCK_SIZE // 100
#define DATA_SIZE 508

//分配磁盘空间
char* allot_disk()
{
	char* vdisk = (char*)malloc(DISK_SIZE*sizeof(char));
	return vdisk;
}

//释放磁盘空间
void relese_disk(char* vdisk)
{
	free(vdisk);
}

char* disk_out()
{
	char* buffer = allot_disk();
	FILE* fp = NULL;
	fp = fopen("disk.dat", "rb");
	if (fp) {
		fread(buffer, DISK_SIZE, 1, fp);
		fclose(fp);
	}
	return buffer;
}

void disk_in(const char* buffer)
{
	FILE* fp = NULL;
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
	disk_in(vdisk);
}

void init_disk()
{
	char* buffer = allot_disk();
	struct i_block* i_blk = (struct i_block*)(buffer);
	for (size_t i = 0; i < 100; i++) {
		i_blk[i].next = -1;
	}
	save_bitmap(create_bitmap());
	struct fcb* dir = (struct fcb*)malloc(20 * sizeof(struct fcb*));
	char* filename = "root";
	int len = strlen("filename");
	for (int i = 0; i < len; i++) {
		dir[0].filename[i] = filename[i];
	}
	dir[0].first_block = *get_free_block(1);
	dir[0].mode = 0;
	dir[0].size = 1;
	char* tem = (char*)dir;
	for (int i = 0; i < DATA_SIZE; i++) {
		i_blk[0].data[i] = tem[i];
	}
	disk_in(buffer);
	relese_disk(buffer);
}

void mount()
{
	// 读位视图
	bitm = read_bitmap();
	// 读取根目录
	struct i_block* blk = get_block(0);
	char* buffer = (char*)malloc(DATA_SIZE * sizeof(char));
	for (int i = 0; i < DATA_SIZE; i++) {
		buffer[i] = blk->data[i];
	}
	root = (struct fcb*)buffer;
	current = root;
}

void release_block(const int b)
{
	recycle_block(b);
}

void write_file(struct fcb* fb, const char* buffer, const int len)
{
	if (buffer == NULL) {
		int* bid = get_free_block(1);
		if (bid == NULL) {
			printf("Insufficient disk space");
			return;
		}
		fb->first_block = bid[0];
		fb->size = 1;
		return;
	}
	int n = len/ DATA_SIZE + ((len%DATA_SIZE != 0) ? 1 : 0);
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

char* read_file(struct fcb* fb)
{
	int n;
	if (fb->mode == 1) {
		n = fb->size;
	}
	else if (fb->mode == 0) {
		n = 1;
	}
	struct i_block* i_blk = (struct i_block*)malloc(n*sizeof(struct i_block));
	int current = fb->first_block;
	if (i_blk != NULL) {
		for (int i = 0; i < n; i++) {
			i_blk[i] = *get_block(current);
			current = i_blk[i].next;
		}
	}
	char* buffer = (char*)malloc(n * DATA_SIZE * sizeof(char*));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < DATA_SIZE; j++) {
			buffer[i * j+j] = i_blk[i].data[j];
		}
	}
	return buffer;
}

void update(struct fcb* fb, const char* buffer, const int len)
{
	if (buffer == NULL) {
		return;
	}
	int n = fb->size;
	int bid = fb->first_block;
	if (bid != NULL) {
		struct i_block* current = get_block(bid);
		for (int i = 0; i < DATA_SIZE; i++) {
			current->data[i] = buffer[i];
		}
		put_block(current, bid);
		bid = current->next;
		current = get_block(bid);
		for (int i = 1; i < n; i++) {
			for (int j = 0; j < DATA_SIZE; j++) {
				current->data[j] = buffer[i + DATA_SIZE];
			}
			put_block(current, bid);
			bid = current->next;
			current = get_block(bid);
		}
	}
}

void write_dir(struct fcb* fb)
{
	char* buffer = (char*)fb;
	struct i_block* current = get_block(fb->first_block);
	for (int i = 0; i < DATA_SIZE; i++) {
		current->data[i] = buffer[i];
	}
	put_block(current, fb->first_block);
}