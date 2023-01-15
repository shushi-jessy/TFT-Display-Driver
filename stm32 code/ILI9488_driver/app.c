#include <app.h>


//file	fs;
//ili9488 driver;


void app_init(void)
{
	uart1_init();
	file_init();
	ili9488_init();




	file_upload();
	file_Display();

	  // 320*480*1.5*2 = 460800

	//HAL_Delay(1000);
	//ili9488_init();

	//file_download("test.txt");
	//driver.init();
}


void app_loop(void)
{
	// Initialization:
	//uart.print("------------------------");
	//file.mount();
	/*if (file.mount_result == FR_OK) 	uart.print("Mounting ERROR!!!");
	else 								uart.print("SD CARD mounted successfully!");

	//f_getfree("", &fre_clust, &pfs);
	//totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	//freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);;
	//sprintf(buf, "TotalSpace: %d bytes.\nFreeSpace: %d bytes.",totalSpace,freeSpace);
	//tft.print(buf);

	if( f_open(&fil, "test.txt",FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) tft.print ("Open ERROR!!!");
	else
	{
		tft.print("File is opened in writing mode!");
		f_puts("Hello Jessy!Hello Jessy!Hello Jessy!Hello Jessy!\n", &fil);
		tft.print("Data has been written!");
		f_close(&fil);
	}

	if(f_open(&fil, "test.txt", FA_READ)!= FR_OK) tft.print ("Open ERROR!!!");
	else
	{
		tft.print("File is opened in reading mode:");
		f_gets(buf, 255, &fil);
		tft.print(buf);
		f_close(&fil);
	}

	if(f_open(&fil, "data.txt", FA_READ)!= FR_OK) tft.print ("Open ERROR!!!");
	else
	{
		tft.print("File is opened successfully in reading mode!");
		tft.WriteCommand(0x2C);
	    char data[2];
		for(uint32_t i=0;i<(uint32_t)(320*480*1.5);i++)
		{
			f_read(&fil,data, 2, &byte_read);
			tft.WriteData((data[0]<<8)+data[1]);
		}
		f_close(&fil);
	}*/


	// Infinite Loop
	while(1)
	{





	}
}

