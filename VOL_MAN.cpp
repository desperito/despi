////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#include 	"tt.h"
#include 	<stdio.h>

#include  	"hSTAGE_PROC.hpp"
#include  	"hBUF_MANAGER.hpp"
#include  	"hFILTER.hpp"
#include  	"hRESAMPLER.hpp"
#include  	"hPlaySession.hpp"
#include  	"hDSP_TAB.hpp"
#include  	"hDSPSTRING.hpp"
#include  	"hDSP_MANAGER.hpp"

			
//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================

#include	"hVOL_MAN.hpp"
//float		*VOL_MAN::PEAK = (float *) tPEAK;
	int			VOL_MAN::LSEG, VOL_MAN::RSEG;
	

	VOL_MAN			VOM[SPK_QTY][CHN_QTY]; 	
	
//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================


void VOL_MAN::Constructor(void)
{
	VOL_MAN::LSEG = 50; VOL_MAN::RSEG = VOL_MAN::LSEG; 
}

//#define		LSEG			50
//#define		RSEG			50
#define		SEG_OFFSET		100

#define		MAX_23bit_REF		8000000
//#define		MAX_23bit		MAX_23bit_REF
#define		MAX_23bit_FULL		8388606

#define		MAX_23bit		MAX_23bit_REF+100000
//#define		MAX_23bit_FULL		MAX_23bit_REF+100000

float		*Q; 

void  VOL_MAN::VOL_TYPE(FILTER	*F, int Stage_Qty)
{
	if (F->DSP_scaler !=1 )
	{
   		F->stages[Stage_Qty-1].stagefun 		= VOL_LIMSCALER;  
		F->stages[Stage_Qty-1].DSPTAB_desc[3] 	= 'V';	F->stages[Stage_Qty-1].DSPTAB_desc[4] 	= '2';
	}   				
   	else 
   	{
   		F->stages[Stage_Qty-1].stagefun 	= VOL_LIMITER;     		
		F->stages[Stage_Qty-1].DSPTAB_desc[3] 	= 'V';	F->stages[Stage_Qty-1].DSPTAB_desc[4] 	= 'L';
   	}   					
	F->stages[Stage_Qty-1].ACTIVE 		= ON_STATE; 		    				
}

