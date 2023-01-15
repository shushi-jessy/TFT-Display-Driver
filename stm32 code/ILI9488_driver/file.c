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

void file_Display_AT(void)
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
void file_Display(char filename[])
{
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
	LOG("\tPack Size:\t");LOGn(1920);LOG("\n");
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

u8 AT_Mode(void)
{
	for(u16 i=0;i<2304;i++)buf[i]=0; // Clear buffer

	LOG("|------------------------------------------------|\n");
	LOG("|		    AT Mode Waiting for Command...	 	  |\n");
	LOG("|------------------------------------------------|\n");

	u8  reg_data_size  = 0;
	u8  reg_index      = 0;

	HAL_UART_AbortReceive(&huart1);
	while(HAL_UART_Receive(&huart1,buf,12,HAL_MAX_DELAY)!=HAL_OK);
	if(buf[0]=='+' && buf[11]=='+')
	{
		// Extract register:
		if     (buf[1]>='0' && buf[1] <='9') reg_index += (buf[1]-'0')   <<4;
		else if(buf[1]>='A' && buf[1] <='F') reg_index += (buf[1]-'A'+10)<<4;
		else if(buf[1]>='a' && buf[1] <='f') reg_index += (buf[1]-'f'+10)<<4;
		else   {LOG("\tRegister [0] should only be hex number(0-9,A-F(a-f)!\n");return 5;}

		if     (buf[2]>='0' && buf[2] <='9') reg_index += buf[2]-'0';
		else if(buf[2]>='A' && buf[2] <='F') reg_index += buf[2]-'A'+10;
		else if(buf[2]>='a' && buf[2] <='f') reg_index += buf[2]-'f'+10;
		else   {LOG("\tRegister [1] should only be hex number(0-9,A-F(a-f)!\n");return 6;}
		WriteCommand(reg_index);

		// Extract data:
		for(u8 i=3;buf[i]!='*' && buf[i]!='+';i=i+2)
		{
			u8  reg_data = 0;
			if     (buf[i]>='0' && buf[i] <='9')     reg_data += (buf[0]-'0')   <<4;
			else if(buf[i]>='A' && buf[i] <='F')     reg_data += (buf[0]-'A'+10)<<4;
			else if(buf[i]>='a' && buf[i] <='f')     reg_data += (buf[0]-'F'+10)<<4;
			else   {LOG("\tData [");LOGn(i);LOG("] should only be hex number(0-9,A-F(a-f)!\n");return 7;}
			if     (buf[i+1]>='0' && buf[i+1] <='9') reg_data += buf[1]-'0';
			else if(buf[i+1]>='A' && buf[i+1] <='F') reg_data += buf[1]-'A'+10;
			else if(buf[i+1]>='a' && buf[i+1] <='f') reg_data += buf[1]-'F'+10;
			else   {LOG("\tData [");LOGn(i);LOG("] should only be hex number(0-9,A-F(a-f)!\n");return 8;}
			WriteData(reg_data);
		}
		LOG("\tOK!\n");
		return 0;
	}
	else {LOG("\tCommand format error!\n");return 1;}
}


u8 AT_DrawPixel(void)
{
	for(u16 i=0;i<2304;i++)buf[i]=0; // Clear buffer

	LOG("|------------------------------------------------|\n");
	LOG("|		    AT Draw Waiting for Command...	 	  |\n");
	LOG("|     Exmaple: +REC,0000,01DF,0000,013F,8f00+    |\n");
	LOG("|------------------------------------------------|\n");

	u8  shape[4] = {0}; // Necessary to add one more '0x00' character to indicate the string ending!

	HAL_UART_AbortReceive(&huart1);
	while(HAL_UART_Receive(&huart1,buf,30,HAL_MAX_DELAY)!=HAL_OK);
	if(buf[0]=='+' && buf[29]=='+')
	{
		shape[0] = buf[1];shape[1] = buf[2];shape[2] = buf[3];
		if(strcmp(shape,"REC")==0)
		{
			// Extract data:
			u16 arg[5]={0};  // arg[1-4]=> position arg[5-6] pixel color
			for(u8 i=5;i<30;i=i+5)
			{
				u16  reg_data = 0;
				if     (buf[i]>='0' && buf[i] <='9')     reg_data += (buf[i]  -'0'   ) <<12;
				else if(buf[i]>='A' && buf[i] <='F')     reg_data += (buf[i]  -'A'+10) <<12;
				else if(buf[i]>='a' && buf[i] <='f')     reg_data += (buf[i]  -'a'+10) <<12;
				else   {LOG("\tData [");LOGn(i);  LOG("] should only be hex number(0-9,A-F(a-f)!\n");return 7;}
				if     (buf[i+1]>='0' && buf[i+1] <='9') reg_data += (buf[i+1]-'0'   ) <<8;
				else if(buf[i+1]>='A' && buf[i+1] <='F') reg_data += (buf[i+1]-'A'+10) <<8;
				else if(buf[i+1]>='a' && buf[i+1] <='f') reg_data += (buf[i+1]-'a'+10) <<8;
				else   {LOG("\tData [");LOGn(i+1);LOG("] should only be hex number(0-9,A-F(a-f)!\n");return 8;}
				if     (buf[i+2]>='0' && buf[i+2] <='9') reg_data += (buf[i+2]-'0'   ) <<4;
				else if(buf[i+2]>='A' && buf[i+2] <='F') reg_data += (buf[i+2]-'A'+10) <<4;
				else if(buf[i+2]>='a' && buf[i+2] <='f') reg_data += (buf[i+2]-'a'+10) <<4;
				else   {LOG("\tData [");LOGn(i+2);LOG("] should only be hex number(0-9,A-F(a-f)!\n");return 9;}
				if     (buf[i+3]>='0' && buf[i+3] <='9') reg_data += buf[i+3]-'0';
				else if(buf[i+3]>='A' && buf[i+3] <='F') reg_data += buf[i+3]-'A'+10;
				else if(buf[i+3]>='a' && buf[i+3] <='f') reg_data += buf[i+3]-'a'+10;
				else   {LOG("\tData [");LOGn(i+3);LOG("] should only be hex number(0-9,A-F(a-f)!\n");return 10;}
				arg[i/5-1]=reg_data;
			}
			_Column(arg[0],arg[1]);
			_Page(arg[2],arg[3]);
			WriteCommand(0x2C);
			u16 rgb666[3] = {0};
			rgb666[0] = (arg[4] & 0b1111100000000000)       + ((arg[4] & 0b0000011111100000)<<2);
			rgb666[1] = ((arg[4] & 0b0000000000011111)<<10) + ((arg[4] & 0b1111100000000000)>>8);
			rgb666[2] = ((arg[4] & 0b0000011111100000)<<6); + ((arg[4] & 0b0000000000011111)<<2);
			for(u32 i=0;i<(arg[1]-arg[0])*(arg[3]-arg[2]);i=i+3)
			{
				WriteData(rgb666[0]);
				WriteData(rgb666[1]);
				WriteData(rgb666[2]);
			}
			LOG("\tOK!\n");
			return 0;
		}
		else {LOG("\Shape error!\n");return 2;}
	}
	else {LOG("\tDraw format error!\n");return 1;}
}
