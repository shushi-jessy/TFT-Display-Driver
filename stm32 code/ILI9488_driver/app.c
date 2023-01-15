#include <app.h>


//file	fs;
//ili9488 driver;


void app_init(void)
{
	uart1_init();
	file_init();
	ili9488_init();



	u16 char_A[32] = { 0x0000,	// Row 0
					   0x0000,	// Row 1
					   0x0000,	// Row 2
					   0x0000,	// Row 3
					   0x07E0,	// Row 4
					   0x0FF0,	// Row 5
					   0x1FF8,	// Row 6
					   0x3FFC,	// Row 7
					   0x381C,	// Row 8
					   0x381C,	// Row 9
					   0x381C,	// Row 10
					   0x381C,	// Row 11
					   0x381C,	// Row 12
					   0x381C,	// Row 13
					   0x3FFC,	// Row 14
					   0x3FFC,	// Row 15
					   0x3FFC,	// Row 16
					   0x3FFC,	// Row 17
					   0x381C,	// Row 18
					   0x381C,	// Row 19
					   0x381C,	// Row 20
					   0x381C,	// Row 21
					   0x381C,	// Row 22
					   0x381C,	// Row 23
					   0x381C,	// Row 24
					   0x381C,	// Row 25
					   0x381C,	// Row 26
					   0x381C,	// Row 27
					   0x0000,	// Row 28
					   0x0000,	// Row 29
					   0x0000,	// Row 30
					   0x0000};	// Row 31


	u16 char_B[32] = { 0x0000,	// Row 0
					   0x0000,	// Row 1
					   0x0000,	// Row 2
					   0x0000,	// Row 3
					   0x3FE0,	// Row 4
					   0x3FF0,	// Row 5
					   0x3FF8,	// Row 6
					   0x3FFC,	// Row 7
					   0x381C,	// Row 8
					   0x381C,	// Row 9
					   0x381C,	// Row 10
					   0x381C,	// Row 11
					   0x381C,	// Row 12
					   0x381C,	// Row 13
					   0x3FF8,	// Row 14
					   0x3FF0,	// Row 15
					   0x3FF0,	// Row 16
					   0x3FF8,	// Row 17
					   0x381C,	// Row 18
					   0x381C,	// Row 19
					   0x381C,	// Row 20
					   0x381C,	// Row 21
					   0x381C,	// Row 22
					   0x381C,	// Row 23
					   0x3FFC,	// Row 24
					   0x3FF8,	// Row 25
					   0x3FF0,	// Row 26
					   0x3FE0,	// Row 27
					   0x0000,	// Row 28
					   0x0000,	// Row 29
					   0x0000,	// Row 30
					   0x0000};	// Row 31

	u16 char_C[32] = { 0x0000,	// Row 0
					   0x0000,	// Row 1
					   0x0000,	// Row 2
					   0x0000,	// Row 3
					   0x07E0,	// Row 4
					   0x0FF0,	// Row 5
					   0x1FF8,	// Row 6
					   0x3FFC,	// Row 7
					   0x3E1C,	// Row 8
					   0x3C0C,	// Row 9
					   0x3804,	// Row 10
					   0x3800,	// Row 11
					   0x3800,	// Row 12
					   0x3800,	// Row 13
					   0x3800,	// Row 14
					   0x3800,	// Row 15
					   0x3800,	// Row 16
					   0x3800,	// Row 17
					   0x3800,	// Row 18
					   0x3800,	// Row 19
					   0x3800,	// Row 20
					   0x3804,	// Row 21
					   0x3C0C,	// Row 22
					   0x3E1C,	// Row 23
					   0x3FFC,	// Row 24
					   0x1FF8,	// Row 25
					   0x0FF0,	// Row 26
					   0x07E0,	// Row 27
					   0x0000,	// Row 28
					   0x0000,	// Row 29
					   0x0000,	// Row 30
					   0x0000};	// Row 31

	u16 char_D[32] = { 0x0000,	// Row 0
					   0x0000,	// Row 1
					   0x0000,	// Row 2
					   0x0000,	// Row 3
					   0x3FE0,	// Row 4
					   0x3FF0,	// Row 5
					   0x3FF8,	// Row 6
					   0x3FFC,	// Row 7
					   0x381C,	// Row 8
					   0x380C,	// Row 9
					   0x380C,	// Row 10
					   0x380C,	// Row 11
					   0x380C,	// Row 12
					   0x380C,	// Row 13
					   0x380C,	// Row 14
					   0x380C,	// Row 15
					   0x380C,	// Row 16
					   0x380C,	// Row 17
					   0x380C,	// Row 18
					   0x380C,	// Row 19
					   0x380C,	// Row 20
					   0x380C,	// Row 21
					   0x380C,	// Row 22
					   0x381C,	// Row 23
					   0x3FFC,	// Row 24
					   0x3FF8,	// Row 25
					   0x3FF0,	// Row 26
					   0x3FE0,	// Row 27
					   0x0000,	// Row 28
					   0x0000,	// Row 29
					   0x0000,	// Row 30
					   0x0000};	// Row 31

	u16 char_E[32] = { 0x0000,	// Row 0
					   0x0000,	// Row 1
					   0x0000,	// Row 2
					   0x0000,	// Row 3
					   0x3FFC,	// Row 4
					   0x3FFC,	// Row 5
					   0x3FFC,	// Row 6
					   0x3FFC,	// Row 7
					   0x3800,	// Row 8
					   0x3800,	// Row 9
					   0x3800,	// Row 10
					   0x3800,	// Row 11
					   0x3800,	// Row 12
					   0x3800,	// Row 13
					   0x3FFC,	// Row 14
					   0x3FFC,	// Row 15
					   0x3FFC,	// Row 16
					   0x3FFC,	// Row 17
					   0x3800,	// Row 18
					   0x3800,	// Row 19
					   0x3800,	// Row 20
					   0x3800,	// Row 21
					   0x3800,	// Row 22
					   0x3800,	// Row 23
					   0x3800,	// Row 24
					   0x3FFC,	// Row 25
					   0x3FFC,	// Row 26
					   0x3FFC,	// Row 27
					   0x0000,	// Row 28
					   0x0000,	// Row 29
					   0x0000,	// Row 30
					   0x0000};	// Row 31

	u16 char_F[32] = { 0x0000,	// Row 0
					   0x0000,	// Row 1
					   0x0000,	// Row 2
					   0x0000,	// Row 3
					   0x3FFC,	// Row 4
					   0x3FFC,	// Row 5
					   0x3FFC,	// Row 6
					   0x3FFC,	// Row 7
					   0x3800,	// Row 8
					   0x3800,	// Row 9
					   0x3800,	// Row 10
					   0x3800,	// Row 11
					   0x3800,	// Row 12
					   0x3800,	// Row 13
					   0x3FFC,	// Row 14
					   0x3FFC,	// Row 15
					   0x3FFC,	// Row 16
					   0x3FFC,	// Row 17
					   0x3800,	// Row 18
					   0x3800,	// Row 19
					   0x3800,	// Row 20
					   0x3800,	// Row 21
					   0x3800,	// Row 22
					   0x3800,	// Row 23
					   0x3800,	// Row 24
					   0x3800,	// Row 25
					   0x3800,	// Row 26
					   0x3800,	// Row 27
					   0x0000,	// Row 28
					   0x0000,	// Row 29
					   0x0000,	// Row 30
					   0x0000};	// Row 31
	u16 char_G[32] = { 0x0000,	// Row 0
					   0x0000,	// Row 1
					   0x0000,	// Row 2
					   0x0000,	// Row 3
					   0x07E0,	// Row 4
					   0x0FF0,	// Row 5
					   0x1FF8,	// Row 6
					   0x3FFC,	// Row 7
					   0x3E1C,	// Row 8
					   0x3C0C,	// Row 9
					   0x3804,	// Row 10
					   0x3800,	// Row 11
					   0x3800,	// Row 12
					   0x3800,	// Row 13
					   0x3800,	// Row 14
					   0x3800,	// Row 15
					   0x3800,	// Row 16
					   0x3800,	// Row 17
					   0x38FC,	// Row 18
					   0x38FC,	// Row 19
					   0x38FC,	// Row 20
					   0x381C,	// Row 21
					   0x3C1C,	// Row 22
					   0x3E1C,	// Row 23
					   0x3FFC,	// Row 24
					   0x1FF8,	// Row 25
					   0x0FF0,	// Row 26
					   0x07E0,	// Row 27
					   0x0000,	// Row 28
					   0x0000,	// Row 29
					   0x0000,	// Row 30
					   0x0000};	// Row 31




	u16 font_color    = 0x001F;
	u16 background    = 0x0000;

	u16 x_pos = 319;
    u16 y_pos = 0;

	_Column(x_pos-15,x_pos);
	_Page(y_pos,y_pos+31);
	x_pos = x_pos -16;
	y_pos = y_pos + 0;
	WriteCommand(0x2C);
    for(u8 i=0;i<32;i++)for(u8 j=0;j<16;j++){if(char_A[i] & (1<<j)) WriteData(font_color);else WriteData(background);}
	_NOP();

	_Column(x_pos-15,x_pos);
	_Page(y_pos,y_pos+31);
	x_pos = x_pos -16;
	y_pos = y_pos + 0;
	WriteCommand(0x2C);
	for(u8 i=0;i<32;i++)for(u8 j=0;j<16;j++){if(char_B[i] & (1<<j)) WriteData(font_color);else WriteData(background);}
	_NOP();

	_Column(x_pos-15,x_pos);
	_Page(y_pos,y_pos+31);
	x_pos = x_pos -16;
	y_pos = y_pos + 0;
	WriteCommand(0x2C);
	for(u8 i=0;i<32;i++)for(u8 j=0;j<16;j++){if(char_C[i] & (1<<j)) WriteData(font_color);else WriteData(background);}
	_NOP();

	_Column(x_pos-15,x_pos);
	_Page(y_pos,y_pos+31);
	x_pos = x_pos -16;
	y_pos = y_pos + 0;
	WriteCommand(0x2C);
	for(u8 i=0;i<32;i++)for(u8 j=0;j<16;j++){if(char_D[i] & (1<<j)) WriteData(font_color);else WriteData(background);}
	_NOP();

	_Column(x_pos-15,x_pos);
	_Page(y_pos,y_pos+31);
	x_pos = x_pos -16;
	y_pos = y_pos + 0;
	WriteCommand(0x2C);
	for(u8 i=0;i<32;i++)for(u8 j=0;j<16;j++){if(char_E[i] & (1<<j)) WriteData(font_color);else WriteData(background);}
	_NOP();

	_Column(x_pos-15,x_pos);
	_Page(y_pos,y_pos+31);
	x_pos = x_pos -16;
	y_pos = y_pos + 0;
	WriteCommand(0x2C);
	for(u8 i=0;i<32;i++)for(u8 j=0;j<16;j++){if(char_F[i] & (1<<j)) WriteData(font_color);else WriteData(background);}
	_NOP();

	_Column(x_pos-15,x_pos);
	_Page(y_pos,y_pos+31);
	x_pos = x_pos -16;
	y_pos = y_pos + 0;
	WriteCommand(0x2C);
	for(u8 i=0;i<32;i++)for(u8 j=0;j<16;j++){if(char_G[i] & (1<<j)) WriteData(font_color);else WriteData(background);}
	_NOP();

	while(1)
	{
		//AT_DrawPixel();
	}



















		/*ADC_ChannelConfTypeDef sConfig = {0};
		float alpha = 0.8;
		float gain  = 1;
		u16 reslut_y1[4] = {0};
		u16 reslut_y0[4] = {0};
		u16 delta[4]   = {0};


		for(u8 i=0;i<5;i++)
		{
			sConfig.Channel = i;
			if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)Error_Handler();
			HAL_ADC_Start(&hadc);
			HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
			reslut_y1[i] =  gain * HAL_ADC_GetValue(&hadc) * (1 - alpha) + reslut_y0[i] * alpha;
			if(reslut_y0[i] > reslut_y1[i])
				delta[i] = reslut_y0[i] - reslut_y1[i];
			else
				delta[i] = reslut_y1[i] - reslut_y0[i];
			reslut_y0[i] =  reslut_y1[i];
			LOG("CH");LOGn(i);LOG(": ");LOGn(delta[i]);LOG("\t");
			HAL_ADC_Stop(&hadc);
		}
		LOG("\n");*/

	//file_Display();
	//while(1)
		//AT_DrawPixel();


	  // 320*480*1.5*2 = 460800

	//HAL_Delay(1000);
	//ili9488_init();

	//file_download("test.txt");
	//driver.init();
}


void app_loop(void)
{



	// Infinite Loop
	while(1)
	{





	}
}

