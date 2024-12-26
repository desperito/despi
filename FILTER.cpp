#include 	"tt.h"
#include 	<stdio.h>

// Delay handling:
// 1. BUF_MANAGER,Init - last paramater &FILter, assigned there to FILTER class DelayBuf attribute
// 2. Since Delay takes form of an offset to buffer start PUT it is done as R_SpkDelay in Resampler that calls F_SpkDelay to encompass the FILTER delays
// 3. 

//================================================================================
//===========  D E C L A R A T I O N          ======================
//================================================================================

#include  	"hDSPSTRING.hpp"
#include  	"hRESAMPLER.hpp"
#include  	"hSR_PROC.hpp"
#include  	"hPlaySession.hpp"
#include  	"hVOL_MAN.hpp"
#include  	"hDSP_MANAGER.hpp"


extern		RESAMPLER 		RSMP[RSMPOBJ_QTY];


#define			FILT_QTY				6
SECTION(("seg_sdram2"))  int		SPK_LETTERS[] = {'B', 'M','T', 'P'};

		   	
//================================================================================
//===========  O B J E C T   A N D   S T A T I C   D A T A   D E F I N I T I O N     ==========
//================================================================================

	//struct filterstage	Filters[SPK_QTY+1][FILT_QTY];
	#include  	"hFILTER.hpp"
	
  //= Static class members definition	
		int				FILTER::OBJ_QTY, FILTER::STAGE_QTY;
	
  //= Class objects definition
	FILTER				FILT[SPK_QTY+1]; 
	
//================================================================================
//===========  F U N C T I O N   I M P L E M E N T A T I O N          ============
//================================================================================
	
extern		STAGE_PROC  	SP_FILT[SPK_QTY+1][CHN_QTY];


	BUF_MANAGER		*B;  
	STAGE_PROC		*SP1; 

	
