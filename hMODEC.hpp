#ifndef 	MODEC_H
#define 	MODEC_H

#include		"hSTAGE_PROC.hpp"

extern int			stage_cnt;
extern float 		*stage_ptr[2];

extern 	float  		RoboDSP0[];
extern 	float  		RoboDSP1[];

  //-- TEST
extern 	int  			BLRAM_Ready;
extern 	int				LR_state; 	

extern struct 	StageManager 	SMBas, SMMid, SMTwt;

#define		MODE_COEFF_QTY			4

#define		FILTER_Q_VALUES			6
#define 	QLABEL_LEN				(MODEC_FULLNAME_LEN-40)
#define 	QNAME_LEN				4

	

//extern 	float 		modes_filts[2][MODES_QTY][MODE_COEFF_QTY];   


class MODEC {
//*****************************************	
public:
  //= Data members
	static 	float 		modes_state[2][MODES_QTY][MODE_COEFF_QTY];
  	static int			filt_qty[2];
	
//	static 	float		modes_Rstate[MODES_QTY][3];

  	static 	char		qlabels[2][FILTER_Q_VALUES][QLABEL_LEN]; // Tablica opisow dla kanalow Lewy i Prawy 
  	static 	char		fullname[2][MODEC_FULLNAME_LEN]; // Tablica opisow dla kanalow Lewy i Prawy 
  	
	
  //= USER Methods Public interface		
	static void	Init(void)
					{};
    
//	static void  FILTER (struct StageManager *SMAct, int StageAct, float *stage_ptr[2], int *pstage_cnt); //FILTER (struct StageManager *SMAct, int StageAct);
	static void  		FILTER (STAGE_PROC *SP, int kStage);
	//static void  		Msg101(int msg[UARTMSG_MAX_SIZE]);
	static void  		Msg101(int *msg);
	
	static char 		*Get_CHN_fullname(int channel)
					{	return	fullname[channel]; };
					
	static void			state_clr(void);
		
	static void  		Generate_filter(int CHANNEL, float freq, int Q, int dB, int Boost);				
  //= Obsluga komunikatu kasujacego aktualne filtry i opisy
	static void	 		Clear_MODEC(void);			
//*****************************************
private:		
  //= Data members
  	static int 			kqtext[2][FILTER_Q_VALUES];
  	//static int 			kfull[2];
  	static int			filt_by_Q_qty[2][FILTER_Q_VALUES];
  	
//*****************************************  	
  //= Private methods
  	//static void  Generate_filter(int CHANNEL, float freq, int Q, int dB);
	static void  		Add_labels(int CHANNEL, float freq, int Q, int dB);
  	
  //= 	
	static void  		Concatenate_qlabels(int CHANNEL);	
	
	static void  		Clear_labels(void);
};


EXTERNC void FMODEC(STAGE_PROC *SP, int kStage);

#endif
