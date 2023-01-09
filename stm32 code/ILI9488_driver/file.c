#include "app.h"
static FATFS   	    	FatFs;
static  FIL      		fil;
static  FATFS 			*pfs;
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

void file_download(const char filename[])
{
	if( f_open(&fil, filename, FA_READ | FA_OPEN_ALWAYS) != FR_OK)LOG("NOTE: File access error!");
		else
		{
			u32 filesize = f_size(&fil);
			LOG("Download file infos:\n");
			LOG("\tFile Name:\t");LOG(filename);	LOG("\n");
			LOG("\tFile Size:\t");LOGn(filesize);	LOG("\n");
			LOG("|------------------------------------------------|\n");
			LOG("|		  		   Process display bar	 		  |\n");
			LOG("|------------------------------------------------|\n");
			LOG("|");
			WriteCommand(0x2C);
			for(u32 i=0;i<filesize;i=i+2304)
			{
				f_gets(buf, 2304, &fil);
				for(u32 j=0;j<2304;j=j+2)WriteData((buf[j]<<8)+buf[j+1]);
			}
			LOG("|\n");
			LOG("NOTE: Data transfer succeeded!\n");
			f_close(&fil);
			LOG("|------------------------------------------------|\n");
		}
}

void file_upload(const char filename[],u32 filesize,u16 packagesize)
{
	if( f_open(&fil, filename, FA_WRITE | FA_READ | FA_CREATE_ALWAYS ) != FR_OK)LOG("NOTE: File access error!");
		else
		{
			u16 packagenumber = filesize/packagesize;
			if(filesize<packagesize){packagesize = filesize;packagenumber=1;}
			LOG("Upload file infos:\n");
			LOG("\tFile Name:\t");LOG(filename);LOG("\n");
			LOG("\tFile Size:\t");LOGn(filesize);LOG("\n");
			LOG("\tPack Size:\t");LOGn(packagesize);LOG("\n");
			LOG("\tPack Num:\t");LOGn(filesize/packagesize);LOG("\n");
			LOG("|------------------------------------------------|\n");
			LOG("|		  		   Process display bar	 		  |\n");
			LOG("|------------------------------------------------|\n");
			u32 previous_filesize = 0;
			u32 current_filesize = 0;
			WriteCommand(0x2C);
			for(u32 i=0;i<packagenumber;i++)
			{
				HAL_UART_AbortReceive(&huart1);	// Remove unwanted arriving package
				if(HAL_UART_Receive(&huart1,buf,packagesize,HAL_MAX_DELAY)!=HAL_OK)
				{
					LOG("|--\tPackage ");LOGn(i);LOG("has error thus resend this package\n");
					i=i-1;
				}
				else
				{
					for(u32 j=0;j<2304;j++)
						f_putc(buf[j], &fil);
					for(u32 j=0;j<2304;j=j+2)WriteData((buf[j]<<8)+buf[j+1]);
					current_filesize = f_size(&fil);
					LOG("|--Recieve:\t");LOGn(i);LOG("\tLeft:\t");LOGn(packagenumber-i);
					LOG("\tPackage size(bytes): ");LOGn(current_filesize-previous_filesize);
					LOG("\tTotal size(bytes): ");LOGn(current_filesize);LOG("\n");
					previous_filesize = current_filesize;
				}

			}
			LOG("NOTE: Data transfer succeeded!\n");
			f_close(&fil);
			LOG("|------------------------------------------------|\n");
		}
}
