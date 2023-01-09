#include <ili9488.h>


void ili9488_init(void)
{
	// Hardware Reset:
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(200);

	_SleepOut();

    WriteCommand(0xE0); //P-Gamma
    WriteData(0x00);
    WriteData(0x13);
    WriteData(0x18);
    WriteData(0x04);
    WriteData(0x0F);
    WriteData(0x06);
    WriteData(0x3A);
    WriteData(0x56);
    WriteData(0x4D);
    WriteData(0x03);
    WriteData(0x0A);
    WriteData(0x06);
    WriteData(0x30);
    WriteData(0x3E);
    WriteData(0x0F);

    WriteCommand(0XE1); //N-Gamma
    WriteData(0x00);
    WriteData(0x13);
    WriteData(0x18);
    WriteData(0x01);
    WriteData(0x11);
    WriteData(0x06);
    WriteData(0x38);
    WriteData(0x34);
    WriteData(0x4D);
    WriteData(0x06);
    WriteData(0x0D);
    WriteData(0x0B);
    WriteData(0x31);
    WriteData(0x37);
    WriteData(0x0F);

    WriteCommand(0XC0);   //Power Control 1
    WriteData(0x18); //Vreg1out
    WriteData(0x17); //Verg2out

    WriteCommand(0xC1);   //Power Control 2
    WriteData(0x41); //VGH,VGL

    WriteCommand(0xC5);   //Power Control 3
    WriteData(0x00);
    WriteData(0x1A); //Vcom
    WriteData(0x80);

    WriteCommand(0x36);
    WriteData(0xE8);  // MY MX MV ML BGR MH HF VF  4A     48  08
    //WriteData(0xE8);  // MY MX MV ML BGR MH X X
                    // MY Row Address Order
                    // MX Column Address Order
                    // MV Row / Column Exchange
                    // ML Vertical Refresh Order
                    // (0=RGB color filter panel, 1=BGR color filter panel)
                    // MH Horizontal Refresh ORDER
                    // Horizontal Flip x
                    // Vertical Flip  y

    WriteCommand(0x3A);   // Interface Pixel Format
    //WriteData(0x55); //16bit
    WriteData(0x66); //18bit

    WriteCommand(0XB0);   // Interface Mode Control
    WriteData(0x00);

    WriteCommand(0xB1);   //Frame rate
    WriteData(0xA0); //60Hz

    WriteCommand(0xB4);   //Display Inversion Control
    //WriteData(0x02); 	//2-dot
    WriteData(0x00); 	//Column inversion

    WriteCommand(0XB6);   //RGB/MCU Interface Control
    WriteData(0x02); //MCU RGB
    WriteData(0x02); //Source,Gate scan dieection

    WriteCommand(0XE9);    // Set Image Function
    WriteData(0x00);  //disable 24 bit data input

    WriteCommand(0xF7);    // Adjust Control
    WriteData(0xA9);
    WriteData(0x51);

    _NormalMode();
    _InversionOff();
    _IDLEModeOff();
	_DisplayOn();

	_Column(0x0000,0x01DF);
	_Page(0x0000,0x013F);
    HAL_Delay(120);

	LOG("TFT now has been initialized:\n");
	LOG("|------------------------------------------------|\n");
}


void WriteCommand(u8 cmd)
{
	// RD:1, WR:0->1, RS:0

	RWS_100;

	GPIOB->ODR = (u32)cmd;

	RWS_110;
}
void WriteData(u16 data)
{
	// RD:1, WR:0->1, RS:1

	RWS_101;

	GPIOB->ODR = (u32)data;

	RWS_111;
}
u8 ReadData(void)
{

	GPIOB->ODR = 0; // Clear

	GPIOB->MODER = 0;  // Set as input mode before read

	RWS_101;

	HAL_Delay(1);

	u8 rtnval = (u8)(GPIOB->IDR);

	HAL_Delay(1);

	RWS_111;

	GPIOB->MODER = 0x55555555;  // Set as output mode after read

	return rtnval;
}

void _NOP(void)						{WriteCommand(0x00);}
void _Reset(void)						{WriteCommand(0x01);HAL_Delay(120);}
void _SleepIn(void)					{WriteCommand(0x10);}
void _SleepOut(void)					{WriteCommand(0x11);HAL_Delay(5);}
void _PartialMode(void)				{WriteCommand(0x12);}
void _NormalMode(void)					{WriteCommand(0x13);}
void _InversionOff(void)				{WriteCommand(0x20);}
void _InversionOn(void)				{WriteCommand(0x21);}
void _PxielOff(void)					{WriteCommand(0x22);}
void _PxielOn(void)					{WriteCommand(0x23);}
void _DisplayOff(void)					{WriteCommand(0x28);}
void _DisplayOn(void)					{WriteCommand(0x29);}
void _Column(u16 begin,u16 end)		{WriteCommand(0x2A);WriteData(begin>>8);WriteData(begin);WriteData(end>>8);WriteData(end);}
void _Page(u16 begin,u16 end)			{WriteCommand(0x2B);WriteData(begin>>8);WriteData(begin);WriteData(end>>8);WriteData(end);}
void _IDLEModeOff(void)				{WriteCommand(0x38);}
void _IDLEModeOn(void)					{WriteCommand(0x39);}


void  DisplayImage(void)
{

	_Column(0x0000,0x01DF);
	_Page(0x0000,0x013F);
	WriteCommand(0x2C);
	_NOP();
}


