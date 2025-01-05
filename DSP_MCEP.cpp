#include 	"tt.h"
#include 	<stdio.h>
	
#include 	"hPlaySession.hpp"
#include 	"hRESAMPLER.hpp"
#include 	"hDSPSTRING.hpp"
#include 	"hMODEC.hpp"
#include 	"hSR_PROC.hpp"
#include 	"hDSP_MANAGER.hpp"


extern 		RESAMPLER		RSMP[RSMPOBJ_QTY]; 
extern 		FILTER			FILT[FILTOBJ_QTY]; 

#include 	".\__MCEP_include\MALS_.h"
#include 	".\__MCEP_include\MALF_.h"


#if UART_TYPE == UART_OLD    	  	
// NULL  
#elif  UART_TYPE == UART_NEW
	#include 	"FSections_CORE.h"
#endif	

#if (SOFT_V == LINUX_HW) 
	#include 	"FSections_RPI.h"
#endif
//==================================================================
// ----------  E X T E R N S  UZYWNAE PRZEZ MODUL    -----------
//==================================================================


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===-----------    C L A S S  ===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



#define 	MCEP_MAXTABSIZE	12

#define 	MCA_MAXTABSIZE	6

#define 	MOV_MAXTABSIZE	10

 #include 	"hDSP_MCEP.hpp"
 #include 	"_MCEP_Config.h"

											   											   
//SECTION(("seg_sdram2"))	MCEP_T		MCEP[MCEP_TABSIZE];

#include ".\__MCEP_include\MCEP_SH.h"

// SECTION  ("seg_dmda2")    float pm 		

SECTION ("seg_pmda") 	float 	pm	MCB_CF[MCB_MAXSECT*4+2], MCT_CF[MCT_MAXSECT*4+2]; 
SECTION ("seg_pmda") 	float 	pm	MCA_CF[MCA_MAXSECT*4+2], MAL_CF[MAL_MAXSECT*4+2];  	
SECTION  ("seg_dmda2")  float  	MCB_L[MCB_MAXSECT*2+ 1], MCB_R[MCB_MAXSECT*2+ 1], MCT_L[MCT_MAXSECT*2+ 1], MCT_R[MCT_MAXSECT*2+ 1], MCA_L[MCA_MAXSECT*2+ 1], MCA_R[MCA_MAXSECT*2+ 1]; 
SECTION  ("seg_dmda2")  float  	MAL_L[MAL_MAXSECT*2+ 1], MAL_R[MAL_MAXSECT*2+ 1]; 	
						int		MCEP::SpkNum;										   
//SECTION  ("seg_dmda2")	float dm 		MCB_SECT, MCT_SECT, MCA_SECT;

#if SOFT_V & SH_BASE 
#define		EP_MAXSECT		20	
SECTION ("seg_pmda") 	float 	pm	EP_CF[EP_MAXSECT*4+2]; 
SECTION  ("seg_dmda2") 	float  	EP_L[MCB_MAXSECT*2+ 1], EP_R[MCB_MAXSECT*2+ 1]; 
SECTION  ("seg_dmda2") 	float  	EP_SECT;
#endif


//  static	char		IPL_TAB[5]; 


int	 const	delay_TWT[] = {133,141,133,139,129,136};   			


void MCEP::Index(char *cSPK)
{
	int j, k;

	for(j=0; j< sizeof(TME)/sizeof(TME[0]); j++)
	{ 		
		for(k=0; k< SPKCODE_LEN; k++)
		{	if (TME[j].spkcode[k] != cSPK[k])
				break;	
			DSPSTRING::String[k] = cSPK[k];
		}
		if (k== SPKCODE_LEN)  ////!TME.spkcode[k] && cSPK[k])
			break;
	}
	SpkNum = j;	
}