void FILT_EXE(STAGE_PROC *SP)
{	
	int 		k, ky, kb, kyn, stage_inc, ep_flag = 0, zero_start;
	
	//float		*BufD;
  // EP part	 
				
  B= SP->BufD; 
/*	
	k=0;	
   //= Special treatment of 	
	if(DSPSTRING::Get_Digitbit(DSPSTR_EXPH) && (SP== &SP_FILT[PRE][LEFT_0] || SP== &SP_FILT[PRE][RIGHT_0]) ) //= Assumed EPP is stage 0 (k==0) in FILTER[PRE] 
	{
	   
	   	for(ky=0; ky< SP->FiltRsmp->stages[k].sect_taps_qty;ky++)
	   	{
	   		SP->FiltRsmp->stages[k].LR_state[LEFT_0][ky]= 0;
	   		SP->FiltRsmp->stages[k].LR_state[RIGHT_0][ky]= 0;
	   	}	
	   	if ( SP->FiltRsmp->stages[k].ACTIVE == ON_STATE) 
			(*SP->FiltRsmp->stages[k].stagefun)(SP, k);
		++k;
	  // Reverse the EP result in RoboDSP buffer so that the PRE filters have proper input
	  // POTENTIAL SPEED OPTIMIZATION - Instead of reversing a dedictaed PUTf reverse could be implemented & used
	  	stage_inc = (SP->stage_cnt+1)%2;
		for(ky=0, kyn=EP_SIZE-1; ky< SP->FiltRsmp->SAVE_SIZE; ky++, kyn--)		// FILTER::Size_RF() has a special EXPH_case where SAVE_SIZE is assigned to RESAMPLER::X_QTY[kSR][BMT_FILTER_X_QTY][RESAMPLER::Level]
			SP->stage_ptr[stage_inc][ky]= SP->stage_ptr[SP->stage_cnt][kyn];			
		++SP->stage_cnt; SP->stage_cnt%=2;	
	}
	
	for (   ; k< FILTER::STAGE_QTY ; k++)
   		if ( SP->FiltRsmp->stages[k].ACTIVE == ON_STATE)  // SP->FiltRsmp->stages[k].stagefun!=0 && 
   			(*SP->FiltRsmp->stages[k].stagefun)(SP, k);
*/ 

		
	for (k=0; k< FILTER::STAGE_QTY ; k++)
	{		
   		if (DSPSTRING::Get_Digitbit(DSPSTR_EXPH) && SP->FiltRsmp->stages[k].param_code =='E')
   		{
	   		for(ky=0; ky< SP->FiltRsmp->stages[k].sect_taps_qty;ky++)
	   		{
	   			SP->FiltRsmp->stages[k].LR_state[LEFT_0][ky]= 0;
	   			SP->FiltRsmp->stages[k].LR_state[RIGHT_0][ky]= 0;
	   		}	
	   		if ( SP->FiltRsmp->stages[k].ACTIVE == ON_STATE) 
				(*SP->FiltRsmp->stages[k].stagefun)(SP, k);
			ep_flag = 1;
					
/*	  	   
	  		stage_inc = (SP->stage_cnt+1)%2;
			for(ky=0, kyn=EP_SIZE-1; ky< SP->FiltRsmp->SAVE_SIZE; ky++, kyn--)		// FILTER::Size_RF() has a special EXPH_case where SAVE_SIZE is assigned to RESAMPLER::X_QTY[kSR][BMT_FILTER_X_QTY][RESAMPLER::Level]
				SP->stage_ptr[stage_inc][ky]= SP->stage_ptr[SP->stage_cnt][kyn];			
			++SP->stage_cnt; SP->stage_cnt%=2;	
*/
      //= Non-EP processing			
   		}
   		else
   		{
   		 	if ( SP->FiltRsmp->stages[k].ACTIVE == ON_STATE)  // SP->FiltRsmp->stages[k].stagefun!=0 && 
   				(*SP->FiltRsmp->stages[k].stagefun)(SP, k);   			
   		}   			
	}   			   	
	if(ep_flag)
	{
	 	// Reverse the EP result in RoboDSP buffer so that the PRE filters have proper input
	  	// POTENTIAL SPEED OPTIMIZATION - Instead of reversing a dedictaed PUTf reverse could be implemented & used

	  	//	  	if (B->k_IN+ EP_SIZE > B->BUF_SIZE/2 && B->k_IN< B->BUF_SIZE/2)
//			for(ky=B->BUF_SIZE/2; ky < B->BUF_SIZE; ky++)
//				B->Buf[ky] = 0;	 			  		

/*  	Handle the EP_TWT mode - to be RE-implemented for the corrected new EP processing woth tail - START
	  	if (B->k_IN+ SP->FiltRsmp->SAVE_SIZE > B->BUF_SIZE)
	 	{
//			for(ky=0; ky < B->BUF_SIZE/2; ky++)
//				B->Buf[ky] = 0;	 		
	  	  	for(ky=B->k_IN, kyn=EP_SIZE-1; ky< B->BUF_SIZE; ky++,kyn--)
				B->Buf[ky] = SP->stage_ptr[SP->stage_cnt][kyn]; //+ B->Buf[ky];					
			for(ky=0; kyn>= EP_SIZE-SP->FiltRsmp->SAVE_SIZE ; ky++,kyn--)   
				B->Buf[ky] = SP->stage_ptr[SP->stage_cnt][kyn]; //+ B->Buf[ky];				
	  	}
	  	else
	  	  for(ky=B->k_IN, kyn=EP_SIZE-1;  kyn>= EP_SIZE-SP->FiltRsmp->SAVE_SIZE ; ky++,kyn--)
			B->Buf[ky] = SP->stage_ptr[SP->stage_cnt][kyn]; //+ B->Buf[ky];				
        Handle the EP_TWT mode - to be RE-implemented for the corrected new EP processing woth tail - END */
        printf("Error - EP_TWT code");
        //exit(0);
			
	  //= Zeroing the buffer with a half BUF_SIZE offset			
/*	  
		ky= 0;
	  	kb = (B->BUF_SIZE/2+ B->k_IN- SP->FiltRsmp->SAVE_SIZE)%B->BUF_SIZE;		
	  	if (kb+ SP->FiltRsmp->SAVE_SIZE > B->BUF_SIZE) 
	  		for(; kb< B->BUF_SIZE ; kb++, ky++)
				B->Buf[kb] = 0;
	  	for(; ky< SP->FiltRsmp->SAVE_SIZE; kb++, ky++)
				B->Buf[kb] = 0;
			
	  	for(kb= 0; ky< SP->FiltRsmp->SAVE_SIZE; kb++, ky++)
			B->Buf[kb] = 0;
													  			
	  //=  End of zeroing		
*/	  
		SP1 = SP; 		
		B->k_IN+= SP->FiltRsmp->SAVE_SIZE;					
		if(B->k_IN > B->BUF_SIZE)
		{
			B->k_IN%= B->BUF_SIZE;
			++B->k_INBUFS;
		}		
	}
	else
		SP->BufD->PUTf(SP->FiltRsmp->SAVE_SIZE, SP->stage_ptr[SP->stage_cnt], 1);					
}


	
void  	IIR (STAGE_PROC *SP, int kStage)
{
	SP->FiltRsmp->_IIR(SP,kStage);
}

