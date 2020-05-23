#ifndef FILE_H
#define FILE_H

#include<stddef.h>
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <sys/mman.h>

#define FILE_NUM_MAX 4

//文件类型
typedef struct file_point {
    void *pointer;
    size_t filesize;
} File_ptr;

void free_clone(void);
void free_original(void);
bool write_clone_file(char *filename);
size_t	read_file(const char *filename);
bool alloc_clone(filesize);
bool alloc_clone(const size_t original_filesize);
bool resize_clone(const size_t added_size);
struct file_point filelist[FILE_NUM_MAX];
struct file_point original_file;
struct file_point clone_file;

#endif