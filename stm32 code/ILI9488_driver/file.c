#include "app.h"
static FATFS   	    	FatFs;
static  FIL      		fil;
static  FATFS 			*pfs;
static  FRESULT         fres;
static  DWORD 			fre_clust;
static UINT				byte_read;
static u32				totalSpace, freeSpace;
static u8			    buf[2304];

void file_init(void)
{
	if (f_mount(&FatFs,"",1)!= FR_OK)LOG("Cannot mount the card SD\n");
	else
	{
		LOG("SD card infos:\n");
		f_getfree("", &fre_clust, &pfs);
		totalSpace = (uint32_t)(((pfs->n_fatent - 2) * pfs->csize * 0.5)/1000);
		freeSpace = (uint32_t)((fre_clust * pfs->csize * 0.5)/1000);
		LOG("\tTotalSpace:\t");			LOGn(totalSpace);
		LOG("\tMB.\n\tFreeSpace:\t");	LOGn(freeSpace);
		LOG("\tMB.\n");
		LOG("|------------------------------------------------|\n");
	}
}

void file_Display(void)
{
	u8 filename[10]={0};
	while(1)
	{
		LOG("|------------------------------------------------|\n");
		LOG("|		  		 Please Send File Name...	 	  |\n");
		LOG("|------------------------------------------------|\n");
		HAL_UART_AbortReceive(&huart1);
		if(HAL_UART_Receive(&huart1,buf,12,HAL_MAX_DELAY)==HAL_OK)
		{
			if(buf[0]!='+' || buf[11]!='+')LOG("\tFile name format error!\n");
			else
			{
				for(u8 i=0;i<10 && buf[i+1]!='*';i++)filename[i]=buf[i+1];
				LOG("\tFile name received:\t");LOG(filename);LOG("\n");
				break;
			}
		}
		else LOG("\tFile name receive timeout!:\t");
	}

	fres = f_open(&fil, filename, FA_READ | FA_OPEN_EXISTING);
	if     (fres==FR_NO_FILE)	LOG("Cannot find the file!\n");
	else if(fres != FR_OK)		LOG("NOTE: File access error!\n");
	else
	{
		LOG("Display file infos:\n");
		LOG("\tFile Name:\t");LOG(filename);   LOG("\n");
		LOG("\tFile Size:\t");LOGn(fil.fsize); LOG("\n");
		LOG("|------------------------------------------------|\n");
		LOG("|		  		  Start Displaying...	 		  |\n");
		LOG("|------------------------------------------------|\n");
		LOG("|");
		WriteCommand(0x2C);
		for(u32 i=0;i<fil.fsize;i=i+2)
		{
			f_read(&fil,buf,2, &byte_read);
			WriteData((buf[0]<<8)+buf[1]);
		}
		LOG("|\n");
		LOG("NOTE: Data display succeeded!\n");
		f_close(&fil);
		LOG("|------------------------------------------------|\n");
	}
}

void file_upload(void)
{
	LOG("|------------------------------------------------|\n");
	LOG("|		  		 Please Send File Name...	 	  |\n");
	LOG("|------------------------------------------------|\n");
	u8 filename[10]={0};
	while(1)
	{
		HAL_UART_AbortReceive(&huart1);
		if(HAL_UART_Receive(&huart1,buf,12,HAL_MAX_DELAY)==HAL_OK)
		{
			if(buf[0]!='+' || buf[11]!='+')LOG("\tFile name format error!\n");
			else
			{
				for(u8 i=0;i<10 && buf[i+1]!='*';i++)filename[i]=buf[i+1];
				LOG("\tFile name received:\t");LOG(filename);LOG("\n");
				break;
			}
		}
		else LOG("\tFile name receive timeout!:\t");
	}
	LOG("|------------------------------------------------|\n");
	LOG("|		  		 Please Send File Size...	 	  |\n");
	LOG("|------------------------------------------------|\n");
	u32 filesize = 0;
	_Bool error = 1;
	while(error)
	{
		HAL_UART_AbortReceive(&huart1);
		if(HAL_UART_Receive(&huart1,buf,12,HAL_MAX_DELAY)==HAL_OK)
		{
			if(buf[0]!='+' || buf[11]!='+'){LOG("\tFile size format error!\n");}
			else
			{
				u32 coefficient = 1;
				for(u8 i=0;i<10 && buf[i+1]!='*';i++)coefficient *= 10;
				for(u8 i=0;i<10 && buf[i+1]!='*';i++)
				{
					if(buf[i+1]>='0' && buf[i+1] <='9')
					{
						coefficient /= 10;
						filesize += (buf[i+1] - '0') * coefficient;
					}
					else
					{
						LOG("\tFile size should be only number!\n");
						break;
					}
					if(buf[i+2]=='*' || buf[i+2]=='+')
					{
						LOG("\tFile size received:\t");LOGn(filesize);LOG("\n");
						error = 0;
					}
				}
			}
		}
		else {LOG("\tFile name receive timeout!:\t");HAL_UART_AbortReceive(&huart1);}
	}
	u16 packagenumber = filesize/2304;
	if(filesize<2304){packagenumber=1;}
	LOG("Upload file infos:\n");
	LOG("\tFile Name:\t");LOG(filename);LOG("\n");
	LOG("\tFile Size:\t");LOGn(filesize);LOG("\n");
	LOG("\tPack Size:\t");LOGn(2304);LOG("\n");
	LOG("\tPack Num:\t");LOGn(packagenumber);LOG("\n");
	LOG("|------------------------------------------------|\n");
	LOG("|		  		   Start Receiving...	 		  |\n");
	LOG("|------------------------------------------------|\n");
	if( f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS ) != FR_OK)LOG("NOTE: File access error!");
		else
		{
			u32 previous_filesize = 0;
			u32 current_filesize = 0;
			WriteCommand(0x2C);

			for(u32 i=0;i<packagenumber;)
			{
				HAL_UART_AbortReceive(&huart1);	// Remove unwanted arriving package
				if(HAL_UART_Receive(&huart1,buf,2304,HAL_MAX_DELAY)==HAL_OK)
				{
					HAL_UART_AbortReceive(&huart1);
					for(u32 j=0;j<2304;j=j+2)
					{
						WriteData((buf[j]<<8)+buf[j+1]);
						f_putc(buf[j], &fil);
						f_putc(buf[j+1], &fil);
					}
					i++;
				}
				else HAL_UART_AbortReceive(&huart1);
			}
		}
		LOG("NOTE: Data transfer succeeded!\n");
		f_close(&fil);
		LOG("|------------------------------------------------|\n");
}


