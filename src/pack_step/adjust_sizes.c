#include"packer_tools.h"

bool adjust_sizes(size_t shift_amount){
    struct entry  clone_entry;
    const size_t  load_code_size=load_end-load_end;
    if(!find_entry(&clone_entry,clone_safe)){
        return errors(ERR_THROW,"adjust clone size");
    }
	clone_entry.safe_last_section_shdr->sh_size+=load_code_size;
	clone_entry.safe_phdr->p_filesz +=shift_amount;
	clone_entry.safe_phdr->p_memsz  +=shift_amount;

	return true;
}