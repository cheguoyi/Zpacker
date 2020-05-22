#include<stdlib.h>
#include"zpacker.h"


int main(int argc, char *argv[])
{
	//reutrn 返回值
	int		ret = EXIT_SUCCESS;
	// 文件大小
	size_t		filesize;

	// 未提供文件
	if (argc != 2)
	{	
		goto exit_failure;
	}
	//读取文件
	filesize = read_file(argv[1]);
	if (filesize == 0)
		goto exit_failure;

	//为文件指针分配缓存
	if (!alloc_clone(filesize))
		goto exit_failure;
	//调用elf64_packer
    if(filepakcer(argv[1],filesize)){
        printf("Pack file fail\n");
	    goto exit_failure;
    }

	if (!write_clone_file())
		goto exit_failure;

	printf("Successfully packed OUTPUT_FILENAME %s", argv[1]);

exit:
	free_clone();
	free_file();
	return (ret);
exit_failure:
	ret = EXIT_FAILURE;
	goto exit;
}
