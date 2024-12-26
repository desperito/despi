#include 	"tt.h"

#include 	<time.h>
#define  	SRUDEBUG  // Check SRU Routings for errors.

#include 	<sru.h>
#include 	<math.h>
#include 	<stdio.h>



#include  	"hPlaySession.hpp"
#include  	"hSR_MANAGER.hpp"
#include  	"hSR_PROC.hpp"
#include  	"hDSP_MANAGER.hpp"
#include  	"hPOLY.hpp"
#include  	"hDSPSTRING.hpp"


extern		FILTER		FILT[FILTOBJ_QTY];

//extern SECTION(("seg_sdram2")) 	float	cpoly[32] = {1/8, 2/8, 3/8, 4/8, 5/8, 6/8, 7/8};
extern SECTION(("seg_dmda"))	 	struct interpol 	 	BS; 
extern SECTION(("seg_dmda"))	 	struct interpol 	 	Lag;
extern SECTION(("seg_dmda")) 		struct interpol 	 	He;
extern SECTION(("seg_dmda")) 		struct interpol 	 	Osc;

#define 	RSMP_FILT_QTY			3

//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================
#include 	"hRESAMPLER.hpp"

//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================


//struct filterstage		LPF[RSMPOBJ_QTY][RSMP_FILT_QTY];	

#define 	X44				SMPBLOCK_SIZE44*RAMSEG_QTY
#define 	X48				SMPBLOCK_SIZE*RAMSEG_QTY 

#define		dCD   			4

//#include 		"C:\Temp\__2022\linipl_d1.h"


//#include 	"linipl.h"
//#include 	"linipl_d3.h"
//#include 	"linipl_d20.h"

section ("seg_sdram4") 	float 		EY[2][DSPBLOCK_SIZE2]; 	 	 	

section ("seg_sdram4") 	struct rsmp_coeffs	 	 	Lin_IPL[CLK_BASE_QTY][OVS_LEVEL_QTY][RSMP_LEVEL_QTY]; 
//,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000 };
//section ("seg_sdram3") 	struct rsmp_coeffs	 	 	Lin_IPL[CLK_BASE_QTY][SR_LEVEL_QTY][RSMP_LEVEL_QTY];
section ("seg_sdram3") 	struct rsmp_coeffs	 	 	R_IPLTab[IPL_QTY+1][CLK_BASE_QTY][OVS_LEVEL_QTY][RSMP_LEVEL_QTY];

	int		RESAMPLER::X_QTY[CLK_BASE_QTY][OVS_LEVEL_QTY][RSMP_LEVEL_QTY] = {  
																				{	{X44/4-dCD/4, X44/4, X44/4+dCD/4}, {X44/2-dCD/2, X44/2, X44/2+dCD/2},
																					{X44-dCD, X44, X44+dCD}, {2*X44-2*dCD, 2*X44, 2*X44+2*dCD}, {4*X44-4*dCD, 4*X44, 4*X44+4*dCD}
																				},
																				{	{X48/4-dCD/4, X48/4, X48/4+dCD/4},	{X48/2-dCD/2, X48/2, X48/4+dCD/2},
																					{X48-dCD, X48, X48+dCD}, {2*X48-2*dCD, 2*X48, 2*X48+2*dCD}, {4*X48-4*dCD, 4*X48, 4*X48+4*dCD}
																				}
																	 		}; 	

		
	RESAMPLER		RSMP[RSMPOBJ_QTY];  //Resampling objects - one per each physical channel
	
//	rsmp_coeffs		(*RESAMPLER::IPL)[OVS_LEVEL_QTY][RSMP_LEVEL_QTY]; // = Lin_IPL; 
																//[0][0],&TLin[0][1],&TLin[0][2],  	// Dla domeny 44k 
												 				 	 	//&TLin[1][0],&TLin[1][1],&TLin[1][2]}; 	// Dla domeny 48k

	int				RESAMPLER::Level, RESAMPLER::MAX_LEVEL, RESAMPLER::SAVE_SIZE_R;
	int				RESAMPLER::FIR_EXT;
//	int				RESAMPLER::OVS_TOTAL;
	enum  ON_OFF	RESAMPLER::ACTIVE;
	int				RESAMPLER::STAGE_QTY, RESAMPLER::OBJ_QTY;
	
