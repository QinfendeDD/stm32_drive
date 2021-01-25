#ifndef __LCT_7_FONTS_H
#define __LCT_7_FONTS_H

#include <stdint.h>

// 字体相关结构定义
typedef struct _pFont
{    
  const uint8_t *table;  //字模数组地址
  uint16_t Width;  //字模宽度
  uint16_t Height; //字模长度
  uint16_t Sizes;	 //字模个数
} pFONT;

extern pFONT Font32;	// 3216 字体
extern pFONT Font24;	// 2412 字体
extern pFONT Font20; // 2010 字体
extern pFONT Font16; // 1608 字体
extern pFONT Font12; // 1206 字体

#endif 
 
