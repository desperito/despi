
#include 	"tt.h"

#include 	<stdio.h>

#include 	"hSR_PROC.hpp"
#include 	"hUARTBUF.hpp"
 #include 	"hDSP_MCEP.hpp"

//#include 	"FPGA_FIRXO.h"

#define 	XOFIR_SIZE		250
#define 	RESFIR_SIZE		980
//#include 	"EP_fpga.h"
// #include 	"RES_FIRS.h"

#define 	RXFRAME_DELAY	3

#define		x55_INPUT_STARTVAL		32
#define		x55_FIR_PACKETQTY		23
#define		x55_LED_STATUS			51

#define		x55_MSGTYPE_POS			1

#define		x55_FREQ_BYTE0			2
#define		x55_FREQ_BYTE1			3
#define		x55_FREQ_BYTE2			4
#define		xPREDEF2CHAR			2
#define		x55_CODE_XOFIR			0x83
#define		x55_CODE_MAL			(36+128)
#define		x55_CODE_EP				(35+128)

#define		x55_FIRNO_EP			2
#define		x55_FIRNO_DD_RES_MAL	3

//*****************************************	
//*****************************************	
#include 	"hDeli_CMD.hpp"

section ("seg_sdram2")  long long  EPTAB[8][1505]; 

extern 	MCEP_T		TME[]; 

extern 	long long 	TMALF[6][MAL_LEN];

 #include "XOFIR.h"
//extern 	long long 	TXOFIR[];

#define cGREEN			0x001000
#define cTURQUISE		0x001007
#define cBLUE 			0x000425
#define cVIOLET 		0x060020
#define cYELLOW 		0x0d0d00
#define cORANGE 		0x120700
#define cPINK 			0x0a000a
#define cRED 			0x0a0000
#define cWHITE 			0x0e1810
#define cGRAY 			0x030403

unsigned int	Col6[] =  {cGREEN,cYELLOW,cVIOLET,cBLUE,cPINK,cRED};			
//unsigned int	ColDSP[] = {Black, Yellow, Blue, Green, Red, Orange, Violet, White};
 
//SECTION(("seg_sdram2"))  MCEP_T			MCEP[2] = { 	{"ABC_SPK",{1440, 1441}, 144, {1480, 1481}, 148, {1442, 1443}, 145, {1482, 1483}, 149,  {11440, 11441}, 1441, {11480, 11481}, 1148},
//														{"DEF_SPK",    2440, 2441, 244, 2480, 2481, 248, 2442, 2443, 245, 2482, 2483, 249,  22440, 22441, 22442, 22480, 22481, 2248		}
//												  };
												  