void  	FIR (STAGE_PROC *SP, int kStage)
{
	SP->FiltRsmp->_FIR(SP,kStage);
}

void  	DOSF (STAGE_PROC *SP, int kStage)
{
	SP->FiltRsmp->_DOSF(SP,kStage);
}

void  	DOSI (STAGE_PROC *SP, int kStage)
{
	SP->FiltRsmp->_DOSI(SP,kStage);
}

void  	OVSI (STAGE_PROC *SP, int kStage)
{
}

void  	OVSEXT (STAGE_PROC *SP, int kStage)
{
}



void 	NUL(STAGE_PROC *SP, int kStage)
{ }

#if (SOFT_V & SHARC_HW)  
 #include 	<filter.h>
 void FILTER::StageInit(void (*pStagefun)(STAGE_PROC *SP, int kStage), int kStage, 
								float pm *pcoeffs,
			   					float *Lstate, float	*Rstate, int sect_taps, int pParamCode, int pDownRatio, int pDelay, char *pDesc)
#else  // RPI_V			   					
 void FILTER::StageInit(void (*pStagefun)(STAGE_PROC *SP, int kStage), int kStage, 
								float  *pcoeffs,
			   					float *Lstate, float	*Rstate, int sect_taps, int pParamCode, int pDownRatio, int pDelay, char *pDesc)
#endif			   					
{
	int 	k=0, kd=0;
	
	stages[kStage].stagefun			= 	pStagefun;   	
	if (pStagefun!= NULL) 	
		stages[kStage].ACTIVE		= 	ON_STATE; 
	else 
		stages[kStage].ACTIVE		= 	OFF_STATE;	
	stages[kStage].coeffs			= 	pcoeffs;
	stages[kStage].LR_state[LEFT_0]	= 	Lstate;
	stages[kStage].LR_state[RIGHT_0]= 	Rstate;	  
	stages[kStage].param_code		= 	pParamCode;		
	stages[kStage].DN_rate			= 	pDownRatio;		
	stages[kStage].delay			= 	0;
	if (pParamCode != 'E' && pParamCode != 'P')
		stages[kStage].delay			= 	pDelay & 0xFFFF;
  //= Handling of the overloaded use of delay param to carry filter size and EP proc size		
	else if (pParamCode == 'P' || pParamCode == 'E')
	{			
		stages[kStage].size			= 	pDelay & 0xFFFF;		
		stages[kStage].EP_OFFSET   	=	(pDelay & 0xFFFF0000)>>16;  				
	}
  //@@@@@@@@@@@  REFACTORING 	DOSF to DOSI	
	if (pStagefun == IIR || pStagefun == DOSI)  // || pStagefun == OVSI   
		stages[kStage].sect_taps_qty = 2* sect_taps+ 1;
	else   // FIR || OVSF
		stages[kStage].sect_taps_qty	= sect_taps+ 1;		
	
	stages[kStage].DSPTAB_desc[k++] = (DSP_TYPE-1) ? 'R' : 'F';	
	stages[kStage].DSPTAB_desc[k++] = SPK_LETTERS[SPK_CODE];
	stages[kStage].DSPTAB_desc[k++] = '_';

	while(k<STAGE_DESC_SIZE-1)
		stages[kStage].DSPTAB_desc[k++] = pDesc[kd++];	
}		


