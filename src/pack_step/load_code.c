#include"packer_tools.h"

# define CALL_INSTR_SIZE	5 /* sizeof "call mark_below" -> e8 2000 0000 */

bool setup_load_code(struct entry *original_entry){
    struct real_entry_info info;
    fill_info(&info,original_entry);

    const size_t code_size=load_end-load_begin;

    const size_t sec_size=endian_8(original_entry->safe_shdr->sh_size);

    const size_t load_off=original_entry->end_of_last_section;

    const size_t sec_off=load_off-info.relative_sec_start_address;

    void *code_location = clone_safe(load_off,code_size);

    void *info_location = clone_safe(load_off+CALL_INSTR_SIZE,sizeof(struct real_entry_info));

    void *sec_location = clone_safe(sec_off ,sec_size);

    if(!code_location||!info_location||!sec_location){
        return errors(ERR_CORRUPT,"memory error at setup_load_code");
    }

    encrypt(32,sec_location,info.key,sec_size);

    memcpy(code_location,load_begin,code_size);

    memcpy(info_location,&info,sizeof(info));

    return true;

}

void fill_info(struct real_entry_info *info,struct entry *original_entry){
    memcpy(info->key,SECRET_SIGNATURE,SECRET_LEN);
    generate_key((char*)info->key+SECRET_LEN,16-SECRET_LEN);
    const size_t end_of_last_section=original_entry->end_of_last_section;
    //Segment file offset
	const Elf64_Off		p_offset  = endian_8(original_entry->safe_phdr->p_offset);
    //入口段在内存中的大小
	const Elf64_Xword	p_memsz   = endian_8(original_entry->safe_phdr->p_memsz);
	//入口Section file offset
	const Elf64_Off		sh_offset = endian_8(original_entry->safe_shdr->sh_offset);
    //入口seciton的大小
	const size_t		sh_size   = endian_8(original_entry->safe_shdr->sh_size);

    info->relative_seg_start_address=end_of_last_section-p_offset;
    info->relative_sec_start_address=end_of_last_section-sh_offset;
    info->relative_entry_address=info->relative_sec_start_address - original_entry->offset_in_section;
    info->seg_size=p_memsz;
    info->text_size=sh_size;

    return;
    
}

void generate_key(char *buf,size_t size){
    srand(time(NULL));
    for(size_t i=0;i<size;i++){
        buf[i]=rand();
    }
    return;
}

