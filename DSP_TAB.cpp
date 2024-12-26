#include 	"tt.h"
#include 	<stdio.h>
	
#include 	"hPlaySession.hpp"
#include 	"hRESAMPLER.hpp"
#include 	"hDSPSTRING.hpp"
#include 	"hMODEC.hpp"
#include 	"hSR_PROC.hpp"
#include 	"hDSP_MANAGER.hpp"

#include 	"hDSP_TAB.hpp"

extern 		RESAMPLER		RSMP[RSMPOBJ_QTY]; 
extern 		FILTER			FILT[FILTOBJ_QTY]; 

#include 	"FT_Config.h"
 
#define		xMCLIN_TWT		'4'


//==================================================================
// ----------  E X T E R N S  UZYWNAE PRZEZ MODUL    -----------
//==================================================================

extern	int		MODCFG_CH;	

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===-----------    C L A S S  ===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


	int					DSP_TAB::k_dsp; 
SECTION(("seg_sdram3"))	char				DSP_TAB::stages[12][DSPTEXT_LEN+DSPSTRING_LEN+1];

#if UART_TYPE == UART_OLD 
struct S_TABS_size		DSP_TAB::S_TABS[7]= {	S_BAS, sizeof(S_BAS)/sizeof(S_BAS[0]), 		S_MID, sizeof(S_MID)/sizeof(S_MID[0]), 	S_TWT, sizeof(S_TWT)/sizeof(S_TWT[0]),S_PRE, sizeof(S_PRE)/sizeof(S_PRE[0]) 
											   ,S_MCBAS, sizeof(S_MCBAS)/sizeof(S_MCBAS[0]), S_MCMID, sizeof(S_MCMID)/sizeof(S_MCMID[0]), S_MCTWT, sizeof(S_MCTWT)/sizeof(S_MCTWT[0]) };
#elif  UART_TYPE == UART_NEW
struct S_TABS_size		DSP_TAB::S_TABS[7]; // = {	S_BAS, sizeof(S_BAS)/sizeof(S_BAS[0]), 		S_MID, sizeof(S_MID)/sizeof(S_MID[0]), 	S_TWT, sizeof(S_TWT)/sizeof(S_TWT[0]),S_PRE, sizeof(S_PRE)/sizeof(S_PRE[0]) 
											//   ,S_MCBAS, sizeof(S_MCBAS)/sizeof(S_MCBAS[0]), S_MCMID, sizeof(S_MCMID)/sizeof(S_MCMID[0]), S_MCTWT, sizeof(S_MCTWT)/sizeof(S_MCTWT[0]) };
#endif			   
int r= 0; 

void DSP_TAB::Pick_Stages(S_TABS_size *STAB, char *DSP_STRING)  
{
	int 	kT_PRE_FIRST,  k=0, kc, ks, kd1, kT, dummy, stageon_flag, t, pdigit, tdigit, digit, post_RSMP=0;
	DSP_Steps_TAB		*TAB;

		
		TAB = STAB->tab; 	
		for(k=0; k< STAB->size; k++)
		{		
			if (k==4)
				r = 1;		
			stageon_flag= 1;
			for(kc=0;kc< DSPSTRING_LEN; kc++)
			{
				kd1 = 	L_PRE_BAS(DSP_STRING[kc], &TAB[k], kc) ;
				kd1 = 	L_88_96(DSP_STRING[kc], &TAB[k], kc) ;
				kd1 = 	x44_x48(TAB[k].dspstr[kc], kc) ;
						
				t= (stages[k_dsp-1][1]-48)*10+ stages[k_dsp-1][2]-48; // Wyliczenie liczbowego numeru ostatniego stage, zeby nie wziac dwa razy tego samego typu stage
				if
				(  (   // Exclude when none of the three &&-ed conditions is true
					    (DSP_STRING[kc] != TAB[k].dspstr[kc] ) // No exact match
					  && 
						( TAB[k].dspstr[kc] != '*' )            // * No global wildcard
				 	  && 
				 	    (   ( (DSP_STRING[kc]<= 'Z') &&  (DSP_STRING[kc]>= 'A') &&  (TAB[k].dspstr[kc] != ':') ) // No wildcard for any Uppercase letter
					     || ( (DSP_STRING[kc]>= 'a') &&  (TAB[k].dspstr[kc] != '.') )
					     || ( DSP_STRING[kc]< 'A' ) // Without it digits will be a loophole for handling '.' and ':'
					     ||  ( DSP_STRING[kc]== '_' ) // Underscore is not low ASCII, it is between lower and upper case
				   	    )
				   	  && L_PRE_BAS(DSP_STRING[kc], &TAB[k], kc)  
				   	  &&   L_88_96(DSP_STRING[kc], &TAB[k], kc)  		
				   	  &&   x44_x48(TAB[k].dspstr[kc], kc)  		
				   )	// Or if there is already a stage with this Stage_No			
				   //|| ( t== TAB[k].Stage_No && stages[k_dsp-1][0]==TAB[k].DSP_text[0]) // Stage with the same number already used				
				) 
				{
					stageon_flag = 0;
					break;
				}
			}  //= for kc in DSPSTRING
			if (stageon_flag)
			{
				TAB[k].FilterObj->StageInit(TAB[k].stagefun, TAB[k].Stage_No, TAB[k].pcoeffs, TAB[k].Lstate, TAB[k].Rstate, TAB[k].sect_taps_qty, TAB[k].param_code,  TAB[k].down_ratio, TAB[k].delay, TAB[k].DSP_text); //TAB[k].txt2[0], TAB[k].contextpar1); 							  
				++DSP_TAB::k_dsp; 
				//Filterstep_Desc(TAB[k].DSP_text, TAB[k].dspstr, TAB[k].FilterObj);
			   //= If FILT_STAGE mark active so that RSMP size is initialized from FILT, otherwise standalone
				if(TAB[k].FilterObj->DSP_TYPE == DSP_FILT)
					TAB[k].FilterObj->F_ACTIVE = ON_STATE;
			}			
		} // k for lines within S_TAB
}
	

