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
	

	struct fcb* dir = NULL;
	if (dup_name(filename)) {
		return dir;
	}
	if (mode == 0) {
		dir = (struct fcb*)malloc(MAX_ENTRY_NUM * sizeof(struct fcb*));
		int len = strlen(filename);
		for (int i = 0; i < len; i++) {
			dir[0].filename[i] = filename[i];
		}
		if (len < 12) {
			dir[0].filename[len] = '\0';
		}
		dir[0].mode = mode;
		dir[0].size = 1;
		dir[0].first_block = *get_free_block(1);
		add(dir);
		write_dir(dir);
	}
	else if (mode == 1) {
		dir = create(filename, mode);
		char* buffer = NULL;
		write_file(dir, buffer, 0);
		add(dir);
		write_dir(dir);
	}
	return dir;
}

int dup_name(const char* name) 
{
	char** nl = ls();
	for (int i = 1; i < current[0].size; i++) {
		if (compare(nl[i - 1], name)) {
			return 1;
		}
	}
	return 0;
}

char** ls() 
{
	char** nl = (char**)malloc((current->size-1)*4);
	for (int i = 1; i < current[0].size; i++) {
		nl[i - 1] = (char*)malloc(strlen(current[i].filename));
		nl[i - 1] = current[i].filename;
	}
	return nl;
}

int add(struct fcb* fb)
{

	if (current[0].size > MAX_ENTRY_NUM)
	{
		return 0;
	}
	else
	{
		current[current[0].size] = *fb;
		current[0].size++;
		write_dir(current);
		return 1;
	}
}

int compare(char* str1, char* str2)
{
	if (strlen(str1) != strlen(str2)) {
		return 0;
	}
	for (int i = 0; i < strlen(str2); i++) {
		if (str1[i] != str2[i]) {
			return 0;
		}
	}
	return 1;
}


struct fcb* read_dir(struct fcb* dir)
{
	if (dir->mode == 0) {
		char* buffer = read_file(&dir[0]);
		struct fcb* fb = (struct fcb*)buffer;
		return fb;
	}
	else if (dir->mode == 1) {
		return dir;
	}
	
}

static struct fcb* get_dir(const char* path)
{
	char* name = (char*)malloc(12);
	struct fcb* now = NULL;
	char c = path[0];
	if (c == '/') {
		now = root;
		path++;
	} else {
		now = current;
	}
	while (1) {
		if (!c) {
			free(name);
			return now;
		}
		int namelen = 0;
		for (namelen = 0; path[namelen] != '/' && path[namelen]; namelen++);
		memmove(name, path, namelen);
		name[namelen] = '\0';
		path += namelen;
		c = path[0];
		if (c == '/') {
			path++;
			c = path[0];
		}
		now = find_entry(name, now);
		now = read_dir(now);
	}
	
}

struct fcb* find_entry(const char* name, struct fcb* dir)
{
	struct fcb* fb = get_empty_fcb();
	int size = dir[0].size;
	for (int i = 1; i < size; i++) {
		if (compare(dir[i].filename, name) ){
			fb = &dir[i];
			return fb;
		}
	}
	return NULL;
}

struct fcb* sys_open(const char* filename)
{
	pre = current;
	struct fcb* fb = NULL;
	fb = get_dir(filename);
	if (fb->mode == 0) {
		if (pre != current) {
			current = fb;
			return NULL;
		}
	} else if (fb->mode == 1) {
		return fb;
	}
	return NULL;
}

int sys_close(const char* filename)
{
	struct fcb* fb = get_dir(filename);
	if (fb->mode == 0) {
		current = pre;
	}
	return 1;
}

int rename_file(const char* name, const char* newname)
{
	if (dup_name(newname)) {
		return 0;
	}
	for (int i = 1; i < current->size; i++) {
		if (compare(current[i].filename, name)) {
			int len = strlen(newname);
			for (int j = 0; j < len; j++) {
				current[i].filename[j] = newname[j];
			}
			if (len < 12) {
				current[i].filename[len] = '\0';
			}
			write_dir(current);
			return 1;
		}
	}
	return 0;
	
}

int change_loc(const char* path, const char* name)
{
	struct fcb* dect = get_dir(path);
	struct fcb* src = get_dir(name);
	if (dect == NULL || src == NULL) {
		return 0;
	}
	pre = current;
	current = dect;
	add(src);
	current = pre;
	rm_file(src->filename);
	return 1;
}

int rm_file(const char* path)
{
	pre = current;
	struct fcb* fb = get_dir(path);
	//回收磁盘空间
	if (fb->mode == 0) {
		if (fb[0].size > 1) {
			return 0;
		}
		recycle_block(fb->first_block);
	} else if (fb->mode == 1) {
		int next = fb->first_block;
		for (int i = 0; i < fb->size; i++) {
			recycle_block(next);
			struct i_block* i_blk = get_block(next);
			next = i_blk->next;
		}	
	}

	current = pre;
	//在目录中删除
	for (int i = 1; i < current->size; i++) {
		if (compare(current[i].filename, path)) {
			// 开始删除
			if (i != MAX_ENTRY_NUM - 1) {
				for (int j = i; j < MAX_ENTRY_NUM; j++) {
					current[j] = current[j + 1];
				}
			} 
			current->size--;
		}
	}
	write_dir(current);
}

// 文件共享
int share(const char* path1, const char* name)
{
	struct fcb* dect = get_dir(path1);
	struct fcb* src = get_dir(name);
	if (dect->mode == 1 && src->mode == 1) {
		src->first_block = dect->first_block;
	}
	return 0;

}

int cd(const char* path)
{
	if (strcmp(path, ".") == 0) {
		current = root;
		return 1;
	}
	struct fcb* fb = NULL;
	fb = get_dir(path);
	current = fb;
	return 1;
}

struct fcb* test()
{
	return get_dir("test");
}