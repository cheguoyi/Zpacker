#ifndef ENTRY_H
#define ENTRY_H

#include<stddef.h>
#include<elf.h>

struct				entry
{
	struct elf64_phdr	*safe_phdr;
	struct elf64_shdr	*safe_shdr;
	struct elf64_shdr	*safe_last_section_shdr;
	size_t			end_of_last_section;
	size_t			offset_in_section;
};

#endif