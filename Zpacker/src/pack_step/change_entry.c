#include"packer_tools.h"

//修改入口
bool change_entry(struct entry *original_entry){
    Elf64_Ehdr *clone_header =clone_safe(0,sizeof(Elf64_Ehdr));
    if (!clone_header) return (errors(ERR_CORRUPT, "can't read clone header"));

    const size_t load_offset = original_entry->end_of_last_section;

    clone_header->e_entry=endian_8(load_offset);

    return true;
}