#include <app.h>

void uart1_init(void)
{
	LOG(" ------------------------------------------------|\n");
	LOG("|		    TFT display Application Rev0.2		  |\n");
	LOG(" ------------------------------------------------|\n");
}
void uart1_printSTR(const char buf[])
{
	u8 len = 0;
	while(buf[len]!=0)len++;
	HAL_UART_Transmit(&huart1,buf, len, HAL_MAX_DELAY);
}
void uart1_printNUM(int64_t num)
{
	char buf[10] = {0};
	sprintf(buf, "%d",num);
	uart1_printSTR(buf);
}