void MCEP::Init(void)
{	
		
	FILT[BAS].StageInit( 	IIR, 	1, 	MCB_CF,  MCB_L, MCB_R, MCB_CF[0],  	3,  	1,  0, 								"MCB_TAB");  
	FILT[BAS].F_ACTIVE = ON_STATE;
	 							  		
	FILT[TWT].StageInit( 	IIR, 	2, MCT_CF,  MCT_L, MCT_R, MCT_CF[0], 	'C',  	1,  0, 						"MCT_TAB");   
	FILT[TWT].F_ACTIVE = ON_STATE;

#if SOFT_V & SH_BASE 				
	RSMP[PRE].StageInit( 	IIR, 	0, EP_CF,   EP_L,  EP_R,  EP_CF[0], 	'P',  	1,  ((4110+2560+1550) <<16 ) | 4110, "EP0_TAB");   							  				
	RSMP[PRE].StageInit( 	IIR, 	1, &EP_CF[0],EP_L, EP_R,  EP_CF[0], 	'E',  	1,  ((2560+1550) <<16) | (2560+1550),"EP1_TAB");   							  				
#endif 
	
   // MAL - MC Antialias  	
	FILT[MID].StageInit( 	IIR, 	1, MAL_CF,  MAL_L, MAL_R, MAL_CF[0], 				'I', 	1,  0, 					"MAL_TAB");
	FILT[MID].F_ACTIVE = ON_STATE;

  //- Constant filters 
  	
	   
//*	
 			

//#########  MODEC stage   #############   	

 	FILT[BAS].StageInit(FMODEC, 2, R0B44A, R0B44A_state, 	R0B44A_state, 		0,		  'M', DWN_1, 0,  	"MODC_ROB44");   				
	FILT[BAS].F_ACTIVE = ON_STATE;

 #if ( SOFT_V == SH_DSP )	
//#########  OVB stage   #############   
	RSMP[BAS].StageInit(		IIR, 0,  OVB19B, OVB19B_state, 	OXB19B_state, 	OVB19B[0],'X', 1, 141,  "XOB_OVB19");   	
//#########  OVT stage   #############   			
 	RSMP[TWT].StageInit(IIR, 	1, OVT19B, OVT19B_state, 	DXB96B_state, 		OVT19B[0],'D', DWN_1, delay_TWT[SR_PROC::SR_ID],  "OVT19B");  				
 #endif	

 	
//*/ 	

	if (FORD_LOW_RSMP == 1)	
		RSMP[BAS].SPK_XO_IPL_TYPE = 'J'; 
	else	
		RSMP[BAS].SPK_XO_IPL_TYPE = IPL_TAB[BAS];  
	
	RSMP[MID].SPK_XO_IPL_TYPE = IPL_TAB[MID];
	
	RSMP[TWT].SPK_XO_IPL_TYPE = IPL_TAB[TWT];
	RSMP[PRE].SPK_XO_IPL_TYPE = RSMP[TWT].SPK_XO_IPL_TYPE;
	
	PlaySession::DEV_MODE	=  xCODAC; 	
	
}

	//int k12; 
									   
void MCEP::Config(void)
{
	//int SpkNum= Index("Ala"), 
//	DSP_TAB::StageInfo_Clr();	
	FILTER::clear_stages_FR();
		
	int k;
	
	for (k=0; k< TME[SpkNum].MCB[SR_44_48][0]*4+ 2; k++)
		MCB_CF[k]= TME[SpkNum].MCB[SR_44_48][k];
	for (k=0; k< TME[SpkNum].MCT[SR_PROC::SR_ID][0]*4+ 2; k++)
		MCT_CF[k]= TME[SpkNum].MCT[SR_PROC::SR_ID][k];
#if SOFT_V & SH_BASE 		
	for (k=0; k< TME[SpkNum].EPS[SR_PROC::SR_ID][0]*4+ 2; k++)
		EP_CF[k]= TME[SpkNum].EPS[SR_PROC::SR_ID][k];
#endif
		
	for (k=0; k< TMALS[SR_PROC::SR_ID][0]*4+ 2; k++)
		MAL_CF[k]= TMALS[SR_PROC::SR_ID][k];
		
	switch(SR_PROC::DAC_SR)
	{
		case 1:
 			FILT[BAS].StageInit(DOSI, 0, DYB88B, DYB88B_state, 	DXB88B_state, 	DYB88B[0],'D', 4, 80+13,  "DN4x_DYB88");  
		  break;			
		case 2:
 			FILT[BAS].StageInit(DOSI, 0, DYB96B, DYB96B_state, 	DXB96B_state, 	DYB96B[0],'D', 8, 80,  "DN4x_DYB88");   				
		  break;			
		case 4:
 			FILT[BAS].StageInit(DOSI, 0, DYB17B, DYB17B_state, 	DXB17B_state, 	DYB17B[0],'D', 16, 80,  "DN4x_DYB88");   				
		  break;			
	};
	FILT[BAS].F_ACTIVE = ON_STATE;
	
	MCEP::Init();
		
//	for (k=0; k< MCA[SR_44_48][0]*4+ 2; k++)
//		MCA_CF[k]= MCA[SR_44_48][2+k];
}

int	   zz; 

#pragma optimize_off
void MCEP::Stage_Switches(void)  
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
		  			zz= (ON_OFF) ( (FlagsDSP & 0xFF)> 0)   ;
					if(F->stages[k].param_code =='M')   	//F->stages[k].stagefun == FMODEC || )
						F->stages[k].ACTIVE = (ON_OFF) ( (FlagsDSP & 0xFF)> 0)  ; 
		  		//= BAS MC Level - activate the 		
					if( (F->SPK_CODE== BAS) && F->stages[k].param_code >= 1 && F->stages[k].param_code <= 3)
					{
						if (1) // F->stages[k].param_code ==  BAS_CORR_LEVEL_GET)  				
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
						F->stages[k].ACTIVE = (ON_OFF) ( (FlagsDSP & (0xFF<<MC_HEX)) >0) ;				
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