void DSP_TAB::Stages_Config(void)  
{
	int 	kT_PRE_FIRST,  k=0, kc, ks, kd1, kT, dummy, stageon_flag, t, pdigit, tdigit, digit, post_RSMP=0;
	char	DSP_STRING[DSPSTRING_LEN+1];
		
	DSP_TAB::StageInfo_Clr();	
	FILTER::clear_stages_FR();

  //= Stage TABs with core functions need to differentiate between D/K mode coded on the upper/lower case of the 1-st DSPSTRING char
	for(k=0;k<DSPSTRING_LEN+1;k++)
		DSP_STRING[k]= DSPSTRING::String[k];
  //= Include all possible stage combinations coded as hex values (0xF is a full combination)
	DSP_STRING[MC_BAS_MT] = 'F';	
	DSP_STRING[PRE_EXPH_MOD] = 'F'; DSP_STRING[FR20k_EQ_LEVEL] = 'F';
  //= Process general MCOVS, XO, MODEC stages in S_BAS, S_MID, S_TWT, S_PRE  
  //= It also includes S_MCBAS since the differentiation between DAC/CODAC allows to exclude bas MC for DAC mode (requires first letter lower/upper case info)
	for(kT=BAS; kT<= PRE+1; kT++)
	{
		kT_PRE_FIRST = (kT+3)%5;
		Pick_Stages(&S_TABS[kT_PRE_FIRST], DSP_STRING) ; //, DSP_STRING);  
	}
  //= Stage TABs with MCB/P functions need to have upper case 1-st DSPSTRING char to include corrections		
  
/*	for(k=0;k<DSPSTRING_LEN+1;k++)
	{
		if (DSPSTRING::String[k] >='a')
			DSP_STRING[k]= DSPSTRING::String[k]-32;  
		else 
			DSP_STRING[k]= DSPSTRING::String[k];
	}	
*/	
	for(k=0;k<DSPSTRING_LEN+1;k++)
		DSP_STRING[k]= DSPSTRING::String[k];
 //= First char of SPKCODE by strict convention should always be UPPER CASE as it needs to be left to the app to use/change it to implement DAC/CODAC mode  		
	if (DSPSTRING::String[0] >='a')
		DSP_STRING[0]= DSPSTRING::String[0]-32;  		

  //= Include all possible stage combinations coded as hex values (0xF is a full combination)
	DSP_STRING[MC_BAS_MT] = 'F';	
	DSP_STRING[PRE_EXPH_MOD] = 'F'; DSP_STRING[FR20k_EQ_LEVEL] = 'F';
  //= Process speaker-specific MCP & MCB stages in S_MCMID, S_MCTWT (without S_MCBAS processed earlier) 
	for(; kT<= PRE+3; kT++)
		Pick_Stages(&S_TABS[kT], DSP_STRING) ; //, DSP_STRING);  
		
  //= After including all possible (also optional/switchable) stages switch on/off as applicable
	//Stage_Switches();
/*	
	if (DSPSTRING::Get_Digitbit(DSPSTR_MCMT))
	{
		FILT[BAS].FilterObj->StageInit(IIR, 1, &MC[k][k44_48][2], 	MCB_L, MCB_R, MC[k][k44_48][0], 3,  1, 0, "MCB"); //TAB[k].txt2[0], TAB[k].contextpar1); 							  
		FILT[TWT].FilterObj->StageInit(IIR, 2, &MC[k][kSR][2], 		MCT_L, MCT_R, MC[k][kSR][0],   'C',	1, 0, "MCT"); //TAB[k].txt2[0], TAB[k].contextpar1); 							  	
	}
	if (DSPSTRING::Get_Digitbit(DSPSTR_E\XPH))
	{
		RSMP[PRE].FilterObj->StageInit(IIR, 0, &EP[k][kSR][2], 		EP_L, EP_R, EP[k][kSR][0],   'P',	1, ((4110+2560+1550) <<16 ) | 4110, "EP0");  							  	
		RSMP[PRE].FilterObj->StageInit(IIR, 1, &EP[k][kSR][2], 		EP_L, EP_R, EP[k][kSR][0],   'P',	1, ((2560+1550) <<16) | (2560+1550), "EP");  							  			
	}
*/	
}

