#include"packer_tools.h"

bool adjust_sizes(size_t shift_amount){
	struct entry	clone_entry;
	const size_t	payload_size = load_end-load_begin;

    //elf64_viewer(clone_safe);
	if (!find_entry(&clone_entry, clone_safe))
		return errors(ERR_THROW, "adjust_sizes");

	size_t		sh_size  = endian_8(clone_entry.safe_last_section_shdr->sh_size);
	Elf64_Xword	p_filesz = endian_8(clone_entry.safe_phdr->p_filesz);
	Elf64_Xword	p_memsz  = endian_8(clone_entry.safe_phdr->p_memsz);

	sh_size  += payload_size;
	p_filesz += shift_amount;
	p_memsz  += shift_amount;

	clone_entry.safe_last_section_shdr->sh_size = endian_8(sh_size);
	clone_entry.safe_phdr->p_filesz = endian_8(p_filesz);
	clone_entry.safe_phdr->p_memsz  = endian_8(p_memsz);

	return true;
}