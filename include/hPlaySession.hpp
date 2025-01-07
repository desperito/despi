#ifndef PLAYSESSION_H
#define PLAYSESSION_H

class PlaySession
{
  //= Data members	
  public:
 static		int					CORRECTION_TYPE;
 static		int					cDSP;


//	int			NOISE_SHAPING;
//static		int					EXPH;	
static	SIM_RECORDING_MODES 	RECORDING_MODE;
//	int			RECMODE_CHG; 
static		int					WORK_CONF;	
static	char					DEV_MODE;
//	char		LMK_SETUP; 	
static		int					PRE_Level; 
static		int					_SMPBLOCK_SIZE; 	
static		PLAY_STATE			PLAY_MODE;	

//static		int					INPUT_ACTIVE; 

static		int					DEBUG_MODE;
static		int					INBUF_SIZE;
static		int					GUI_Volume;
static		int					GUI_VOL_CHG;
static		int					X1;	
static		int					ZERORUN; 
static	unsigned long 			modecmsg_clk; 			
  //======  Methods
   	public:
 static	void 					Init(void);
 static	int 					allowed_working_mode(void);	
 static  void					SR_Set(int, int);
// static  void					SR_Read_I2SOFF(void);
 
 static	void 					Msg0x50_Proc(int *, PLAY_STATE);
 static	void 					Msg0x52_Proc(int *, PLAY_STATE);		
 static	void 			 		Msg0x58_Proc(int *msg);
 static	void 					Msg0x58_Proc_NEW(int *msg);
 
 static	char 					ipl_code(int ipl_type, char lov );
   private:
 static	void 					Full_Reconfig(void);   
};


#endif
