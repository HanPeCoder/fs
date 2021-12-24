#pragma once
#ifndef FCB_H
#define FCB_H

struct fcb
{
	char* filename;
	int first_block;
	int end_block;
	int size;
} typedef fcb ;

#endif // !FCB_H
