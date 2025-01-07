
#include 	<time.h>
#include 	"tt.h"

#include 	<stdio.h>
#include 	<string.h>

#include 	"hPlaySession.hpp"
#include 	"hMODEC.hpp"
#include 	"hDSPSTRING.hpp"
//#include 	"hSR_MANAGER.hpp"
#include 	"hSR_PROC.hpp"
#include 	"hDELI_CMD.hpp"

#include 	"hDSP_MCEP.hpp"


#define 	UART0_DCx_SIZE		64
#define 	UART1_FPGA_SIZE		64
#define 	x65_MSGLEN			67
#define 	A_MSGLEN			4


int const	UARTBUFSIZE_TAB[2] = {UART0_DCx_SIZE, UART1_FPGA_SIZE};

SECTION(("seg_sdram2"))	int				buffer0[UART0_DCx_SIZE][UARTMSG_MAX_SIZE], buffer1[UART1_FPGA_SIZE][UARTMSG_MAX_SIZE]; // buffer_UART1[UART0_SIZE][UARTMSG_MAX_SIZE];
//*****************************************	
//*****************************************	
#include 	"hUARTBUF.hpp"

//int 			(*UARTBUF::Buf)[UART0_SIZE][UARTMSG_MAX_SIZE], UARTBUF::wline, UARTBUF::rline, UARTBUF::BUFSIZE;
//int				UARTBUF::msg_len, UARTBUF::chr_left, UARTBUF::msg[UARTMSG_STD_SIZE];

  // Object creation/definition 	
	UARTBUF		UARTBUFS[2];
  // static data members definition	
    UARTBUF 	* UARTBUF::PTRS[2];

void UARTBUF::Constructor(void)
{
	int  k;
	
	for(k=0; k<2; k++)
	{
 		UARTBUF::PTRS[k] = &UARTBUFS[k];
 		//UARTBUF::PTRS[k]->Buf = (int **) &(buffer[k][0][0]); 		//( int (*)[UART0_SIZE][UARTMSG_MAX_SIZE] ) buffer[k][0][0];
 		UARTBUF::PTRS[k]->BUFSIZE =	UARTBUFSIZE_TAB[k]; 		    		   

 		UARTBUF::PTRS[k]->Init(UARTBUFSIZE_TAB[k]);
	}
	UARTBUF::PTRS[0]->Buf = buffer0;
	UARTBUF::PTRS[1]->Buf = buffer1;
//	UARTBUF0.Init(buffer_UART0);
//	UARTBUF1.Init(buffer_UART1);
}	
	

void UARTBUF::Init(int pBUF_SIZE)
{
	int j,k;
	
    wline= 0; rline= 0; 
    //Buf = ( int (*)[UART0_SIZE][UARTMSG_MAX_SIZE]) buffer; 
    for(k=0; k< UART0_SIZE; k++)
    	for (j=0; j<UARTMSG_MAX_SIZE; j++)
    		Buf[k][j]=0;
    		
    //BUFSIZE =	pBUF_SIZE; 		    		   
}

void UARTBUF::PUT_MSG(int *msg, int LEN)
{
	int j;
	
    for (j=0; j<LEN; j++)
		Buf[wline][j]=msg[j];    			//(*Buf)[wline][j]=msg[j];
 	++wline; 
 	if (wline>= BUFSIZE )	
 		wline= 0;     	   
 	for(j=0;j< UARTMSG_STD_SIZE; j++)
 		msg[j] = 0; 	
 	msg_len = 0;
}

#define		x55_INPUT_STARTVAL		32
#define		x55_MSGTYPE_POS			1


void 	UARTBUF::UART_Monitor_NEW(void)
{
	int  k;
	
	//return; 
                                                       	for(k=0; k<2; k++)
 		UARTBUF::PTRS[k]->UARTBUF::UART_Monitor_NEWx();
	
}