void RESAMPLER::Constructor(void)
{
	int kSpk;

	RESAMPLER::STAGE_QTY 	= sizeof(FILT[kSpk].stages)/sizeof(FILT[kSpk].stages[0]);
	RESAMPLER::OBJ_QTY		= RSMPOBJ_QTY; 
	RESAMPLER::SAVE_SIZE_R 	= DSPBLOCK_SIZE2;
	RESAMPLER::MAX_LEVEL 	= RSMP_LEVEL_QTY-1;
		
	for(kSpk=0; kSpk< RESAMPLER::OBJ_QTY; kSpk++) 
	{	
		//RSMP[kSpk].stages = 	(filterstage *) &LPF[kSpk]; 
		RSMP[kSpk].SPK_CODE = 	kSpk;
		RSMP[kSpk].DSP_TYPE = 	DSP_RSMP; 		 		
		RSMP[kSpk].DN_rate= 	1; 		 				
	}
	Make_IPL();	
}

int kshow; 

//void RSMP_EXE(STAGE_PROC *SP)
void RSMP_CREATE_FILT_EXE(STAGE_PROC *SP)
{
	int					k, tmp;
	float				*IN, *OUT;
	RESAMPLER  			*pRSMP; 
struct 	rsmp_coeffs		*T;
	
	pRSMP = (RESAMPLER  *) SP->FiltRsmp;
	kshow = MID_TWT_RSMP_X_QTY; 		
	if (pRSMP->ACTIVE)
	{	
		 #if (BUFSRC_UART_TRACE == ON) 			
			++kshow; kshow%=900;
			if( !kshow)
			{
				dbgtab[0]= pRSMP->Level+48; dbgtab[1]= 0;
				trace("R",0);trace(dbgtab,0);
			}
		 #endif
 		k= SR_44_48;
 		tmp = POLY::Map_Poly(pRSMP->SPK_XO_IPL_TYPE);
 		if (SP->FiltRsmp->SPK_CODE == BAS)
 			T = &(R_IPLTab[POLY::Map_Poly(pRSMP->SPK_XO_IPL_TYPE)][SR_44_48][BAS][pRSMP->Level]);
 		else if (SP->FiltRsmp->SPK_CODE == MID)
 			T = &(R_IPLTab[POLY::Map_Poly(pRSMP->SPK_XO_IPL_TYPE)][SR_44_48][MID][pRSMP->Level]); 			
 		else
 			T = &(R_IPLTab[POLY::Map_Poly(pRSMP->SPK_XO_IPL_TYPE)][SR_44_48][MID_TWT_RSMP_X_QTY][pRSMP->Level]); 		
 			
 		OUT = SP->stage_ptr[(SP->stage_cnt+1)%2];
				 		
		switch (pRSMP->SPK_XO_IPL_TYPE)
 		{	case 'L':
 				IN 	= SP->stage_ptr[SP->stage_cnt]-1;  // One extra preceding sample - the last sample of the previous block 		
 				pRSMP->LIN(SP, T,IN,OUT);
		      break;
 			case 'H': case 'W':  case 'G': case 'B': case 'O':  
 				IN 	= SP->stage_ptr[SP->stage_cnt]-3;  // One extra preceding sample - the last sample of the previous block 		 			
 				pRSMP->POLY4P(SP, T,IN,OUT);
 		  	  break;
/* 			case 'B': case 'G':  				
 				IN 	= SP->stage_ptr[SP->stage_cnt]-5;  // One extra preceding sample - the last sample of the previous block 		 			
 				pRSMP->POLY6P(SP, T,IN,OUT);
 		  	  break; 		  	  
*/ 		  	  
 			case 'I': case 'J': 
 				IN 	= SP->stage_ptr[SP->stage_cnt]-5;   		 			
 				pRSMP->POLY6P(SP, T,IN,OUT);
  		  	  break; 		  	  
 			case 'Z':
 				IN 	= SP->stage_ptr[SP->stage_cnt];   			
 			  	pRSMP->ZEROS(SP, T,IN,OUT); 		
 	      	  break;
 			case 'S':
 				IN 	= SP->stage_ptr[SP->stage_cnt];  
 			  	pRSMP->STEP(SP, T,IN,OUT); 		
 		      break; 	
 		}
	}	
	else	
	{
		printf("OLD poly"); return;
	}
	
	++SP->stage_cnt; SP->stage_cnt%= 2;	
	for(k=0; k< RESAMPLER::STAGE_QTY; k++)
	{	  
		if (pRSMP->stages[k].ACTIVE== ON_STATE)
		{			
			//if (pRSMP->stages[k].param_code !='F' || pRSMP->FIR_CH)     //= Exec additional FIR filtering 
				(*pRSMP->stages[k].stagefun)(SP, k);
		}
	}	
   //= Save resampling results	
	SP->BufD->PUTf(RESAMPLER::SAVE_SIZE_R, SP->stage_ptr[SP->stage_cnt], 1);
	
}


