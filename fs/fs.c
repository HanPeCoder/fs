#include <stdio.h>
#include "fs.h"
#include "fcb.h"

int main(int argc, char* argv[])
{
	printf("建立基于磁盘存储设备的隐式链接文件系统\n");
	printf("*****************************************\n");
	printf("*\t系统使用说明\t\t\t*\n");
	printf("*\tinit: 初始化磁盘\t\t*\n");
	printf("*\tls: 显示当前目录下的文件列表\t*\n");
	printf("*\tdiskinfo:显示磁盘情况\t\t*\n");
	printf("*\trm:删除文件/目录\t\t*\n");
	printf("*\tmkdir:创建目录\t\t\t*\n");
	printf("*\tmkfile:创建文件\t\t\t*\n");
	printf("*\tcd:更改当前目录\t\t\t*\n");
	printf("*\tchname:更改文件名\t\t*\n");
	printf("*\tchloc:更改文件/目录位置\t\t*\n");
	printf("*\top:打开文件\t\t\t*\n");
	printf("*\tsave:保存文件，在op状态下进行\t*\n");
	printf("*\tq:关闭文件并退出\t\t*\n");
	printf("*\te:编辑文件\t\t\t*\n");
	printf("*\tr:读文件\t\t\t*\n");
	printf("*****************************************\n");
	printf("\n");
	mount();
	init_ibuf();
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
			if (!cd(cmd)) {
				printf("路径转移失败");
			}
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
		}else if (strcmp(cmd, "op") == 0) {
			if (!scanf("%s", cmd)) {
				printf("请输入文件名");
				continue;
			}
			if (open(cmd)) {
				// 进入文件状态
				while (1) {
					printf(">");
					scanf("%s", cmd);
					if (strcmp(cmd, "q") == 0) {
						close();
						break;
					} else if (strcmp(cmd, "e") == 0) {
						/*char* data = (char)malloc(1);*/
						char* buffer = (char*)malloc(1);
						getchar();
						scanf("%[^\n]", buffer);
						write_buffer(buffer);
					} else if (strcmp(cmd, "save") == 0) {
						save();
					} else if (strcmp(cmd, "r") == 0) {
						char* buffer = read();
						printf(buffer);
						printf("\n");
						free(buffer);
					}
				}
			}
		} else if (strcmp(cmd, "q") == 0) {
			break;
		} else if (strcmp(cmd, "test1") == 0) {
			struct fcb* fb = test();
			printf("测试文件使用的磁盘块号: ");
			printf("%d ",fb->first_block);
			struct i_block* b = get_block(fb->first_block);
			for (int i = 0; i < fb->size && b->next != -1; i++) {
				printf("%d ", b->next);
				b = get_block(b->next);
			}
			printf("\n");
		}
		else {
			printf("命令不合法\n");
		}
	}
}

