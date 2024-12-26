
#include "tt.h"
#include <stdio.h>
#include <math.h>


//#include  "_Sharc_EXPH_DEF.h"
//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================
#include 	"hBUF_MANAGER.hpp"
#include 	"hSIM_SDRAM.hpp"
#include  	"hPlaySession.hpp"
#include 	"hSR_PROC.hpp"
#include 	"hRESAMPLER.hpp"
#include 	"hDSP_MANAGER.hpp"
#include 	"hDSPSTRING.hpp"
#include 	"hPOLY.hpp"

#if (SOFT_V & SHARC_HW)
	#include  	"hSR_MANAGER.hpp"
#endif


//= Main input and output data buffers for DSP processing 
SECTION(("seg_dmda")) 	float  		RoboDSP0[DSPBUF_SIZE] = {0};  
SECTION(("seg_dmda")) 	float 	 	RoboDSP1[DSPBUF_SIZE] = {0};   

extern 	FILTER	  		FILT[FILTOBJ_QTY]; 
extern 	RESAMPLER  		RSMP[RSMPOBJ_QTY];

extern 	BUF_MANAGER  	X[2];

void 	FILT_EXE(STAGE_PROC *SP);
//void 	FILTRSMP_PROC(STAGE_PROC *SP); 
void 	RSMP_CREATE_FILT_EXE(STAGE_PROC *SP); 
void 	RSMP_EP_FILT_EXE(STAGE_PROC *SP); 

//void 	RSMP_EXE(STAGE_PROC *SP);
void 	EPRT_EXE(STAGE_PROC *SP);

//================================================================================
//===========  C L A S S   D E C L A R A T I O N          ======================
//================================================================================

#include  	"hSTAGE_PROC.hpp"

//================================================================================
//===========  ATTRIBUTES  DEFINITION           ======================
//================================================================================

//================================================================================
//===========  O B J E C T S   DEFINITION           ======================
//================================================================================
	
STAGE_PROC  	SP_FILT[SPK_QTY+1][CHN_QTY];
STAGE_PROC  	SP_RSMP[RSMPOBJ_QTY][CHN_QTY];

//================================================================================
//===========          M E T H O D S            ======================
//================================================================================

void STAGE_PROC::Constructor(void)
{
	float 	*Rob0, *Rob1;
	int		k, kch, k_sp;
	
	Rob0= RoboDSP0+xTAIL_ITPL; Rob1= RoboDSP1+xTAIL_ITPL;
	for(kch=0;kch< CHN_QTY;kch++)
		for(k_sp=0;k_sp< FILTER::OBJ_QTY;k_sp++)
			SP_FILT[k_sp][kch].Init(Rob0, Rob1, 0, 	0, 	&(FILT[k_sp]), k_sp, kch); // , FILT_EXE);	
	
	for(kch=0;kch< CHN_QTY;kch++)
		for(k_sp=0;k_sp< RESAMPLER::OBJ_QTY;k_sp++)
			SP_RSMP[k_sp][kch].Init(Rob0, Rob1, 0, 	0, 	&(RSMP[k_sp]), k_sp, kch); //, RSMP_EP_FILT_EXE); //RSMP_EXE);	
			
	//for(kch=0;kch< CHN_QTY;kch++)
	//	SP_RSMP[PRE][kch].Init(Rob0, Rob1, 0, 	0, 	&(RSMP[PRE]), PRE, kch, RSMP_EP_FILT_EXE); // EPRT_EXE);	 // 
			
}
				
void STAGE_PROC::Init(float *stage_ptr0, float *stage_ptr1, int pX_GET_SIZE, int pSAVE_SIZE, FILTER  *pDSP, int pStream_INDEX, int pCHN) //, void (*pDSP_proc)(STAGE_PROC *SP))
{	
	int k;
	
	stage_ptr[0]	= stage_ptr0; 
	stage_ptr[1]	= stage_ptr1;
	X_GET_SIZE 		= pX_GET_SIZE;
	SAVE_SIZE 		= pSAVE_SIZE;
	FiltRsmp 		= pDSP;
	Stream_INDEX 	= pStream_INDEX;
	Chn 			= pCHN; 
	DSP_proc 		= NULL; // pDSP_proc;  Filled in by DSP_Manager Streamlines
	
}