void RSMP_EP_FILT_EXE(STAGE_PROC *SP)
{
	int					k, ky, kyn, stage_post;
	RESAMPLER  			*pRSMP = (RESAMPLER  *) SP->FiltRsmp;
	
	pRSMP->EP_counter = 0;						
	for(k=0; k< RESAMPLER::STAGE_QTY; k++)
	{	  
		if (pRSMP->stages[k].ACTIVE== ON_STATE)
		{					
   			if (DSPSTRING::Get_Digitbit(DSPSTR_EXPH) && (pRSMP->stages[k].param_code =='E' || pRSMP->stages[k].param_code =='P') )
   			{
   				if(!pRSMP->EP_counter++)  // zero init 
   				{
	   				for(ky=0; ky< pRSMP->stages[k].sect_taps_qty;ky++)
	   				{	pRSMP->stages[k].LR_state[LEFT_0][ky]= 0;
	   					pRSMP->stages[k].LR_state[RIGHT_0][ky]= 0;
	   				}
   				}	   		  
	   			SP->BufIN[0]->GETF_EX(SP, k, 1);   						
				(*pRSMP->stages[k].stagefun)(SP, k);   								
				
	  			stage_post = (SP->stage_cnt+1)%2;
/*	  			
				for(ky=0, kyn=EP_SIZE-1; ky< pRSMP->SAVE_SIZE ; ky++, kyn--)		// FILTER::Size_RF() has a special EXPH_case where SAVE_SIZE is assigned to RESAMPLER::X_QTY[kSR][BMT_FILTER_X_QTY][RESAMPLER::Level]
					SP->stage_ptr[stage_post][ky]= SP->stage_ptr[SP->stage_cnt][kyn];			
*/					
				if (pRSMP->stages[k].param_code == 'E') // 'E' is the final EP stage so take the EP results
				{
  				  #if   (EP_TAILMODE == EP_XPRETAIL)			
					for(ky=0, kyn=pRSMP->stages[k].size-1; ky< pRSMP->SAVE_SIZE; ky++, kyn--)	  //pRSMP->stages[k].size											
						SP->stage_ptr[stage_post][ky]= SP->stage_ptr[SP->stage_cnt][kyn];													
  				  #elif (EP_TAILMODE == EP_PRETAIL)	
			       //= Load the main future EP DSPBLOCK_SIZE2 block stored in the EY sdram buffer the prev step in 
				  	for(ky=0; ky< pRSMP->SAVE_SIZE ; ky++)		
						SP->stage_ptr[stage_post][ky]= EY[SP->Chn][ky];			
			       //= Store the main future EP DSPBLOCK_SIZE2 block in the EY sdram buffer since it will be used for the next input block					
				 	//for(ky=0, kyn=pRSMP->SAVE_SIZE+EP_POSTSIZE; ky< pRSMP->SAVE_SIZE ; ky++, kyn--)		
				 	//for(ky=0, kyn=pRSMP->SAVE_SIZE+pRSMP->stages[k].EP_PRESIZE; ky< pRSMP->SAVE_SIZE ; ky++, kyn--)						 
				 	for(ky=0, kyn=pRSMP->stages[k].size-pRSMP->stages[k].EP_PRESIZE; ky< pRSMP->SAVE_SIZE ; ky++, kyn--)						 		
						EY[SP->Chn][ky] = SP->stage_ptr[SP->stage_cnt][kyn];
			       //= Add the tail of EP_PRESIZE from the current EP filter result 										
					//for(ky=pRSMP->SAVE_SIZE-EP_PRESIZE, kyn=pRSMP->SAVE_SIZE+EP_PRESIZE+EP_POSTSIZE; ky< EP_PRESIZE ; ky++, kyn--)
					//for(ky=pRSMP->SAVE_SIZE-pRSMP->stages[k].EP_PRESIZE, kyn=pRSMP->stages[k].size; ky< pRSMP->stages[k].size; ky++, kyn--)	  //pRSMP->stages[k].size
				   #endif						
					++SP->stage_cnt; SP->stage_cnt%=2;
				}		   			
   			}
   			else
   				(*pRSMP->stages[k].stagefun)(SP, k);
		}
	}	
   //= Save resampling results	
	SP->BufD->PUTf(RESAMPLER::SAVE_SIZE_R, SP->stage_ptr[SP->stage_cnt], 1);	
}


	 	
void RESAMPLER::LIN(STAGE_PROC *SP,struct rsmp_coeffs *T, float *IN, float *OUT)
{
	int			k, kX0=-1, YSIZE; 
 	float		IN0, IN1, dIN;
 	double 		RSMP_STEP; 
 	
 	
	IN[0]= SP->tail[2]; 				// One extra sample = the last sample of the previous block		
	
	for(k=0; k< SP->FiltRsmp->SAVE_SIZE; k++)	  
	{			
		kX0 = T->kX0[k];
		if (T->fp1[k]==0)
			OUT[k] = IN[kX0];
		else
		{
			dIN = (IN[kX0+1] - IN[kX0]);
			OUT[k] = IN[kX0]+ T->fp1[k]*dIN;
		}
	}
	SP->tail[2] = IN[kX0+1];  // store the last sample of the block for the next roun		
}

		 	
void RESAMPLER::POLY4P(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT)
{
	int			k, kX0;
	float		tp2, tp1, t_0, tm1; 
 	
 	//T = &(R_IPLTab[POLY::Map_Poly(SPK_XO_IPL_TYPE)][SR_44_48][SR_PROC::DAC_SR/2][Level]); 
		
 	
 	for (k=0; k< xTAIL_POLY4; k++)
		IN[k]= SP->tail[k]; 		
	
	for(k=0; k< SP->FiltRsmp->SAVE_SIZE; k++)	  //	for(k=0, j=1; j<YSIZE+3 -2; k++, j++)	
	{	
		kX0 = T->kX0[k];							
		OUT[k]= 	IN[kX0+3]* T->fp2[k]+ IN[kX0+2]* T->fp1[k]+ IN[kX0+1]* T->f_0[k]+ IN[kX0]* T->fm1[k];						
	}		
 	for (k=0; k< xTAIL_POLY4; k++)
		SP->tail[k]= IN[kX0+k+1]; 				
}

