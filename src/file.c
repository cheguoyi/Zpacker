#include"file.h"
void free_clone(void){
    free(clone_file.pointer);
}

void free_original(void){
	if (original_file.pointer)
	{
		if (munmap(original_file.pointer, original_file.pointer)){
            printf("free_file false\n");
            return false;
        }
	}
	return (true);
}

bool write_clone_file(char *filename){

    char packedname[sizeof(filename)+10];
    strcpy(packedname,"packed_");
    strcat(packedname,filename);

    int fd=open(packedname,O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);  

    if(fd==-1){
        printf("failed creating file %s\n",packedname);
        return false;
    } 

    if(write(fd,clone_file.pointer,clone_file.filesize)==-1){
        close(fd);
        printf("failed to write %s\n",packedname);
        return false;
    }
	close(fd);
	return (true);
}