//= Peaks compression procedure
//= Checking a stage size historical buffer data for peaks and compressing them over the LSEG and RSEG
//== #pragma 	optimize_off
void VOL_MAN::VOL_Stages_Setup(void)
{
	int 	k, kf,  Stage_Qty;
	FILTER	*F; 
	
	for(kf=0; kf< DSP_BLOCK_QTY; kf++)
	{
		F= DSP_MANAGER::Streams[kf][LEFT_0]->FiltRsmp; 
		if (F == NULL) continue; 		
		
		if (F->DSP_TYPE == DSP_FILT)
			Stage_Qty = FILTER::STAGE_QTY;
		else 
			Stage_Qty = RESAMPLER::STAGE_QTY;		
					
		if (PlaySession::DEV_MODE == xCODAC )    // || (PlaySession::DEV_MODE == CODAC+32))
		{
			if(F->DSP_TYPE== DSP_FILT)
			{ 
				if (F->SPK_CODE== BAS && BAS_CORR_LEVEL_GET>0 )
				{
					F->stages[Stage_Qty-1].stagefun 	=  VOL_MCB;  // VOL_SCALER;   // VOL_MCMT;  //  
					F->stages[Stage_Qty-1].ACTIVE 		= ON_STATE; 		 				
					F->stages[Stage_Qty-1].DSPTAB_desc[3] 	= 'V';  F->stages[Stage_Qty-1].DSPTAB_desc[4] 	= 'M';
				}			
				else if(F->SPK_CODE== PRE || F->SPK_CODE== MID)
					VOL_TYPE(F, Stage_Qty);			
			  #if (SOFT_V == SH_XOEP_PROD)
				else if(F->SPK_CODE== PRE || F->SPK_CODE== TWT)
					VOL_TYPE(F, Stage_Qty);			
			  #endif
			  						
			}
			else if (F->DSP_TYPE== DSP_RSMP && F->SPK_CODE== PRE)
				VOL_TYPE(F, Stage_Qty);
		  #if (SOFT_V == SH_XOEP_PROD)			
			else if (F->DSP_TYPE== DSP_RSMP && F->SPK_CODE== TWT)
				VOL_TYPE(F, Stage_Qty);		  	
		  #endif				
		}
		else if( PlaySession::DEV_MODE == xDAC  && F->SPK_CODE== TWT && F->DSP_TYPE== DSP_RSMP ) 
			VOL_TYPE(F, Stage_Qty);
		else
		{
   			F->stages[Stage_Qty-1].stagefun 	= NULL;  
			F->stages[Stage_Qty-1].ACTIVE 		= OFF_STATE;
		}
	  //= Default volume scaler for non-specific cases	
		if(F->stages[Stage_Qty-1].stagefun == NULL && F->DSP_scaler !=1 && F->DSP_scaler !=0) 
		{			
   			F->stages[Stage_Qty-1].stagefun 	= VOL_SCALER;  		
			F->stages[Stage_Qty-1].ACTIVE 		= ON_STATE; 		    						
			F->stages[Stage_Qty-1].DSPTAB_desc[3] 	= 'V';	F->stages[Stage_Qty-1].DSPTAB_desc[4] 	= 'S';
		}   						 		 							
/*		if (F->SPK_CODE ==PRE)
		{
			//F->stages[Stage_Qty-1].size 		= EP_SIZE;
			F->stages[Stage_Qty-1].param_code 	= 'V';
		}
		else	
		{
*/			
			F->stages[Stage_Qty-1].size 		= F->SAVE_SIZE;		
			F->stages[Stage_Qty-1].param_code 	= 'V';			
//		}
	}
	//DSP_TAB::Stage_Switches();
}		

#pragma 	optimize_on


int 	VOL_MAN::Peak_Correction2(float *ROB, int stage_size)
{
	int			k, k1, kb, pL=-1, pR=-1, CORR_SIZE, CORR_START ; 
	float		peakdir_val= 0, ratio, step, Lstep, Rstep; 
	float 		min= 1E+10, max = 0;
	
	
  //= Find min and max values for the current block
	for(k=LSEG; k< LSEG+ stage_size; k++)
	{
		if (pL< 0)
		{
			if ( ROB[k] < -MAX_23bit)
			{
				pL = 	k;
				pR= 	pL;
				min = 	ROB[k];
			}
			if ( ROB[k] > MAX_23bit)
			{
				pL = 	k;
				pR= 	pL;
				max = 	ROB[k];
			}
								
		}
		else
		{		
			if (ROB[k] < -MAX_23bit)
			{	
				pR = 	k;				
				if (ROB[k]< min )
					min = 	ROB[k];
			}
			if (ROB[k] > MAX_23bit)
			{	
				pR = 	k;				
				if (ROB[k]> max )
					max = 	ROB[k];
			}
		}	
	}
  //= Set peak flag and handle double (plus and minus peak)	
  	if (pL>0)
  	{	
  		if (max >-min)
  		{
  			peakdir_val = max;
			ratio =  MAX_23bit_REF/peakdir_val;
  		}  		
  		else 	
  		{
			peakdir_val = min;
			ratio =  MAX_23bit_REF/-peakdir_val;
  		}
			
//		sprintf(dbgtab, "\n\rB-Peak:%f2.0%", (1-ratio)*100);
//		trace(dbgtab, 0);		
			
		Lstep = (1-ratio)/(LSEG+1);
		Rstep = (1-ratio)/(RSEG+1); //(SEG_SIZE-kpeak-1);
		CORR_START = pL-LSEG;
  	   //= Apply diminishing ratio to the LSEG points left to and the peak point itself
		for(k=CORR_START, k1=1 ; k<= pL; k++, k1++)
			ROB[k] *= (1-k1*Lstep);
		for( ; k<= pR; k++)
			ROB[k] *= (1-k1*Lstep);			
  	   //= Apply ratio gradually returning to 100% to the RSEG point from the peak whose index is in k 
		//for(k1=SEG_SIZE-k  ; k<SEG_SIZE; k++, k1--)
		for(k1=RSEG ; k1>=1; k++, k1--)
			ROB[k] *= (1-k1*Rstep);
		return(1);
  	}
  	else
		return(0);
}