//-------------------------------------------------------------------------
SECTION(("seg_sdram2")) const unsigned int crc_table[] = {
0x00000000,0x77073096,0xee0e612c,0x990951ba,0x076dc419,0x706af48f,0xe963a535,0x9e6495a3,
0x0edb8832,0x79dcb8a4,0xe0d5e91e,0x97d2d988,0x09b64c2b,0x7eb17cbd,0xe7b82d07,0x90bf1d91,
0x1db71064,0x6ab020f2,0xf3b97148,0x84be41de,0x1adad47d,0x6ddde4eb,0xf4d4b551,0x83d385c7,
0x136c9856,0x646ba8c0,0xfd62f97a,0x8a65c9ec,0x14015c4f,0x63066cd9,0xfa0f3d63,0x8d080df5,
0x3b6e20c8,0x4c69105e,0xd56041e4,0xa2677172,0x3c03e4d1,0x4b04d447,0xd20d85fd,0xa50ab56b,
0x35b5a8fa,0x42b2986c,0xdbbbc9d6,0xacbcf940,0x32d86ce3,0x45df5c75,0xdcd60dcf,0xabd13d59,
0x26d930ac,0x51de003a,0xc8d75180,0xbfd06116,0x21b4f4b5,0x56b3c423,0xcfba9599,0xb8bda50f,
0x2802b89e,0x5f058808,0xc60cd9b2,0xb10be924,0x2f6f7c87,0x58684c11,0xc1611dab,0xb6662d3d,
0x76dc4190,0x01db7106,0x98d220bc,0xefd5102a,0x71b18589,0x06b6b51f,0x9fbfe4a5,0xe8b8d433,
0x7807c9a2,0x0f00f934,0x9609a88e,0xe10e9818,0x7f6a0dbb,0x086d3d2d,0x91646c97,0xe6635c01,
0x6b6b51f4,0x1c6c6162,0x856530d8,0xf262004e,0x6c0695ed,0x1b01a57b,0x8208f4c1,0xf50fc457,
0x65b0d9c6,0x12b7e950,0x8bbeb8ea,0xfcb9887c,0x62dd1ddf,0x15da2d49,0x8cd37cf3,0xfbd44c65,
0x4db26158,0x3ab551ce,0xa3bc0074,0xd4bb30e2,0x4adfa541,0x3dd895d7,0xa4d1c46d,0xd3d6f4fb,
0x4369e96a,0x346ed9fc,0xad678846,0xda60b8d0,0x44042d73,0x33031de5,0xaa0a4c5f,0xdd0d7cc9,
0x5005713c,0x270241aa,0xbe0b1010,0xc90c2086,0x5768b525,0x206f85b3,0xb966d409,0xce61e49f,
0x5edef90e,0x29d9c998,0xb0d09822,0xc7d7a8b4,0x59b33d17,0x2eb40d81,0xb7bd5c3b,0xc0ba6cad,
0xedb88320,0x9abfb3b6,0x03b6e20c,0x74b1d29a,0xead54739,0x9dd277af,0x04db2615,0x73dc1683,
0xe3630b12,0x94643b84,0x0d6d6a3e,0x7a6a5aa8,0xe40ecf0b,0x9309ff9d,0x0a00ae27,0x7d079eb1,
0xf00f9344,0x8708a3d2,0x1e01f268,0x6906c2fe,0xf762575d,0x806567cb,0x196c3671,0x6e6b06e7,
0xfed41b76,0x89d32be0,0x10da7a5a,0x67dd4acc,0xf9b9df6f,0x8ebeeff9,0x17b7be43,0x60b08ed5,
0xd6d6a3e8,0xa1d1937e,0x38d8c2c4,0x4fdff252,0xd1bb67f1,0xa6bc5767,0x3fb506dd,0x48b2364b,
0xd80d2bda,0xaf0a1b4c,0x36034af6,0x41047a60,0xdf60efc3,0xa867df55,0x316e8eef,0x4669be79,
0xcb61b38c,0xbc66831a,0x256fd2a0,0x5268e236,0xcc0c7795,0xbb0b4703,0x220216b9,0x5505262f,
0xc5ba3bbe,0xb2bd0b28,0x2bb45a92,0x5cb36a04,0xc2d7ffa7,0xb5d0cf31,0x2cd99e8b,0x5bdeae1d,
0x9b64c2b0,0xec63f226,0x756aa39c,0x026d930a,0x9c0906a9,0xeb0e363f,0x72076785,0x05005713,
0x95bf4a82,0xe2b87a14,0x7bb12bae,0x0cb61b38,0x92d28e9b,0xe5d5be0d,0x7cdcefb7,0x0bdbdf21,
0x86d3d2d4,0xf1d4e242,0x68ddb3f8,0x1fda836e,0x81be16cd,0xf6b9265b,0x6fb077e1,0x18b74777,
0x88085ae6,0xff0f6a70,0x66063bca,0x11010b5c,0x8f659eff,0xf862ae69,0x616bffd3,0x166ccf45,
0xa00ae278,0xd70dd2ee,0x4e048354,0x3903b3c2,0xa7672661,0xd06016f7,0x4969474d,0x3e6e77db,
0xaed16a4a,0xd9d65adc,0x40df0b66,0x37d83bf0,0xa9bcae53,0xdebb9ec5,0x47b2cf7f,0x30b5ffe9,
0xbdbdf21c,0xcabac28a,0x53b39330,0x24b4a3a6,0xbad03605,0xcdd70693,0x54de5729,0x23d967bf,
0xb3667a2e,0xc4614ab8,0x5d681b02,0x2a6f2b94,0xb40bbe37,0xc30c8ea1,0x5a05df1b,0x2d02ef8d
};

/*
922531081   36FCB509 0xb6fcb509
3814061991  E355F7A7
1213293500  485163BC
3427018486  CC4426F6
3014278664  B3AA3E08
241705484   E68220C
123972667   763AC3B
1454679951  56B4A78F
2102471190  7D512A16
4201802825  FA726C49
3896682079  E842A65F
4215017155  FB3C0EC3
206382902   C4D2736
672069049   280EF5B9
573184076   222A184C
2135435171  7F4827A3
*/
  #pragma optimize_off
