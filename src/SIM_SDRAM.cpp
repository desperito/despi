
#include  	"tt.h"

#if (SOFT_V & SHARC_HW)
 #include 	<21369.h>
 #include 	<Cdef21369.h>
 #include 	<def21369.h>
#endif

#include  	<stdio.h>
#include  	<stdlib.h>
#include  	<string.h>

#include  	"SIM.h"

#include  	"math.h"

#include	"hPlaySession.hpp"
#include 	"hSR_MANAGER.hpp"
#include 	"hDSPSTRING.hpp"
#include 	"hSIM_INPUT.hpp"
#include 	"hSR_PROC.hpp"
#include 	"hMODEC.hpp"
#include 	"hBUF_MANAGER.hpp"

//================================================================================
//================================================================================

#define		FILENAME_PREFIX_OFFSET	1

//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================

#include		"hSIM_SDRAM.hpp"
	
//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================

 //= Static members definition 	
	int												SIM_SDRAM::CH_MIX;
	int												SIM_SDRAM::D_LEN,SIM_SDRAM::E_LEN,SIM_SDRAM::I_LEN;
	int												SIM_SDRAM::js, SIM_SDRAM::ks, SIM_SDRAM::kname;
	char SECTION(("seg_sdram2"))					SIM_SDRAM::fname[FILENAME_LEN];



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===========         M E T H O D S        ======================
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
/*
#define		X_MASK 					0x01000000
#define		XP_MASK					0x02000000
#define		EP_MASK					0x04000000
#define		DB_MASK					0x00010000
#define		DM_MASK					0x00020000
#define		DT_MASK					0x00040000
#define		RB_MASK					0x00100000
#define		RM_MASK					0x00200000
#define		RT_MASK					0x00400000
*/


	extern BUF_MANAGER 	*BUFSIM_List[];

	char				SPKSYMBOL[]={'B','M','T'};
	char				PRESYMBOL[]={'I','P','E'};	

void	SIM_SDRAM::MDump(float *BUF, int data_type, int dump_size, int bufsim_mask)
{
	int k, SIM_COMPLETE = 1;
	
	if ( (bufsim_mask & 0xFFFFFF00) == ET_MASK)
		fname_BUF_CHN(bufsim_mask, bufsim_mask & 0x0F-1, 'E', 'T');
	else if ( (bufsim_mask & 0xFFFFFF00) == EB_MASK)
		fname_BUF_CHN(bufsim_mask, bufsim_mask & 0x0F-1, 'E', 'B');		
	else if (bufsim_mask>0x00F00000)
		fname_BUF_CHN(bufsim_mask, bufsim_mask & 0x0F-1, PRESYMBOL[(bufsim_mask>>24)/2], 'F');
	else if (bufsim_mask> 0x000F0000)
		fname_BUF_CHN(bufsim_mask, bufsim_mask & 0x0F-1, 'R', 							SPKSYMBOL[(bufsim_mask>>20)/2]);		
	else if (bufsim_mask> 0x0000F000)
		fname_BUF_CHN(bufsim_mask, bufsim_mask & 0x0F-1, 'D', 							SPKSYMBOL[(bufsim_mask>>16)/2]);
	//else 
	//	fname_BUF_CHN(bufsim_mask, bufsim_mask & 0x0F-1, 'E');
		
//	MemDump(BUF, NULL, DUMP_FLOAT, dump_size, SIM_INPUT::LRVOL[(bufsim_mask & 0x0F)-1], fname);  //-1 w oststnim parametrze zeby zmapowac LR_LR_0
	MemDump(BUF, NULL, DUMP_FLOAT, dump_size, 0, fname);  //-1 w oststnim parametrze zeby zmapowac LR_LR_0
	for(k=0; k<10; k++) 
	{
		if (BUFSIM_List[k] != NULL)
			SIM_COMPLETE&= BUFSIM_List[k]->SIM_FLAG; 
	}
	if (SIM_COMPLETE)
		Restart_Playback();		
}	
		

