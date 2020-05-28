#include"packer_tools.h"

bool	foreach_phdr(check_safe_func_pointer safe, f_iter_callback callback)
{
	//获得header信息
	const Elf64_Ehdr	*elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (elf64_hdr == NULL) return (errors(ERR_CORRUPT, "no elf64 hdr"));

	//program header table 偏移量
	const Elf64_Off		phoff     = endian_8(elf64_hdr->e_phoff);
	//Size of program header entry
	const Elf64_Half	phentsize = endian_2(elf64_hdr->e_phentsize);
	//Number of program header entries
	Elf64_Half		phnum     = endian_2(elf64_hdr->e_phnum);
	//用来存放segment信息的char指针
	char			(*segments)[phnum][phentsize] = NULL;
	//二维数组大小
	const size_t		array_size = phentsize * phnum;

	if (phentsize < sizeof(Elf64_Phdr)//segment大小不对
	|| (array_size / phentsize != phnum)//二维数组大小不够
	|| (!(segments = safe(phoff, array_size))))//段信息获取
		return (errors(ERR_CORRUPT, "invalid segments table"));
    //开始遍历所有的segment header entry
	while (phnum--)
	{
		size_t	elf64_seg_hdr = (size_t)(*segments)[phnum];
		size_t	offset        = (elf64_seg_hdr - (size_t)elf64_hdr);

		if (!callback(safe, offset))
			return (errors(ERR_THROW, "foreach_phdr"));
	}
	return (true);
}

bool	foreach_shdr(check_safe_func_pointer safe, f_iter_callback callback)
{
	//header
	const Elf64_Ehdr	*elf64_hdr = safe(0, sizeof(Elf64_Ehdr));

	if (elf64_hdr == NULL) return (errors(ERR_CORRUPT, "no elf64 hdr"));
	//Section header table file offset
	const Elf64_Off		shoff     = endian_8(elf64_hdr->e_shoff);
	//Size of section header entry
	const Elf64_Half	shentsize = endian_2(elf64_hdr->e_shentsize);
	//Number of section header entries
	Elf64_Half		shnum     = endian_2(elf64_hdr->e_shnum);
	// seciton信息
	char			(*sections)[shnum][shentsize] = NULL;
	const size_t		array_size = shentsize * shnum;
    //错误检查
	if (shentsize < sizeof(Elf64_Shdr)//section header的大小不对
	|| (array_size / shentsize != shnum)//二维数组大小不够
	|| (!(sections = safe(shoff, array_size))))//section信息获取
		return (errors(ERR_CORRUPT, "invalid sections table"));
    //遍历所有的section header entry
	while (shnum--)
	{
		size_t	elf64_section_hdr = (size_t)(*sections)[shnum];
		size_t	offset = (elf64_section_hdr - (size_t)elf64_hdr);

		if (!callback(safe, offset))
			return (errors(ERR_THROW, "foreach_shdr"));
	}
	return (true);
}
