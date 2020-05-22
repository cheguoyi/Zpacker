#ifndef ZPACKER_H
#define ZPACKER_H

#include<linux/elf.h>
#include<stdlib.h>
#include<stdbool.h>
#include"file.h"
#include"call_back_func.h"
# define OUTPUT_FILENAME	"packed_file"
//打包函数，负责给文件打包
bool filepakcer(char* file,size_t size);

#endif