void 	SIM_SDRAM::fname_MODEC(int channel)
{
	char * mstring;  

   //= Get MODEC fullname for the channel 
	mstring = MODEC::Get_CHN_fullname(channel);
  //= Append modec fullname
   //fname[kname++]= '_';
 	for(ks=0;mstring[ks]; ks++) 
 		fname[kname++]= mstring[ks];
 	fname[kname++]='_';	
}


void 	SIM_SDRAM::Restart_Playback(void)
{
	int k;

   #if (SOFT_V & SHARC_HW)
   //=  Set UART to back 115200 to receive GUI commands							
	UART_speed(1, UART1_CONSOLE);	
	//	for(k=0;k<1E+6;k++); 
//	Flags_Reset(); //SIM_SDRAM_Config();	
   #endif	
	switch(PlaySession::RECORDING_MODE )
	{
		case SIM_I2SOFF_D_ONLY:
			switch(PlaySession::PLAY_MODE)
			{			
				case SIMTEST_ON:
					Print_Debug(sprintf(dbgtab, "\nSIM Mode OFF"), NOTIME);															 	
					PlaySession::PLAY_MODE = PLAY_SIM_OFF; //SIMTEST_OFF;
				  break;
				case SIM_1OFF:			
					DSPSTRING::Print_DSPstring_Modes();								   				
					Print_Debug(sprintf(dbgtab, "\nExit()"), NOTIME);															 	
					exit(0);	
				  break;
				case DUAL_SIM:			
					Print_Debug(sprintf(dbgtab, "\nRestart Audio Play"), NOTIME);															 										 	
					PlaySession::RECORDING_MODE = AUDIO_NOREC;					
					PlaySession::PLAY_MODE = DUAL_PLAY;		
					SR_PROC::SAMPLE_RATE = 0;											
				  break;
			}		
			DSPSTRING::Print_DSPstring_Modes();								   
		  break;   
	 	case SIM_I2SON_D_E:
			//Audio_Stop(); 									
			Print_Debug(sprintf(dbgtab, "Dump-I2SON END"), NL_NOTIME);										
		  break;
		case REAL_I2SON_D_E:
			//Audio_Stop(); 
			PlaySession::RECORDING_MODE = AUDIO_NOREC;
			//PlaySession::RECMODE_CHG = 1; 
		  break;								  
	}
	//Audio_Stop(); // zeby wyczyscic bufory SPORT i struktury SM
}


void SIM_SDRAM::fname_BUF_CHN(int jbuf, int kch, char buftype_code1, char buftype_code2) 
{ 	
		
	kname = 0;
	for(js=0; js< FILENAME_LEN; js++)
		fname[js]= 0;
 
   //= DSP String z RSMP i BUF_TYPE
 	fname_DSPSTRING();
	fname[kname++]= '_';	
	fname[kname++]= buftype_code1;
	fname[kname++]= buftype_code2;
 	
   //= Channel i Stream				
	Dump_BUF_CHN_NamePrep(jbuf, kch);
   //= Lista Modec	
	if ( (jbuf>= 0) && ( MODEC_filt_qty(LEFT_0) ||  MODEC_filt_qty(RIGHT_0) ) && DSPSTRING::Get_Digitbit(DSPSTR_MODEC) ) 		//(PlaySession.ACOUSTIC_CORR) 	
		fname_MODEC(kch);
   //= .txt file extension
	fname[kname++]='.'; fname[kname++]='t'; fname[kname++]='x'; fname[kname++]='t'; 	
}

void SIM_SDRAM::Dump_BUF_CHN_NamePrep(int bufname, int channel)
{
     
	fname[kname++]='_';  
	
  //= 1-letter channel code	
 	if (channel==LEFT_0)	fname[kname++]='L';	
 	else					fname[kname++]='R';
	
	switch(bufname& 0xFFFFFF00)
	{
		case 0x01000000: fname[kname++]='X'; fname[kname++]='I' ; fname[kname++]='N';  break;
		case 0x02000000: fname[kname++]='P'; fname[kname++]='R' ; fname[kname++]='E';  break;
		case 0x04000000: fname[kname++]='E'; fname[kname++]='X' ; fname[kname++]='P';  break;
		case 0x00010000: case 0x00000100: fname[kname++]='B'; fname[kname++]='A' ; fname[kname++]='S';  break;	
		case 0x00020000: case 0x00000200: fname[kname++]='M'; fname[kname++]='I' ; fname[kname++]='D';  break;
		case 0x00040000: case 0x00000400: fname[kname++]='T'; fname[kname++]='W' ; fname[kname++]='T';  break;
	}
 	//fname[kname++]='_';		
}


