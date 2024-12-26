#include 	"tt.h"
#include 	<stdio.h>
#include 	<string.h>


#include 	"hPlaySession.hpp"
#include 	"hRESAMPLER.hpp"
#include 	"hPOLY.hpp"
#include 	"hMODEC.hpp"
#include 	"hDSP_TAB.hpp"

extern  char DSPSTRING_default[];

extern 		RESAMPLER		RSMP[RSMPOBJ_QTY]; 
extern 		FILTER			FILT[FILTOBJ_QTY]; 


SECTION(("seg_sdram2"))  int		dspstr_underscores[] = {SR_2CHAR-1,IPL_BAS-1, IPL_EXT-1}; //{0, SR_2CHAR-1,RSMP_FLAG,IPL_BAS-1, IPL_EXT-1};

extern void New_Playrun_Config(void);


//EXTERNC char * MODEC_CHN_fullname(int channel);
//==================================================================
// ----------  E X T E R N S  UZYWNAE PRZEZ MODUL    -----------
//==================================================================

extern	int		MODCFG_CH;	

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===-----------    C L A S S  ===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


#include 	"hDSPSTRING.hpp"
#include 	"hBUF_MANAGER.hpp"
#include 	"hDSP_MANAGER.hpp"

extern	BUF_MANAGER		RB, RT; 

//@@@@@   D S P   S T E P S    TA B L E   @@@@@@@@@@@@@@ 

	char				DSPSTRING::String[DSPSTRING_LEN+2]; 
	
/*
void DSPSTRING::Print_DSPstring_Modes(void)
{
	int		k, kf, kch, tmp, stage_qty, block_newlina, stage_address;
	char 	modstr[MODEC_FULLNAME_LEN], tmpstr[20];
	FILTER		*FR;
	
	//return; 
	tmp = sprintf( modstr, MODEC::Get_CHN_fullname(LEFT_0));  //MODEC_CHN_fullname(LEFT_0));		
	
   #if  		(WORK_CONFIG &  RUNMODE_MASK) == PROD_MODE  	
	if (tmp >0 )
		tmp = sprintf( dbgtab, "\n%s%s", String, modstr); 
	else 
		tmp = sprintf( dbgtab, "\n%s", String); 		//dbgtab+2
	Print_Debug( tmp, NOTIME);			
   #else		
	if (tmp >0 )
		tmp = sprintf( tmpstr, "\n%s%s", String, modstr); 
	else 
		tmp = sprintf( tmpstr, "\n%s", String); 		//dbgtab+2
	strcat(dbgtab, tmpstr);		
   #endif	
	
   	for(kch=0; kch<2; kch++)
   	{
		for(kf=0; kf< DSP_BLOCK_QTY; kf++)
		{		
			FR= DSP_MANAGER::Streams[kf][kch]->FiltRsmp; 
			if (FR == NULL)   continue;		
			if 	(FR->DSP_TYPE == DSP_FILT)	stage_qty = FILTER::STAGE_QTY;
			else 							stage_qty = RESAMPLER::STAGE_QTY;		
			block_newlina= 1;
			if (FR->DSP_TYPE == DSP_RSMP)
			{
				tmp = sprintf( tmpstr, "\n%d.", kf);
				strcat(dbgtab, tmpstr);
				stage_address = (unsigned int) DSP_MANAGER::Streams[kf][kch];			
				switch (stage_address)
				{
					case 0x000bd002:	tmp = sprintf( tmpstr, "%-11s ", "RSMP BAS-L"); break;
					case 0x000bd014:	tmp = sprintf( tmpstr, "%-11s ", "RSMP BAS-R"); break;								
					case 0x000bd028:	tmp = sprintf( tmpstr, "%-11s ", "RSMP MID-L"); break;
					case 0x000bd038:	tmp = sprintf( tmpstr, "%-11s ", "RSMP MID-R"); break;				
					case 0x000bd04c:	tmp = sprintf( tmpstr, "%-11s ", "RSMP TWT-L"); break;
					case 0x000bd05e:	tmp = sprintf( tmpstr, "%-11s ", "RSMP TWT-R"); break;
				}	
				strcat(dbgtab, tmpstr);		
				block_newlina= 0;
			}
			for(k=0; k< stage_qty; k++)
			{	if(FR->stages[k].stagefun)
				{  	
					if (block_newlina) 
					{	//if (PlaySession::DEBUG_MODE == PRINTF_DBG || PlaySession::DEBUG_MODE == UART_PC_PRINTF)
						//printf("\n");
  				  	#if  		(WORK_CONFIG &  RUNMODE_MASK) == PROD_MODE  	
  				 		tmp = sprintf( dbgtab, "\n%d-%d.%-11s ", kf, k, FR->stages[k].DSPTAB_desc); 								
  				  	#else 	
  				 		tmp = sprintf( tmpstr, "\n%d-%d.%-11s ", kf, k, FR->stages[k].DSPTAB_desc); 								
  				  	#endif	
						block_newlina= 0;
					}	
					else 
					{
  				  	#if  		(WORK_CONFIG &  RUNMODE_MASK) == PROD_MODE  					
  				 		tmp = sprintf( dbgtab, "%d-%d.%-11s ", kf, k, FR->stages[k].DSPTAB_desc);
  				  	#else 	  				 	 		
  				 		tmp = sprintf( tmpstr, "%d-%d.%-11s ", kf, k, FR->stages[k].DSPTAB_desc); 		
  				  	#endif	
					}
  			   	#if   (WORK_CONFIG &  RUNMODE_MASK) == PROD_MODE  					
  					Print_Debug(0, NOTIME);				
  			   	#else 	  				 	 		
  					strcat(dbgtab, tmpstr);
  			   	#endif	  				    				    				 	  				 	
				}
			}
		}
        #if   (WORK_CONFIG &  RUNMODE_MASK) == TEST_MODE  							
		 Print_Debug(0, NOTIME);				
		#endif

		k=k+1;
	} // for .. kch
   #if   (WORK_CONFIG &  RUNMODE_MASK) == PROD_MODE  						
  	Print_Debug(0, NOTIME);				
   #endif
			
		
//  	for(k=0; k<DSP_TAB::k_dsp; k++)
//  	{
//		tmp = sprintf( dbgtab, "\n%s", DSP_TAB::stages[k]); 		//dbgtab+2  		
//  		printf(dbgtab);			
//  	}
  	
#if	(SOFT_V & SHARC_HW)  		
  	//trace(dbgtab,0);
#endif
  	
	//Print_Debug( sprintf(dbgtab, "\nD1:%d", RB.k_IN-RT.k_IN), NOTIME);
}
*/

