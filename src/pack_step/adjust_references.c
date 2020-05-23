#include"packer_tools.h"

static struct{
	size_t	shift_amount;
	size_t	end_last_sect;
}		closure;

bool adjust_references(size_t shift_amount, struct entry *original_entry){
    //无需调整，因为其他section无偏移
    if(shift_amount==0){
        return true;
    }
    //保存参数信息
    closure.shift_amount=shift_amount;
    closure.end_last_sect=original_entry->end_of_last_section;
    //读取elf_header
    Elf64_Ehdr *elf_header=clone_safe(0,sizeof(Elf64_Ehdr));
    if(elf_header==NULL) return errors(ERR_CORRUPT,"fail to read header in adjust_reference");
    //不需要更该segment信息
    //这里修改了section table的位置
    adjust_sheader_table_offset(elf_header);
    //遍历所有的segment，如果在插入代码段后，则修改偏移量
    if(!foreach_phdr(clone_safe,shift_seg_header_position)){
        return errors(ERR_THROW,"adjust seg header reference ");
    }
    //遍历所有的section，如果在插入代码段后，则修改偏移量
    if(!foreach_shdr(clone_safe,shift_sec_header_position)){
        return errors(ERR_THROW,"adjust sec header reference ");
    }
    return true;
}
static void adjust_sheader_table_offset(Elf64_Ehdr *header){
    Elf64_Off sec_header_off=endian_8(header->e_phoff);
    sec_header_off+=closure.shift_amount;
    header->e_shoff=endian_8(sec_header_off);
    return;
}
//回调函数，用来调整插入代码段后所有section的位置信息
static bool shift_sec_header_position(check_safe_func_pointer csf,const size_t offset){
    Elf64_Shdr *sec_header=csf(offset,sizeof(Elf64_Shdr));
    if(sec_header==NULL) return errors(ERR_CORRUPT,"fail to get section header : shift sec position");
    Elf64_Off sec_offset=endian_8(sec_header->sh_offset);
    if(sec_offset>closure.end_last_sect){
        sec_offset+=closure.shift_amount;
        sec_header->sh_offset=endian_8(sec_offset);
    }
    return true;
}
//回调函数，用来调整插入代码段后所有segment的位置信息
static bool shift_seg_header_position(check_safe_func_pointer csf,const size_t offset){
    Elf64_Phdr *seg_header=csf(offset,sizeof(Elf64_Phdr));
    if(seg_header==NULL) return errors(ERR_CORRUPT,"fail to get segment header : shift seg position" );
    Elf64_Off seg_offset=endian_8(seg_header->p_offset);
    if(seg_offset>closure.end_last_sect){
        seg_offset+=closure.shift_amount;
        seg_header->p_offset=seg_offset;
    }
    return true;
}
