#ifndef 	SR_PROC_H
#define 	SR_PROC_H

#include 	"hBUF_SRC.hpp"

class SR_PROC
{
  //= Data members	
  public:
	static	int					DAC_SR; 
	static	int					SAMPLE_RATE; 	
	static	int					SR_ID; 		
	static	int					SR_CHG; 
	static	int					SRTAB_FPGA[6][3]; 
	static	int					INPUT_ACTIVE; 
		
  //======  Methods
   	public:
 static  void					SR_Set(int, int);
// static  void					SR_Read_I2SOFF(void);
 static  void					SRcheck_I2S(int pSR_val, int BUFSIZE);

 static  void  					Check_SR_CHG(int cSR_ID); 
// static  void					x55_Proc(int *msg);
 static  void 					Mute_OutBuf(void);
};

#endif