extern void	New_Playrun_Config(void);
extern void	New_Playrun_Config_SH(void);
	
	int  	cc, cc1, cc2, cc3; 

	int		flagx55; 
	
void 	UARTBUF::UART_Monitor_NEWx(void)

{
	int 	i;
	int 	*msg;

		
	  while(rline != wline)
	  {	
	  	msg = &( Buf[rline][0] ); //(*Buf)[rline];
		switch(msg[0])
		{			
	   #if	(1) //(SOFT_V & DD_BASE || SOFT_V & FP_BASE)	   
			case 0x55:
/*			
				if (msg[1] != rline && !flagx55)
				{ 
					i=i+2;
					flagx55 = i; 
					rline = msg[1]; 
					wline = (rline+1)%(UART0_DCx_SIZE);
					flagx55 = 1;
				}
*/				
				if (PlaySession::RECORDING_MODE != SIM_I2SOFF_D_ONLY)
					DELI_CMD::x55RX(msg);	
			  break;				
	   #endif			
//*	   

			case 'a':
				cc1= (msg[1]&0xFF)<<16;
				cc2 = (msg[2]&0xFF)<<8 ;
				cc3 = msg[3]&0xFF;
				cc= cc1| cc2 | cc3; 
				DELI_CMD::LED_Color(0x1000, cc) ;
				//cc=0; 
			  break;
				//MCEP::SpkNum = msg[1]-48;
				//New_Playrun_Config(); //UART EP config
				//New_Playrun_Config_SH(); //UART EP config											
			  break;				
	   
			case 0x50:   //   Biezace potwierdzenie zwrotne dzialania do Smok-a
				PlaySession::PLAY_MODE = PLAY_ON; 
				//PlaySession::Msg0x50_Proc(msg, PLAY_ON);
			  break;	
			case 0x52:
				PlaySession::PLAY_MODE = PLAY_SIM_OFF;
				//PlaySession::Msg0x52_Proc(msg, PLAY_SIM_OFF) ; // PLAY_OFF);
		      break;			
			case 0x58: //  0x10: //    		// Konfiguracja opcji DSP Sharc
			  #if 	UART_TYPE == UART_OLD
				PlaySession::Msg0x58_Proc(msg);			      	  	
			  #elif UART_TYPE == UART_NEW
			  	PlaySession::Msg0x58_Proc_NEW(msg);
			  #endif							
				// DSPSTRING::Print_DSPstring_Modes();
				// sprintf( dbgtab, "\n%x %x %x %x %x %c%c%c", (int) msg[0], (int) msg[1], (int) msg[2], (int) msg[3], (int) msg[4], (int) msg[5], (int) msg[6], (int) msg[7] );  Print_Debug(0, NOTIME);
				break;
				
			case 0x80:		// Regulacja glosnosci ponizej zadanego progu
				PlaySession::GUI_Volume = msg[3];
				PlaySession::GUI_VOL_CHG = 1;
				DELI_CMD::Send_VOL(msg);				
			  break;			
			case 0x92:		// Konfiguracja opcji sygnalu Audio
				//PlaySession::Msg0x92_Proc(msg);																	 				
				//LED(GREEN_2, SWITCH);	
			  break;	
		  	case 0x65:   //Konfiguracja korekcji pomieszczenia
//		  		MODEC::Msg101(msg);  //MODEC_Msg101(msg);
//		  		for(i=0;i<1E+5;i++);
//		  		PlaySession::modecmsg_clk = clock();		  		
		  		//PlaySession.SPORT_EXT_RATE = -1E+6; 
			  break;				  
		  	case 102:   //Konfiguracja korekcji pomieszczenia
		  		//Msg102_Proc();
			  break;	
		  	case 0xEE:   //Konfiguracja korekcji pomieszczenia
   			  #if xDSPMODE == xMULTISPK 	
				MCEP::SpkNum = msg[1]; // 3; //
				DELI_CMD::Send_EP(SET_FIR);
				//DELI_CMD::Send_MAL();				
   			  #endif			  		
		  		//Msg102_Proc();
			  break;
//*/			  				  
		}
		++rline; 
		if (rline>= BUFSIZE) 
			rline= 0;
	}
}


