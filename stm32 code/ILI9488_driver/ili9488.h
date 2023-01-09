#ifndef ILI9488_H
#define ILI9488_H

#include "app.h"

#define RWS_000 (GPIOC->ODR = 0x00000000)
#define RWS_001 (GPIOC->ODR = 0x00002000)
#define RWS_010 (GPIOC->ODR = 0x00004000)
#define RWS_011 (GPIOC->ODR = 0x00006000)
#define RWS_100 (GPIOC->ODR = 0x00008000)
#define RWS_101 (GPIOC->ODR = 0x0000A000)
#define RWS_110 (GPIOC->ODR = 0x0000C000)
#define RWS_111 (GPIOC->ODR = 0x0000E000)



void init(void);
void DisplayImage(void);
void WriteData(u16 data);
void WriteCommand(u8 cmd);

void _NOP(void);
void _Reset(void);
void _SleepIn(void);
void _SleepOut(void);
void _PartialMode(void);
void _NormalMode(void);
void _InversionOff(void);
void _InversionOn(void);
void _PxielOff(void);
void _PxielOn(void);
void _DisplayOn(void);
void _DisplayOff(void);
void _Column(u16 begin,u16 end);
void _Page(u16 begin,u16 end);
void _IDLEModeOff(void);
void _IDLEModeOn(void);


#endif
