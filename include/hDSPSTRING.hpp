#ifndef DSPSTRING_H
#define DSPSTRING_H

class FILTER; 
class STAGE_PROC;

//#include	"FT_Config.h"

class DSPSTRING
{
  //= Data members	
  public:
// static struct S_TABS_size		S_TABS[4]; 

// static	FILTER			*S_MAP[4][2]; 
 static	char				String[DSPSTRING_LEN+2]; 
 
// static	int					k_dsp; 
// static	char				stages[12][DSPTEXT_LEN+DSPSTRING_LEN+1];

  //= Methods
  
 static	void 			Set_Digitbit(int bitpos, int ON_OFF, int offset);
 static	int 			Get_Digitbit(int bitpos);
 static	void 			Set_BasDigit(int change, int directval);
 
 static	void 			Apply_Default_String(void);    
 static	void 			Apply_Current_String(void);    
 static	int 			User_Update(int RSMP, int pFIR_CH, int pFIR_EXT, int pFR20k_EQ, int pBAS_IPL, int pMID_IPL, int pTWT_IPL, int pEXT_IPL_TYPE, int pOVS_EXT );
 static	void 			SR_Update(int SAMPLE_RATE);
 static	void 			Print_DSPstring_Modes(void);	
 static	void 			SR_DSPSTRING(void);
 static	int 			set_IPL(int pos, int ipl_val, int FIR_flag);
 static	void 			Fill_DSPSTRING_blanks(void);
 static	void 			SPK_IPL_mapping(char *IPL_variable, int *FIR_CH, char IPL_VAL); 
 
private:
 static int 			char2hex(char sdigit);
 static char 			hex2char(int hdigit);
};
#endif

 //static struct stages_tab	
 //static struct DSP_Steps_TAB	*S_TABS[4];  
 //static struct DSP_Steps_TAB	(*S_TABS[4])[] ;

 /*
#define		DSPTEXT_LEN				(17 +DSPSTRING_LEN)




struct DSP_Steps_TAB
{
	FILTER 				*FilterObj;
	int 				Stage_No;
	void 				(*stagefun)(STAGE_PROC *SP, int kStage);
	float pm 			*pcoeffs;
	float				*Lstate;
	float				*Rstate;	
	char 				dspstr[DSPSTRING_LEN+2]; 
	char 				param_code;  
	int					sect_taps_qty;
	int					down_ratio;
	int					delay;
	char				DSP_text[DSPTEXT_LEN];	
};

struct S_TABS_size 
{		
	struct DSP_Steps_TAB		*tab;		
					int 		size;
};			
*/