void RESAMPLER::POLY6P(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT)
{
	int			k, kX0;			
 	
 	for (k=0; k< xTAIL_POLY6; k++)
		IN[k]= SP->tail[k]; 		
	
	for(k=0; k< SP->FiltRsmp->SAVE_SIZE; k++)	  //	for(k=0, j=1; j<YSIZE+3 -2; k++, j++)	
	{	
		kX0 = T->kX0[k];	
		OUT[k]= IN[kX0+5]* T->fp3[k]+ IN[kX0+4]* T->fp2[k]+ IN[kX0+3]* T->fp1[k]+ IN[kX0+2]* T->f_0[k]+ IN[kX0+1]* T->fm1[k]+ IN[kX0]* T->fm2[k];						
	}		
 	for (k=0; k< xTAIL_POLY6; k++)
		SP->tail[k]= IN[kX0+k+1]; 				
}



void RESAMPLER::ZEROS(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT)
{
	int		k, kX0, last_kX0=-1, YSIZE, q; 
	 	
	for(k=0; k< SP->FiltRsmp->SAVE_SIZE; k++)	//ovs_level* dwn_ratio)
	{
		if (T->kX0[k]!=last_kX0)
		{
			OUT[k]= IN[T->kX0[k]];
			last_kX0 = T->kX0[k];
		}
		else
			OUT[k]= 0;		
	}
}

