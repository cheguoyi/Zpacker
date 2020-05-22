#ifndef PACKER_TOOLS_H
#define PACKER_TOOLS_H
#include"entry.h"
#include"call_back_func.h"
#include"file.h"
//获取入口信息
bool find_entry(struct entry *original_entry, check_safe_func_pointer original_safe);
//确定如何插入，如果segment剩余量足以插入就不再realloc
bool define_shift_amount(struct entry *original_entry, size_t &shift_amount);
//拷贝源文件信息，留出插入代码空间
bool copy_to_clone(size_t end_of_last_sect,size_t shift_amount, size_t original_file_size);
//修改入口信息以及各个section和segment的偏移位置等信息
bool adjust_references(size_t shift_amount, struct entry *original_entry);
//修改入口所在segment和section的大小
bool adjust_sizes(size_t shift_amount);
//装入新的入口
bool setup_payload(struct entry *original_entry);
//修改入口地址，主要是对elf_header的修改
bool change_entry(struct entry *original_entry);

#endif