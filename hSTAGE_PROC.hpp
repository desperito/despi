#ifndef STAGE_PROC_H
#define STAGE_PROC_H

//#include 	"hSTAGE_BASE.hpp"
//#include 	"hBUF_MANAGER.hpp"

class 	BUF_MANAGER; 
class 	STAGE_BASE;
class	FILTER; 

//#include 	"hDSP_BASE.hpp"


class STAGE_PROC //: public STAGE_BASE 
{
//***************************************************************************************************************************************	
//= DSP Runtime Implementation
//***************************************************************************************************************************************
public:
	float 				*stage_ptr[2];
	int					stage_cnt;
//	STAGE_BASE			
	FILTER				*FiltRsmp;
						//*pRSMP;
	int					Chn; 
	int					(*pStart_DSP_Delay)(void);
//	void				(*pDSP_Proc)(void);
	float				tail[xTAIL_ITPL];

public: //= INTERFACE_METHODS  -DSP Runtime- 	
static	void 			Constructor(void);
  	void				Init(float *stage_ptr0, float *stage_ptr1, int pX_GET_SIZE, int pSAVE_SIZE, FILTER  *pDSP, int pStream_INDEX, int pCHN); //, void (*pDSP_proc)(STAGE_PROC *SP));
  	void 				DSP_ChannelProc(void);	
	void 				Init_RoboDSP(void);
  	
  	int					Stream_INDEX;   //  Position in DSP_MANAGER  control table
  	BUF_MANAGER			*BufIN[2];
//  	int					k_GET[2];
//  	int					k_GETBUFS[2];
  	BUF_MANAGER			*BufD; 
//	BUF_MANAGER			*BufE;   	
//	int					LOAD_SIZE; 	
	int					X_GET_SIZE; 
	int					SAVE_SIZE; 
//protected: //= DSP Channel implementation
	//********************************************************************************	
	int					LoadIN_NEW(void) ;//, struct Stream_Params *pstream);	
	int					LoadIN_EX(void);					
	void				(*DSP_proc)(STAGE_PROC *SP);
static	void 			FILTX2S_PROC(STAGE_PROC *SP); 
static	void 			RSMPX2S_PROC(STAGE_PROC *SP); 	
static	void 			FILT1S_PROC(STAGE_PROC *SP); 
static	void 			FILT_FPTEST(STAGE_PROC *SP); 

//static	void 			FILTRSMP_PROC(STAGE_PROC *SP); 
static	void 			RSMP_CREATE_FILTPROC(STAGE_PROC *SP); 
static	void 			RSMP_EP_FILT_PROC(STAGE_PROC *SP); 
	
static	void 			FSUM_PROC(STAGE_PROC *SP); 

static	void 			RSMP1S_PROC(STAGE_PROC *SP); 
//static	void 			EPRT_PROC(STAGE_PROC *SP); 	

static	void 			UPSAMPLE_2x(STAGE_PROC *SP); // RSMP1S_PROC(STAGE_PROC *SP) //  
static	void 			DUMMY(STAGE_PROC *SP);



static	void 			DELI_DATA_RSMP(STAGE_PROC *SP);

static	void 			DELI_FLAGS_RSMP(STAGE_PROC *SP); 
//private:
//	int 				std_load(void);	
};	
#endif