void STAGE_PROC::DSP_ChannelProc(void)
{
	int 			i, j, k,m, len_vol, len_ovs, ovs_offset, stage_ovs;
	
	for (i=0; i<DSPBUF_SIZE; i++)
	{	
 		*(&stage_ptr[0][i]-xTAIL_ITPL)	= 0;	
		*(&stage_ptr[1][i]-xTAIL_ITPL)	= 0;
	}
	stage_cnt = 0;
	(*DSP_proc)(this); 
	
}  //== END DSP_Channel_Proc()

int 	kx; 

void STAGE_PROC::DUMMY(STAGE_PROC *SP) 
{
	return; 
 	}   

void STAGE_PROC::FILTX2S_PROC(STAGE_PROC *SP) 
{
  // Data load from interleaved X buffer and FILT processing
	if (SP->BufIN[No1_INPUT_BUF]->GETF2(SP->FiltRsmp->LOAD_SIZE, SP->stage_ptr[SP->stage_cnt], SP->BufD->SIM_MASK, SP->Chn ))
		FILT_EXE(SP);	 
}

void STAGE_PROC::RSMPX2S_PROC(STAGE_PROC *SP) 
{
	
	if (SP->BufIN[No1_INPUT_BUF]->GETF2(SP->FiltRsmp->LOAD_SIZE, SP->stage_ptr[SP->stage_cnt], SP->BufD->SIM_MASK, SP->Chn ))
		RSMP_CREATE_FILTPROC(SP);  // RSMP_EP_FILT_EXE(SP); //RSMP_EXE(SP); // 	 
}

void STAGE_PROC::FILT1S_PROC(STAGE_PROC *SP) 
{
	
	if (SP->BufIN[No1_INPUT_BUF]->GETF(SP->FiltRsmp->LOAD_SIZE, SP->stage_ptr[SP->stage_cnt], SP->BufD->SIM_MASK ) )
		FILT_EXE(SP);
}

void STAGE_PROC::FILT_FPTEST(STAGE_PROC *SP) 
{
	
	SP->BufIN[No1_INPUT_BUF]->GETF(2560, SP->stage_ptr[SP->stage_cnt], SP->BufD->SIM_MASK );
	SP->BufD->PUTf(2560, SP->stage_ptr[SP->stage_cnt], 1);
}

void STAGE_PROC::RSMP_CREATE_FILTPROC(STAGE_PROC *SP) // RSMP1S_PROC(STAGE_PROC *SP) //  
{
	
	if (SP->BufIN[No1_INPUT_BUF]->GETF(SP->FiltRsmp->LOAD_SIZE, SP->stage_ptr[SP->stage_cnt], SP->BufD->SIM_MASK ) )
		RSMP_CREATE_FILT_EXE(SP); // RSMP_EP_FILT_EXE(SP); //RSMP_EXE(SP);	  // 
}


void STAGE_PROC::UPSAMPLE_2x(STAGE_PROC *SP) // RSMP1S_PROC(STAGE_PROC *SP) //  
{
	int k;
	
	for (k=0;k<2; k++)
		if (SP->BufIN[No1_INPUT_BUF]->GETF(DSPBLOCK_SIZE2/2, SP->stage_ptr[SP->stage_cnt], SP->BufD->SIM_MASK ) )
		{
			switch (DSPSTRING::String[IPL_EXT])
			{
				case 'l':  case 'z':
					POLY::LIN2x(SP);					
			  	  break;
				case 'g':	case 'b':
					POLY::IPL_H6(SP);
			  	  break;			  
			}
		}
}

/*
void STAGE_PROC::RSMP_CREATE_FILTPROC(STAGE_PROC *SP) 
{
	if (SP->BufIN[No1_INPUT_BUF]->GETF(SP->FiltRsmp->LOAD_SIZE, SP->stage_ptr[SP->stage_cnt], 1, SP->BufD->SIM_MASK ) )
		RSMP_CREATE_FILTPROC(SP);
		
}
*/


