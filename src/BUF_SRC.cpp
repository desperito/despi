////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#include 	<stdio.h>
#include 	"tt.h"
#include  	"SIM.h"
#include  	"hBUF_MANAGER.hpp"
#include  	"hSR_PROC.hpp"
#include  	"hPlaySession.hpp"

#include  	"hRESAMPLER.hpp"
	
//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================

#include	"hBUF_SRC.hpp"
	
	BUF_SRC					X_SRC, PIX1; // PIX2; 	
	
//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================

extern void New_Playrun_Config(void);

void BUF_SRC::Init(BUF_MANAGER *pBuf, int pMINDIST_XGET)
{
	int START_PUT, kFLOW, start; 
	
		Buf1 = pBuf; Buf2 = pBuf+1;
		
		if( (PlaySession::WORK_CONF & RUNMODE_MASK )== PROD_MODE )
		{
			switch(SR_PROC::DAC_SR)
			{
				case 1:
					LEVEL_MIN 		= 1E+4; 
					LEVEL_MAX 		= 3E+4; 				
					start			= 3.5E+4 ;
				  break;
				case 2:
					LEVEL_MIN 		= 2E+4; 
					LEVEL_MAX 		= 4E+4; 								
					start 			= 5E+4;  									
				  break;
				case 4:
					LEVEL_MIN 		= 4E+4; 
					LEVEL_MAX 		= 5E+4;
					start			= 5E+4;  								
				  break;
			}
			Buf1->k_INBUFS 	= start/Buf1->BUF_SIZE;				
			Buf1->k_IN 		= start- Buf1->k_INBUFS*Buf1->BUF_SIZE; 												
		} 
		//else 	
		//	Buf1->k_IN = 0;
			
		MINDIST_XGET = pMINDIST_XGET;
	  //= Since there are two channels in one X buffer the right flow should start with offset of 1 (RIGHT_0) (interleaved stereo samples)
		for(kFLOW=0; kFLOW<FLOW_QTY; kFLOW++) 
			Buf1->Flow[RIGHT_0][kFLOW][K_GET_FLOW]= RIGHT_0;
}

	
//template<typename T>		
void 	BUF_SRC::PUT_SRC(int DATA_QTY, int *Data, int DATA_STEP)
{
   //
	Calc_kGET();  // REFACTOR ? - de facto uzywane docelowo tylko w IPL_TUNE, teraz w SH_V ma jeszcze zwiazek z wykorzystaniem w RX_LINE do wyswietlania kontrolnego poziomu IPL_TUNE
	Buf1->PUTi(DATA_QTY, Data, DATA_STEP); 	
}	

//= INTEGER OVERFLOW to refactor
void 	BUF_SRC::Calc_kGET(void)
{
	int kf, kch, kch_max, kf_max=-1; 
	
  #if	(SOFT_V & SHARC_HW)
  	long long	kval, kval_max=0;
  #else  // RPI_V 
	int			kval, kval_max=0;
  #endif		
	
	for(kf=0; kf< FLOW_QTY; kf++)
	{	for(kch=0; kch< CHN_QTY; kch++)
		{
			if (Buf1->Flow[kch][kf][K_GET_FLOW] >0 || Buf1->Flow[kch][kf][K_GETBUF_FLOW] >0)  //check the Flow is used
			{ 	
				kval = Buf1->Flow[kch][kf][K_GETBUF_FLOW]*Buf1->BUF_SIZE + Buf1->Flow[kch][kf][K_GET_FLOW];
				if(kval> kval_max)
				{
					kch_max = kch;
					kf_max= kf;
					kval_max = kval;
				}
			}
		}
	}
	k_GET = Buf1->Flow[kch_max][kf_max][K_GET_FLOW];
	k_GETBUFS = Buf1->Flow[kch_max][kf_max][K_GETBUF_FLOW];
}

#define		IPL_TUNE_LAG		500
#define		RSMP_SHOW 			500

int			kOV, kUN, dUN, dOV; 

void BUF_SRC::IPL_TUNE(void)
{
	int		k;
	
  #if	(SOFT_V & SHARC_HW)
	long long	put_level, get_level;
  #else   // RPI_V
	int			put_level, get_level;
  #endif
  	
	put_level = Buf1->k_INBUFS*Buf1->BUF_SIZE + Buf1->k_IN; 
	get_level = k_GETBUFS*Buf1->BUF_SIZE + k_GET; 
	DIFF =  	put_level - get_level; 	//  (Buf1->k_INBUFS-k_GETBUFS)*Buf1->BUF_SIZE + Buf1->k_IN- k_GET;

  if(get_level >150000)
  {
	if ( DIFF > LEVEL_MAX)
	{	
		if (RESAMPLER::Level < RESAMPLER::MAX_LEVEL  && dOV >IPL_TUNE_LAG)	
		{
			RESAMPLER::LevelChg(+1);
	       #if (BUFSRC_UART_TRACE == ON)
			//trace("R-UP",0);
	       #endif
			dOV= 0;
		}
		else
		{ 
			++kOV; kOV%=RSMP_SHOW; 
			++dOV; 
		   #if (BUFSRC_UART_TRACE == ON) 	
			if (!kOV) trace("R-OVF",0);
		   #endif	
		}		
	}
	else if (DIFF < LEVEL_MIN)
	{	
		if (RESAMPLER::Level >0 && dUN>IPL_TUNE_LAG)	
		{
			RESAMPLER::LevelChg(-1);
		   #if (BUFSRC_UART_TRACE == ON) 	
			//trace("R-DOWN",0);
	       #endif					
			dUN = 0;
		}
		else 
		{
			++kUN; kUN%=RSMP_SHOW; 
			++dUN; 
		   #if (BUFSRC_UART_TRACE == ON) 				
			if (!kUN) trace("R-OVF",0);
		   #endif
		}
	}
  }
}

//####    END OF CODE   ############