int SIM_SDRAM::fname_DSPSTRING(void)
{
		//kname = FILENAME_PREFIX_OFFSET;
	  //= Aktualne ustawienia DSP i typy oversamplingu
	  	for (ks=0; ks<(sizeof(DSPSTRING::String)/sizeof(char))-1; ks++) // DSPSTRING_LEN; ks++)
		{	fname[kname++]= DSPSTRING::String[ks]; }
		return (kname-1);
}

		

void UARTisr_1(int temp);

void 	SIM_SDRAM::Send1Byte_U1(int byte2send)
{
	int k; 
	
	 /* loop to transmit source array in core driven mode */   		
	while ((*pUART1LSR & UARTTHRE) == 	0) {;}
    	*pUART1THR = byte2send;  			
	/* poll to ensure UART has completed the transfer */
  	while ((*pUART1LSR & UARTTEMT) == 0) {;} 
}

	SECTION(("seg_sdram4"))	float 	fDUMP[2][FDUMP_SIZE];			

void SIM_SDRAM::MemDump(float *BUF_FDATA, int *BUF_IDATA,int data_type, int lenfull, int max_level, char *fname) 
{
//= This body implements Sharc-specific UART-based test dump. With Linux HW it needs to be replaced woth some dump to a file
#if (SOFT_V & SHARC_HW) 			
	//float  			UART_DIV_float;
	float 			f, f2; 
	int 			j1; 
	int 			fk;
	int				k, ks0, int1;
	int 			len, len1, len2, len0;
			
	UART_speed(1, UART1_DUMPSPEED);			     	
  //===========================================================

  //= Send identifier to correctly interpret number representation float vs int		
	if (data_type == DUMP_FLOAT) 
	{
//		for(k=0;k<lenfull; k++)
//			fDUMP[k]= BUF_FDATA[k];
//		BUF_FDATA = fDUMP;
		Send1Byte_U1(0x41);
	}
	else
	{
//		for(k=0;k<lenfull; k++)
//			tDUMP[k]= IDATA[k];
//		IDATA = tDUMP;		 
		Send1Byte_U1(0x40);
	}
	
//=  Calculate lenghts of filename and data =============================
  //= Calculate and send filename length
	ks=0;	//fname[1] = '.'; fname[2] = 't'; fname[3] = 'x'; 	fname[4] = 't'; fname[5] = 0;		
	while (fname[ks])
		ks++;		
	Send1Byte_U1(ks);  //Filename length
	
	//lenfull+= 1; //Skaler Volume jako pierwsza liczba
  //===============================================
  //= Calculate and send data buffer length	
	len2 = 	lenfull/65536;
	len1 = (lenfull-len2*65536)/256;
	len0 = lenfull-len2*65536-len1*256;
	Send1Byte_U1(len0);	
	Send1Byte_U1(len1);	
	Send1Byte_U1(len2);
  //===============================================
//=======================================================================		
  //= Send disk filename text  =======================	
  	ks=0;	
	while (fname[ks])
	{	Send1Byte_U1(fname[ks]);
		ks++;	
	}
	
   //= Add first byte with SIM Volume level
//   	for (ks=0; ks< 1; ks++)	
//		BUF_FDATA[ks] = (float) 1.0/0.0; // 1E+37; //
/*	if (data_type == DUMP_FLOAT)
		BUF_IDATA[0] = 1E+37; 	
	else 
		BUF_IDATA[0] = 1E+37;
*/		 	
   	/*	
	if (data_type == DUMP_FLOAT)
	{
		BUF_FDATA[0] = 1E+37;  	// 1.#INF; // max_level; //SIM_INPUT::LRVOL[chn];		
//		BUF_FDATA[1] = 1E+37;  	// 1.#INF; // max_level; //SIM_INPUT::LRVOL[chn];		
//		BUF_FDATA[2] = (float) 1.0/0.0;  	// 1.#INF; // max_level; //SIM_INPUT::LRVOL[chn];				
	}
	else
	
	{
		BUF_IDATA[0] = 1E+37; // max_level;
//		BUF_FDATA[1] = (float) 1.0/0.0;  	// 1.#INF; // max_level; //SIM_INPUT::LRVOL[chn];		
//		BUF_FDATA[2] = (float) 1.0/0.0;  	// 1.#INF; // max_level; //SIM_INPUT::LRVOL[chn];			
	}
*/	
	ks=0; 	
  //= Send data buffer  =======================
	//for (ks0=0; ks0<lenfull; ks0++)
	for (ks=0; ks< lenfull; ks++)	
	{	
		//ks = ks0+1; 
		if (data_type == DUMP_FLOAT)
		{
			f = BUF_FDATA[ks];		
			memcpy( (void *) &fk, (void *) &f, 1);
		}
		else 
		{
			fk = BUF_IDATA[ks];						
		}
		Send1Byte_U1((int) (fk&0xFF));Send1Byte_U1(fk>>8); Send1Byte_U1(fk>>16);Send1Byte_U1(fk>>24); 
		//if (lenfull- ks== 10000)
		//	for(k=0;k<5E+6; k++) ;  		
	}
 //= Data transfer completed character	
 	for(k=0;k<5E+6;k++) ; 

	Send1Byte_U1(0xCC); 
   #if (xRUN_MODE == TEST_MODE)
 	Print_Debug( sprintf(dbgtab, "\nFile %s saved", fname), NOTIME );
   #endif	
//	UART_Setup (); //	interrupt(SIG_P18,UARTisr_1); 	
 #endif
}