unsigned int DELI_CMD::CalcCRC32(unsigned int crc, char *buffer, unsigned int length)
{
    unsigned int 	i, k;
    int 			dat;
    
    for (i=0; i<length; i++)
    {
    	dat = *buffer++;
    	dat &= 0xFF;
        crc = (crc >> 8) ^ crc_table[dat ^ (crc & 0xFF)];
        if(i>=0)
          	k=0; 
    }
    
  	for(k=0;k<4;k++)
  	 	MSG[16+k]= (crc>>(k*8)) & 0xFF;
  	
    return crc;
}

  #pragma optimize_on
//-------------------------------------------------------------------------												  
												  
/*								
	char		*spkcode; 
float		MCB44[2];
int 		MCB44_SECT;
float		MCB48[2];
int 		MCB48_SECT;
float		MCT44[2];
int 		MCT44_SECT;
float		MCT48[2];
int 		MCT48_SECT;
float		EPM44[2];
int			EPM44_SECT;
float		EPM48[2];
int			EPM48_SECT;
; 	 
*/

unsigned int				DELI_CMD::frame_no, DELI_CMD::cRXQTY, DELI_CMD::tRXQTY, DELI_CMD::FIRQTY[FPGAFIR_QTY], DELI_CMD::tFIRQTY[FPGAFIR_QTY], DELI_CMD::LED[4];	
long long					DELI_CMD::FIR_SUM;


