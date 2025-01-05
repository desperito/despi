
#include 	"tt.h"
#include 	<stdio.h>


#if (SOFT_V & SHARC_HW)
	#include 	"hI2S_PORTS.hpp"
	#include 	"hSR_MANAGER.hpp"
	#include 	"hI2STX_LINE.hpp"	
   extern 	I2STX_LINE	*Out3A, *Out3B, *Out5A;  	
#endif

//#include 	"hSTAGE_BASE.hpp"
#include 	"hSTAGE_PROC.hpp"
#include  	"hBUF_SRC.hpp"
#include 	"hBUF_MANAGER.hpp"
#include 	"hSIM_SDRAM.hpp"
#include  	"hDSPSTRING.hpp"
#include 	"hPlaySession.hpp"
#include 	"hRESAMPLER.hpp"
#include 	"hSR_PROC.hpp"

                                   
//BUF_MANAGER *PM_BUFS[I2STX_QTY][2];

extern		BUF_MANAGER 		XP[2], DB[2], DM[2], DT[2], RB[2], RM[2], RT[2], ET[2], EB[2]; 	
extern		BUF_MANAGER 		X[2], YM[2]; //YML, YMR, YBL, YBR, YTL, YTR; 						//YLB, YRB; // XL, XR, 
extern		BUF_SRC				X_SRC;
extern 		RESAMPLER  			RSMP[RSMPOBJ_QTY];
//void 		RSMP_EXE(STAGE_PROC *SP);
void 		SM_Proc(STAGE_PROC *SP);


extern 		STAGE_PROC  		SP_RSMP[RSMPOBJ_QTY][CHN_QTY];

extern		int 		LR_state; 

extern 	FILTER  	bSMPre, bSMBas, bSMTwt, bSMMid;


#define			PRE_INPUT		2
#define			BLRAM_INPUT		1
#define			NON_STD_INPUT	0

#define			ZERO_SEG		0

extern STAGE_PROC  	SP_FILT[SPK_QTY+1][CHN_QTY], SP_RSMP[RSMPOBJ_QTY][CHN_QTY]; 

//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================
#include 	"hDSP_MANAGER.hpp"

//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================

//======  Static variables Definition
int					DSP_MANAGER::cStep_DSP, DSP_MANAGER::kch, DSP_MANAGER::ks; 

extern	FILTER		FILT[SPK_QTY+1];

STAGE_PROC			*DSP_MANAGER::Streams[DSP_BLOCK_QTY][CHN_QTY]; 


