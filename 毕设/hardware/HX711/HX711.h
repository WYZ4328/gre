#ifndef __HX711_H
#define __HX711_H

#include "sys.h"

#define HX711_SCK  PAout(2)  // PA2 - SCK
#define HX711_DOUT PAin(1)   // PA1 - DT


extern void Init_HX711pin(void);
extern u32 HX711_Read(void);
void Get_Maopi(void);
void Get_Weight(void);

extern u32 HX711_Buffer;
extern u32 Weight_Maopi;
extern s32 Weight_Shiwu;
extern u8 Flag_Error;

#endif