void FILTER::Constructor(void)    //(DSP_Steps_TAB  *pS_TAB, int pSPK_CODE)
{
	int 	k, kSpk;		
	
	FILTER::STAGE_QTY 	= sizeof(FILT[0].stages)/sizeof(FILT[0].stages[0]);
	FILTER::OBJ_QTY 	= PRE+1;
//	FILTER::List_FR();
		
	for(kSpk=0; kSpk< FILTER::OBJ_QTY; kSpk++)
	{	//FILT[kSpk].stages = Filters[kSpk]; 			
		FILT[kSpk].DSP_TYPE = DSP_FILT;
		FILT[kSpk].SPK_CODE  = kSpk;
		FILT[kSpk].DN_rate = 	1; 		 		
//		FILT[kSpk].S_TAB = DSPSTRING::S_TABS[kSpk].tab;		
	}
}

int ktest1; 

void FILTER::Size_Vol(void)
{
	int 	kSpk;		
	
		
	for(kSpk=BAS; kSpk< FILTER::OBJ_QTY; kSpk++)
	{
		FILT[kSpk].DN_rate_Calc();	
		FILT[kSpk].Vol(FILTER::STAGE_QTY);
		FILT[kSpk].F_SpkDelay(FILTER::STAGE_QTY);
				
	//	if (FILT[kSpk].DelayBuf != 0) 
	//		FILT[kSpk].DelayBuf->k_IN += FILT[kSpk].Delay;	
	}	
	FILTER::Size_RF();
	//clear_stages();
   //= Turn on DSP VOl Scaler 	
	
	// DSP_MANAGER::Poly_Init();	
}		


void FILTER::Size_RF(void)
{
	int 	k, kSR, csize=0, ks, EXPH_case;		
	
	if (RESAMPLER::ACTIVE)
		kSR = SR_44_48;
	else
		kSR = 1;
	int 	kSpk;		
	
	//stages[kStage].delay			= 	pDelay;
	
	for(kSpk=0; kSpk< FILTER::OBJ_QTY; kSpk++)
	{	
		//EXPH_case = (kSpk == PRE && DSPSTRING::Get_Digitbit(DSPSTR_EXPH) );		  			
	   //= DSP Filter stage blocks have common input for all stages including BAS 
//	  	if (EXPH_case)
//	  	{		  			
//			FILT[kSpk].LOAD_SIZE = DSPBLOCK_SIZE2; // Assumption that EP_SIZE will never make sense to be less than DSPBLOCK_SIZE2;
//			//csize = RESAMPLER::X_QTY[kSR][BMT_FILTER_X_QTY][RESAMPLER::Level];			
//			//FILT[kSpk].SAVE_SIZE = 
//	  	}
//		else
		ktest1 = kSpk;  
			FILT[kSpk].LOAD_SIZE = RESAMPLER::X_QTY[kSR][BMT_FILTER_X_QTY][RESAMPLER::Level];  // BMT_FILTER_X_QTY		(SR_PROC::DAC_SR/2+1) -> DAC_SR 1, 2 ,3 
		if ( (PlaySession::DEV_MODE == xCODAC) && kSpk == MID)		
		{  	
			FILT[kSpk].LOAD_SIZE = DSPBLOCK_SIZE2;
			FILT[kSpk].SAVE_SIZE = DSPBLOCK_SIZE2;
		}
	  //= Stage size handling	
	  	if (FILT[kSpk].F_ACTIVE )
	  	{		
	  		//if (!csize) // not specific EXPH case when it is set differently
			csize = FILT[kSpk].LOAD_SIZE;
			for(k=0; k< FILTER::STAGE_QTY; k++)  // -1 bcs technically the last stage is reserved for VOL_SCALER
			{
				//EXPH_case = FILT[kSpk].stages[k].size >0; 
				if (FILT[kSpk].stages[k].ACTIVE ) //&& !EXPH_case)  
				{
					FILT[kSpk].stages[k].size = csize; 
					if (FILT[kSpk].stages[k].DN_rate >0)  
						csize /= FILT[kSpk].stages[k].DN_rate; 
//					if (FILT[kSpk].stages[k].param_code == 'E' ) //|| FILT[kSpk].stages[k].param_code == 'P') // || kSpk == MID)
//						FILT[kSpk].stages[k].size =  EP_SIZE2;  												
						//FILT[kSpk].stages[k].size =  FILT[kSpk].stages[k].delay;											
					ks= k;  // store the stage no to get the last one when loop finished 
				}
			}
		  //= SAVE_SIZE as the size of the last stage
			FILT[kSpk].SAVE_SIZE = csize; //FILT[kSpk].stages[ks].size/FILT[kSpk].stages[ks].DN_rate;
	  	}
	  	else  if (PlaySession::DEV_MODE == xDAC)//= NOT ACTIVE - like passthrough DT for simple DAC
	  		FILT[kSpk].SAVE_SIZE = 	RESAMPLER::X_QTY[kSR][MID_TWT_RSMP_X_QTY][RESAMPLER::Level];   //RSMP[kSpk].LOAD_SIZE; 			// 
	  		
		  // Special case for FILT[1] MID in CODAC 
		//if ( (PlaySession::DEV_MODE == 'k' || PlaySession::DEV_MODE == 'K') && kSpk == MID)
	  	if (kSpk< RESAMPLER::OBJ_QTY)
			RSMP[kSpk].LOAD_SIZE = FILT[kSpk].SAVE_SIZE;
		
	   #if 		(EP_MODE == EP_PRE)	
		if ( kSpk == PRE && DSPSTRING::Get_Digitbit(DSPSTR_EXPH)  ) // Correc the standard SAVE_SIZE derived from LOAD_SIZE
	   #elif	(EP_MODE == EP_MID)
		if ( kSpk == MID && DSPSTRING::Get_Digitbit(DSPSTR_EXPH)  ) // Correc the standard SAVE_SIZE derived from LOAD_SIZE
	   #elif	(EP_MODE == EP_TWT)
		if ( kSpk == TWT && DSPSTRING::Get_Digitbit(DSPSTR_EXPH)  ) // Correc the standard SAVE_SIZE derived from LOAD_SIZE	
	   #elif	(EP_MODE == EP_BT)			
	    if (0)
	   #endif
	   {	   		
			FILT[kSpk].SAVE_SIZE = FILT[kSpk].LOAD_SIZE;
		}			
	}
   #if	(EP_MODE == EP_TWT || EP_MODE == EP_BT)			
   	RSMP[PRE].LOAD_SIZE = RESAMPLER::X_QTY[kSR][BMT_FILTER_X_QTY][RESAMPLER::Level];
   #endif
	//RSMP[PRE].LOAD_SIZE = DSPBLOCK_SIZE2; 											  		
	
}


