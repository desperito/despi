#ifndef BUF_MANAGER_H
#define BUF_MANAGER_H

#include	"hSTAGE_PROC.hpp"

class BUF_MANAGER 			//: public XTAB  
{
public:
		BUF_MANAGER(float *pBUF, int pBUF_SIZE, int START_PUT, int START_GET);
		BUF_MANAGER(void);
	
	friend class DSP_MANAGER;
	friend class STAGE_PROC; 
//********************************************************************************	

//***************************************************************************************************************************************	
//= Stage Initialization Implementation
//***************************************************************************************************************************************
public: 	//= ATTRIBUTES
		int			(*bStart_DSP_Delay)(void);
				
public: 	//= INTERFACE_METHODS  -Initialization- 
static	void 		BUF_MANAGER_INIT(void);
		void 		Init(float *pfBUF, int *piBUF, long long pBUF_SIZE, int START_PUT, int START_GET, int pSIM_MASK, FILTER *Delay_FILTER);
		// void 		PUT(int DATA_QTY, float *Dataf, int *Datai, int DATA_STEP);
//template<typename TP>		
		void 		PUTf(int DATA_QTY, float *Data, int DATA_STEP);
		void 		PUTi(int DATA_QTY, int *Data, int DATA_STEP); 
		void 		PUTiX(int DATA_QTY, int *Data, int DATA_STEP, int rep);
//template<typename TG>		
		int 		GETF(int DATA_QTY, float *Data, int BUF_MASK_ID);
		int 		GETF2(int DATA_QTY, float *Data, int BUF_MASK_ID, int chn);
		int			GETF_EX(STAGE_PROC *SP, int stage_no, int STEP);
		int			GET2_EX(STAGE_PROC *SP, int LOAD_QTY);
		int 		GET2(int DATA_QTY, float *Data, float *Data2, BUF_MANAGER *IN2, int SHARED_BUFIN);				
  //@@@@@@  UNUSED ?		
  //@@@@@@  		
	public:		
		int			k_IN; 
		int			k_INBUFS; 
		
		int			k_OUT;		
		int			k_OUTBUFS;
		
		int			Flow[CHN_QTY][FLOW_QTY][k_and_kBUFS];
		
		float 		*Buf;
		int			*Bufi;
		
  //= Monitoring  		
		int			diff_INOUT; 
		int			GETPUT_DIST;	
		
	//private:
		int			SIM_FLAG; 
		int			SIM_MASK; 		

public:  // PUBLIC_DATA 
  //= Liczniki blokow 	
   #if	(SOFT_V & SHARC_HW)
	long long		BUF_SIZE;				
   #else
	int				BUF_SIZE;				  	
   #endif
     
	int				BufType; 
	
	int				SPK_DELAY_BUF;
 private: // DATA	
 	int				SIM_process(int DATA_QTY);
 	int				Flow_select(int BUF_MASK_ID);
 	int				DATA_AVAILABLE(int jF, int chn);
	
};

#endif