int DSPSTRING::char2hex(char sdigit)
{

	if (sdigit>='0' && sdigit<='9')
		return(sdigit- '0');
	else if (sdigit>='A' && sdigit<='F')
		return(sdigit-'A'+10);
}

char DSPSTRING::hex2char(int hdigit)
{
	char tmp;

	if (hdigit>=0 && hdigit<=9)
	{
		tmp = hdigit+'0';
		return(hdigit+'0');
	}
	else if (hdigit>=10 && hdigit<=15)
	{
		tmp = hdigit+'A'-10;
		return(hdigit+'A'-10);
	}
}

void DSPSTRING::Set_Digitbit(int bitpos, int ON_OFF, int offset)
{
	int	pos;
	char val;
	
	pos = (bitpos& 0xF0)>>4;
	
	if (String[pos]=='X' || String[pos]=='0')
		val= 0+offset;
	else 
		val = char2hex(String[pos]);
	if (ON_OFF == 1)
		val|= ( 1<<(bitpos & 0x0F) );
	else 	if (ON_OFF == 0)
		val&= ~( 1<<(bitpos & 0x0F) );
	String[pos] = hex2char(val);	
//	DSP_TAB::Stage_Switches();
}


#if UART_TYPE == UART_OLD  
 int DSPSTRING::Get_Digitbit(int bitpos)
 {
	int	pos;
	char val;
	
	pos = (bitpos& 0xF0)>>4;	
	val = char2hex(String[pos]);	
   //= Returns the String val 
   val = val& (1<<(bitpos & 0x0F)); 
	return( val& (1<<(bitpos & 0x0F)) );
 }
#elif  UART_TYPE == UART_NEW
 int DSPSTRING::Get_Digitbit(int bitpos)
 {
	return( cDSP & (1<<(bitpos )) );
 }
#endif

void DSPSTRING::Set_BasDigit(int change, int directval)
{
	int 	cval;
	
	if (change!=0) // Apply change 
	{
		cval = Get_Digitbit(DSPSTR_BAS0) | Get_Digitbit(DSPSTR_BAS1)>>1; 
		directval= cval+change;	
		if (directval<0) 	directval = 0;
		if (directval>3) 	directval = 3;
	}		
   //= Set to val either aboslute or just changed
	Set_Digitbit(DSPSTR_BAS0, directval& 0x01, 0);
	Set_Digitbit(DSPSTR_BAS1, (directval& 0x02)>>1, 0);	
//	DSP_TAB::Stage_Switches();	
}


