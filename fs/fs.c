#include <stdio.h>
#include "fs.h"
#include "fcb.h"

int main(int argc, char* argv[])
{
	printf("建立基于磁盘存储设备的隐式链接文件系统\n");
	printf("********************************\n");
	printf("*\t系统使用说明\t\t\t\t*\n");
	printf("*\tinit: 初始化磁盘\t\t\t*\n");
	printf("*\tls: 显示当前目录下的文件列表*\n");
	printf("*\tdiskinfo:显示磁盘情况\t\t*\n");
	printf("*\trm:删除文件/目录\t\t\t*\n");
	printf("*\tmkdir:创建目录\t\t\t*\n");
	printf("*\tmkfile:创建文件\t\t\t*\n");
	printf("*\tcd:更改当前目录\t\t\t*\n");
	printf("*\tchname:更改文件名\t\t\t*\n");
	printf("*\tchloc:更改文件/目录位置\t*\n");
	printf("*******************************\n");
	printf("\n");
	mount();
	char* cmd = (char*)malloc(1);
	while (1) {
		printf("%s>", current->filename);
		int res = scanf("%s", cmd);
		if (strcmp(cmd, "init") == 0) {
			init_disk();
			mount();
			printf("磁盘初始化完成\n");
		} else if (strcmp(cmd,"ls") == 0) {
			char** nl = ls();
			for (int i = 1; i < current[0].size; i++) {
				printf(nl[i - 1]); printf(" ");
			}
			printf("\n");
		} else if (strcmp(cmd, "diskinfo") == 0) {
			bitm = read_bitmap();
			int used = (BLOCK_NUM - bitm->free_size)*BLOCK_SIZE;
			printf("********************************\n");
			printf("*\t磁盘使用情况：%d/%d\t*\n", used, DISK_SIZE);
			for (int i = 0; i < LOW; i++) {
				printf("*\t");
				for (int j = 0; j < COL; j++) {
					printf("%d ", bitm->map[i][j]);
				}
				printf("\t\n");
			}
			int* used_block = get_used_block();
			printf("*\t已使用盘块:");
			for (int i = 0; i < BLOCK_NUM - bitm->free_size; i++) {
				printf(" %d", used_block[i], DISK_SIZE);
			}
			printf("\n");
		} else if (strcmp(cmd, "cd") == 0) {
			if (!scanf("%s", cmd)) {
				printf("请输入路径");
				continue;
			}
			cd(cmd);
		} else if (strcmp(cmd, "mkdir") == 0) {
			if (!scanf("%s", cmd)) {
				printf("请输入目录名");
				continue;
			}
			mkdir(cmd,0);
		} else if (strcmp(cmd, "mkfile") == 0) {
			if (!scanf("%s", cmd)) {
				printf("请输入文件名");
				continue;
			}
			mkdir(cmd,1);
		} else if (strcmp(cmd, "rm") == 0) {
			if (!scanf("%s", cmd)) {
				printf("请输入文件名");
				continue;
			}
			rm_file(cmd);
		} else if (strcmp(cmd, "chname") == 0) {
			char* name = (char*)malloc(1);
			char* newname = (char*)malloc(1);
			if (!scanf("%s", name)) {
				printf("请输入原文件名");
				continue;
			}if (!scanf("%s", newname)) {
				printf("请输入新文件名");
				continue;
			}
			if (rename_file(name, newname)) {
				printf("文件已经存在\n");
				continue;
			}
		}else if (strcmp(cmd, "chloc") == 0) {
			char* dect = (char*)malloc(1);
			char* src = (char*)malloc(1);
			if (!scanf("%s", dect)) {
				printf("请输入路径名");
				continue;
			}if (!scanf("%s", src)) {
				printf("请输入文件名");
				continue;
			}
			if (change_loc(dect, src)) {
				printf("成功修改\n");
				continue;
			}
		} else {
			printf("命令不合法\n");
		}
	}
}
