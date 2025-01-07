#ifndef DSP_MANAGER_H
#define DSP_MANAGER_H

#include 	"hBUF_MANAGER.hpp"

class DSP_MANAGER {
//********************************************************************************	
public: //= INTERFACE_METHODS
   //= Device Mode level change stream execution reconfiguration with Streams table used for Runtime Control 
  	static void 			Streams_Config(void);  	// Used for initial start-up and DEVICE_RUNMODE changes
   //= Main DSP Block Process Call when input data blocks are ready
	static void				RunDSP(void);  	
private: //= Stream runtime configuration for  implementation
  		static void		  	Stream_RuntimeLine(STAGE_PROC *pStream, int CHN, int INPUT_MODE, BUF_MANAGER  *BufIN1, BUF_MANAGER  *pBufD,  BUF_MANAGER  *BufIN2, void (*SP_fun)(STAGE_PROC *SP), BUF_MANAGER **PM_BUF); //, BUF_MANAGER *pBufE);

//********************************************************************************
public:  // DATA
  	static STAGE_PROC		*Streams[DSP_BLOCK_QTY][CHN_QTY]; 
private: // INTERNAL DATA Common		
  	static int				cStep_DSP;
//= Solely to implement CStream_Outblock() for use in OVSEXT  	
  	static	int				ks, kch;
};	

#endif
