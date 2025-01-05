   
#include 	"tt.h"
#include 	<stdio.h>
#include 	<math.h>

#include 	"hDSPSTRING.hpp"
#include 	"hMODEC.hpp"
#include 	"hDSP_MANAGER.hpp"
#include  	"hSR_PROC.hpp"
#include 	"hBUF_MANAGER.hpp"
#include 	"hSTAGE_PROC.hpp"
#include 	"hRESAMPLER.hpp"
#include 	"hVOL_MAN.hpp"
#include 	"hDSPSTRING.hpp"
#include 	"hDSP_TAB.hpp"
#include 	"hDELI_CMD.hpp"

#include 	"hDSP_MCEP.hpp"


#if (SOFT_V & SHARC_HW) 	
	#define  	SRUDEBUG  // Check SRU Routings for errors.
	#include 	<sru.h>
	#include 	"hI2SRX_LINE.hpp"
	#include  	"hI2STX_LINE.hpp"
	#include  	"hI2S_PORTS.hpp"
	#include 	"hLMK.hpp"
	
	extern	I2SRX_LINE			*In1;
	extern	I2STX_LINE			*Out1A;
	extern STAGE_PROC  			SP_FILT[SPK_QTY+1][CHN_QTY]; 
	extern 	BUF_MANAGER 		X[2], YM[2], RP[2]; 		
#endif 

EXTERNC char * MODEC_CHN_fullname(int channel);
/*
_D_44_X_X_l1_b8
012345678901234

00-01) 	_D		1) DAC_CODAC_MODE_0    // za nazwa zestawu idzie korekcja amplitudowa MC i zwrotnica
02)     _
03-04) 	44		3-4) SR_2CHAR 
05)     _
//============================================================
Value set for IPL: b/g/l/B/G/L/F/X 
06) 	b   	6)  IPL_BAS = SMBas.SPK_XO_IPL_TYPE; 
07)     _		
08)     g		8)  IPL_MID = SMMid.SPK_XO_IPL_TYPE; 
09)     _		
10-11)  B1		10) IPL_TWT = SMTwt.SPK_XO_IPL_TYPE; 
				11) FR20k_EQ_LEVEL_3= PlaySession.FR20k_EQ+48; 
12)     _
13) 	F		13) IPL_EXT = PlaySession.EXT_IPL_TYPE; 
14) 	8		14) xOVS_EXT_LEVEL_7= PlaySession.OVS_EXT+48, 
*/

//======  P L A Y S E S S I O N   C O N S T A N T S =================
//==  Message  0x58 bit values decoding  ===================================

#define			Z_IPL				0
#define			L_IPL				1
#define			G_IPL				2
#define			B_IPL				3

#define			M_CORR_A			0
#define			M_CORR_B			1
#define			M_CORR_C			2
#define			M_CORR_D			3

#define			x4OVS				0
#define			x8OVS				1
#define			x16OVS				2
#define			x32OVS				3

#define			M_PHASE_CORR		(0+1*8)     // bajt 0 - Sharc_config
#define			M_CORR_TYPE			(1+1*8)     // bajt 0 - Sharc_config
#define			M_TEST_SETUP		(3+1*8)     // bajt 0 - Sharc_config
#define			M_NOISE_SH			(5+1*8)     // bajt 0 - Sharc_config
#define			M_IPL_BAS			(6+1*8)     // wartosci 0..3 'Z', 'L', 'G', 'B'

#define			M_IPL_TWT			(0+2*8)     // bajt 0 - Sharc_config
#define			M_FIR_CH			(3+2*8)
#define			M_OVS_TOT			(4+2*8)     // bajt 2 - urzadzenie
#define			M_IPL_EXT			(6+2*8)     // bajt 2 - urzadzenie

#define			M_FIR_EXT			(0+0*8)
#define			M_FR20k_EQ			(1+0*8)

#define			M_TEST1				(3+0*8) 
#define			M_TEST2				(4+0*8)

#define			M_EXPH				(4+0*8)
#define			M_MCMT				(7+0*8)


#define			M_ACOUSTIC_CORR		(5+1*8)
#define			M_DEV_MODE_XLR		(6+0*8)
			
#define			UART101_OFFSET		105

// ----------  E X T E R N S  UZYWNAE PRZEZ MODUL    -----------


extern 		STAGE_PROC			SP_RSMP[RSMPOBJ_QTY][CHN_QTY];
extern 	void   New_Playrun_Config(void);


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===-----------    C L A S S  ===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

 #include 	"hPlaySession.hpp"