void RESAMPLER::STEP(STAGE_PROC *SP, struct rsmp_coeffs	*T, float *IN, float *OUT)
{
	int			k, kX0, last_kX0=-1, YSIZE, q; 
 	
	for(k=0; k< SP->FiltRsmp->SAVE_SIZE; k++)	
	{
//		if (T->fp1[k]>0.5 && k<SP->FiltRsmp->SAVE_SIZE-1)
			OUT[k]= IN[T->kX0[k]];
	}
}

	
void RESAMPLER::Rsmp_Size_Vol_Delay(void)
{	
	int 	csize, k, kSpk, kSR;
	
	if (RESAMPLER::ACTIVE)
		kSR = SR_44_48;
	else  // Setting for (old) non-resampler mode
		kSR = 1;

	// @@@@@@  Needs to be properly set when POLY USED
	//POLY::LOAD_SIZE = X_QTY[kSR][SR_PROC::DAC_SR/2][Level];
	//LevelChg(0); // zeby ustawic LOAD_SIZE - do optymalizcji
	for(kSpk= BAS; kSpk< RESAMPLER::OBJ_QTY; kSpk++)
	{
		// RSMP[kSpk].LOAD_SIZE = X_QTY[kSR][SR_PROC::DAC_SR/2][Level];
		RSMP[kSpk].SAVE_SIZE = RESAMPLER::SAVE_SIZE_R;				
		for(k=0; k< RESAMPLER::STAGE_QTY; k++)
		{			
			if (RSMP[kSpk].stages[k].ACTIVE)
			{
//				if (RSMP[kSpk].stages[k].param_code =='E' ) //|| RSMP[kSpk].stages[k].param_code =='P')
//					RSMP[kSpk].stages[k].size = EP_SIZE2;
					//RSMP[kSpk].stages[k].size = RSMP[kSpk].stages[k].delay;				
//                         				else
				if (RSMP[kSpk].stages[k].param_code !='E' &&  RSMP[kSpk].stages[k].param_code !='P')
					RSMP[kSpk].stages[k].size = RSMP[kSpk].SAVE_SIZE;
			}
		}
	  //=  VOLUME
		RSMP[kSpk].Vol(RESAMPLER::STAGE_QTY);		
	  //= DELAY		
		RSMP[kSpk].R_SpkDelay(RESAMPLER::STAGE_QTY, kSpk);	
		if (RSMP[kSpk].DelayBuf != NULL) 
		{
			RSMP[kSpk].DelayBuf[LEFT_0]->k_IN -= RSMP[kSpk].Delay;
			RSMP[kSpk].DelayBuf[RIGHT_0]->k_IN -= RSMP[kSpk].Delay;
		}			
	  // UP rate correction for DWN rate	
		if (FILT[kSpk].DN_rate> 1) 			
			RSMP[kSpk].OVS_CH = 4*FILT[kSpk].DN_rate;	
	  //= scale in SIM mode for Matlab Dirac IR plot to account for magnitude boost due to poly (non-zero) OVS	
		if(PlaySession::RECORDING_MODE ==   SIM_I2SOFF_D_ONLY && RSMP[kSpk].F_ACTIVE!=0 )  
			RSMP[kSpk].DSP_scaler *= (float) RSMP[kSpk].LOAD_SIZE/(float) DSPBLOCK_SIZE2;  //X_QTY[kSR][SR_PROC::DAC_SR/2+1][Level]/(float) DSPBLOCK_SIZE2; //( (float) RSMP[kSpk].LOAD_SIZE/(float) RSMP[kSpk].SAVE_SIZE); //RSMP[kSpk].OVS_CH;
	}	
	//clear_stages();
}	

void		RESAMPLER::R_SpkDelay(int stage_qty, int kSpk)
{	int k;
	
	Delay= 0;	
	Delay += FILT[kSpk].Delay;
	Delay += RSMP[kSpk].F_SpkDelay(RESAMPLER::STAGE_QTY);
	//if (FILT[kSpk].DN_rate> 1) 
	//	Delay += FILT[kSpk].DN_rate* INTERP_Delay_factor();
}


void 	RESAMPLER::clear_stages(void)
{
		int 	k, kSpk;
		
	for(kSpk=BAS; kSpk< RESAMPLER::OBJ_QTY; kSpk++)
	{				
		for(k=0;k<sizeof(RSMP[kSpk].stages)/sizeof(RSMP[kSpk].stages[0]);k++)
		{
				RSMP[kSpk].StageInit(NULL, k, NULL, NULL, NULL, 0, 0,  0, 0, 0); //TAB[k].txt2[0], TAB[k].contextpar1); 
		}
		RSMP[kSpk].F_ACTIVE = OFF_STATE;
		RSMP[kSpk].DSP_scaler = 0;		
	}
}


float 	RESAMPLER::INTERP_Delay_factor(void)
{
	float	   delay_factor;

		switch(SPK_XO_IPL_TYPE)
		{
			case 'Z': delay_factor = 0; 	break;
			case 'S': delay_factor = 0.25; 	break;			
			case 'L': delay_factor = 1; 	break;
			default	: delay_factor = 2; 	break; // Factor for all polynomials
		}
		return(delay_factor);
}

