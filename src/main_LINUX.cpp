
#include 	"tt.h"	// Ekrem tt.h must be on top
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#if (SOFT_V == LINUX_HW )
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

 #include 	"hUARTBUF.hpp"
 #include 	"hDSP_MANAGER.hpp"
 #include 	"hPlaySession.hpp"
 #include  	"hSR_PROC.hpp"
 #include 	"hPACK_MAN.hpp"
 #include 	"hRESAMPLER.hpp"
 #include 	"hDELI_CMD.hpp"


extern 	int			I2SOUT_Ready;

//******  EXTERNS  **********

extern	BUF_SRC			X_SRC;
extern	RESAMPLER   	RSMP[RSMPOBJ_QTY]; 	

 
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ---------------------  		M A I N   C O D E   	--------------------------------------
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


extern void test(void);
//extern 	I2SRX_LINE	*In2;

extern void	PowerOn_setup(void);

// Ekrem: CPP EXPECTS main to be return int

extern 		STAGE_PROC		SP_RSMP[][CHN_QTY], SP_FILT[][CHN_QTY];
extern		BUF_MANAGER		DB[CHN_QTY];
//extern		VOL_MAN			VOM[SPK_QTY][CHN_QTY];


#if  (SOFT_V == FP_DSP || (SOFT_V & FP_TEST2RX) == FP_TEST2RX ) 
 int 	DUMP_ENABLE = 0;
#endif

 #if	(SOFT_V & SHARC_HW)
   void 	Sharc_startup(void);	
 #endif	
//#if	(SOFT_V & LINUX_HW) 		

extern MCEP_T			MCEP[2];

//#include 	"RES_FIRS.h"

void DSP_Linux(void)
{
 	if	(PACK_MAN::YSPACE_OK) // I2SOUT_Ready) // I2SOUT_Ready - rhythm of DSP processing is set by output - each new output consumed by I2S block sets the flag and launches the DSP processing
    {	
 		DSP_MANAGER::RunDSP(); 							//DSP_MANAGER_RunDSP();
   	   #if	(!(SOFT_V & DELI_RSMP)) 		     
		if (RESAMPLER::ACTIVE)
			X_SRC.IPL_TUNE(); 	
	   #endif								
    	if (PlaySession::ZERORUN ==2 && PlaySession::PLAY_MODE == PLAY_ON)
    	{
    		//DSP_MANAGER_BufClr3();		// Stage and DSP-related buffers clean							
			PlaySession::ZERORUN =0;
    	}		
  	} 		   			
}

 void Deli_Linux_mainproc(void);
 
//=============================================
//  =======   MAIN  ============
//=============================================


 int main(void)   
 {	   
	PowerOn_setup();
	
	DELI_CMD::LED_Color(0x1111, 0x000000);
   			
	while(DELI_CMD::frame_no ==0)
		UARTBUF::UART_Monitor_NEW();
	  	   		  	 	
 // -------------------  M A I N   L O O P    --------------------------------	
	while(1)
	{   					
	  Deli_Linux_mainproc();				
   	}  //= while(1)   		
	return 0;
 } // main()
#endif