SIM_RECORDING_MODES 	PlaySession::RECORDING_MODE;
	int					PlaySession::WORK_CONF;	
	PLAY_STATE			PlaySession::PLAY_MODE;	
	
	int					PlaySession::DEBUG_MODE, PlaySession::_SMPBLOCK_SIZE, PlaySession::X1; 
	char				PlaySession::DEV_MODE;
	int					PlaySession::CORRECTION_TYPE;
						//PlaySession::EXPH; 
						//	, PlaySession::ACOUSTIC_CORR, PlaySession::PRE_Level; 
	int					PlaySession::GUI_Volume, PlaySession::GUI_VOL_CHG; 
	int					PlaySession::INBUF_SIZE, PlaySession::ZERORUN; 
	unsigned long 		PlaySession::modecmsg_clk;
		int				PlaySession::cDSP;


	
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===-----------    C L A S S  ===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



int		PlaySession::allowed_working_mode(void)
{
	int			mode_playback, mode_sim_off;   

   	 	mode_playback = ( (RECORDING_MODE == AUDIO_NOREC ) && 
   				          (PLAY_MODE== PLAY_ON) )  ;
    	mode_sim_off  = (RECORDING_MODE  == SIM_I2SOFF_D_ONLY)  ;
	return (mode_playback | mode_sim_off);
}


//=======  0x 5 0   =========================================
void 	PlaySession::Msg0x50_Proc(int *msg, PLAY_STATE pPLAY_STATE)	
{
		long ilong;
		int 	*iPLAY_MODE = (int *) &PLAY_MODE; 
		
//		Print_Debug( sprintf( dbgtab, "0x50 msg4:%d msg5:%d",msg[4], msg[5]), NL_NOTIME );
	  //==  Ustawienie parametrow konfiguracyjnych przetwarzania DSP
	//PLAY_MODE = pPLAY_STATE;	
	if (PLAY_MODE == DUAL_SIM || PLAY_MODE == DUAL_PLAY)	
	{  
		*iPLAY_MODE = -(*iPLAY_MODE);
		if (PLAY_MODE >0)  
			RECORDING_MODE = AUDIO_NOREC;
		else 
			RECORDING_MODE = SIM_I2SOFF_D_ONLY;		
	}
	{
		switch(PLAY_MODE )
		{
			case	DUAL_PLAY:  // After Off msg 0x52 0x52 do automatic SIM dump
				PLAY_MODE = DUAL_SIM; 
				RECORDING_MODE = SIM_I2SOFF_D_ONLY; // SIM_I2SOFF;
		  	  break;
			case	PLAY_SIM_OFF:  // No matter what was the On/Off button state initiate SIM dump if the state is Off in SIMOFF mode
				if (RECORDING_MODE == SIM_I2SOFF_D_ONLY)  // SIM_I2SOFF)
					PLAY_MODE = SIMTEST_ON;  
		      break;
			default:  // Only PLAY_ON makes sense here
				PLAY_MODE = PLAY_ON;  			
		      break; 
		}
	}
  
	 //if (!PLAY_MODE)
	 //	ZERORUN = 5;
}	

void 	PlaySession::Msg0x52_Proc(int *msg, PLAY_STATE pPLAY_STATE)	
{
		long ilong;
		int 	*iPLAY_MODE = (int *) &PLAY_MODE; 
	
//		Print_Debug( sprintf( dbgtab, "0x50 msg4:%d msg5:%d",msg[4], msg[5]), NL_NOTIME );
	  //==  Ustawienie parametrow konfiguracyjnych przetwarzania DSP
	//PLAY_MODE = pPLAY_STATE;	
	if (PLAY_MODE == DUAL_SIM || PLAY_MODE == DUAL_PLAY)	
	{  
		*iPLAY_MODE = -(*iPLAY_MODE);
		if (PLAY_MODE >0)  
			RECORDING_MODE = AUDIO_NOREC;
		else 
			RECORDING_MODE = SIM_I2SOFF_D_ONLY;		
	}
	else
	{
		switch(PLAY_MODE )
		{
			case	DUAL_PLAY:  // After Off msg 0x52 0x52 do automatic SIM dump
				PLAY_MODE = DUAL_SIM; 
				RECORDING_MODE = SIM_I2SOFF_D_ONLY; // SIM_I2SOFF;
		  	  break;
			case	PLAY_SIM_OFF:  // No matter what was the On/Off button state initiate SIM dump if the state is Off in SIMOFF mode
				if (RECORDING_MODE == SIM_I2SOFF_D_ONLY)  // SIM_I2SOFF)
					PLAY_MODE = SIMTEST_ON;  
		      break;
			default:  // Only PLAY_ON makes sense here
				PLAY_MODE = PLAY_SIM_OFF;  			
		      break; 
		}
	}
	 //if (!PLAY_MODE)
	// 	ZERORUN = 5;
}	