void UARTBUF::PUT_CHAR(char uart_char)
{
	int		k;
	
 	if (!msg_len)
 	{
		switch(uart_char)
		{
			
			case 0x55:
				msg_len= DELI_FRAMESIZE;
			  break; 
			  
			case 0x65:
				msg_len= x65_MSGLEN;
			  break; 
			  					  
			case 'a':
				msg_len= A_MSGLEN;
			  break; 
			  											
			case 0x50: 
			case 0x52: 	
			case 0x80: 		
			case 0x58:
		       #if 	UART_TYPE == UART_OLD
				msg_len= 16;		   
			   #elif UART_TYPE == UART_NEW			  	
				msg_len= 20;		   
			   
    			MCEP::Config();
			   #endif			  			
			  break; 			
						
			case 0x92: 	
				msg_len= 10; 
		 	  break;					
			case 0xEE: 	
				msg_len= 2; 
		 	  break;							 	  
		}
		chr_left = msg_len;
 	}
 	if(chr_left >0)
 	{
 		//msg[UARTMSG_STD_SIZE-chr_left]= uart_char;
 		msg[msg_len-chr_left]= uart_char;
 		--chr_left;
 		if (chr_left<14)
 			k=5;		
 		if (!chr_left) 			
 			PUT_MSG(msg, msg_len); 
 	}
}

/*
void UARTBUF::Deli_CMD(int cmd_type)
{
	int k1, k2, kbyte, j;
	
	return; 
	switch(cmd_type)
	{
		case xFIRXO_TAB:
			UARTBUF::Deli_ACK();
			DELI_MSG[1] = 3;
			for (k1=0, k2=0; k2< sizeof(FPGA_FIRXO)/sizeof(FPGA_FIRXO[0]); k1++, k2+=2)
			{
				DELI_MSG[2] = k1; DELI_MSG[3] = DELI_MSG[2];
				for(kbyte=0; kbyte<5; kbyte++)
				{
					DELI_MSG[kbyte+4] = FPGA_FIRXO[k2]>>(kbyte*8);
					DELI_MSG[kbyte+9] = FPGA_FIRXO[k2+1]>>(kbyte*8);
				}
				DELI_MSG[15] = CRC_calc(DELI_MSG);
				trace( (char *) DELI_MSG, 15);
				// for(j=0; j<7E+4; j++);		
			}	
			//for(k1=0;k1<8E+6;k1++);		
			k1 = UARTBUF::Deli_ACK();
		  break;
		case xLEDRGB_SRTAB:
			DELI_MSG[2]= 0x2f;
			DELI_MSG[3]= 0x2f;			
			DELI_MSG[15] = CRC_calc(DELI_MSG);
			trace( (char *) DELI_MSG, 15);
		  break;		  	
	}		
}

int 	UARTBUF::Deli_ACK(void)
{
	int temp;
	
	temp= pDELI_RXQTY;
	pDELI_RXQTY = cDELI_RXQTY;
	return(cDELI_RXQTY-temp);
}

int UARTBUF::CRC_calc(int *MSG)
{
	int 	k, crc_sum=0xFF;
	
	for(k=0; k<15; k++)
		crc_sum+= MSG[k];
		
	return(crc_sum & 0xFF);
}
*/

/*
EXTERNC 	void UARTBUF_PUT(int *msg, int LEN)
{
	UARTBUF::PUT(msg, LEN);	
}
*/

EXTERNC 	void UARTisr(char uart_char)
{
	//UARTBUF::PUT_CHAR(uart_char);	
	UARTBUF::PTRS[0]->PUT_CHAR(uart_char);	
}