/*
#else 

unsigned int CalcCRC32(unsigned int crc, char *buffer, unsigned int length); 
extern "C"  void Delay_ms(int ms);

SECTION(("seg_sdram2"))  char 	TX_Buffer[64];

unsigned int 			fk0, fk1, fk2, fk3, fk4, fk5, fk6, fk7, fk8, fk9, fk10, fk11;
unsigned int 			CRC_;

void SIM_SDRAM::MemDump(float *BUF_FDATA, int *BUF_IDATA,int data_type, int lenfull, int max_level, char *fname) 
{
#if	(SOFT_V == SHARC_V || SOFT_V & FP_BASE) 			
	//float  			UART_DIV_float;
	float 					f, f2; 
	unsigned int 			j, j1; 
	unsigned int 			fk;
	unsigned int			k, ks;
	unsigned int 			len, len1, len2, len0;
	
	//Audio_Stop();

	UART_speed(1, UART1_DUMPSPEED);			
     	
  //===========================================================

  //= Send identifier to correctly unsigned interpret number representation float vs unsigned int		
  
	TX_Buffer[0] = 0x55;  
	if (data_type == DUMP_FLOAT) 
		TX_Buffer[1] = 0x41;
	else 
		TX_Buffer[1] = 0x40;


//=  Calculate lenghts of filename and data =============================
  //= Calculate and send filename length
	ks=0; //fname[1] = '.'; fname[2] = 't'; fname[3] = 'x'; fname[4] = 't'; fname[5] = 0; 
	while (fname[ks])
		ks++; 

	TX_Buffer[2] = ks;  //Filename length

	lenfull+= 1; //Skaler Volume jako pierwsza liczba
  //===============================================
  //= Calculate and send data buffer length 
  //===============================================
  //= Calculate and send data buffer length	
	len2 = 	lenfull/65536;
	len1 = (lenfull-len2*65536)/256;
	len0 = lenfull-len2*65536-len1*256;
  
	//dummy.XSB = lenfull;

	TX_Buffer[3] = len0;
	TX_Buffer[4] = len1;
	TX_Buffer[5] = len2;

  //===============================================
//======================================================================= 
  //= Send disk filename text  ======================= 
  ks=0; 
	while (fname[ks])
	{
		TX_Buffer[6+ks] = fname[ks]; 
		ks++;
        if (ks >= 59)
        	exit;
	}

	TX_Buffer[59] = max_level;

    CRC_ = CalcCRC32(0xFFFFFFFF, (char*)(TX_Buffer), 60);
    TX_Buffer[60] = CRC_ & 0xFF;
    TX_Buffer[61] = CRC_ >> 8;
    TX_Buffer[62] = CRC_ >> 16;
    TX_Buffer[63] = CRC_ >> 24;

    for (j=0; j<64; j++)
   		Send1Byte_U1(TX_Buffer[j]);

    Delay_ms(1);

  //= Send data buffer  =======================


	for (ks=0; ks<lenfull; ks+=8)
    {
        TX_Buffer[0] = 0x55;
        TX_Buffer[1] = 0x40;
               
        TX_Buffer[3] = ks & 0xFF;
        TX_Buffer[4] = ks >> 8;
        TX_Buffer[5] = ks >> 16;
        TX_Buffer[6] = ks >> 24;

		memcpy( (void *) &fk0, (void *) &(BUF_FDATA[ks]), 1);
          
        TX_Buffer[10] = fk0 & 0xFF;
        TX_Buffer[11] = fk0  >> 8;
        TX_Buffer[12] = fk0 >> 16;
        TX_Buffer[13] = fk0 >> 24;

		memcpy( (void *) &fk1, (void *) &(BUF_FDATA[ks+1]), 1);
        
        TX_Buffer[10+4] = fk1 & 0xFF;
        TX_Buffer[11+4] = fk1  >> 8;
        TX_Buffer[12+4] = fk1 >> 16;
        TX_Buffer[13+4] = fk1 >> 24;

		memcpy( (void *) &fk2, (void *) &(BUF_FDATA[ks+2]), 1);        
        
        TX_Buffer[10+8] = fk2 & 0xFF;
        TX_Buffer[11+8] = fk2  >> 8;
        TX_Buffer[12+8] = fk2 >> 16;
        TX_Buffer[13+8] = fk2 >> 24;

		memcpy( (void *) &fk3, (void *) &(BUF_FDATA[ks+3]), 1);
        
        TX_Buffer[10+12] = fk3 & 0xFF;
        TX_Buffer[11+12] = fk3  >> 8;
        TX_Buffer[12+12] = fk3 >> 16;
        TX_Buffer[13+12] = fk3 >> 24;

		memcpy( (void *) &fk4, (void *) &(BUF_FDATA[ks+4]), 1);
        
        TX_Buffer[10+16] = fk4 & 0xFF;
        TX_Buffer[11+16] = fk4  >> 8;
        TX_Buffer[12+16] = fk4 >> 16;
        TX_Buffer[13+16] = fk4 >> 24;

		memcpy( (void *) &fk5, (void *) &(BUF_FDATA[ks+5]), 1);
        
        TX_Buffer[10+20] = fk5 & 0xFF;
        TX_Buffer[11+20] = fk5  >> 8;
        TX_Buffer[12+20] = fk5 >> 16;
        TX_Buffer[13+20] = fk5 >> 24;

		memcpy( (void *) &fk6, (void *) &(BUF_FDATA[ks+6]), 1);
        
        TX_Buffer[10+24] = fk6 & 0xFF;
        TX_Buffer[11+24] = fk6  >> 8;
        TX_Buffer[12+24] = fk6 >> 16;
        TX_Buffer[13+24] = fk6 >> 24;

		memcpy( (void *) &fk7, (void *) &(BUF_FDATA[ks+7]), 1);
        
        TX_Buffer[10+28] = fk7 & 0xFF;
        TX_Buffer[11+28] = fk7  >> 8;
        TX_Buffer[12+28] = fk7 >> 16;
        TX_Buffer[13+28] = fk7 >> 24;

		memcpy( (void *) &fk8, (void *) &(BUF_FDATA[ks+8]), 1);
        
        TX_Buffer[10+32] = fk8 & 0xFF;
        TX_Buffer[11+32] = fk8  >> 8;
        TX_Buffer[12+32] = fk8 >> 16;
        TX_Buffer[13+32] = fk8 >> 24;

		memcpy( (void *) &fk9, (void *) &(BUF_FDATA[ks+9]), 1);
        
        TX_Buffer[10+36] = fk9 & 0xFF;
        TX_Buffer[11+36] = fk9  >> 8;
        TX_Buffer[12+36] = fk9 >> 16;
        TX_Buffer[13+36] = fk9 >> 24;

		memcpy( (void *) &fk10, (void *) &(BUF_FDATA[ks+10]), 1);
        
        TX_Buffer[10+40] = fk10 & 0xFF;
        TX_Buffer[11+40] = fk10  >> 8;
        TX_Buffer[12+40] = fk10 >> 16;
        TX_Buffer[13+40] = fk10 >> 24;

		memcpy( (void *) &fk11, (void *) &(BUF_FDATA[ks+11]), 1);
        
        TX_Buffer[10+44] = fk11 & 0xFF;
        TX_Buffer[11+44] = fk11  >> 8;
        TX_Buffer[12+44] = fk11 >> 16;
        TX_Buffer[13+44] = fk11 >> 24;

        
    	CRC_ = CalcCRC32(0xFFFFFFFF, (char*)(TX_Buffer), 60);
    	TX_Buffer[60] = CRC_ & 0xFF;
    	TX_Buffer[61] = CRC_ >> 8;
    	TX_Buffer[62] = CRC_ >> 16;
    	TX_Buffer[63] = CRC_ >> 24;

        for (j=0; j<64; j++)
            Send1Byte_U1(TX_Buffer[j]);
		if (ks>100)
			j = j1;
        Delay_ms(1);
    }
#endif    
}


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

static unsigned int CalcCRC32(unsigned int crc, char *buffer, unsigned int length)
{
    unsigned int 	i;

    for (i=0; i<length; i++)
        {
        crc = (crc >> 8) ^ crc_table[*buffer++ ^ (crc & 0xFF)];
        }

    return crc;
}
//-------------------------------------------------------------------------
#endif
*/