char 	PlaySession::ipl_code(int ipl_type, char LoV )
{
	char code;  
	
	switch(LoV)
	{
		case 'B': case 'M':
		  switch(ipl_type)
		  {
			case 0: code = 'Z'; break;
			case 1: code = 'L'; break;
			case 2: code = 'G'; break;
			case 3: code = 'B'; break;
		  }; 					 break;			
		case 'T':
		  switch(ipl_type)
		  {
			case 0: code ='W'; break;
			case 1: code ='H'; break;		//'S'
			case 2: code ='L'; break;
			case 3: code ='G'; break;
			case 4: code ='I'; break;
			case 5: code ='J'; break;
//			case 6: code ='O'; break;		
		  }; 					 break;
	}
	return(code);
}

void 	PlaySession::Full_Reconfig(void)
{
	New_Playrun_Config(); //Full_Reconfig
	
 #if UART_TYPE == UART_OLD    	  	
   #if (SOFT_V == SH_XOEP_PROD) 
//	if (EP_change)
	{
  		if (PlaySession::RECORDING_MODE < SIM_I2SOFF_D_ONLY && DSPSTRING::Get_Digitbit(DSPSTR_EXPH)  )		
   			DELI_CMD::Send_EP(SET_FIR); 
   		else
   			DELI_CMD::Send_EP(CLR_FIR);   		  	
	}
   #endif
	//trace("\nPS NPC", 0);	
 #elif  UART_TYPE == UART_NEW
  	if (DSPSTRING::Get_Digitbit(DSPSTR_EXPH)  )		
   		DELI_CMD::Send_EP(SET_FIR); 
   	else
   		DELI_CMD::Send_EP(CLR_FIR);   		  	
 #endif		
}


void 	PlaySession::Msg0x58_Proc_NEW(int *msg)  // used with new Deli client msgs 
{
	switch(msg[2])
	{
		//case 1: case 2: case 3:
		//case 4: 
		//	DELI_CMD active_input
		//  break;
		case 16: case 17:  // SPK/HDP-CODE change
			MCEP::Index((char *) msg+8);
			MCEP::Config();
		  break;			
		case 32: case 33: 
			cDSP = msg[3] & 0x07;
  			DSP_TAB::Stage_Switches(); 
			VOL_MAN::VOL_Stages_Setup(); //DSP_TAB::Stage_Switches();				
		  break;
		case 34:
			cDSP = msg[3] & 0x07;
			Full_Reconfig();		
		  break;
	}
}

int new_devmode; //, acoustic_corr_msg_bug;


