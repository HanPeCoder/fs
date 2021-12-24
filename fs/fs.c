#include <stdio.h>
#include "fs.h"
int main()
{
	init_disk();
	fcb fb;
	struct bitmap* bm = read_bitmap();
	printf("free_size = %d\n", bm->free_size);
	for (int i = 0; i < LOW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%d ", bm->map[i][j]);
		}
		printf("\n");
	}
	char* buffer = "123456";
	fb.filename = "hahah";
	fb.size = 1024;
	write_disk(fb, buffer, fb.size);
	printf("\n");
	bm = read_bitmap();
	printf("free_size = %d\n", bm->free_size);

	for (int i = 0; i < LOW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%d ", bm->map[i][j]);
		}
		printf("\n");
	}
}