int 	VOL_MAN::Peak_Correction1(float *ROB, int stage_size)

{
	int			k, k1, kpeak, kmin, kmax, SEG_SIZE, DATA_START; 
	float		peakdir_val= 0, ratio, step, Lstep, Rstep; 
	float 		min= 1E+10, max = 0;
	 
	kmin = -1;
	kmax= -1; 
	SEG_SIZE = LSEG+stage_size+RSEG;
  //= Find min and max values for the current block
	for(k=LSEG; k< LSEG+ stage_size; k++)
	{
		if (ROB[k]< min && ROB[k] < -MAX_23bit)
		{
			min = ROB[k];
			kmin = k;
		}
		if (ROB[k]> max && ROB[k]> MAX_23bit)
		{	
			max = ROB[k];
			kmax = k;
		}		
	}
  //= Set peak flag and handle double (plus and minus peak)	
  	if (kmin>-1 || kmax>-1)
  	{
  		peakdir_val = min;
		kpeak = kmin;
  		if (kmax>-1 && max> -min  )
		{
			peakdir_val = max;
			kpeak = kmax;
		}
  			
		if (peakdir_val > 0)
			ratio =  MAX_23bit_REF/peakdir_val;
		else  
			ratio =  MAX_23bit_REF/-peakdir_val;
			
//		sprintf(dbgtab, "\n\rB-Peak:%f2.0%", (1-ratio)*100);
//		trace(dbgtab, 0);		
			
		Lstep = (1-ratio)/LSEG;
		Rstep = (1-ratio)/(RSEG-1); //(SEG_SIZE-kpeak-1);
  	   //= Apply diminishing ratio to the Left SEG 
		for(k=kpeak-LSEG, k1=0 ; k<= kpeak; k++, k1++)
			ROB[k] *= (1-k*Lstep);
  	   //= Apply ratio gradually returning to 100% to the Right SEG 		
		//for(k1=SEG_SIZE-k  ; k<SEG_SIZE; k++, k1--)
		for(k1=RSEG-1 ; k1>=0; k++, k1--)
			ROB[k] *= (1-k1*Rstep);
  		return(1);
  	}	
  	else 
  	return(0);
}



void 	VOL_MAN::Proc_Peaks(BUF_MANAGER *B, float *ROB, int stage_size)
{
	int			k, k1, kb, SEG_SIZE, SEG_START, CORR_SIZE; 
	float 		*SEG; 
	 
	//stage_size = 200;
	SEG_SIZE = LSEG+stage_size+RSEG+SEG_OFFSET;
	CORR_SIZE = LSEG+RSEG+1;
	Q= ROB; 
	//SEG_DIST = LSEG+DATA_START;
  //= Copy SDRAM buffer segment to the free RoboDSP L2 buffer for processing	 
  //#####    RPI  Ekrem   R E F A C T O R  - RPi handles cache autoamtically
  	if ( B->k_IN > SEG_SIZE)
  	{
  		SEG = &B->Buf[B->k_IN-SEG_SIZE];  // Beginning of the buffer segment under processing
		for(k=0; k< SEG_SIZE; k++)
			ROB[k] = SEG[k];
  	}
  	else 
  	{
		for(k=0, kb= B->BUF_SIZE-(SEG_SIZE-B->k_IN); kb< B->BUF_SIZE; kb++)
			ROB[k++] = B->Buf[kb];
		for(kb=0; k< SEG_SIZE; kb++)
			ROB[k++] = B->Buf[kb];
  	}
  	//return;
  	if (Peak_Correction2(ROB, stage_size) )
  	{
  	   //= Copy compressed peaks to the original SDRAM buffer segment    	   
  		if ( B->k_IN > SEG_SIZE)  		
  		{
			for(k=0; k< SEG_SIZE; k++)
				SEG[k]= ROB[k];
  		}
  		else 
  		{
			for(k=0, kb= B->BUF_SIZE-(SEG_SIZE-B->k_IN); kb< B->BUF_SIZE; kb++)
				B->Buf[kb]= ROB[k++];
			for(kb=0; k< SEG_SIZE; kb++)
				B->Buf[kb]= ROB[k++];
  		}  		
  	}	
}

