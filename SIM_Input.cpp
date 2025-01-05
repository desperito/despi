
#include 	"tt.h"
#include 	<stdio.h>

#if (SOFT_V & SHARC_HW)
	#include 	"hI2SRX_LINE.hpp"
	#include 	"hSR_MANAGER.hpp"
#else
	#include	<time.h> // REMOVE ?? 
#endif

#include 	"hPlaySession.hpp"
#include 	"hDSPSTRING.hpp"
#include 	"hSR_PROC.hpp"

#include 	"hBUF_MANAGER.hpp" 
#include 	"hBUF_SRC.hpp" 

#include 	"SIM.h" 
	

	extern 	BUF_MANAGER 		X[2]; 
	extern 	BUF_SRC 			X_SRC; 
	
	extern		int				k_simaudioin_isr;	


#define		SIM_LONG				1
#define		SIM_SHORT				2

#define		SIM_LEN					SIM_SHORT //SIM_LONG // 

#define		xKEFQ350			1
#define		xSPKDSP 			xKEFQ350

#if 		(WORK_CONFIG &  RUNMODE_MASK) == PROD_MODE  
	#if	(SOFT_V == FP_DSP || SOFT_V == FP_TWT2RX) 
		#include 	"HP_DIRAC_TAB24_10k.h"
	#elif	(SOFT_V == FP_XOEP_TEST)
		#include 	"C:\__PROJEKT_2020\Kolumny\SPKIR_TABS.h"  		 		
		#include 	"HP_DIRAC_TAB24_10k.h"		
	#else
		#if		SIM_LEN	== SIM_LONG
			#include 	"HP_DIRAC_TAB24_33k.h"
		#else
			#include 	"include\HP_DIRAC_TAB24_40k_SHORT.h"
		#endif
	#endif
#else  
	#include 	"HP_DIRAC_TAB24_40k.h"
//		#include 	"HP_DIRAC_TAB24_40k_SHORT.h"	
	//#include 	"P1_ST48.h"  //"DF_SA48.h"
	#include 	"C:\__PROJEKT_2020\Kolumny\SPKIR_TABS.h"  
#endif
	
#define		LVOL					2E+2*32768 // MAXINT24// MAXINT24_2
#define		RVOL					2E+2*32768 //(MAXINT24/10)
#define		LRTEST_DIFFSCALER		2

	
//================================================================================
//   ===========      C L A S S   D E C L A R A T I O N   ===================
//================================================================================
#include   <hSIM_INPUT.hpp>

//===  S T A T I C   M E M B E R S   D E F I N I T I O N   =============================================
 //= Static members definition 	
 	int 								SIM_INPUT::tabsize = 0; 
								int 	*SIM_INPUT::SIML;
								int 	*SIM_INPUT::SIMR;
//	SECTION(("seg_sdram3")) 	int		SIM_INPUT::SIM_LRTAB[sizeof(HP_DIRAC_TAB24_40k)/sizeof(int)][2];	
	
	SECTION(("seg_sdram2")) 	int	  	SIM_INPUT::LRVOL[2]= {LVOL, RVOL} ;	
	

 	int								SIM_INPUT::tabpos=0 ; // , SIM_INPUT::PONG= 0, SIM_INPUT::OUT_POS= 0, SIM_INPUT::OUT_PONG= 0, SIM_INPUT::clock_in, SIM_INPUT::clock_out ;	
 #if (SOFT_V & SHARC_HW) 		
 	I2SRX_LINE						*SIM_INPUT::SIM_I2SRX;
 #endif	
//======================================================================================================