//=======  0x 5 8   =========================================
void 	PlaySession::Msg0x58_Proc(int *msg)
{
	int 	pDEV_MODE, pRSMP, pFIR_CH, pFIR_EXT, pFR20k_EQ, pBAS_IPL_TYPE, pMID_IPL_TYPE, pTWT_IPL_TYPE, pEXT_IPL_TYPE, pOVS_EXT 	; 
	int		confdac, tmp, tmp2, Test1, Test2, new_acooustic_mode, EP_change = 0, IPL_FR20kEQ_change =0, FIRCH_change= 0; //, new_devmode=0, ;
		
	//IPL_change = 0;
   //=  Ustawienie parametrow sesji DSP STRING
	  //=  ACOUSTIC_Correction	   
	  	
		confdac = msg[1] | msg[2]<<8 | msg[4]<<16;
		new_acooustic_mode = (confdac & (1<<M_ACOUSTIC_CORR) )>>M_ACOUSTIC_CORR; 			
		
		//if (new_acooustic_mode != DSPSTRING::Get_Digitbit(DSPSTR_ACOUSTIC_CORR) || acoustic_corr_msg_bug==1)		
		//	++acoustic_corr_msg_bug;			
		DSPSTRING::Set_Digitbit(DSPSTR_MODEC, new_acooustic_mode, 0);
		Test1 = (1<<M_EXPH);
		//tmp = confdac & (1<<M_EXPH);
		tmp = (ON_OFF) (DSPSTRING::Get_Digitbit(DSPSTR_EXPH) >0);		
		tmp2 = (confdac & (1<<M_EXPH) )>>M_EXPH; 
		DSPSTRING::Set_Digitbit(DSPSTR_EXPH, tmp2, 0);
		if 	(tmp !=  tmp2)					
			++EP_change;

		//if(!ACOUSTIC_CORR) 	
		//	MODEC::Clear_MODEC();  //MODEC_Clear_MODEC();
	  //=  DEVICE_MODE
		new_devmode=0; 	  	   			
		tmp = 2-( (confdac & (1<<M_DEV_MODE_XLR) )>>M_DEV_MODE_XLR) ; 
		if ( tmp!= PlaySession::DEV_MODE )
	 		++new_devmode;
	 	PlaySession::DEV_MODE = tmp;	
	 	if (PlaySession::DEV_MODE== xCODAC) 
			DSPSTRING::String[DAC_CODAC_MODE_0] = msg[3];
		else
			DSPSTRING::String[DAC_CODAC_MODE_0] = msg[3]+32;
		if (DSPSTRING::String[DAC_CODAC_MODE_1] != msg[5] || DSPSTRING::String[SPK_CODE_CH3] != msg[6] || DSPSTRING::String[SPK_CODE_CH4] != msg[7])
	 		++new_devmode;		
		DSPSTRING::String[DAC_CODAC_MODE_1] = msg[5]; 
		DSPSTRING::String[SPK_CODE_CH3] = msg[6]; 
		DSPSTRING::String[SPK_CODE_CH4] = msg[7]; 

				
		//if ( msg[3] != DSPSTRING::String[DAC_CODAC_MODE_0]  || msg[5] != DSPSTRING::String[DAC_CODAC_MODE_1] )
		//	++new_devmode;
		
		 //= tmp<='Z';
			
		//if (tmp) 
		//	pDEV_MODE = 'K';
		//else 	pDEV_MODE = 'k';
	 	//if (pDEV_MODE != DSPSTRING::String[DAC_CODAC_MODE_0])
	 	//if ( (tmp+PlaySession::DEV_MODE)
	 	//	new_devmode= 1;
		pEXT_IPL_TYPE = 	ipl_code( (confdac & 0x03<<M_IPL_EXT)>>M_IPL_EXT, 'M' );	// MID pelni role OVS EXT ze wzgledu na konstrukcje procedury ovs w bibliotece DSP 
		pFIR_EXT 	= (confdac & 0x01<<M_FIR_EXT)	>>	M_FIR_EXT; 	  		
	 	//if( new_devmode   &&  		!pFIR_EXT)
		//	return;
		//else  
		//	Test1=10;
		pFIR_CH 		= (confdac & 0x01<<M_FIR_CH)	>>	M_FIR_CH; 	   				
		if ( (DSPSTRING::String[IPL_TWT]>='a'? 0: 1) != pFIR_CH)
			FIRCH_change = 1;
						
		pFIR_EXT 	= (confdac & 0x01<<M_FIR_EXT)	>>	M_FIR_EXT; 	  
		pFR20k_EQ	= (confdac & 0x03<<M_FR20k_EQ)	>>	M_FR20k_EQ;
		
		//DAC_SR_OVS_set();  // -> PlaySession.DAC_SR, PlaySession.OVS_CH, 			
		  //= Ustawienie korekcji MCBAS i jednoczesnie interpolacji basu - DO DECYZJI, czy nie powinna byc ustawiana na sztywno
		tmp = (confdac & 0x03<<M_IPL_BAS)>>M_IPL_BAS; 
		
		DSPSTRING::Set_BasDigit(0, 3*((confdac & 0x01<<M_TEST1)>>M_TEST1) ); // 3* to gat either 3 as max bass correction or 0 
		DSPSTRING::Set_Digitbit(DSPSTR_MCMT, (confdac & 0x01<<M_MCMT)>>M_MCMT,0);
		
		//tmp2=   (confdac & 0x01<<M_DACTYP_PRE)>>M_DACTYP_PRE;  // ( S_PRE[0].SM != NULL);  
			
		tmp = (confdac & 0x07<<M_IPL_TWT)>>M_IPL_TWT;
		pTWT_IPL_TYPE	= 	ipl_code(	tmp, 'T');			
		pBAS_IPL_TYPE	= 	ipl_code(	0, 'B'); // Ustawienie na sztywno 'Z' bo i tak jest ostry LowPass			
		pMID_IPL_TYPE	= 	ipl_code(	(confdac & 0x03<<M_IPL_EXT)>>M_IPL_EXT, 'M' );	// MID pelni role OVS EXT ze wzgledu na konstrukcje procedury ovs w bibliotece DSP 
		pEXT_IPL_TYPE = 	ipl_code( (confdac & 0x03<<M_IPL_EXT)>>M_IPL_EXT, 'M' );	// MID pelni role OVS EXT ze wzgledu na konstrukcje procedury ovs w bibliotece DSP 
		
		pOVS_EXT 	= 1<< ( (confdac & 0x03<<M_OVS_TOT)>>M_OVS_TOT );		
		if ( DSPSTRING::String[IPL_TWT]!= pOVS_EXT+48)
			++new_devmode;		
		
	//@@@@@@@@@@@@  		
	   //= IPL_change contains 
		IPL_FR20kEQ_change = DSPSTRING::User_Update(new_devmode, pFIR_CH, pFIR_EXT, pFR20k_EQ, pBAS_IPL_TYPE, pMID_IPL_TYPE, pTWT_IPL_TYPE, pEXT_IPL_TYPE, pOVS_EXT );
	//@@@@@@@@@@@@  
		
		//set_dspcode();		
	//@@@@@@@@@@@@   Bez odzwierciedlenia w DSPSTRING	
		// NOISE_SHAPING		=	( msg[2] & (0x01<<5) )>>5;		// 	Noise-Shaping 		0/1	
		//PHASE_CORRECTION	= 	  msg[2] & (0x01<<0);			// 	Phase-Correction 	0/1

		tmp = ( msg[2] & (0x03<<1) )>>1;		
		CORRECTION_TYPE	= 	tmp;
	   #if (OUT1A_ACTIVE == 1)
		//trace("\n10",0); 

/*   		if (PlaySession::DEV_MODE == xDAC)
			Out1A->TX_Buffers_Runtime_Reconf(SP_RSMP[TWT][LEFT_0].BufD,	SP_RSMP[TWT][RIGHT_0].BufD, NULL, NULL); 							 	   	
   		else
			Out1A->TX_Buffers_Runtime_Reconf(SP_FILT[MID][LEFT_0].BufD,	SP_FILT[MID][RIGHT_0].BufD, NULL, NULL); 							 	   		   	   
*/
		//trace("\n11",0); 
						
		switch (CORRECTION_TYPE)
		{			
			case CORRECTION_A: 
				VOL_MAN::LSEG = 20; VOL_MAN::RSEG = VOL_MAN::LSEG; 
			  break;
			case CORRECTION_B:
				VOL_MAN::LSEG = 50; VOL_MAN::RSEG = VOL_MAN::LSEG; 
		      break;
			case CORRECTION_C: 
				VOL_MAN::LSEG = 200; VOL_MAN::RSEG = VOL_MAN::LSEG; 
			  break;
			case CORRECTION_D:  // Tryb  
				VOL_MAN::LSEG = 400; VOL_MAN::RSEG = VOL_MAN::LSEG; 
			   break;				
		}
	   #endif
		//I2S_PORT::pcg_Init();
		//@@@@@@@@@@@@   				
		if (RECORDING_MODE != SIM_I2SOFF_D_ONLY && PLAY_MODE == PLAY_ON)
			ZERORUN = 5;
		if ( new_devmode || IPL_FR20kEQ_change || EP_change || (FIRCH_change && PlaySession::DEV_MODE== xCODAC)  ) // && acoustic_corr_msg_bug==0)
		{
			Full_Reconfig();
		}
		else 
		{
			// if Stage_Switches() returns > 0 then new_stage_config variable has been set == reconfig required  which is in 2 cases:
			// 1) EP filter ON/OFF since DSP_MANAGER config is required
			// 2) OVS TWT CORR FIR in CODAC mode change - because the delays having impact on XO filters change 
			//if (DSP_TAB::Stage_Switches()>0 )  
			//	New_Playrun_Config();
			//else 
			//FILTER::Size_Vol();
  			//RESAMPLER::Rsmp_Size_Vol_Delay();
  			DSP_TAB::Stage_Switches(); 
			VOL_MAN::VOL_Stages_Setup(); //DSP_TAB::Stage_Switches();
			//trace("\nPS Switch", 0);  			 			
		}
/*		if ((confdac & 0x01<<M_TEST2)	>>	M_TEST2 )
    	{	SRU(DIT_O, 				DAI_PB01_I) ;	 }
		else    		
    	{	SRU(DAI_PB18_O, 		DAI_PB01_I) ;	 }
*/			
		//if(acoustic_corr_msg_bug==2)	acoustic_corr_msg_bug= 0; 
}
	

	
//#####################################################################################################
//#####################################################################################################
//#####################################################################################################



EXTERNC int PlaySession_DEBUG_MODE(void)
{
	return(PlaySession::DEBUG_MODE);
}