void FILTER::Vol(int Stage_Qty)
{
	int 	k;
	
	DSP_scaler = 1;
	for(k=0; k<Stage_Qty-1; k++)
		if(stages[k].ACTIVE && stages[k].param_code !='P')      // stagefun != 0
			DSP_scaler *= stages[k].coeffs[1];			
}

void FILTER::DN_rate_Calc(void)
{
	int 	k;
	
	DN_rate = 1;
	for(k=0; k< FILTER::STAGE_QTY; k++)
	{
		if (stages[k].DN_rate >0)  // assumption that there is always just one (takes last effectively)
			DN_rate *= stages[k].DN_rate; 
	}
}

int		FILTER::F_SpkDelay(int Stage_Qty)
{	int k;
	
	Delay= 0;
	for(k=0; k< Stage_Qty; k++)
		if (stages[k].ACTIVE)
			Delay+= stages[k].delay;
	return(Delay);
}


void 	FILTER::clear_stages_FR(void)
{
	int 		k, kf, kSpk, kStage;
	FILTER		*FR;

	for(kf=0; kf< DSP_BLOCK_QTY; kf++)
	{
		FR= DSP_MANAGER::Streams[kf][LEFT_0]->FiltRsmp; 
		if (FR == NULL) continue; 		
		for(k=0;k<sizeof(FR->stages)/sizeof(FR->stages[0]);k++)
		{
			FR->StageInit(NULL, k, NULL, NULL, NULL, 0, 0,  0, 0, 0); //TAB[k].txt2[0], TAB[k].contextpar1); 
		}		
		FR->F_ACTIVE = OFF_STATE;			
		FR->DSP_scaler = 0;		
	}		
}