void STAGE_PROC::RSMP_EP_FILT_PROC(STAGE_PROC *SP) 
{
	int pass_no = 1;
	
//	if (SP->BufIN[No1_INPUT_BUF]->GETF(SP->FiltRsmp->LOAD_SIZE, SP->stage_ptr[SP->stage_cnt], 1, SP->BufD->SIM_MASK ) )
//	if (SP->BufIN[0]->GETF_EX(SP, SP->FiltRsmp->SAVE_SIZE, 1, pass_no) )
		RSMP_EP_FILT_EXE(SP);	 
}

void STAGE_PROC::FSUM_PROC(STAGE_PROC *SP) 
{

	if ( SP->BufIN[No1_INPUT_BUF]->GET2(SP->FiltRsmp->LOAD_SIZE, SP->stage_ptr[SP->stage_cnt], SP->stage_ptr[(SP->stage_cnt+1)%2], SP->BufIN[No2_INPUT_BUF], SP->BufD->SIM_MASK) )
		FILT_EXE(SP);	 
}




void STAGE_PROC::DELI_DATA_RSMP(STAGE_PROC *SP) 
{
	int 			j, j0, DATA_QTY, DATA_STEP=1, cnt1;
	float 	 		*Dataf, *Dataf1;
	BUF_MANAGER		*BM; 	  
	
	
	cnt1 = (SP->stage_cnt+1)%2; 
	
	DATA_QTY = SP->FiltRsmp->LOAD_SIZE;
	if (SP==&SP_RSMP[BAS][LEFT_0])
	{	for(j=0; j<	SR_PROC::DAC_SR+1; j++)
			DATA_STEP*= 2;
	}
	else
		DATA_STEP= 2;	
	BM = SP->BufD; 
	Dataf =  RoboDSP0; //SP->stage_ptr[SP->stage_cnt];
	Dataf1 = RoboDSP1; // SP->stage_ptr[cnt1];
	 
	SP->BufIN[No1_INPUT_BUF]->GETF(SP->FiltRsmp->LOAD_SIZE, Dataf,	SP->BufD->SIM_MASK );
	SP->BufIN[No2_INPUT_BUF]->GETF(SP->FiltRsmp->LOAD_SIZE, Dataf1, 	SP->BufD->SIM_MASK );
	
	for(j=BM->k_IN, j0=0; j0< DATA_QTY && j+1 < BM->BUF_SIZE;j0++, j+=DATA_STEP) 
	{
		BM->Buf[j] = 	Dataf[j0] ;
		BM->Buf[j+1] = 	Dataf1[j0] ;
	}		
	if (j0< DATA_QTY)
	{
		j = 0 ; 	++BM->k_INBUFS;   		
		for(; j0< DATA_QTY ; j0++, j+=DATA_STEP)
		{
			BM->Buf[j] = 	Dataf[j0] ;
			BM->Buf[j+1] = 	Dataf1[j0] ;
		}						  		
	}	 		
	BM->k_IN = j; 	
}

void STAGE_PROC::DELI_FLAGS_RSMP(STAGE_PROC *SP) 
{
	int 	j, j0, k, DATA_STEP=1, DATA_QTY;
	BUF_MANAGER		*BM; 	  
	
		
	DATA_QTY = SP->FiltRsmp->LOAD_SIZE;	
	if (SP==&SP_RSMP[BAS][RIGHT_0])
	{	for(j=0; j<	SR_PROC::DAC_SR+1; j++)
			DATA_STEP*= 2;
	}
	else
		DATA_STEP= 2;	
	BM = SP->BufD; 
	
	for(j=BM->k_IN, j0=0; j0< DATA_QTY && j+1 < BM->BUF_SIZE;j0++, j+=DATA_STEP) 
	{
		BM->Buf[j] = 	0;
		BM->Buf[j+1] = 	1;
		for (k=0; k<DATA_STEP-2; k++)
			BM->Buf[j+2+k]= 256+k; 
	}		
	if (j0< DATA_QTY)
	{
		j = 0 ; 	++BM->k_INBUFS;   		
		for(; j0< DATA_QTY ; j0++, j+=DATA_STEP)
		{
			BM->Buf[j] = 	0;
			BM->Buf[j+1] = 	1;
			for (k=0; k<DATA_STEP-2; k++)
				BM->Buf[j+2+k]= 256+k; 
		}						  		
	}	 		
	BM->k_IN = j; 	
}


