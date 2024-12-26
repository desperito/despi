#ifndef DSP_TAB_H
#define DSP_TAB_H

class 	FILTER; 
class 	STAGE_PROC;

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


class DSP_TAB
{
  //= Data members	
  public:
 static struct S_TABS_size	S_TABS[7]; 

 static	int					k_dsp; 
 static	char				stages[12][DSPTEXT_LEN+DSPSTRING_LEN+1];
  //= Methods
 static	void 			StageInfo_Clr(void);      
 static void			Stages_Config(void);
 static int				L_88_96(char sdigit, DSP_Steps_TAB	*TAB, int kc);
 static int				L_PRE_BAS(char sdigit, DSP_Steps_TAB	*TAB, int kc);
 static int				x44_x48(char tdigit, int kc);
 static int				Stage_Switches(void);
 
private:
 static void 			Pick_Stages(S_TABS_size	*STAB, 	char *DSP_STRING);
 static int 			bit_digit(char *p_sdigit, char *p_tdigit);
};
#endif