//= Calling Peaks compression check
void 	VOL_MCB(STAGE_PROC *SP, int kStage)
{
	int k;

	float *IN, *OUT;
			
    IN= 	SP->stage_ptr[SP->stage_cnt];
    OUT= 	SP->stage_ptr[(SP->stage_cnt+1)%2];
	
	for(k=0; k<SP->FiltRsmp->stages[kStage].size; k++)
		OUT[k]= IN[k]* SP->FiltRsmp->DSP_scaler;  	
	++SP->stage_cnt; SP->stage_cnt%=2;	
   //= Check and correct max Vol excesses
	VOL_MAN::Proc_Peaks(SP->BufD, IN-3, SP->FiltRsmp->stages[kStage].size);
//	if (SP->BufD->k_IN>4500)
//		k=0; 
}

//#pragma optimize_for_speed	
//= Simple peaks flat cutting 
void 	VOL_MCMT(STAGE_PROC *SP, int kStage)
{
	int 		k, kex= 0;
	float 		*IN, *OUT;
BUF_MANAGER		*B; 
			
    IN= 	SP->stage_ptr[SP->stage_cnt];
    OUT= 	SP->stage_ptr[(SP->stage_cnt+1)%2];
	B = 	SP->BufD;
    
	for(k=0; k<SP->FiltRsmp->stages[kStage].size; k++)
	{
		OUT[k]= IN[k]* SP->FiltRsmp->DSP_scaler;  	    	
		if (OUT[k]< -MAX_23bit_FULL)
		{
			OUT[k] = -MAX_23bit_FULL;
			++kex;
		}
		if (OUT[k]> MAX_23bit_FULL)
		{
			OUT[k] = MAX_23bit_FULL;
			++kex;
		}
	}
/*			
	for(k=0; k<SP->FiltRsmp->stages[kStage].size; k++)
	{
		if (OUT[k]< -MAX_23bit_FULL)
		{
			OUT[k] = -MAX_23bit_FULL;
			++kex;
		}
		if (OUT[k]> MAX_23bit_FULL)
		{
			OUT[k] = MAX_23bit_FULL;
			++kex;
		}
	}
*/	
/*	
	switch(SP->FiltRsmp->SPK_CODE)
	{
		case	MID:
			sprintf(dbgtab, "\n\rM-Peak:%dsmp", kex);					
		  break;
		case	TWT:
			sprintf(dbgtab, "\n\rT-Peak:%dsmp", kex);								
		  break;			
	}
	trace(dbgtab, 0);
*/						
	++SP->stage_cnt; SP->stage_cnt%=2;		
}

void 	VOL_LIMITER(STAGE_PROC *SP, int kStage)
{
	int 		k, kex= 0;
	float 		*IN, *OUT;
BUF_MANAGER		*B; 

    IN= 	SP->stage_ptr[SP->stage_cnt];
	for(k=0; k<SP->FiltRsmp->stages[kStage].size; k++)
	{
		if (IN[k]< -MAX_23bit_FULL || IN[k]> MAX_23bit_FULL)
		{
			if (IN[k]<0)
				IN[k] = -MAX_23bit_FULL;
			else 
				IN[k] = MAX_23bit_FULL;			
			++kex;
		}
	/*	
		if (IN[k]> MAX_23bit_FULL)
		{
			IN[k] = MAX_23bit_FULL;
			++kex;
		}
	*/	
	}
}