/*
union uint32_t_LSB_MSB
    {
    struct
        {
        char XSB_0;
        char XSB_1;
        char XSB_2;
        char XSB_3;
        };
		
        int 	XSB;
    };


union float_LSB_MSB
    {
    struct
        {
        char XSB_0;
        char XSB_1;
        char XSB_2;
        char XSB_3;
        };
		
        float FLT;
    };


SECTION(("seg_sdram2"))  char 	TX_Buffer[64];
union 	uint32_t_LSB_MSB 		dummy;

union 	uint32_t_LSB_MSB 		CRC_;


union float_LSB_MSB 			fk0;
union float_LSB_MSB 			fk1;
union float_LSB_MSB 			fk2;
union float_LSB_MSB 			fk3;

        TX_Buffer[3] = dummy.XSB_3;
        TX_Buffer[4] = dummy.XSB_2;
        TX_Buffer[5] = dummy.XSB_1;
        TX_Buffer[6] = dummy.XSB_0;

        fk0.FLT = BUF_FDATA[ks+0];
        fk1.FLT = BUF_FDATA[ks+1];
        fk2.FLT = BUF_FDATA[ks+2];
        fk3.FLT = BUF_FDATA[ks+3];

        TX_Buffer[10] = fk0.XSB_0;
        TX_Buffer[11] = fk0.XSB_1;
        TX_Buffer[12] = fk0.XSB_2;
        TX_Buffer[13] = fk0.XSB_3;

        TX_Buffer[14] = fk1.XSB_0;
        TX_Buffer[15] = fk1.XSB_1;
        TX_Buffer[16] = fk1.XSB_2;
        TX_Buffer[17] = fk1.XSB_3;

        TX_Buffer[18] = fk2.XSB_0;
        TX_Buffer[19] = fk2.XSB_1;
        TX_Buffer[20] = fk2.XSB_2;
        TX_Buffer[21] = fk2.XSB_3;

        TX_Buffer[22] = fk3.XSB_0;
        TX_Buffer[23] = fk3.XSB_1;
        TX_Buffer[24] = fk3.XSB_2;
        TX_Buffer[25] = fk3.XSB_3;



*/
//===================================================================================================================
//===  Public interface Wrappers so that they are callable from Standard C in the rest of the project   =============
//===================================================================================================================

