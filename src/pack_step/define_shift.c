#include"packer_tools.h"
# define WOODY_ALIGNMENT	4096
# define ALIGN(x, n)		(((x) + (n)) & ~((n) - 1))

bool define_shift_amount(const struct entry *original_entry, size_t *shift_amount)
{
	//Segment size in file
	const size_t	p_filesz        = endian_8(original_entry->safe_phdr->p_filesz);
	//Segment file offset
	const size_t	p_offset        = endian_8(original_entry->safe_phdr->p_offset);
	const size_t	segment_end     = p_offset + p_filesz;
	const size_t	payload_size    = load_end - load_end;
	const size_t	segment_padding = segment_end - original_entry->end_of_last_section;

	if (payload_size < segment_padding)
	{
		*shift_amount = 0;
		return true;
	}
	// Segment size in memory
	const size_t	p_memsz = endian_8(original_entry->safe_phdr->p_memsz);
	// Segment alignment, file & memory
	const size_t	p_align = endian_8(original_entry->safe_phdr->p_align);
	// 得到偏移大小，页取整
	*shift_amount = ALIGN(payload_size, WOODY_ALIGNMENT);
	//调整输出文件缓存大小
	if (!resize_clone(*shift_amount))
		return errors(ERR_THROW, "define_shift_amount");
	// 如果不需要对齐 最后的位置应该是增加的页面大小+原来的大小  如果需要对齐就是p_align+增加的页面
	const size_t	end_padding = (p_memsz % p_align) + *shift_amount;

	if (end_padding > p_align)
		return errors(ERR_USAGE, "insufficient memory padding "
		"in file (overflow of %lu bytes)", (end_padding - p_align));

	return true;
}