void 	FILTER::clear_state_FR(void)
{
	int 	k, kf, kch, ktaps;
	FILTER		*FR;

	for(kf=0; kf< DSP_BLOCK_QTY; kf++)
	{
		FR= DSP_MANAGER::Streams[kf][LEFT_0]->FiltRsmp; 
		if (FR == NULL) continue; 
		for(k=0;k<sizeof(FR->stages)/sizeof(FR->stages[0]);k++)
			for(kch=0; kch< CHN_QTY; kch++)
				for(ktaps=0; ktaps< FR->stages[k].sect_taps_qty; ktaps++)				
					FR->stages[k].LR_state[kch][ktaps]= 0; 
	}
}		



void  	FILTER::_IIR (STAGE_PROC *SP, int kStage)
{
	iir(SP->stage_ptr[SP->stage_cnt%2], SP->stage_ptr[(SP->stage_cnt+1)%2], stages[kStage].coeffs+2, stages[kStage].LR_state[SP->Chn], stages[kStage].size, *(SP->FiltRsmp->stages[kStage].coeffs) );  //SP->FiltRsmp->DSP_SIZE,
	++SP->stage_cnt; SP->stage_cnt%=2;	
}

void  	FILTER::_FIR (STAGE_PROC *SP, int kStage)
{
	fir(SP->stage_ptr[SP->stage_cnt%2], SP->stage_ptr[(SP->stage_cnt+1)%2], stages[kStage].coeffs+2, stages[kStage].LR_state[SP->Chn], stages[kStage].size, *(stages[kStage].coeffs) );
	++SP->stage_cnt; SP->stage_cnt%=2;	
}

void 	FILTER::_DOSF(STAGE_PROC *SP, int kStage)
{
  //== Low-pass filter before downsampling		
   	//FIR(SP, kStage);
	IIR(SP, kStage);		
	DOSX(SP, kStage);
}

void 	FILTER::_DOSI(STAGE_PROC *SP, int kStage)
{
  //== Low-pass filter before downsampling		
   	//FIR(SP, kStage);
	IIR(SP, kStage);		
	DOSX(SP, kStage);
}

void 	FILTER::DOSX(STAGE_PROC *SP, int kStage)
{
	int 	i,	j;
	float 	*IN, *OUT;
			
    IN= SP->stage_ptr[SP->stage_cnt];
    OUT= SP->stage_ptr[(SP->stage_cnt+1)%2];
  //== Downsampling (sample decimation)
	for (i=0, j=0; i< stages[kStage].size; i+=stages[kStage].DN_rate, j++) 
		OUT[j]= IN[i]; //* SMAct->dwn_ratio;
	++SP->stage_cnt; SP->stage_cnt%=2;	
}



#if	(SOFT_V & SHARC_HW) 		
 #include 	<filter.h>
#else
 void iir(const DSPdata *input, DSPdata *output, const DSPdata *coeffs, DSPdata *state, int samples, int sections)
 {
    int i;
    int j;
    DSPdata X;
    DSPdata Y = 0;

    for (i = 0; i < samples; i++)
    {
        X = input[i];

        for (j = 0; j < sections; j++)
        {
            Y = X + state[j * 2 + 0];

            state[j * 2 + 0] = X * coeffs[j * 4 + 3] + Y * coeffs[j * 4 + 1] + state[j * 2 + 1];
            state[j * 2 + 1] = X * coeffs[j * 4 + 2] + Y * coeffs[j * 4 + 0];

            X = Y;
        }

        output[i] = Y;
    }
 }

 void  fir(const DSPdata *input, DSPdata *output, const DSPdata *coeffs, DSPdata *state, int samples, int  taps)
 {
    int 	i;
    int  	j;
    DSPdata 	Y;
    
    for (i=0; i<samples; i++)
    {
        Y = 0;
        
        for (j=0; j<taps-1; j++)
        {
            Y = Y + state[j] * coeffs[j];
            
            state[j] = state[j+1];
        }
        
        Y = Y + state[taps-2] * coeffs[taps-2];
        
        state[taps-2] = input[i]; 
        
        Y = Y + input[i] * coeffs[taps-1];
        
        output[i] = Y;
    }
 }
#endif
