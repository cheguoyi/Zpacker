#ifndef CALL_BACK_FUNC_H
#define CALL_BACK_FUNC_H

#include"safe_func.h"
//回调函数类型 参数分别为内存检查器函数，以及偏移量
typedef	 bool  (*f_iter_callback)(check_safe_func_pointer safe, const size_t offset);

#endif