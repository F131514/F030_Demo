#include "nb_driver.h"
#include "usart.h"
#include <string.h>





static teAT_CmdsNumber AT_RecNumber;

AT_Cmds at_Cmds[] =
{
 {"AT+CFUN=0\r\n","OK",2000,No_Rec,3},
 {"AT+CGSN=1\r\n","OK",2000,No_Rec,3},
 {"AT+NRB\r\n","OK",2000,No_Rec,3},
 {"AT+NCDP=180.101.147.115,5683\r\n","OK",2000,No_Rec,3},
 {"AT+CFUN=1\r\n","OK",2000,No_Rec,3},
 {"AT+CIMI\r\n","OK",2000,No_Rec,3},
 {"AT+CMEE=1\r\n","OK",2000,No_Rec,3},
 {"AT+NNMI=1\r\n","OK",2000,No_Rec,3},
};

void AT_CmdSend(teAT_CmdsNumber AT_CmdNumber)
{
	//清空接收缓冲区
	at_Cmds[AT_CmdNumber].at_Status =  No_Rec;
     
	AT_RecNumber = AT_CmdNumber;
	
	HAL_UART_Transmit(&huart1,(uint8_t*)at_Cmds[AT_CmdNumber].AT_SendStr,strlen(at_Cmds[AT_CmdNumber].AT_SendStr),100);
	


}