#pragma optimize_off
int DSP_TAB::Stage_Switches(void)  
{
	int			k, kf, tmp, stage_qty; //, new_stage_config=0;
	ON_OFF 		new_twt_ovs_fir;
	FILTER		*F;
	 
	FILTER::clear_state_FR();

	for(kf=0; kf< DSP_BLOCK_QTY; kf++)
	{		
		F= DSP_MANAGER::Streams[kf][LEFT_0]->FiltRsmp; 
		if (F->DSP_TYPE == DSP_FILT)
			stage_qty = FILTER::STAGE_QTY;
		else 
			stage_qty = RESAMPLER::STAGE_QTY;		
		for(k=0; k< stage_qty; k++)
		{
			if (F->stages[k].param_code == 'E')
				F->stages[k].ACTIVE = (ON_OFF) (DSPSTRING::Get_Digitbit(DSPSTR_EXPH) >0);		  						
			switch( F->SPK_CODE )
			{
				case PRE:
		  		//= EXPH filters	
//					if ( F->stages[k].param_code == 'E')
//						F->stages[k].ACTIVE = (ON_OFF) (DSPSTRING::Get_Digitbit(DSPSTR_EXPH) >0);		  		
/*					
{
						
						tmp = (ON_OFF) (DSPSTRING::Get_Digitbit(DSPSTR_EXPH) >0);
						if 	(F->stages[k].ACTIVE !=  tmp)
						{
							F->stages[k].ACTIVE = (ON_OFF) (DSPSTRING::Get_Digitbit(DSPSTR_EXPH) >0);
							++new_stage_config;
						}
					}
				*/
			      break; 
				case BAS:
		  		//= Acoustic Correction	
					if(F->stages[k].param_code =='M')   	//F->stages[k].stagefun == FMODEC || )
						F->stages[k].ACTIVE = (ON_OFF) (DSPSTRING::Get_Digitbit(DSPSTR_MODEC) >0);
		  		//= BAS MC Level - activate the 		
					if( (F->SPK_CODE== BAS) && F->stages[k].param_code >= 1 && F->stages[k].param_code <= 3)
					{
						if ( F->stages[k].param_code ==  BAS_CORR_LEVEL_GET)  				
				 			F->stages[k].ACTIVE = ON_STATE;
						else
				 			F->stages[k].ACTIVE = OFF_STATE;
					}		
					//if( (F->SPK_CODE== BAS) && F->stages[k].param_code == 'D')
					//{
					//	if ( BAS_CORR_LEVEL_GET >0 )  				
				 	//		F->stages[k].ACTIVE = ON_STATE;
					//	else
				 	//		F->stages[k].ACTIVE = OFF_STATE;
					//}															
				  break;
				case TWT:
		 		//= TWT Mag Correction
					if(F->stages[k].param_code == 'C')
						F->stages[k].ACTIVE = (ON_OFF) (DSPSTRING::Get_Digitbit(DSPSTR_MCMT) >0);				
		 		//= TWT OVS MagCorrection Level - identified by absolute number 1 or 2 in param code
					if( F->stages[k].param_code >= 1 && F->stages[k].param_code <= 2)
					{
						if ( F->stages[k].param_code ==  OVSFILT_EQ_LEVEL_GET)  				
				 				F->stages[k].ACTIVE = ON_STATE;
						else
				 				F->stages[k].ACTIVE = OFF_STATE;
					}
		 		//= TWT OVS FIR 
		 	/*			 			
					if( F->stages[k].param_code == 'F' || F->stages[k].param_code == 'G')
					{
						F->stages[k].ACTIVE = (ON_OFF) ((int) DSPSTRING::String[IPL_TWT]<='Z');
						//new_twt_ovs_fir = (ON_OFF) ((int) DSPSTRING::String[IPL_TWT]<='Z');
					  //= If it is CODAC not DAC and XO filters are involved then delay need to be changed and a full stage reconfig is required
					  //= REFACTOR to be considered - maybe for simplicity (also Fullcodac.h config simplification) in botdh D & K cases can be  full stage reconfig 
						//if (F->stages[k].param_code == 'G' && F->stages[k].ACTIVE != new_twt_ovs_fir)
						//		new_stage_config = 1;
						// F->stages[k].ACTIVE = new_twt_ovs_fir;
					}												
			*/						
				  break;
			}			
			
			if (F->stages[k].stagefun)
				F->stages[k].DSPTAB_desc[2] = (F->stages[k].ACTIVE) ? STAGE_ON_MARK : STAGE_OFF_MARK; 	
				
			F->Vol(stage_qty);	
			
		}  // for stage_qty
	}
	//return (new_stage_config);	
	FILT[TWT].F_ACTIVE = ON_STATE;   	

}