void SIM_INPUT::Signal_Init(int SIGNAL_MODE)
{
	
	if (PlaySession::RECORDING_MODE == SIM_I2SOFF_D_ONLY)
	{	
		//Print_Debug( sprintf(dbgtab, DSPSTRING::String), NOTIME );		
		//Print_Debug( sprintf(dbgtab, "\nSIM Dump\n"), NOTIME );		
	}
   #if (SOFT_V & SHARC_HW) 		
	SIM_I2SRX->BUF_seg = 0;		 	
   #endif		
	tabpos = 0; 
	
   //= Table values and sim_block_qty initialization
	if (SIGNAL_MODE == HP_DIRAC)	
	{
		SIML= HP_DIRAC_TAB24_40k; //HP_DIRAC_TAB;		
		//sim_block_qty= 	(sizeof(HP_DIRAC_TAB24_40k)/sizeof(int))/(SMPBLOCK_SIZE* SR_PROC::DAC_SR);
		tabsize= 	sizeof(HP_DIRAC_TAB24_40k)/sizeof(int);
		
		SIM_INPUT::LRVOL[LEFT_0] = 	HP_DIRAC_TAB24_40k[3]; 
		SIM_INPUT::LRVOL[RIGHT_0] = HP_DIRAC_TAB24_40k[3]/LRTEST_DIFFSCALER; 		
	}	
#if 	( ( WORK_CONFIG &  RUNMODE_MASK) == TEST_MODE  )  //&&  ( SIGNAL_MODE == SPK_IR	)
	else if (SIGNAL_MODE == SPK_IR)	
	{
		switch (SR_PROC::SAMPLE_RATE)
		{
			case 	CD44_16:	SIML= SPKIR_44; 	tabsize= sizeof(SPKIR_44)/sizeof(int);	break;		
//			case 	CD48_16:	SIML= DF_PAZ_SA48; 	tabsize= sizeof(DF_PAZ_SA48)/sizeof(int);	break;		
			case 	CD48_16:	SIML= SPKIR_48; 	tabsize= sizeof(SPKIR_48)/sizeof(int);	break;			
			case 	HD88_24:	SIML= SPKIR_88; 	tabsize= sizeof(SPKIR_88)/sizeof(int);	break;				
			case 	HD96_24:	SIML= SPKIR_96; 	tabsize= sizeof(SPKIR_96)/sizeof(int);	break;		
			case 	UD176_24:	SIML= SPKIR_17; 	tabsize= sizeof(SPKIR_17)/sizeof(int);	break;		
			case 	UD192_24:	SIML= SPKIR_19; 	tabsize= sizeof(SPKIR_19)/sizeof(int);	break;						
		}
		SIM_INPUT::LRVOL[LEFT_0] = 	8388600;
		SIM_INPUT::LRVOL[RIGHT_0] = SIM_INPUT::LRVOL[LEFT_0]/LRTEST_DIFFSCALER;  			
	}
#endif	
	SIMR= SIML; 
}	

void SIM_INPUT::AudioIN_ISR(int dummy)
{
	int k,SRC2, SIM_SIZE ;
	
#if (SOFT_V & SHARC_HW) 		
  #if (IN1_ACTIVE ==1) 				
		SRC2 = (SIM_I2SRX->BUF_cnt+1)%2;  //= needed to be synchronized with BUF_cnt that is incremented in the beginning of AudioIN_ISR() called below  
  #endif
	SIM_SIZE= PlaySession::_SMPBLOCK_SIZE* SR_PROC::DAC_SR;		
	for(k=0; k<SIM_SIZE; k++, tabpos++)
	{
		if (tabpos< tabsize)
		{
			SIM_I2SRX->sportbuf[SRC2][2*k]=		SIML[tabpos]/2;
			SIM_I2SRX->sportbuf[SRC2][2*k+1]=	SIML[tabpos]/LRTEST_DIFFSCALER;
		}
		else  // when the input table is used up continue with the last value in the table
		{
			SIM_I2SRX->sportbuf[SRC2][2*k]= 	SIML[tabsize-1] ;
			SIM_I2SRX->sportbuf[SRC2][2*k+1]=	SIML[tabsize-1];
		}										
   		//++POS ;
		//if  ( SIGNAL_MODE != HP_DIRAC && SIGNAL_MODE != SPK_IR)
			//POS%= sim_block_qty;			
	}	
	// #elif dot. RPI, przesuniete na koniec 	
   //= Call to the actual real-time I2S RX procedure to simulate real-time
//   #if (SOFT_V & SHARC_HW) 	 
  	SIM_I2SRX->AudioIN_ISR(0);
#endif
}


//  #elif	(SOFT_V == RPI_MIX_V) 			
//		X_SRC.PUT_SRC(SIM_SIZE*RAMSEG_QTY, (int *) (SIM_LRTAB+POS*SIM_SIZE*RAMSEG_QTY), 2);
//		BLRAM_Ready = 1;		
//  #elif	(SOFT_V == RPI_V) 			
//	 // Ekrem code to feed buffers in X_SRC 
//	 // a new implementatio of BUF_SRC::PUT_SRC(int DATA_QTY, int *Data, int DATA_STEP) is needed here 		
//  #endif