int		kr, ksrx, kload; 

void  RESAMPLER::LevelChg(int step)
{
	int k;
	
	Level+= step; 
	if (Level == 3) 
		k=0; 
	 #if (BUFSRC_UART_TRACE == ON) 		
		trace("LevCHG",0);
	 #endif
	Level%= RSMP_LEVEL_QTY;
	
	RSMP[BAS].LOAD_SIZE = X_QTY[SR_44_48][BAS][Level];			
	RSMP[MID].LOAD_SIZE = X_QTY[SR_44_48][MID][Level];			
	
	for(k=MID+1; k< RSMPOBJ_QTY; k++)
	{
		kr = MID_TWT_RSMP_X_QTY;
		ksrx =  SR_44_48; 
		kload = X_QTY[SR_44_48][kr][Level]; 
		RSMP[k].LOAD_SIZE = X_QTY[SR_44_48][MID_TWT_RSMP_X_QTY][Level];
		
	}
	FILTER::Size_RF();	
}


void  RESAMPLER::Make_IPL(void)
{
	int 	j, kSR, klevel, kY, kbase, kx1 ;
	float	kx, s, k44f;
	double	kXR, kX0, kYd, a, dist, RSMP_STEP;
	
	//clk_44_48 = SR_44_48; 
	for(kbase =0; kbase< CLK_BASE_QTY; kbase++)
	{
		for(kSR=0; kSR< SR_LEVEL_QTY+1; kSR++)
		{
			for(klevel=0; klevel< RSMP_LEVEL_QTY; klevel++)
			{
				RSMP_STEP = (double) X_QTY[kbase][kSR][klevel]/RESAMPLER::SAVE_SIZE_R;  								
				//Lin_IPL[kbase][kSR][klevel].RSMP_OUT_QTY = RESAMPLER::SAVE_SIZE_R; 
   				//Lin_IPL[kbase][kSR][klevel].RSMP_STEP = RSMP_STEP;   				
//				for (j=0; j<4; j++)
//      			{
//					R_IPLTab[j][kbase][kSR][klevel].RSMP_OUT_QTY = RESAMPLER::SAVE_SIZE_R; 				
//   					R_IPLTab[j][kbase][kSR][klevel].RSMP_STEP = RSMP_STEP;
//      			}
      			
   				//for(kY=1; kY< RESAMPLER::SAVE_SIZE_R; kY++)
   				for(kY=0; kY< RESAMPLER::SAVE_SIZE_R; kY++)   				
   				{
   					kYd = kY; 
   					kXR = kYd* RSMP_STEP; 	
   					kX0 = floor(kXR);
   					if(kY==RESAMPLER::SAVE_SIZE_R) 
   						kx1 = 1;
   					
   				//@@@@@@ UWAGA - RPI - do sprawdzenia czy tam tez jest taki blad i jesli nie usunac
   					if (kXR-kX0 >= 0.99993) 
   						kX0 += 1;
   						
   					dist = kXR- kX0;
     				a= dist; 
				//@@@@@@@  UWAGA - RPI - do sprawdzenia czy tam tez jest taki blad i jesli nie usunac     	
     				if (kY== DSPBLOCK_SIZE2)
     					a=0;
     				//Lin_IPL[kbase][kSR][klevel].fp1[kY]  = dist;
     			  //
					R_IPLTab[0][kbase][kSR][klevel].fp1[kY]  = dist;     				
      				//Lin_IPL[kbase][kSR][klevel].kX0[kY]  = kX0;	
					R_IPLTab[0][kbase][kSR][klevel].kX0[kY]  = kX0;      					   		      			     				
      				for (j=1; j<IPL_QTY+1; j++)
      				{
      					POLY::ValCalc(POLY::IPL_codes[j].Code, a, &R_IPLTab[j][kbase][kSR][klevel].fp2[kY], &R_IPLTab[j][kbase][kSR][klevel].fp1[kY], &R_IPLTab[j][kbase][kSR][klevel].f_0[kY], &R_IPLTab[j][kbase][kSR][klevel].fm1[kY], &R_IPLTab[j][kbase][kSR][klevel].fp3[kY], &R_IPLTab[j][kbase][kSR][klevel].fm2[kY]);
						R_IPLTab[j][kbase][kSR][klevel].kX0[kY]  =  kX0;
      				}		   		      			      		      				
   				}
   			}		     	
   		}
	}		
}