SECTION(("seg_sdram2")) int 	DELI_CMD::MSG[DELI_FRAMESIZE+1]={0x55,xLEDRGB_SRTAB,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

	int FRAME_SUM; 

void DELI_CMD::x55RX(int *msg)
{
	int		k, test; 

//	if 	(CRC_calc(msg) != msg[15])
//		return; 
	switch( msg[x55_MSGTYPE_POS] )
	{
		case 25:
			test = msg[2] + (msg[3]<<8)+ (msg[4]<<16)+ (msg[5]<<24);
			if (!test)
				test = -1; 
			cRXQTY = test;
		  break;
		case x55_FIR_PACKETQTY:
			if (msg[x55_MSGTYPE_POS]  == x55_FIR_PACKETQTY)			
			{
				++frame_no;				
				for(k=0; k<4; k++)	
				{							
					FIRQTY[k] = 256*msg[k*2+4]+ msg[k*2+3]; 
				}

				if(tRXQTY>0 &&  frame_no>= tRXQTY  )
					Check_ACKFIR();
			}	
		  break;
		case 60:
			FRAME_SUM = 256*msg[8]+ msg[7]; 				    
		  break;
		  
/*		case x55_LED_STATUS:		    
			for(k=0; k<4; k++)
				LED[k] = 2^16*msg[k*3+4]+ 2^8*msg[k*3+3]+ msg[k*3+2]; 			
		  break;
*/		  		
		default: 
			if ( msg[x55_MSGTYPE_POS]  == x55_INPUT_STARTVAL+ SR_PROC::INPUT_ACTIVE)
			{				
				Proc_SR(msg);	
							
			}
		  break;
	}
}

int 	msg_SR;

void DELI_CMD::Proc_SR(int *msg)
{
	int cSR_ID=-1,  k;
	
	msg_SR = msg[x55_FREQ_BYTE2]<<8 | msg[x55_FREQ_BYTE1] & 0xF0; 
	switch(msg_SR) 
	{
		case 0x00A0:
			cSR_ID = 0; 											
		  break;
		case 0x00B0:
			cSR_ID = 1; 													
		  break;
		case 0x0150:
			cSR_ID = 2; 													
		  break;
		case 0x0170:
			cSR_ID = 3; 													
	      break;
	 	case 0x02B0:
			cSR_ID = 4; 													
	      break;
		case 0x02E0:
			cSR_ID = 5; 													
		  break;						  
	}
	SR_PROC::Check_SR_CHG(cSR_ID);	
	if (SR_PROC::SR_CHG)
	{
		Print_Debug(sprintf(dbgtab, "%s: %x", "\nSample Rate Chg" , SR_PROC::SAMPLE_RATE), NOTIME);	
	  // Light up the related LED	
	  	msg_SR= Col6[SR_PROC::SR_ID];
	  	for (k=0; k<20; k++)		
		//DELI_CMD::LED_Color(numLED_SR, Col6[SR_PROC::SR_ID]);			
		DELI_CMD::LED_Color(0x1111, Col6[SR_PROC::SR_ID]);			
		
		//Send_EP(SET_FIR);
		
		//Send_FIR((long long (*))  TXOFIR, SET_FIR, XOFIR_SIZE, x55_CODE_XOFIR);
		
		Send_MAL();
		
	}
}

int 	kcol; 
  
#pragma optimize_off

void DELI_CMD::LED_Color(int LED_NO, int color_rgb)
{
 	int		k, krgb, kled, kframe, x; //, kcol; //, kcol= 0;
	
	
 	kcol = (color_rgb & 0xFF000000)>>24;
 	if ( kcol > 0)
 		color_rgb = Col6[kcol-1]; 
 	
 	kcol = color_rgb; 			
 	
 	MSG[1] = xLEDRGB_SRTAB;		
 
 	for(k=0; k<5; k++)
 	{	
 		if( (1<<(4*k) ) & LED_NO) 
 		{				
 			LED[k]= color_rgb;
 			MSG[14]|= 1<<(k);
 		}
 	}
 	for (kled=0; kled<4; kled++)
 	{
		for(krgb=2; krgb>=0; krgb--)
			DELI_CMD::MSG[2+kled*3+ krgb]= (LED[kled]>>(krgb*8)) & 0x0000FF;			
	}
	//DELI_CMD::MSG[1]= 0x00;
	//DELI_CMD::MSG[2]= 0x01;
	//DELI_CMD::MSG[3]= 0x01;	
						
	//MSG[DELI_FRAMESIZE-1] = CRC_calc(MSG);
	CalcCRC32(0xFFFFFFFF, (char*)MSG, 16);

	
	//for(k=0;k<100000;k++);
	for(kframe=0; kframe<DELI_FRAMESIZE; kframe++)
		Send1Byte(MSG[kframe]);
	
//	trace( (char *) MSG, DELI_FRAMESIZE-1);
}
#pragma optimize_on


void DELI_CMD::SEND_EXEC(void)
{
	int	kframe, tmp;
	
	// MSG[DELI_FRAMESIZE-1] = CRC_calc(MSG);
	CalcCRC32(0xFFFFFFFF, (char*)MSG, 16);
	for(kframe=0; kframe<DELI_FRAMESIZE; kframe++)
	{
		Send1Byte(MSG[kframe]);
		tmp = kframe;
	}
}


void 	DELI_CMD::Send_FIR(long long (*FIR), int mode, int FIR_SIZE, int code)
{
	int 		j, k1, k2, k3, kbyte, kframe; //, EPFIR_SIZE;
	//long long 		*EPF;
	
	
	MSG[1] = code;
	//EPFIR_SIZE = 1504; //sizeof(EP)/sizeof(EP[0]);
	
	if(mode == CLR_FIR)
	{
		for(kbyte=2; kbyte<DELI_FRAMESIZE; kbyte++)
			MSG[kbyte] = 0;
		MSG[4] = 1E+8;
		MSG[9] = 1E+8;			
	}
	
	//for(k1=0; k1<EPFIR_SIZE; k1++)  {EPTAB[0][0][k1] = 0; EPTAB[1][0][k1] = 0;  }; EPTAB[0][0][0] = 0x00400000000;  EPTAB[1][0][0] = 0x00400000000; //0x00411223344;  // 3E+10+1; 
	for (k1=0, k2=0; k1< FIR_SIZE ; k1++, k2+=2)
	{
		MSG[2] = k1 & 0xFF; MSG[3] = k1 >> 8;
		for(kbyte=0; kbyte<5; kbyte++)
		{
			//MSG[kbyte+4] = (EPTAB[SR_PROC::SR_ID][0][k1]>>(kbyte*8) )&0xFF;
			//MSG[kbyte+9] = (EPTAB[SR_PROC::SR_ID][0][k1]>>(kbyte*8) )&0xFF;
			if(mode == SET_FIR)
			{
				MSG[kbyte+4] = (FIR[k1]>>(kbyte*8) )&0xFF;
				MSG[kbyte+9] = (FIR[k1]>>(kbyte*8) )&0xFF;
			}			
		}		
		//for(k3=0; k3<10; k3++)
		FIR_SUM+= 2* FIR[k1]; 	  // MSG[k3+4];
		SEND_EXEC();		
	}
   // Control frame	
	MSG[2] = 0x00; MSG[3] = 0x80;
	for(kbyte=4; kbyte<DELI_FRAMESIZE; kbyte++)
		MSG[kbyte] = 0;
	
	for(k1=0; k1<5; k1++, k2++)
		MSG[k1+4]= (FIR_SUM>>(k1*8) )&0xFF;
				
	SEND_EXEC();		

//	for(k1=0; k1<10;k1++)
//		Send1Byte(0xAA);
		
	FIR_SUM=0; 
	
//	Delay_ms(2000); 
	
//	for(k1=0;k1<10;k1++)	
//			UARTBUF::UART_Monitor_NEW();
}

// Executed in 2 cases:
// 1) DELI_CMD::Proc_SR(int *msg)-> SR_PROC::Check_SR_CHG(cSR_ID) thatr includes Send_EP & Send_RES
// 2) PlaySession::Msg0x58_Proc(int *msg)
void 	DELI_CMD::Send_EP(int mode)
{
	
	//Send_FIR((long long (*)[3])  EPTAB[SR_PROC::SR_ID], mode, EPFIR_SIZE, (35+128));
	Print_Debug(sprintf(dbgtab, "\nSend EP  "), NOTIME);		
	Send_FIR((long long (*))  TME[MCEP::SpkNum].EPF[SR_PROC::SR_ID], mode, EPFIR_SIZE, x55_CODE_EP);
	
	Setup_ACKFIR(x55_FIRNO_EP);

}

// Executed only in 1 case:
// 1) DELI_CMD::Proc_SR(int *msg)-> SR_PROC::Check_SR_CHG(cSR_ID) thatr includes Send_EP & Send_RES

void 	DELI_CMD::Send_MAL(void)
{
	
	//SR_PROC::SR_ID = 0;
	Print_Debug(sprintf(dbgtab, "\nSend MAL  "), NOTIME);		
	
//	Send_FIR( (long long (*)[3]) RESFIR[SR_PROC::SR_ID], SET_FIR, RESFIR_SIZE, (36+128));
	Send_FIR( (long long (*)) TMALF[SR_PROC::SR_ID], SET_FIR, MAL_LEN, x55_CODE_MAL);
	Setup_ACKFIR(x55_FIRNO_DD_RES_MAL);	
}

void 	DELI_CMD::Send_VOL(int *msg)
{
	int k;
	
	MSG[1] = 0x10;
	for(k=2;k<=9; k++)
		MSG[k]= msg[k-1];
	SEND_EXEC();
}


int 	DELI_CMD::Setup_ACKFIR(int FIR_NO)
{
				
	tRXQTY = frame_no+ RXFRAME_DELAY;  // target tRXQTY to be monitored to trigger check on FIR packet succcessful delivery
	tFIRQTY[FIR_NO] = FIRQTY[FIR_NO]; 
	Print_Debug(sprintf(dbgtab, "ACKSET T%d: %d" , FIR_NO, FIRQTY[x55_FIRNO_EP]), NOTIME);		
	
	//LED_Color(0x0010, SR_PROC::SRTAB_FPGA[0][2] );
/*		
	for(k=0;k<1000;k++);			
		UARTBUF::UART_Monitor_NEW();	
	temp= pRXQTY;
	pRXQTY = cRXQTY;
	return(cRXQTY-temp);
*/	
}

int 	DELI_CMD::Check_ACKFIR(void)
{
	int k, status_LED;
	
	Print_Debug(sprintf(dbgtab, "\nACK_Chg check "), NOTIME);
	if (frame_no>= tRXQTY  )
	{		
		for(k=0;k<FPGAFIR_QTY;k++)
		{

			if(tFIRQTY[k]> 0)
			{
				Print_Debug(sprintf(dbgtab, " T%d: C%d P%d " , k, FIRQTY[k], tFIRQTY[k]), NOTIME);											
				if(FIRQTY[k] - tFIRQTY[k] > 0)
				{
					Print_Debug(sprintf(dbgtab, " T%d + " , k, FIRQTY[k]), NOTIME);							
					status_LED+=(k-1);
				}
				else
				{
					//Print_Debug(sprintf(dbgtab, " T%d: %d" , k, FIRQTY[k]), NOTIME);												
					status_LED = 5;
				}
					
				tFIRQTY[k]= 0;	
			}
		}
		//LED_Color(0x0010, SR_PROC::SRTAB_FPGA[status_LED][2] );																		
		frame_no = 0;
		tRXQTY = 0;
	}
}		
	

int DELI_CMD::CRC_calc(int *MSG)
{
	int 	k, crc_sum=0xFF;
	
	for(k=0; k<DELI_FRAMESIZE-1; k++)
		crc_sum+= MSG[k];
		
	return(crc_sum & 0xFF);
}
