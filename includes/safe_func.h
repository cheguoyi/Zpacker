#ifndef SAFE_FUNC_H
#define SAFE_FUNC_H

#include<stddef.h>
# include <stdbool.h>
//内存检查器类型的函数 参数分别为偏移量和大小
typedef  void *(*check_safe_func_pointer)(const size_t offset,const size_t size);

void *original_safe(const size_t offset, const size_t size);

#endif