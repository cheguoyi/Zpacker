#ifndef ENDIANS_H
#define ENDIANS_H

#include<stddef.h>
#include<stdbool.h>
#include<stdint.h>

void endian_big_mode(bool is_big_endian);
uint16_t endian_2(uint16_t n);
uint32_t endian_4(uint32_t n);
uint64_t endian_8(uint64_t n);



#endif