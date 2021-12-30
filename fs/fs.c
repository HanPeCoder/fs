#include <stdio.h>
#include "fs.h"
#include "fcb.h"
int main(int argc, char* argv[])
{
	init_disk();
	struct i_block* blk = get_block(0);
	struct fcb* root;
	root = (struct fcb*)blk->data;
	printf(root[0].filename);
}


/*
init_disk();
	struct fcb fb;
	save_bitmap(create_bitmap());
	char* buffer = "123456";
	fb.filename = "hahah";
	fb.size = 3;
	fb.first_block = 0;
	write_disk(&fb, buffer, 1024);
	struct bitmap* bm = read_bitmap();
	printf("free_size = %d\n", bm->free_size);
	for (int i = 0; i < LOW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%d ", bm->map[i][j]);
		}
		printf("\n");
	}
	struct i_block* i_blk = read_disk(&fb);
	for (int i = 0; i < 3; i++) {
		printf("%d->", i_blk[i].next);
	}
*/