void 	VOL_SCALER(STAGE_PROC *SP, int kStage)
{
	int k;
	float *IN, *OUT;
			
    IN= 	SP->stage_ptr[SP->stage_cnt];
    OUT= 	SP->stage_ptr[(SP->stage_cnt+1)%2];
	
	for(k=0; k<SP->FiltRsmp->stages[kStage].size; k++)
		OUT[k]= IN[k]* SP->FiltRsmp->DSP_scaler;  
	++SP->stage_cnt; SP->stage_cnt%=2;	
}

void 	VOL_LIMSCALER(STAGE_PROC *SP, int kStage)
{
	VOL_SCALER(SP, kStage);
	VOL_LIMITER(SP, kStage);
	
}

/*
int 	VOL_MAN::find_reverse(int k)
{ 
	int 	ks, k1, BUF_LEFT;
	
	while(1)
	{
		if (B->BUF_SIZE> k+BUF_SEG)
		{	
			for(k1=k; k1<k+BUF_SEG; k1++)
			OUT[k1]= B->Buf[B->k_IN+k1];
		}
		else 
		{
			BUF_LEFT = B->BUF_SIZE- k;
			for(k1=k; k1<k+BUF_LEFT; k1++)
				OUT[k1]= B->Buf[B->k_IN+k1];
			for( ; k1<k+BUF_SEG; k1++)
				OUT[k1]= B->Buf[B->k_IN+k1];
		}
		for(k1=k-1;  k1<k+BUF_SEG-1; k1++)
		{
			if (OUT[k1] > OUT[k1+1] && OUT[k1+2] > OUT[k1+1] && OUT[k1+1]< MAX_24BIT)
				return(k1+1)
		}
		k= k1+1;
	}
}

	if (min < -MAX_23bit)
		peakdir_val= min; 					  
	if (max > MAX23bit)
	{
		if (peakdir_val 1!= 0)
		{
			if (max> -min  )
			{
				peakdir_val = max;
				
		}
		else 
			peakdir_val = max;
	}

*/



/*
void 	VOL_MAN::Proc_Peaks(BUF_MANAGER *B, float *ROB, int stage_size)
{
	int			k, k1, kb, kpeak, kmin, kmax, start, SEG_SIZE, DATA_START; 
	float		peakdir_val= 0, ratio, step, Lstep, Rstep, *SEG, *DROB; 
	float 		min= 1E+10, max = 0;
	 
	kmin = -1;
	kmax= -1; 
	SEG_SIZE = LSEG+stage_size+RSEG;
	DATA_RSIZE = stage_size+RSEG;
	DROB= ROB+DATA_START; 
	//SEG_DIST = LSEG+DATA_START;
  //= Copy SDRAM buffer segment to the free RoboDSP L2 buffer for processing	 
  //#####    RPI  Ekrem   R E F A C T O R  - RPi handles cache autoamtically
  	if ( B->k_IN > stage_size)
  	{
  		SEG = &B->Buf[B->k_IN-stage_size];  // Beginning of the buffer segment under processing
		for(k=0; k< stage_size; k++)
			DROB[k] = SEG[k];
  	}
  	else 
  	{
		for(k=0, kb= B->BUF_SIZE-(SEG_SIZE-B->k_IN); kb< B->BUF_SIZE; kb++)
			ROB[k++] = B->Buf[kb];
		for(kb=0; k< SEG_SIZE; kb++)
			ROB[k++] = B->Buf[kb];
  	}
  	//return;
  //= Find min and max values for the current block
	for(k=LSEG; k< LSEG+ stage_size; k++)
	{
		if (ROB[k]< min && ROB[k] < -MAX_23bit)
		{
			min = ROB[k];
			kmin = k;
		}
		if (ROB[k]> max && ROB[k]> MAX_23bit)
		{	
			max = ROB[k];
			kmax = k;
		}		
	}
  //= Set peak flag and handle double (plus and minus peak)	
  	if (kmin>-1 || kmax>-1)
  	{
  		peakdir_val = min;
		kpeak = kmin;
  		if (kmax>-1 && max> -min  )
		{
			peakdir_val = max;
			kpeak = kmax;
		}
  			
		if (peakdir_val > 0)
			ratio =  MAX_23bit_REF/peakdir_val;
		else  
			ratio =  MAX_23bit_REF/-peakdir_val;
			
//		sprintf(dbgtab, "\n\rB-Peak:%f2.0%", (1-ratio)*100);
//		trace(dbgtab, 0);		
			
		Lstep = (1-ratio)/LSEG;
		Rstep = (1-ratio)/(RSEG-1); //(SEG_SIZE-kpeak-1);
  	   //= Apply diminishing ratio to the Left SEG 
		for(k=kpeak-LSEG, k1=0 ; k<= kpeak; k++, k1++)
			ROB[k] *= (1-k*Lstep);
  	   //= Apply ratio gradually returning to 100% to the Right SEG 		
		//for(k1=SEG_SIZE-k  ; k<SEG_SIZE; k++, k1--)
		for(k1=RSEG-1 ; k1>=0; k++, k1--)
			ROB[k] *= (1-k1*Rstep);
  	   //= Copy compressed peaks to the original SDRAM buffer segment  
  		if ( B->k_IN > (LSEG+RSEG-1))
  		{
			for(k=kpeak-LSEG, kb=0; kb< (LSEG+RSEG-1); k++, kb++)
				SEG[k]= ROB[k];
  		}
  		else 
  		{
			for(k=kpeak-LSEG, kb= B->BUF_SIZE-(LSEG+RSEG-B->k_IN); kb< B->BUF_SIZE; kb++, k++)
				B->Buf[kb]= ROB[k++];
			for(kb=0; k< kpeak+RSEG; k++)
				B->Buf[kb++]= ROB[k];
  		}
  	}	
}
*/