int  DSPSTRING::set_IPL(int pos, int ipl_val, int FIR_flag)
{
	char old_IPL=0; 
	
	if (ipl_val == 'X' || ipl_val == 'x')
	{	
		if (FIR_flag)
			String[pos] = 'F'; 
		else
			String[pos] = 'x';
	}
	else
	{
		old_IPL = String[pos];
		String[pos] = ipl_val+ 32*(1-FIR_flag);  
	}
	if ( (ipl_val != old_IPL) && (ipl_val != old_IPL-32) )
		return(1);
	else
		return(0);	
}
	
void DSPSTRING::Fill_DSPSTRING_blanks(void)
{
	int k=0;
	
		while(k<(int)sizeof(dspstr_underscores))
			String[dspstr_underscores[k++]]='_';	
		//String[2]= 'X'; String[6]= 'X';  
}

						//	pDEV_MODE, ACOUSTIC_CORR,        0,    pFIR_CH,       pFIR_EXT,      pFR20k_EQ, PRE_Level,       pBAS_IPL_TYPE,   pMID_IPL_TYPE, pTWT_IPL_TYPE, pEXT_IPL_TYPE
int  DSPSTRING::User_Update(int new_devmode, int pFIR_CH, int pFIR_EXT, int pFR20k_EQ, int pBAS_IPL, int pMID_IPL, int pTWT_IPL, int pEXT_IPL_TYPE, int pOVS_EXT )
{
	int k=0, IPL_FR20kEQ_change= 0;
		
		//IPL_change=0;
		Fill_DSPSTRING_blanks();	
		
		IPL_FR20kEQ_change+= set_IPL(IPL_BAS, pBAS_IPL, pFIR_CH);  	
		IPL_FR20kEQ_change+= set_IPL(IPL_MID, pMID_IPL, pFIR_CH); 
		IPL_FR20kEQ_change+= set_IPL(IPL_TWT, pTWT_IPL, pFIR_CH); 
		set_IPL(IPL_EXT, pEXT_IPL_TYPE, pFIR_EXT);  // RPI: do sprawdzenia czy ostatni par: PlaySession.FIR_EXT && PlaySession.OVS_EXT >1
		
//		if (pFR20k_EQ > 0  )  
//		{
		if (String[FR20k_EQ_LEVEL] != pFR20k_EQ+48)
			++IPL_FR20kEQ_change;
		String[FR20k_EQ_LEVEL] = pFR20k_EQ+48; 			// docelowo:	PlaySession.FR20k_EQ, ktore ma przyjmowac wartosci 0, 1, 2 - poziom korekcji amplitudowej
//		}
//		else
//		{	
//			String[FR20k_EQ_LEVEL] = 'X';
//				//String[CH_IPL_TYPE] = 'X'; 								
//		}									
		String[xOVS_EXT_LEVEL] = pOVS_EXT+48 ;		
			
			//set_OVS_VOLSCALER(); //PlaySession.OVS_VOLSCALER = 1; //
			//DSP_MANAGER_Streams_Config();
			
		Apply_Current_String();	
		return(IPL_FR20kEQ_change);
}
/*
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	PlaySession.CORRECTION_TYPE	= 	CORRECTION_A; //CORRECTION_B; //  1; //CORRECTION_A; //  CORRECTION_D; //CORRECTION_TYPE 		[0..3]
	PlaySession.PHASE_CORRECTION= 	ON;		 	// Phase-Correction 	0/1	
	PlaySession_SMPBLOCK_SIZE();	
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/
void DSPSTRING::SR_Update(int SAMPLE_RATE)
{
	int offset= 0, tes;
	 
	if ( (SAMPLE_RATE & 0xF00) > 0)
	{	offset = 4; } 
	String[SR_2CHAR] = ( (SAMPLE_RATE>>(4+offset) ) & 0x00F) + '0' ; 
	String[SR_2CHAR+1] = ((SAMPLE_RATE>>(0+offset)) & 0xF)+'0';  
	if (offset > 0) 
		String[SR_2CHAR+1] += (SAMPLE_RATE & 0xF000)>>12; 	
}  


void DSPSTRING::SPK_IPL_mapping(char *IPL_variable, int *FIR_CH, char IPL_VAL)
{
	//if (String[IPL_EXT]>'Z')
	if (IPL_VAL>'Z')
	{
		*IPL_variable	= 	IPL_VAL-32;
		*FIR_CH			= 0;
	}
	else
	{
		*IPL_variable	= 	IPL_VAL;
		*FIR_CH			= 1;
	}	
}

void DSPSTRING::Apply_Default_String(void)
{
	int		msg[6], k;
	
	for (k=0; k<DSPSTRING_LEN+1; k++)
		String[k] = DSPSTRING_default[k];	
	
	//PlaySession::ACOUSTIC_CORR = 	String[DAC_CODAC_MODE_0]<'a';
    //if (String[DAC_CODAC_MODE_0] == 'd')    
    //    String[DAC_CODAC_MODE_0] = 	'D';
    //else
    //    String[DAC_CODAC_MODE_0] =  'K';
    
    //PlaySession::DEV_MODE		= String[DAC_CODAC_MODE_0]; 

	//SMTwt.xo_iplx 	=   			PlaySession::OVS_CH; 	//PlaySession.DAC_SR;
	RSMP[TWT].xo_iplx 	=   			RSMP[TWT].OVS_CH; 
	//SMTwt.ext_iplx 	=   			1; 						//PlaySession.OVS_EXT; // 01-MAJ-17(PlaySession.OVS_RATIO_* 	SMTwt.dwn_ratio)			/ S->txo_iplx;
	
        	 	
//	PlaySession::PHASE_CORRECTION= 	ON;		 	// Phase-Correction 	0/1	
	PlaySession::CORRECTION_TYPE = CORRECTION_A;  //  CORRECTION_B;   //     	CORRECTION_C;   // //  A -> DAC, B -> CODAC 
		
	Apply_Current_String(); 
		
}


void DSPSTRING::Apply_Current_String(void)
{
	int		msg[6], k;

	if (SOFT_V == FP_FORD)
		PlaySession::DEV_MODE = xSPK_DEV; 
	else 
    	PlaySession::DEV_MODE	=  String[DAC_CODAC_MODE_0] <='Z' ? xCODAC : xDAC; 
//	SPK_IPL_mapping(&SMBas.SPK_XO_IPL_TYPE, String[IPL_BAS]); 
//	if (PlaySession::DEV_MODE != 'S')  // MID pelni role OVS EXT w CO/DAC
//	{	SPK_IPL_mapping(&SMMid.SPK_XO_IPL_TYPE, String[IPL_EXT]); }
//	SPK_IPL_mapping(&SMTwt.SPK_XO_IPL_TYPE, String[IPL_TWT]);

	//SPK_IPL_mapping(&RSMP[BAS].SPK_XO_IPL_TYPE, &RSMP[BAS].FIR_CH, String[IPL_BAS]); 
  //###########  TEMPORARY - FOR CORRECTION ###################
  
	//SPK_IPL_mapping(&RSMP[BAS].SPK_XO_IPL_TYPE, &RSMP[BAS].FIR_CH, 'S'); 
	if (FORD_LOW_RSMP == 1)	
		RSMP[BAS].SPK_XO_IPL_TYPE = 'J'; 
	else	
		RSMP[BAS].SPK_XO_IPL_TYPE = 'S';  
	
//	if (PlaySession::DEV_MODE != 'S')  // MID pelni role OVS EXT w CO/DAC
//	{	SPK_IPL_mapping(&RSMP[MID].SPK_XO_IPL_TYPE, String[IPL_EXT]); }
	//SPK_IPL_mapping(&RSMP[MID].SPK_XO_IPL_TYPE, &RSMP[MID].FIR_CH, String[IPL_MID]); 
	RSMP[MID].SPK_XO_IPL_TYPE = (String[IPL_MID]>='a') ? String[IPL_MID]-32: String[IPL_MID];
	//SPK_IPL_mapping(&RSMP[TWT].SPK_XO_IPL_TYPE, &RSMP[TWT].FIR_CH, String[IPL_TWT]);
	RSMP[TWT].SPK_XO_IPL_TYPE = (String[IPL_TWT]>='a') ? String[IPL_TWT]-32: String[IPL_TWT];
	RSMP[PRE].SPK_XO_IPL_TYPE = RSMP[TWT].SPK_XO_IPL_TYPE;
	//SPK_IPL_mapping(RESAMPLER::EXT_IPL_TYPE, String[IPL_EXT]); 
	
		                
    //PlaySession::FR20k_EQ    =      	String[FR20k_EQ_LEVEL]-48;                                        
	RESAMPLER::FIR_EXT      =   	String[IPL_EXT]<='a';
	//RSMP[TWT].FIR_CH      	=   	String[IPL_TWT]<='a';

    if (String[IPL_EXT] >= 'a')
    {	POLY::EXT_IPL_TYPE =	String[IPL_EXT]- 32; }
    else 
    {	POLY::EXT_IPL_TYPE =		String[IPL_EXT]; }
        
	POLY::OVS_EXT = 	String[DSPSTRING_LEN]-48;

}

