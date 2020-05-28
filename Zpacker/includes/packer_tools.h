#ifndef PACKER_TOOLS_H
#define PACKER_TOOLS_H

#include<stdint.h>
#include<stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
#include<time.h>

# include "errors.h"
#include"entry.h"
#include"call_back_func.h"
#include"file.h"
#include"errors.h"
#include"endians.h"

//获取入口信息
bool find_entry(struct entry *original_entry, check_safe_func_pointer original_safe);
//确定如何插入，如果segment剩余量足以插入就不再realloc
bool define_shift_amount(const struct entry *original_entry, size_t *shift_amount);
//拷贝源文件信息，留出插入代码空间
bool copy_to_clone(size_t end_of_last_sect,size_t shift_amount, size_t original_file_size);
//修改入口信息以及各个section和segment的偏移位置等信息
bool adjust_references(size_t shift_amount, struct entry *original_entry);
//修改入口所在segment和section的大小
bool adjust_sizes(size_t shift_amount);
//装入新的入口
bool setup_load_code(struct entry *original_entry);
//修改入口地址，主要是对elf_header的修改
bool change_entry(struct entry *original_entry);


//遍历program header table
bool	foreach_phdr(check_safe_func_pointer safe, f_iter_callback callback);
//遍历section header table
bool	foreach_shdr(check_safe_func_pointer safe, f_iter_callback callback);

//插入代码段开头位置
void	load_begin(void);
//插入代码段结束位置
void	load_end(void);


void		encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);
void		decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);


//入口信息
struct real_entry_info{
    uint32_t key[4];
    uint64_t relative_seg_start_address;
    uint64_t seg_size;
    uint64_t relative_sec_start_address;
    uint64_t relative_entry_address;
    uint64_t text_size;
}__attribute__((packed));

#define SECRET_SIGNATURE "zpacker"
#define SECRET_LEN sizeof(SECRET_SIGNATURE)

void adjust_phdr_table_offset(Elf64_Ehdr *safe_elf_hdr);
void adjust_sheader_table_offset(Elf64_Ehdr *header);
//回调函数，用来调整插入代码段后所有section的位置信息
bool shift_sec_header_position(check_safe_func_pointer csf,const size_t offset);
//回调函数，用来调整插入代码段后所有segment的位置信息
bool shift_seg_header_position(check_safe_func_pointer csf,const size_t offset);

void fill_info(struct real_entry_info *info,struct entry *original_entry);

void generate_key(char *buf,size_t size);






bool	elf64_identifier(check_safe_func_pointer safe);
bool	elf64_viewer(check_safe_func_pointer safe);

#endif