#include "jaeho.h"

typedef struct inode
{
	char ftype;
	char time[31];
	char size[17];
	char direct[10];
	char single[10];
	char double[10];
} inode;


