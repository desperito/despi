#ifndef RESAMPLER_H
#define RESAMPLER_H

#include 	"tt.h" 
#define		SR_LEVEL_QTY				3
#define		OVS_LEVEL_QTY				(SR_LEVEL_QTY+DWN_LEVEL_QTY)

#define		CLK_BASE_QTY				2
#define		ZERO_RSMP					2
#define		ZERO_RSMP_POS				1
#define		RSMP_LEVEL_QTY				3

#define 	RSMP_YQTY					DSPBLOCK_SIZE2  // 4 	// 


//#include  	"hSTAGE_BASE.hpp"

struct	rsmp_coeffs	
{
	float	fm2[RSMP_YQTY];	
	float	fm1[RSMP_YQTY];
	float	f_0[RSMP_YQTY];
	float	fp1[RSMP_YQTY];
	float	fp2[RSMP_YQTY];		
	float	fp3[RSMP_YQTY];				
	int		kX0[RSMP_YQTY];
	
//	double	RSMP_STEP; 
//	int 	RSMP_OUT_QTY; 
};

struct	test1	
{
	float	fm1;
};

#include  	"hFILTER.hpp" 

class RESAMPLER : public FILTER	
{
	friend class DSP_MANAGER;
	friend class STAGE_PROC; 	
//*****************************************	
public:
  //= Data members
    //= IPL parameters
  			int				xo_iplx; 
			char			SPK_XO_IPL_TYPE;
//			int				FIR_CH; 
static		int				OBJ_QTY; 		
	
	struct interpol			*BS;
			int				POLY_TYPE;
static 		int				OVS_CDx4;
static		enum  ON_OFF	ACTIVE; 
			int				OVS_CH;	
static		float			OVS_VOLSCALER;
static		int				FIR_EXT;	
	
//	STAGE_PROC				*SP;
	
    //= RSMP implementation data  
	static	int				X_QTY[CLK_BASE_QTY][OVS_LEVEL_QTY][RSMP_LEVEL_QTY]; 	
static rsmp_coeffs			(*IPL)[SR_LEVEL_QTY][RSMP_LEVEL_QTY];
static		int				Level;
static		int				SAVE_SIZE_R;	
static		int				MAX_LEVEL;				
	//int						Chn;
			float			R48_3[3];
static  	int				STAGE_QTY; 
			float			Delay;
	
  //= USER Methods Public interface		
	static 	void 			Init(void);  
	static 	void  			Constructor();
	static	void 			Rsmp_Size_Vol_Delay(void);
			void 			R_SpkDelay(int stage_qty, int kSpk);
  	static	void  			LevelChg(int step);  	
  		
	static	void			clear_stages(void);
						
  	static	void			Make_IPL(void);
			void 			EXE(STAGE_PROC *SP);
			void 			LIN(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT);	
			void 			POLY4P(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT);					
			void 			POLY6P(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT);					
			
			void 			STEP(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT);					
			void 			ZEROS(STAGE_PROC *SP, struct rsmp_coeffs *T, float *IN, float *OUT);					
  	
private:
			float 			INTERP_Delay_factor(void);
};

#endif