/*
		Lstep = (1-ratio)/(LSEG+1);
		Rstep = (1-ratio)/(RSEG+1); //(SEG_SIZE-kpeak-1);
		CORR_START = kpeak-LSEG;
  	   //= Apply diminishing ratio to the LSEG points left to and the peak point itself
		for(k=CORR_START, k1=1 ; k<= kpeak; k++, k1++)
			ROB[k] *= (1-k1*Lstep);
  	   //= Apply ratio gradually returning to 100% to the RSEG point from the peak whose index is in k 
		//for(k1=SEG_SIZE-k  ; k<SEG_SIZE; k++, k1--)
		for(k1=RSEG ; k1>=1; k++, k1--)
			ROB[k] *= (1-k1*Rstep);
  	   //= Copy compressed peaks to the original SDRAM buffer segment  
  	   
  		if ( B->k_IN > SEG_SIZE)  		
  		{
			for(k=0; k< SEG_SIZE; k++)
				SEG[k]= ROB[k];
  		}
  		else 
  		{
			for(k=0, kb= B->BUF_SIZE-(SEG_SIZE-B->k_IN); kb< B->BUF_SIZE; kb++)
				B->Buf[kb]= ROB[k++];
			for(kb=0; k< SEG_SIZE; kb++)
				B->Buf[kb]= ROB[k++];
  		}
*/  		
/*  		
  		if ( B->k_IN > (SEG_SIZE-CORR_START))  		
  		{
			for(k=CORR_START, kb=0; kb< SEG_SIZE-CORR_START; k++, kb++)
				SEG[k]= ROB[k];
  		}
  		else 
  		{
			for(k=CORR_START, kb= B->BUF_SIZE-((SEG_SIZE-CORR_START)-B->k_IN); kb< B->BUF_SIZE; kb++)
				B->Buf[kb]= ROB[k++];
			//for(kb=0; k<= kpeak+RSEG; k++, kb++)  
			for(kb=0; k<= SEG_SIZE-CORR_START; k++, kb++)
				B->Buf[kb]= ROB[k++];
  		}
*/  				  		
