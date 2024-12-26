#ifndef BUF_SPEC_H
#define BUF_SPEC_H

#include "hBUF_MANAGER.hpp"

class BUF_SRC // : public BUF_MANAGER
{
 public:  //Attributes
		BUF_MANAGER 		*Buf1, *Buf2;
		
		int					XLOAD_SIZE;
		
		float				LEVEL_MIN; 
		float				LEVEL_MAX; 
		int					MINDIST_XGET;

		int					k_GETBUFS;
		int					k_GET; 
		float				DIFF;		
	   				
		int 				CLK_msg;				
 public:  //Methods
 	void					Init(BUF_MANAGER *pBuf, int pMINDIST_XGET  );
	void 					PUT_SRC(int DATA_QTY, int *Data, int DATA_STEP); 	
	void 					PUT_PI(int DATA_QTY, int *Data); 
  //== Ekrem to call		
	void 					PUT_DMA(int DATA_QTY, int *Data); 		
	
	void 					Calc_kGET(void); 	
		
	void 					IPL_TUNE(void); 	
 			
}; 		
#endif
