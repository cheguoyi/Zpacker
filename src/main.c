#include"zpacker.h"
#include"file.h"

int main(int argc, char *argv[])
{

	// 文件大小
	size_t filesize;
	// 未提供文件
	if (argc < 2)
	{	
		goto exit;
	}

	//读取文件
    for(int i=1;i<argc;i++){
        filesize = read_file(argv[i]);
	    if (filesize == 0){
            goto exit;
        }
        //复制信息，无需memcpy
        original_file=filelist[i];
        //为文件指针分配缓存
	    if (!alloc_clone(filesize)){
            goto exit;
        }
	    //调用elf64_packer
        if(filepakcer(argv[i],filesize)){
            printf("Pack file %d :%s fail\n",i,argv[i]);
	        goto exit;
        }
	    if (!write_clone_file(argv[i])){
            goto exit;
        }
	    printf("Successfully packed OUTPUT_FILENAME %s", argv[i]);
    }


	
exit:
	free_clone();
	free_original();
	return 0;

}
