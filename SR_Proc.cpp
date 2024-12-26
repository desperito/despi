
#include 	"tt.h"
#include 	<stdio.h>
#include 	<math.h>

#include 	"hDSPSTRING.hpp"
#include 	"hMODEC.hpp"
#include 	"hDSP_MANAGER.hpp"
#include 	"hPlaySession.hpp"
#include 	"hBUF_SRC.hpp"
#include 	"hRESAMPLER.hpp"
#include 	"hUARTBUF.hpp"
#include 	"hDELI_CMD.hpp"

extern		RESAMPLER		RSMP[RSMPOBJ_QTY];  

#if (SOFT_V & SHARC_HW)
	#include 	"hI2SRX_LINE.hpp"
	extern	I2SRX_LINE			*In_SPDIF;
	extern	I2SRX_LINE			*In_TWT;	
#endif 


#define		DIM_RGB				2
//SECTION(("seg_sdram3")) 	int 		SR_TABLE[]={CD44_16,CD48_16,HD88_24,HD96_24,UD176_24,UD192_24}; 


extern	BUF_SRC		X_SRC;
// ----------  E X T E R N S  UZYWNAE PRZEZ MODUL    -----------

//extern 	struct AUDIO_PLAY_CONFIGURATION 	PlaySession;
extern struct StageManager 	SMBas, SMMid, SMTwt;
extern void   New_Playrun_Config(void);
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===-----------    C L A S S  ===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include 	<hSR_PROC.hpp>


	int 				SR_PROC::SAMPLE_RATE, SR_PROC::SR_ID=-1, 
						SR_PROC::DAC_SR, 
						SR_PROC::SR_CHG;  
//	int 				SR_PROC::SRTAB_FPGA[6][3]={ {CD44_16,0x00A0, Granny_Smith}, {CD48_16,0x00B0, Wave}, {HD88_24,0x0150, Electric_Blue}, {HD96_24, 0x0170, Violet}, {UD176_24,0x02B0, Grapefruit}, {UD192_24, 0x02E0, Hot_Pink} }; 
//	int 				SR_PROC::SRTAB_FPGA[6][3]={ {CD44_16,0x00A0, DeepGreen}, {CD48_16,0x00B0, Yellow}, {HD88_24,0x0150, Electric_Blue}, {HD96_24, 0x0170, Violet}, {UD176_24,0x02B0, Grapefruit}, {UD192_24, 0x02E0, Hot_Pink} }; 
	int 				SR_PROC::SRTAB_FPGA[6][3]={ {CD44_16,0x00A0, 0x000500}, {CD48_16,0x00B0, 0x050500}, {HD88_24,0x0150, 0x040100}, {HD96_24, 0x0170, 0x000005}, {UD176_24,0x02B0, 0x050005}, {UD192_24, 0x02E0, 0x050000} }; 

	int					SR_PROC::INPUT_ACTIVE;	
	
/*	
#if (SOFT_V == FPGA_V)
 void SR_PROC::SR_Set(int sr_code)
 {	
	int kSR, k; 
		
	SAMPLE_RATE = SR_TABLE[sr_code-1]; 		
	DAC_SR = floor( ( (float) sr_code-1)/2 );
	if (DAC_SR == 0) 
		DAC_SR = 1;

#else
*/	
#if (SOFT_V & SHARC_HW)
 void  SR_PROC::SR_Set(int pSR_LEVEL, int clk48)
 {
	int kSR, k; 
	
	if (pSR_LEVEL >0)
	{	
		if (pSR_LEVEL==4)  pSR_LEVEL=3;
		// clk48+=1;  
		kSR = (pSR_LEVEL-1)*2+ clk48;
		SR_PROC::SAMPLE_RATE = SRTAB_FPGA[kSR][0]; 	
		SR_PROC::SR_ID = kSR;	
 	}
	
    for(k=BAS; k<=TWT; k++)     
		RSMP[k].OVS_CH = 4/DAC_SR; 
	
	if (TEST_SR_44K_DOMAIN && RESAMPLER::ACTIVE)
		PlaySession::_SMPBLOCK_SIZE = SMPBLOCK_SIZE44;		
	else
		PlaySession::_SMPBLOCK_SIZE = SMPBLOCK_SIZE; 	
		
   #if IN1_ACTIVE
	In_SPDIF->Set_INBUFLEN(PlaySession::_SMPBLOCK_SIZE, DAC_SR);
   #endif			
   #if (INTWT_ACTIVE ==1)
	In_TWT->Set_INBUFLEN(PlaySession::_SMPBLOCK_SIZE, DAC_SR);   	
   #endif			
    		 
   	X_SRC.XLOAD_SIZE = PlaySession::_SMPBLOCK_SIZE*2* DAC_SR; //* RAMSEG_QTY; 
		
	DSPSTRING::SR_Update(SAMPLE_RATE);
}
#endif 

#if (1) // (SOFT_V & DD_BASE)

 void  SR_PROC::Check_SR_CHG(int cSR_ID)
 {
 	
 	if(cSR_ID < 0)
 		Mute_OutBuf();
	else if (cSR_ID != SR_ID )
	{
		//Mute_OutBuf();
   		  // Send EP related to new SR 			
  		//if (DSPSTRING::Get_Digitbit(DSPSTR_EXPH)  )
  				
   		// DELI_CMD::Send_EP(SET_FIR);    			   		  	  			   				   		  			
		// DELI_CMD::Send_RES();
		
		SR_CHG = 1;
		SR_ID = cSR_ID;
		SR_PROC::SAMPLE_RATE = SRTAB_FPGA[cSR_ID][0];
		DAC_SR = SR_ID/2+1;
		SR_PROC::SR_Set(0,0);
	
	}

 }
	
#endif  

/*
void 	SR_PROC::SR_Read_I2SOFF(void)
{
	switch (DSPSTRING::String[SR_2CHAR])
    {
    	case '4': case '8': case '9':                
        	SAMPLE_RATE =  (DSPSTRING::String[SR_2CHAR]-48)<<4 | (DSPSTRING::String[SR_2CHAR+1]-48) ;
           	  break;
       	case '1':                 
      		SAMPLE_RATE =  6<<12 | (DSPSTRING::String[SR_2CHAR]-48)<<8 | (DSPSTRING::String[SR_2CHAR+1]-54)<<4 ;
              break;
   	}
   	DAC_SR = SR_XBUF;
   	SR_Set(DAC_SR, 1-CLK_BASE);  	
}
*/

void 	SR_PROC::Mute_OutBuf(void)
{
	int	kch, ks, k;
	
	for (kch=0; kch< CHN_QTY; kch++)
	{	  			
		for (ks=0; ks< DSP_BLOCK_QTY; ks++)
		{
			if (DSP_MANAGER::Streams[ks][kch] != NULL ) //&&  ( (kch+1) & CHN_WORKMODE) )  //Streams[ks].inblock[kch] != NULL)								
			{
				for(k=0; k< DSP_MANAGER::Streams[ks][kch]->BufD->BUF_SIZE; k++)
					DSP_MANAGER::Streams[ks][kch]->BufD->Buf[k]= 0;
			} 
		}
	}
}

