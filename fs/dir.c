/*
* Realize dir
*/

/*
* ʵ��Ŀ¼�����ݽṹ��Ŀ¼����Ҳ��һ���ļ�������ʹ��FCB����������
* ��Ŀ¼������̣����ļ�д����ʽ��
* ����ļ����ƿ飨FCB��
* ʵ�ְ�����ȡ
* Ŀ¼��֯�ṹ---����
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
