#include"safe_func.h"
#include"errors.h"
# include <sys/stat.h>
__attribute__((warn_unused_result))
void *original_safe(const size_t offset, const size_t size)
{
	if (offset + size > original_file.filesize || offset + size < offset){
        return (NULL);
    }
	return (original_file.pointer + offset);
}
//输出文件缓存内存检查器
__attribute__((warn_unused_result))
void		*clone_safe(const size_t offset, const size_t size)
{
	if (offset + size > clone_file.filesize || offset + size < offset)
		return (NULL);
	return (clone_file.pointer + offset);
}
//读取要加密的二进制文件
__attribute__((warn_unused_result))
size_t read_file(const char *filename){
    void *ptr; 
	struct stat	buf;
	int	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (errors(ERR_SYS, "open failed"));
	if (fstat(fd, &buf) < 0)
		return (errors(ERR_SYS, "fstat failed"));
    if (buf.st_mode & S_IFDIR)
		return (errors(ERR_USAGE, "can't parse directories"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(ERR_SYS, "mmap failed"));
	if (close(fd))
		return (errors(ERR_SYS, "close failed"));
	
	original_file.pointer = ptr;
	original_file.filesize = buf.st_size;
	return (original_file.filesize);
}

__attribute__((warn_unused_result))
bool alloc_clone(const size_t original_filesize)
{
	clone_file.filesize = original_filesize;
	clone_file.pointer = malloc(clone_file.filesize);
	if (clone_file.pointer == NULL)
		return (errors(ERR_SYS, "while allocating clone"));
	return (true);
}
__attribute__((warn_unused_result))
bool resize_clone(const size_t added_size)
{
	clone_file.filesize += added_size;
	clone_file.pointer = realloc(clone_file.pointer,clone_file.filesize);
	if (clone_file.pointer == NULL)
		return (errors(ERR_SYS, "while reallocating clone"));

	return (true);
}