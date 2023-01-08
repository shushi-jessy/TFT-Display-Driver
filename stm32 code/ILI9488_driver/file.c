#include "app.h"

static FATFS   	    	FatFs;
static FIL      		fil;
static FATFS 			*pfs;
static DWORD 			fre_clust;
static UINT				byte_read;
static uint32_t 		totalSpace, freeSpace;

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
	if( f_open(&fil, filename, FA_READ) != FR_OK)LOG("NOTE: File access error!");
		else
		{
			u32 filesize = f_size(&fil);
			float process = 0;
			LOG("Download file infos:\n");
			LOG("\tFile Name:\t");LOG(filename);	LOG("\n");
			LOG("\tFile Size:\t");LOGn(filesize);	LOG("\n");
			LOG("\tPack Size:\t");LOGn(255);		LOG("\n");
			LOG("\tPack Num:\t");LOGn(filesize/255+1);LOG("\n");
			LOG("|------------------------------------------------|\n");
			LOG("|		  		   Process display bar	 		  |\n");
			LOG("|------------------------------------------------|\n");
			LOG("|");
			for(u32 i=0;i<filesize;i=i+255)
			{
				char buf[255]={0};
				process += 255;
				if(filesize<255)LOG("************************************************|\n");

				else if((process/filesize)>0.02)
				{
					LOG("*");
					process = 0;
				}
				f_gets(buf, 255, &fil);
				HAL_UART_Transmit(&huart2,buf,255,HAL_MAX_DELAY);
			}
			LOG("|\n");
			LOG("NOTE: Data transfer succeeded!\n");
			f_close(&fil);
			LOG("|------------------------------------------------|\n");
		}
}

void file_upload(const char filename[],u32 filesize)
{
	if( f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)LOG("NOTE: File access error!");
		else
		{
			u8 packagesize = 255;
			if(filesize<255)packagesize = filesize;

			LOG("Upload file infos:\n");
			LOG("\tFile Name:\t");LOG(filename);LOG("\n");
			LOG("\tFile Size:\t");LOGn(filesize);LOG("\n");
			LOG("\tPack Size:\t");LOGn(packagesize);LOG("\n");
			LOG("\tPack Num:\t");LOGn(filesize/packagesize);LOG("\n");
			LOG("|------------------------------------------------|\n");
			LOG("|		  		   Process display bar	 		  |\n");
			LOG("|------------------------------------------------|\n");
			for(u32 i=0;i<filesize/packagesize;i++)
			{
				HAL_UART_AbortReceive(&huart1);	// Remove unwanted arriving package
				u8 buf[packagesize];
				LOG("|--\t");LOGn(i);LOG("\t[Recieved]\t");LOGn(filesize/packagesize-i); LOG("\t[Left]\t");LOG("Waiting for package... \n");
				if(HAL_UART_Receive(&huart1,buf,packagesize,HAL_MAX_DELAY)!=HAL_OK)
				{
					LOG("|--\tPackage ");LOGn(i);LOG("has error thus resend this package\n");
					i=i-1;
				}
				else
				{
					f_puts(buf, &fil);
				}
			}
			LOG("NOTE: Data transfer succeeded!\n");
			f_close(&fil);
			LOG("|------------------------------------------------|\n");
		}
}
