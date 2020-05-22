#ifndef FILE_H
#define FILE_H

#include<stddef.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
# include<fcntl.h>
# include<sys/stat.h>
# include <sys/mman.h>
#define FILE_NUM_MAX 4
struct file_point filelist[FILE_NUM_MAX];

void free_clone(void);
void free_original(void);

bool write_clone_file(char *filename);
//文件类型
typedef struct file_point {
    void *pointer;
    size_t filesize;
} File_ptr;

struct file_point original_file;
struct file_point clone_file;
#endif