void DSP_TAB::StageInfo_Clr(void)
{ 
	int k, m;

	for(k=0;k<k_dsp;k++)
		for (m=0; m<DSPTEXT_LEN; m++)		
			stages[k][m]=0;
	k_dsp = 0;

}


#define 	xSTAGE_ON		0
#define 	xSTAGE_OFF		1

int DSP_TAB::bit_digit(char *p_sdigit, char *p_tdigit)
{

	if (*p_sdigit>='0' && *p_sdigit<='9') 
		*p_sdigit-= '0';
	else if (*p_sdigit>='A' && *p_sdigit<='F')
		*p_sdigit= *p_sdigit-'A'+10;	
	else	return  xSTAGE_OFF; 
	if (*p_tdigit>='0' && *p_tdigit<='9') 
		*p_tdigit-= '0';
	else if (*p_tdigit>='A' && *p_tdigit<='F')
		*p_tdigit= *p_tdigit-'A'+10;	
	else	return  xSTAGE_OFF;
	
	return xSTAGE_ON;
}

int DSP_TAB::L_88_96(char sdigit, DSP_Steps_TAB	*TAB, int kc)
{
	char tdigit= TAB->dspstr[kc];

	if (kc<DSPSTRING_LEN-1)
	{
		if (bit_digit(&sdigit, &tdigit) == xSTAGE_OFF)
			return xSTAGE_OFF;
	 //= only for 88 and 96 - check if the first digit after nulling Bit0 is 8 with '*' following 						
		if ( (sdigit & 0xFE) == tdigit && (TAB->dspstr[kc+1]=='*') )
			return xSTAGE_ON;
		else  			
			return xSTAGE_OFF;
	}
	else 	return xSTAGE_OFF;
}

int DSP_TAB::L_PRE_BAS(char sdigit, DSP_Steps_TAB	*TAB, int kc)
{
	char tdigit= TAB->dspstr[kc];
	
	if (kc<DSPSTRING_LEN-1)
	{
		if (bit_digit(&sdigit, &tdigit) == xSTAGE_OFF)
			return xSTAGE_OFF;
	 //= only for PRE & BAS - check for bit combination 1 - PRE and 2 - BAS and the _ following						
		if ( (sdigit & tdigit) == tdigit && TAB->dspstr[kc+1]=='_' )
			return xSTAGE_ON;
		else  			
			return xSTAGE_OFF;
	}
	else 	return xSTAGE_OFF;
}

int DSP_TAB::x44_x48(char tdigit, int kc)
{
	
	if (kc== SR_2CHAR)
	{
		if ( (tdigit=='-' && TEST_SR_44K_DOMAIN) || (tdigit=='=' && !TEST_SR_44K_DOMAIN)) 
			return xSTAGE_ON;
		else  			
			return xSTAGE_OFF;
	}
}