//================================================================================
//===========  C L A S S   M E T H O D S     I M P L E M E N T A T I O N   =======
//================================================================================

	
//= Configure control table with active Stage Managers corresponding to streams depending on the current device mode
void DSP_MANAGER::Streams_Config(void) 
{
  		int k_step, kch, k, kpre, PRE_ACTIVE= 0;

		
  		for (k_step=0; k_step< DSP_BLOCK_QTY; k_step++)
  		{ 	
  			Streams[k_step][LEFT_0] = NULL; 
  			Streams[k_step][RIGHT_0] = NULL; 
  		}  
	   #if (EP_MODE == EP_PRE)  			   	  		
  		PRE_ACTIVE = DSPSTRING::Get_Digitbit(DSPSTR_EXPH) ? 1 : 0 ;				//PlaySession::PRE_Level & 0x01; //  
  	   #endif	
  	    k = CHN_WORKMODE;
  	    		  		
  		for(kch=LEFT_0; kch <=RIGHT_0; kch++)
  	   	{
  	   		 cStep_DSP = 0;
  	   		if (CHN_WORKMODE & (kch+1))
  	   		{ 
			  //  @@@@@@@@@@@  EXPH - MID  @@@@@@@@@@@@@@	  	  
  	   			if (PRE_ACTIVE)		  					  								// IN-1		//OUT-D  //IN-2    //OUT-E   	   			
  	   				Stream_RuntimeLine(&SP_FILT[PRE][kch], kch, 	BLRAM_INPUT, 		NULL, 		&XP[kch], NULL, NULL, NULL) ; //, 0, 0);
  	   				
  	   		  #if (SOFT_V == SH_XOEP_PROD || SOFT_V == FP_FORD || SOFT_V == LINUX_HW)	  		
	   			switch(PlaySession::DEV_MODE)
	   			{
  					case xCODAC:	case xSPK_DEV:
  					  	switch(SR_PROC::DAC_SR)
  					  	{  					
  					  		case 1: case 2: 					  
  								Stream_RuntimeLine(&SP_FILT[BAS][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE, 	NULL, 		&DB[kch], 	NULL, 		NULL, NULL); //, 0, DSPBLOCK_SIZE2);  						  								
 								Stream_RuntimeLine(&SP_FILT[TWT][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE,	NULL, 		&DT[kch], 	NULL, 	 	NULL, NULL);  // v PRE
  								//Stream_RuntimeLine(&SP_FILT[MID][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE, 	NULL, 		&DM[kch], 	NULL, 		NULL, NULL); //, 0, DSPBLOCK_SIZE2);  						 								
  							   #if (SOFT_V & SHARC_HW)
  	  							Stream_RuntimeLine(&SP_RSMP[TWT][kch], kch, 	NON_STD_INPUT, 				&DT[kch],	&RT[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	&Out3B->PM.YS[kch]);   //&PM_BUFS[I2S1_SPORT3B][kch]); // &PM_BUFS[I2S1_SPORT3B][kch]);
  								Stream_RuntimeLine(&SP_RSMP[BAS][kch], kch, 	NON_STD_INPUT, 				&DB[kch],	&RB[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	&Out3A->PM.YS[kch]);  // &PM_BUFS[I2S0_SPORT3A][kch]) ; //&PM_BUFS[I2S0_SPORT3A][kch]);  								   	 								  	  							
  								//Stream_RuntimeLine(&SP_RSMP[MID][kch], kch, 	NON_STD_INPUT, 				&DM[kch],	&RM[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	&Out3A->PM.YS[kch]);  // &PM_BUFS[I2S0_SPORT3A][kch]) ; //&PM_BUFS[I2S0_SPORT3A][kch]);  								   	 								  	  							
  							   #elif (SOFT_V & LINUX_HW)	
  	  							Stream_RuntimeLine(&SP_RSMP[TWT][kch], kch, 	NON_STD_INPUT, 				&DT[kch],	&RT[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	NULL);   
  								Stream_RuntimeLine(&SP_RSMP[BAS][kch], kch, 	NON_STD_INPUT, 				&DB[kch],	&RB[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	NULL);  
  							   #endif
  							    //= Out5A is linked in I2TX to Out3A  				
  					   		   break; 
  					   		 case 4:
  					   		   #if (SOFT_V & SHARC_HW)
  	  							Stream_RuntimeLine(&SP_RSMP[TWT][kch], kch, 	NON_STD_INPUT, 				NULL,		&RT[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	&Out3B->PM.YS[kch]);   //&PM_BUFS[I2S1_SPORT3B][kch]); // &PM_BUFS[I2S1_SPORT3B][kch]);
  								Stream_RuntimeLine(&SP_RSMP[BAS][kch], kch, 	NON_STD_INPUT, 				NULL,		&RB[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	&Out3A->PM.YS[kch]);  // &PM_BUFS[I2S0_SPORT3A][kch]) ; //&PM_BUFS[I2S0_SPORT3A][kch]);  								   	 								  					   		  	 	
  							   #elif (SOFT_V & LINUX_HW)
  	  							Stream_RuntimeLine(&SP_RSMP[TWT][kch], kch, 	NON_STD_INPUT, 				NULL,		&RT[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	NULL);   
  								Stream_RuntimeLine(&SP_RSMP[BAS][kch], kch, 	NON_STD_INPUT, 				NULL,		&RB[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	NULL);  
							   #endif  							  	
  			  	  	  	      break;
  					  	}  				
  			  	  	   break; //= xCODAC
  				}
  			//*****  DELI_RSMP  *****  	
  			  #elif (SOFT_V & DELI_BASE)
 						Stream_RuntimeLine(&SP_FILT[TWT][kch], 		kch, 	BLRAM_INPUT + PRE_ACTIVE,	NULL, 			&DT[kch], 		NULL, 	 		NULL, 						&Out3B->PM.YS[kch]);    			  
 						//Stream_RuntimeLine(&SP_RSMP[TWT][LEFT_0], 	kch, 	NON_STD_INPUT,				&DT[LEFT_0], 	&RT[LEFT_0], 	&DT[RIGHT_0], 	STAGE_PROC::DELI_DATA_RSMP, &Out3B->PM.YS[LEFT_0]);    			  
 						//Stream_RuntimeLine(&SP_RSMP[TWT][RIGHT_0], 	kch, 	NON_STD_INPUT,				NULL, 			&RT[RIGHT_0], 	NULL, 			STAGE_PROC::DELI_FLAGS_RSMP,&Out3B->PM.YS[RIGHT_0]);    			  
 						
  						Stream_RuntimeLine(&SP_FILT[BAS][kch], 		kch, 	BLRAM_INPUT + PRE_ACTIVE, 	NULL, 			&DB[kch], 		NULL, 			NULL, 						NULL);    												  								
 						Stream_RuntimeLine(&SP_RSMP[BAS][LEFT_0], 	kch, 	NON_STD_INPUT,				&DB[LEFT_0], 	&RB[LEFT_0], 	&DB[RIGHT_0], 	STAGE_PROC::DELI_DATA_RSMP, &Out3A->PM.YS[LEFT_0]);    			  
 						Stream_RuntimeLine(&SP_RSMP[BAS][RIGHT_0], 	kch, 	NON_STD_INPUT,				NULL, 			&RB[RIGHT_0], 	NULL, 			STAGE_PROC::DELI_FLAGS_RSMP,&Out3A->PM.YS[RIGHT_0]);    			    						
						
  	          #elif (SOFT_V == SH_DSP)	
	   			switch(PlaySession::DEV_MODE)
	   			{
  					case xDAC:
  					  // BLRAM_INPUT + PRE_ACTIVE == 2 == PRE_INPUT
  					  	switch(SR_PROC::DAC_SR)
  					  	{
  					  		case 1: case 2:
   								Stream_RuntimeLine(&SP_FILT[TWT][kch], 	kch, 	BLRAM_INPUT+PRE_ACTIVE, 	NULL, 		&DT[kch], NULL, 	STAGE_PROC::FILT1S_PROC, NULL);                            
  								Stream_RuntimeLine(&SP_RSMP[TWT][kch], 	kch, 	NON_STD_INPUT, 				&DT[kch], 	&RT[kch], NULL,		STAGE_PROC::RSMP_CREATE_FILTPROC, NULL) ; // RSMP1S_PROC);  
  								if(DSPSTRING::Get_Digitbit(DSPSTR_EXPH))  	
  								{				
  									Stream_RuntimeLine(&SP_RSMP[PRE][kch], kch, NON_STD_INPUT, 				&RT[kch],	&EB[kch], NULL, 	STAGE_PROC::RSMP_EP_FILT_PROC, NULL);   												
  									//Stream_RuntimeLine(&SP_RSMP[PRE][kch], kch, NON_STD_INPUT, 				&ET[kch],	&DM[kch], NULL, 	STAGE_PROC::RSMP_UPSMP);   
  									if ( DSPSTRING::String[xOVS_EXT_LEVEL] >='2' )
  									{
  										Stream_RuntimeLine(&SP_RSMP[MID][kch], kch, NON_STD_INPUT, 				&EB[kch],	&ET[kch], NULL, 	STAGE_PROC::UPSAMPLE_2x, NULL);												  									  										  									
  										trace("UP-EP ", 0);
  									}
  									trace("EP ", 0);
  								}
  								else if (DSPSTRING::String[xOVS_EXT_LEVEL] >='2' )
  								{								
  									Stream_RuntimeLine(&SP_RSMP[MID][kch], kch, NON_STD_INPUT, 				&RT[kch],	&ET[kch], NULL, 	STAGE_PROC::UPSAMPLE_2x, NULL);												  									  											  								
  									trace("UP-xEP ", 0);
  								}
  								else 
  									trace("\nRSMP ", 0);
  					  		  break;
  					  		case 4:
   								//Stream_RuntimeLine(&SP_FILT[TWT][kch], 	kch, 	BLRAM_INPUT+PRE_ACTIVE, 	NULL, 		&DT[kch], NULL, 	NULL);                              					  		
  								Stream_RuntimeLine(&SP_RSMP[TWT][kch],  kch, 	BLRAM_INPUT+PRE_ACTIVE, 	NULL, 		&RT[kch], NULL, 	NULL, NULL);
  					  		  break;
  					  	}  					  		
  			  	  	  break;
  					case xCODAC:
  					  	switch(SR_PROC::DAC_SR)
  					  	{  					
  					  		case 1: case 2: 			  					  				  
  								Stream_RuntimeLine(&SP_FILT[BAS][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE, 	NULL, 		&DB[kch], 	NULL, 		NULL, NULL); 
  								//Stream_RuntimeLine(&SP_FILT[MID][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE, 	NULL, 		&DM[kch], 	NULL, 		NULL, NULL); //, 0, DSPBLOCK_SIZE2);  						  								
  								Stream_RuntimeLine(&SP_FILT[TWT][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE,	NULL, 		&DT[kch], 	NULL, 	 	NULL, NULL);  
  								
  								Stream_RuntimeLine(&SP_RSMP[BAS][kch], kch, 	NON_STD_INPUT, 				&DB[kch],	&RB[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	NULL) ; // &Out5A->PM.YS[kch]); //  
  								//Stream_RuntimeLine(&SP_RSMP[MID][kch], kch, 	NON_STD_INPUT, 				&DM[kch],	&RM[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	NULL); //&Out3A->PM.YS[kch]);    								   	 								  	  						  								
  								Stream_RuntimeLine(&SP_RSMP[TWT][kch], kch, 	NON_STD_INPUT, 				&DT[kch],	&RT[kch],	NULL, 		STAGE_PROC::RSMP_CREATE_FILTPROC, 	NULL) ; //&Out5A->PM.YS[kch]); //  
  					   		   break; 
  					   		 case 4:
  			  	  	  	     	Stream_RuntimeLine(&SP_FILT[BAS][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE, 	NULL, 		&RB[kch], 	NULL, 		NULL, NULL);  						
  								Stream_RuntimeLine(&SP_FILT[TWT][kch], kch, 	BLRAM_INPUT + PRE_ACTIVE,	NULL, 		&RT[kch], 	NULL, 	 	NULL, NULL); 
  			  	  	  	       break;
  					  	}    				
  						if(DSPSTRING::Get_Digitbit(DSPSTR_EXPH))  					
  						{	
 							Stream_RuntimeLine(		&SP_FILT[MID][kch], kch, 	NON_STD_INPUT, 				&RB[kch],	&ET[kch], 	&RT[kch], 	STAGE_PROC::FSUM_PROC, 				&Out3B->PM.YS[kch]);   // &PM_BUFS[I2S1_SPORT3B][kch]												  							
  							Stream_RuntimeLine(		&SP_RSMP[PRE][kch], kch, 	NON_STD_INPUT, 				&ET[kch],	&DM[kch], 	NULL, 		STAGE_PROC::RSMP_EP_FILT_PROC, 		&Out5A->PM.YS[kch]);   	// &PM_BUFS[I2S0_SPORT3A][kch]											
  						}
  						else	
  						{	
  							Stream_RuntimeLine(		&SP_FILT[MID][kch], kch, 	NON_STD_INPUT, 				&RB[kch],	&DM[kch], 	&RT[kch], 	STAGE_PROC::FSUM_PROC, 				&Out5A->PM.YS[kch]) ; //NULL ); //	
  						}
  				}  	      
  		     #endif  //=  SOFT_V  
  	   		} 	//= if (CHN_WORKMODE & (kch+1))
  	   	}  	  //= for(kch=LEFT_0; kch <=RIGHT_0; kch++)
}


void	DSP_MANAGER::Stream_RuntimeLine(STAGE_PROC *pStream, int CHN, int INPUT_MODE, BUF_MANAGER  *BufIN1, BUF_MANAGER  *pBufD,  BUF_MANAGER  *BufIN2, void (*SP_fun)(STAGE_PROC *SP), BUF_MANAGER **PM_BUF ) //, BUF_MANAGER *pBufE) //, int LOAD_SIZE, int SAVE_SIZE )
{
	int k, Stream_No;
	
	
	Stream_No = pStream->Stream_INDEX;
	
	switch(INPUT_MODE)
	{
		case BLRAM_INPUT:
			pStream->BufIN[0] = &X[0];
			pStream->BufIN[1] = NULL;
			if (pStream->FiltRsmp->DSP_TYPE == DSP_FILT)
				pStream->DSP_proc = STAGE_PROC::FILTX2S_PROC;
			else
				pStream->DSP_proc = STAGE_PROC::RSMPX2S_PROC;			
		  break;
		case PRE_INPUT:
			pStream->BufIN[0] = &XP[CHN];
			pStream->BufIN[1] = NULL;				
			if (pStream->FiltRsmp->DSP_TYPE == DSP_FILT)
				pStream->DSP_proc = STAGE_PROC::FILT1S_PROC;
			else
				pStream->DSP_proc = STAGE_PROC::RSMP_CREATE_FILTPROC; // RSMP1S_PROC;						
		  break;
		case NON_STD_INPUT:
			pStream->BufIN[0] = BufIN1;
			pStream->BufIN[1] = BufIN2;								
			pStream->DSP_proc = SP_fun;
		  break;		  
	}
	
	pStream->BufD = pBufD;
									
	Streams[cStep_DSP][CHN]= pStream;
  	++cStep_DSP;

	*PM_BUF = pBufD;  
}


	extern float 	PACK_DIFF;

	long long 		j=0, m;
	extern void PRINT_kBUFS(void);

//===================================================================================================
//#pragma optimize_off
void DSP_MANAGER::RunDSP(void)
{
	
	for (kch=0; kch< CHN_QTY; kch++)
	{	  			
		for (ks=0; ks< DSP_BLOCK_QTY; ks++)
		{
			//LR_state = kch+1; 
		   //= Process active combinations of Streams and L/R channels, i.e. configured in the Streams control table  			
			if (Streams[ks][kch] != NULL ) //&&  ( (kch+1) & CHN_WORKMODE) )  //Streams[ks].inblock[kch] != NULL)								
			{
				Streams[ks][kch]->DSP_ChannelProc();
			} 
		}
	}
	++j;
	if (!RT[0].Buf[1000])
		 m= j+1;
	if (0) // (j>30)  //	 	
	{
		sprintf(dbgtab,"\n%s%.1f%s%.1f%c", "P:", PACK_DIFF/1000, " S:", X_SRC.DIFF/1000, ' ');
		trace(dbgtab, 0);
		PRINT_kBUFS();
		j=0; 
	}									
}

