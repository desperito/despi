#ifndef SIM_INPUT_H
#define SIM_INPUT_H

#include	"tt.h"

#if (SOFT_V & SHARC_HW)
#include 	"hI2SRX_LINE.hpp"
#endif

//extern 		int HP_DIRAC_TAB24_40k[]; 
#define 	SIMDATA_SIZE			10000 //100000 //RECBUF_SIZE

class SIM_INPUT	{
//********************************************************************************	
public:
//static	SIM_SDRAM	* const ptr;
  //= Data members
	static	int			tabsize;		
	static 	int			LRVOL[2];	
  //= Methods		
  	static	void 		AudioIN_ISR(int);
	static	void 		Signal_Init(int signal);
  	
	static int			*SIML, *SIMR; 	
//	static int 			SIM_LRTAB[][2];		
	
  #if	(SOFT_V & SHARC_HW) 	
	static	I2SRX_LINE	*SIM_I2SRX;
  #endif
//********************************************************************************
private:		
  //= Data members		
	static 	int			tabpos;  
  //= PRIVATE Methods	
};	
#endif 


