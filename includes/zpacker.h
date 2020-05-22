

#include<linux/elf.h>
#include<stdlib.h>


# define OUTPUT_FILENAME	"packed_file"



//打包函数，负责给文件打包
int filepakcer(char* file,size_t size);