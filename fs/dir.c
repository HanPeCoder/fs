/*
* Realize dir
*/

/*
* 实现目录的数据结构（目录本身也是一个文件，可以使用FCB进行描述）
* 将目录存入磁盘（用文件写的形式）
* 添加文件控制块（FCB）
* 实现按名存取
* 目录组织结构---树形
*/

#define MAX_ENTRY_NUM 20

#include "fcb.h"
#include "fs.h"


struct fcb* mkdir(const char* filename, int mode)
{
	struct fcb* dir = (struct fcb*)malloc(MAX_ENTRY_NUM*sizeof(struct fcb*));
	dir[0].filename = filename;
	dir[0].mode = mode;
	dir[0].size = 1;
	return dir;
}

void find_entry(const char* path, struct fcb* fb)
{

	// /A/B
	//int len = strlen(path);
	//char* now;
	//for (int i = 0; i < len; i++)
	//{
	//	if (path[i] != '/')
	//	{
	//		now += path[i];
	//	}else
	//	{
	//	}
	//}
}


void readdir(const char* name, struct fcb* fb)
{
	char* buffer = read_file(fb);
	struct fcb* entry = (struct fcb*)buffer;
	int n = entry[0].size;
	for (int i = 1; i < n - 1; i++) {
		if (entry[i].filename == name) {
			fb = &entry[i];
		}
	}
}
