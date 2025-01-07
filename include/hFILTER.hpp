#ifndef FILTER_H
#define FILTER_H

#include  	"hSTAGE_PROC.hpp"

#define				xSTAGE_QTY		6

struct	filterstage 
{
enum  ON_OFF		ACTIVE;
	void 			(*stagefun)(STAGE_PROC *SP, int kStage);	
#if (SOFT_V & SHARC_HW)	
	float pm  		*coeffs; 
#else
	float   		*coeffs;
#endif	
	float 			*LR_state[2];
	int				sect_taps_qty;
	int				size;
	char			param_code;		 
	int				DN_rate;
	int				EP_OFFSET;	
	int				delay;	
	char 			DSPTAB_desc[STAGE_DESC_SIZE];	
};

class FILTER {
public:  // Attributes for public access
//	DSP_Steps_TAB  	*S_TAB; 
struct filterstage 	stages[xSTAGE_QTY]; 			//(*stages)[]; 
static	int			OBJ_QTY; 
static int			STAGE_QTY;
enum  ON_OFF		F_ACTIVE;
	int				LOAD_SIZE;
	int				SAVE_SIZE;	
//	int				DSP_SIZE;	
	
	float			DSP_scaler;
	int				Delay;  
	int				DN_rate;
	
// static	FILTER		*List_Filt_Rsmp[SPK_QTY+1+RSMPOBJ_QTY];
	BUF_MANAGER		*DelayBuf[2]; 

	int				EP_counter;			
  //= CHECK if Needed	
	int				DSP_TYPE; 
	int				SPK_CODE;
  //= CHECK if Needed	


public: //= INTERFACE_METHODS  -Initialization- 
#if (SOFT_V & SHARC_HW)    
	void 			StageInit(void (*pStagefun)(STAGE_PROC *SP, int kStage), int kStage, 
								float pm *pcoeffs,  // here is the difference in use of 'pm' mem  qualifier for Sharc_HW
			   					float *Lstate, float	*Rstate, int sect_taps, int pParamCode, int pDownRatio, int pDelay, char *pDesc);
#else			   					
	void 			StageInit(void (*pStagefun)(STAGE_PROC *SP, int kStage), int kStage, 
								float  *pcoeffs,
			   					float *Lstate, float	*Rstate, int sect_taps, int pParamCode, int pDownRatio, int pDelay, char *pDesc);
#endif			   					
static	void 		Init(void);
static	void 		Size_RF(void);
static	void 		Size_Vol(void);
// static	void 		List_FR(void);

static	void		Constructor(void);			//(DSP_Steps_TAB  *pS_TAB, int pSPK_CODE);
static void 		Config();	
	void 			Vol(int Stage_Qty);
	
static	void		clear_stages_FR(void);
static	void 		clear_state_FR(void);
 
	void			_IIR (STAGE_PROC *SP, int kStage); 
	void			_FIR (STAGE_PROC *SP, int kStage); 
	void			_DOSF(STAGE_PROC *SP, int kStage); 	
	void			_DOSI(STAGE_PROC *SP, int kStage); 	
	
//	void 			VOL_SCALER(STAGE_PROC *SP, int kStage);
	void 			DN_rate_Calc(void);
	int				F_SpkDelay(int Stage_Qty);
	
private:	
	void			DOSX(STAGE_PROC *SP, int kStage); 		
};

void  	IIR (STAGE_PROC *SP, int kStage);

void  	FIR (STAGE_PROC *SP, int kStage);

void  	DOSF (STAGE_PROC *SP, int kStage);

void  	DOSI (STAGE_PROC *SP, int kStage);

void  	OVSI (STAGE_PROC *SP, int kStage);

void  	OVSEXT (STAGE_PROC *SP, int kStage);

void  	NUL (STAGE_PROC *SP, int kStage);

#endif

