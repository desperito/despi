   
#ifndef TT_H
#define TT_H

/*
#define		SHARC_V						8

#define		xDAC_V						0 	
#define		SH_I2SDAC_V					(SHARC_V+1)  // 1
#define		SH_SPDIFDAC_V				(SHARC_V+2) // 2

//#define		RPI_MIX_V					3
//#define		RPI_V						2

#define		FPGA_BASE_V					16
#define		FPGA_V						(FP_BASE_V+1)
#define		FPGADSP_V					(FP_BASE_V+2)
#define		FPGADSP_PP_V				(FP_BASE_V+3)
#define		FPGA_TWT_V					(FP_BASE_V+4)


#define		RPI_V						128
*/ 

#define		SH_BASE 					16
#define		SH_1RX_1TX					2
#define		SH_1RX_1TX					2


#define		SH_DSP						(SH_BASE+1)
#define		SH_SPDIFDAC					(SH_BASE+SH_1RX_1TX)
#define		SH_I2SDAC					(SH_BASE+3)
#define		SH_XOEP_PROD				(SH_BASE+4)
#define		FP_FORD						(SH_BASE+5)

#define		FP_BASE 					32
#define		FP_TAB						(FP_BASE+1)
#define		FP_DSP						(FP_BASE+SH_1RX_1TX)
#define		FP_CONF2RX					(FP_BASE+8)

#define		FP_TEST2RX					(FP_CONF2RX+4)
#define		FP_TWT2RX					(FP_TEST2RX+1)
#define		FP_XOEP_TEST				(FP_TEST2RX+2)

//#define		FP_FORD						(FP_CONF2RX+2)
#define		FP_XOEP_PROD				(FP_FORD+1)

#define		DELI_BASE 					(64)
#define		DELI_RSMP					(DELI_BASE+2)
#define		DELIR_ZEROS					(DELI_BASE+3)


#define		SHARC_HW					(SH_BASE+FP_BASE+DELI_BASE)	
#define		LINUX_HW					128


#define		PROD_LIVE_MODE				(1<<4)
#define		TEST_MODE					(0<<4)
#define		PROD_XAUDIO_MODE			(2<<4)
#define		PROD_MODE					PROD_XAUDIO_MODE   

#define		SOFT_V						LINUX_HW
#if 	(SOFT_V == SH_XOEP_PROD)
	#define		XOEP_SPDIF_SPORT5			0
#else	
	#define		XOEP_SPDIF_SPORT5			1
#endif

#define		FORD_LOW_RSMP				0 //    1 //  

#define		xRUN_MODE					PROD_MODE //TEST_MODE  //   


#define		VERSION_LED					1
#define		SUBVERSION_LED				1


#define		xSTART_INBUFS				8

#define		EP_OFF						0
#define		EP_PRE						1
#define		EP_MID						2	
#define		EP_TWT						3	
#define		EP_BT						4	
#define		EP_MODE						EP_MID  // EP_BT  // EP_TWT // EP_PRE 	// 

#define		EP_PRETAIL					1
#define		EP_XPRETAIL					2	
#define		EP_TAILMODE					EP_XPRETAIL	 

//EKREM { 
// IMPORTANT
// 1. REPLACED ALL extern "C" with EXTERNC in all files 


#if (SOFT_V & SHARC_HW) 
	#define	SECTION(mem_area)	   section mem_area
#else
	#define	SECTION(mem_area)	   
#endif

#ifdef	__cplusplus
	#define	EXTERNC	extern "C" 
#else
	#define	EXTERNC
#endif
// 2. There is no section or pm, dm in linux gcc
/*
#if	(SOFT_V == LINUX_HW)
//	section(X)
	struct stages_tab; // Declaration is much later than first reference
#endif
//EKREM }
*/

//=========   SIM CONTROL CONSTANTS  ==========
//#define 	RECORDING_ONOFF
#define		DSPdata						float
//#define		DAC							'D'
//#define		CODAC						'K'
//#define		SPEAKER_DEVICE				'S'

#define		xDAC						2	//'D'
#define		xCODAC						1	//'K'
#define		xSPK_DEV					3	//'S'

#define		LEFT 			1
#define		RIGHT 			2
#define		LEFT_0 			0
#define		RIGHT_0 		1


#define		DEV_MASK					0xF
#define		RUNMODE_MASK				(0xF<<4)
#define		CHN_SHIFT					8
#define		CHN_MASK					(0xF<<CHN_SHIFT)

#define		DILO_OVS_MASK				(0xF<<12)

#define		DUARK_DEV					0
#define		SPDIF_DEV					1
#define		RPI_DEV						2
#define		PI_RX_DEV					3
#define		PI_PROD_DEV					4
#define		SHARC_TX_DEV				5
#define		FPGA_DEV					6



#define 	NOVS_LMK					(1<<12)
#define 	NOVS_SPDIF					(2<<12)
#define 	OVS_LMK						(3<<12)
#define 	OVS_LMK_LOTHD				(4<<12)

//#define		TEST_A_STD_CONFIG			(DUARK_DEV|LEFT<<8|TEST_MODE)  //(2<<4) 


//#define		TEST_A_STD_CONFIG			(DUARK_DEV|LEFT<<8|TEST_MODE)  //(2<<4) 
#define		DUARK_LEFT_TEST				(DUARK_DEV|LEFT<<8|TEST_MODE)   
#define		TEST_B_OPTIM_CONFIG			(DUARK_DEV|LEFT<<8|TEST_MODE)  
#define		DUARK_D_CONFIG				(DUARK_DEV|(LEFT+RIGHT)<<8|PROD_MODE)
#define		DUARK_LR_TEST				(DUARK_DEV|LEFT<<8|TEST_MODE) 
 
#define		DUARK_LEFT_PROD				(DUARK_DEV|LEFT<<8|PROD_MODE) 
#define		DUARK_RIGHT_PROD			(DUARK_DEV|RIGHT<<8|PROD_MODE) 

#define		SPDIF_TEST					(SPDIF_DEV|(LEFT+RIGHT)<<8|TEST_MODE)  						//(2<<4))
//#define		SPDIF_LEFT_TEST				(SPDIF_DEV|LEFT<<8|TEST_MODE)						//3<<4))
#define		SPDIF_LEFT					(SPDIF_DEV|TEST_MODE)

#define		SPDIF_RIGHT_PROD			(SPDIF_DEV|RIGHT<<8|PROD_MODE)
#define		SPDIF_2CH_PROD				(SPDIF_DEV|(LEFT+RIGHT)<<8|PROD_MODE)
#define		SPDIF_LEFT_PROD				(SPDIF_DEV|LEFT<<8|PROD_MODE)

#define		SPDIF_LEFT_TEST				(SPDIF_DEV|LEFT<<8|TEST_MODE)
#define		SPDIF_RIGHT_TEST			(SPDIF_DEV|RIGHT<<8|TEST_MODE)
#define		SPDIF_2CH_TEST				(SPDIF_DEV|(LEFT+RIGHT)<<8|TEST_MODE)


#define		RPI_SPDIF_2CH_PROD			(SPDIF_DEV}| RPI_DEV|(LEFT+RIGHT)<<8|PROD_MODE)
#define		RPI_2CH_TEST				(RPI_DEV|(LEFT+RIGHT)<<8|TEST_MODE)
#define		RPI_2CH_PROD				(RPI_DEV|(LEFT+RIGHT)<<8|PROD_MODE)

#define		PIRX_2CH_PROD				(PI_RX_DEV|(LEFT+RIGHT)<<8|PROD_MODE| NOVS_SPDIF)
#define		PIPROD_2CH_PROD				(PI_PROD_DEV|(LEFT+RIGHT)<<8|PROD_MODE | OVS_LMK);
#define		SHARCTX_2CH_PROD			(SHARC_TX_DEV|(LEFT+RIGHT)<<8|PROD_MODE)  // SPORT3 & PCG_B are not used

#define		FPGA_2CH_PROD				(FPGA_DEV|(LEFT+RIGHT)<<8|PROD_MODE)
#define		FPGA_LEFT_PROD				(FPGA_DEV|(LEFT)<<8|PROD_MODE)



#define 	LOADX_ISR					1	
#define 	LOADX_SIM					2
#define 	LOADX_MODE					LOADX_ISR

//#define		DUARK_CONFIG				(WORK_CONFIG==USER_CONFIG||WORK_CONFIG==REAL_PLAY_PWM_CONFIG||WORK_CONFIG==REAL_PLAY_XPWM_CONFIG)
//#define		SHARKC_CONFIG				(WORK_CONFIG==CONSTRUCTOR_CONFIG||WORK_CONFIG==REAL_DEV_PWM_CONFIG||WORK_CONFIG==REAL_DEV_XPWM_CONFIG)


#if  	(xRUN_MODE == PROD_MODE)
  #if  	(SOFT_V & FP_BASE)
//	#define		WORK_CONFIG					 (FPGA_2CH_PROD|NOVS_SPDIF) // // SPDIF_LEFT_TEST //  (SPDIF_LEFT_PROD|OVS_LMK_LOTHD)   // (SPDIF_2CH_PROD|OVS_LMK) // (SPDIF_RIGHT_PROD|OVS_LMK) // PIRX_2CH_PROD // (SPDIF_2CH_PROD|NOVS_SPDIF)  // (SPDIF_2CH_PROD|NOVS_LMK)  // (RPI_2CH_PROD|OVS_LMK)  //  RPI_2CH_PROD // SPDIF_2CH_TEST // DUARK_LR_TEST // RPI_2CH_TEST // SPDIF_RIGHT_PROD //  SPDIF_RIGHT_TEST //  SPDIF_LEFT_TEST // SPDIF_2CH_PROD		//  DUARK_D_CONFIG	 // 	DUARK_RIGHT_PROD  // DUARK_D_CONFIG	 // 	SPDIF_LEFT_TEST //  SPDIF_TEST // TEST_A_STD_CONFIG  	// DUARK_D_CONFIG	 // 	TEST_B_OPTIM_CONFIG // 
	#define		WORK_CONFIG					(FPGA_2CH_PROD | OVS_LMK_LOTHD)  // (FPGA_LEFT_PROD	|OVS_LMK_LOTHD) //   (FPGA_LEFT_PROD	|OVS_LMK_LOTHD) 
  #elif 	(SOFT_V == SH_XOEP_PROD || SOFT_V == DELI_RSMP)	
	#define		WORK_CONFIG					(SPDIF_2CH_PROD|OVS_LMK_LOTHD) //(SPDIF_2CH_PROD|NOVS_SPDIF) //  (SPDIF_LEFT_PROD|OVS_LMK_LOTHD)  // 
  #elif 	(SOFT_V & SH_BASE)	
	#define		WORK_CONFIG					(SPDIF_LEFT_PROD|OVS_LMK_LOTHD)  //`(SPDIF_2CH_PROD|OVS_LMK_LOTHD) // (FPGA_2CH_PROD | OVS_LMK_LOTHD)  //      (SPDIF_RIGHT_PROD|OVS_LMK_LOTHD) //     (FPGA_2CH_PROD|NOVS_SPDIF) // // SPDIF_LEFT_TEST //  (SPDIF_LEFT_PROD|OVS_LMK_LOTHD)   // (SPDIF_2CH_PROD|OVS_LMK) // (SPDIF_RIGHT_PROD|OVS_LMK) // PIRX_2CH_PROD // (SPDIF_2CH_PROD|NOVS_SPDIF)  // (SPDIF_2CH_PROD|NOVS_LMK)  // (RPI_2CH_PROD|OVS_LMK)  //  RPI_2CH_PROD // SPDIF_2CH_TEST // DUARK_LR_TEST // RPI_2CH_TEST // SPDIF_RIGHT_PROD //  SPDIF_RIGHT_TEST //  SPDIF_LEFT_TEST // SPDIF_2CH_PROD		//  DUARK_D_CONFIG	 // 	DUARK_RIGHT_PROD  // DUARK_D_CONFIG	 // 	SPDIF_LEFT_TEST //  SPDIF_TEST // TEST_A_STD_CONFIG  	// DUARK_D_CONFIG	 // 	TEST_B_OPTIM_CONFIG // 
  #else 
	#define		WORK_CONFIG					(SPDIF_2CH_PROD|OVS_LMK_LOTHD) //SPDIF_LEFT_TEST //        (FPGA_2CH_PROD|NOVS_SPDIF) // // (SPDIF_LEFT_PROD|OVS_LMK_LOTHD)   // (SPDIF_2CH_PROD|OVS_LMK) // (SPDIF_RIGHT_PROD|OVS_LMK) // PIRX_2CH_PROD // (SPDIF_2CH_PROD|NOVS_SPDIF)  // (SPDIF_2CH_PROD|NOVS_LMK)  // (RPI_2CH_PROD|OVS_LMK)  //  RPI_2CH_PROD // SPDIF_2CH_TEST // DUARK_LR_TEST // RPI_2CH_TEST // SPDIF_RIGHT_PROD //  SPDIF_RIGHT_TEST //  SPDIF_LEFT_TEST // SPDIF_2CH_PROD		//  DUARK_D_CONFIG	 // 	DUARK_RIGHT_PROD  // DUARK_D_CONFIG	 // 	SPDIF_LEFT_TEST //  SPDIF_TEST // TEST_A_STD_CONFIG  	// DUARK_D_CONFIG	 // 	TEST_B_OPTIM_CONFIG // 		
  #endif
#elif	(xRUN_MODE == TEST_MODE)
	#define		WORK_CONFIG					SPDIF_LEFT_TEST //      (SPDIF_2CH_PROD|OVS_LMK_LOTHD) //  (FPGA_2CH_PROD|NOVS_SPDIF) // // (SPDIF_LEFT_PROD|OVS_LMK_LOTHD)   // (SPDIF_2CH_PROD|OVS_LMK) // (SPDIF_RIGHT_PROD|OVS_LMK) // PIRX_2CH_PROD // (SPDIF_2CH_PROD|NOVS_SPDIF)  // (SPDIF_2CH_PROD|NOVS_LMK)  // (RPI_2CH_PROD|OVS_LMK)  //  RPI_2CH_PROD // SPDIF_2CH_TEST // DUARK_LR_TEST // RPI_2CH_TEST // SPDIF_RIGHT_PROD //  SPDIF_RIGHT_TEST //  SPDIF_LEFT_TEST // SPDIF_2CH_PROD		//  DUARK_D_CONFIG	 // 	DUARK_RIGHT_PROD  // DUARK_D_CONFIG	 // 	SPDIF_LEFT_TEST //  SPDIF_TEST // TEST_A_STD_CONFIG  	// DUARK_D_CONFIG	 // 	TEST_B_OPTIM_CONFIG // 
#endif  
  
// DUARK_D_CONFIG - dwa kanaly, inaczej jeden oraz krotka tablica sygnalu filtru testowego plus potencjalna roznica w main w konfiguracjach UART i AUDIO _NOREC // TEST_A_STD_CONFIG i TEST_B_OPTIM_CONFIG - takie same

#if  	(SOFT_V & FP_BASE ) //|| SOFT_V == SH_XOEP_PROD)
	#define		TX_SLEN			SLEN32   // SLEN24 //  
#elif 	(SOFT_V & SH_BASE)	
	#define		TX_SLEN			SLEN24 //  
#endif

// #if 		WORK_CONFIG==DUARK_D_CONFIG 

#define 			CHN_WORKMODE					((WORK_CONFIG& CHN_MASK)>>8) 

#if 		(CHN_WORKMODE & LEFT > 0 )
	#define 	CLK_CHN_ACTIVE		LEFT_0  //  RIGHT_0  //  RIGHT_0  // LEFT // RIGHT //   
#else 		
 	#define 	CLK_CHN_ACTIVE		RIGHT_0  // LEFT_0  // LEFT_0  // (LEFT+RIGHT)  // RIGHT //  
#endif	

#define 	CH_SETUP			FULL_SPK //     TWT // BAS // MID // 
#define 	CH_MIDSUM			FULL_SPK //  BAS  //  TWT //  MID // 
/*#define 	CH_REC				MID_OVS  //   TWT //  MID //BAS //   CH_SETUP  //    
#if (CH_REC	== MID_OVS || CH_REC	== MID)
	#define 	CH_SETUP		FULL_SPK
#endif			
*/

// used in main & UART
#define		RUNTIME_MONITOR_LEN			1000


#define		DSP_BLOCK_QTY				(SPK_QTY+1+RSMPOBJ_QTY)

#define 		CODECT 			int
typedef unsigned long long  		UL;

#define		MAXINT24			8388608 //4140000 //
#define		MAXINT24_2			4194304

//===========  S I M  CONSTANTS  ==========================
enum BOOLEAN  {
	FALSE = 0, TRUE= 1, NONREC =3, ALREADY_COMPLETED = 4 };


#ifndef  SIM_RECORDING_MODES
typedef enum 
{
	AUDIO_NOREC=0,   REAL_I2SON_D_E = 1, SIM_I2SON_D_E = 1<<1 | 0  , SIM_I2SOFF_D_ONLY = 1<<1 | 1
} SIM_RECORDING_MODES;
#endif

enum E_PWM_SOURCES
{
	PWM_AUDIO_IN_float_BLRAM = 1, PWM_DSP_float = 2, PWM_I2S_OUT_float = 3, PWM_I2S_OUT_INT = 4  
};

enum SIM_INPUT_TYPES
{
	DIRAC=1, HP_DIRAC, SAWTOOTH_1, SAWTOOTH_NEGPOS10, SINUS, SIG_RSMP48
};

enum ON_OFF
{
	OFF_STATE=0, ON_STATE= 1
};


typedef enum 
{
    SIM_1OFF		 	= 	-3, 		
    SIMTEST_ON		 = 	-2, 	
    DUAL_SIM		 = 	-1, 
    PLAY_SIM_OFF 	= 	 0, 
    DUAL_PLAY	 	 = 	 1,
    PLAY_ON  		 = 	 2
} PLAY_STATE;


// Speaker and Stream types
#define		BAS					0
#define		MID					1
#define		TWT					2
#define		FULL_SPK			3
#define		PRE					3

#define 	BAS_B		(1<<BAS)
#define 	MID_B		(1<<MID)
#define 	TWT_B		(1<<TWT)

#define 	XIN_V		0
#define 	PRE_V		1
#define 	EXP_V		2
#define 	XIN_B		(1<<XIN_V)
#define 	PRE_B		(1<<PRE_V)
#define 	EXP_B		(1<<EXP_V)

// TEST modes
#define		BAS					0
#define		MID					1
#define		TWT					2
#define		FULL_SPK			3
#define		BUFOR_WE			4

#define		MID_TEST			(1<<4)
#define		TWT_TEST			(2<<4)
#define		BAS_TEST_STANDARD	(3<<4)

#define		MID_OVS				((1<<4)+1)

#define		DSP_BUF		1
#define		EXC_BUF		2

//===========  CLOCK CONTROL CONSTANTS  ==========================
#define 	CLK_1xOVS_YBUF_MODE	1


#define 	DUMP_FLOAT		1
#define 	DUMP_INT		2


#define		CHN_QTY				2
#define		DSP_WAY_QTY			4
#define 	MODEC_FULLNAME_LEN	240
//=  UART SAVE CONTROL CONSTANTS  ==========
#define		FILENAME_LEN			(MODEC_FULLNAME_LEN+23)


//======================================================
//=============   S T A L E   D A  C  ==================
//======================================================

#define			OFF				0	
#define			ON				1
#define			SWITCH			2
		 

//===========================================================
//=======   U A R T  &  P R I N T  C O N S T A N T S   ====
//===========================================================

#define 	UART0_DMA_TXSIZE		8
#define 	UART0_SIZE				30

#define			NO_DBG 				0
#define			UART_DBG 			1
#define			UART_PC 			2
#define			PRINTF_DBG			3
#define			UART_PRINTF_DBG		4
#define			UART_PC_PRINTF		5

#define		TIME			1
#define		TIME_NL			2
#define		NL_TIME			3
#define		NL_TIME_NL		4
#define		NOTIME			5
#define		NOTIME_NL		6
#define		NL_NOTIME		7
#define		NL_NOTIME_NL	8

// used UARTBuf & LMODES

#define			DELI_FRAMESIZE			20

#define 	UARTMSG_MAX_SIZE			(DELI_FRAMESIZE+1)

#define 	UART1_DUMPSPEED			921.6 //  115.2 // 460.8   // 230.4 //  // 3000 // 1460 //    1500 //   
#define 	UART1_CONSOLE			115.2 //460.8    // 230.4 //   460.8   //  UART1_DUMPSPEED //  3000 // 1460 //    1500 //    
#define 	UART0_COMMANDS			460.8 //230.4    // 115.2 //

#define 	DBGTAB_SIZE				320
extern char		dbgtab[DBGTAB_SIZE];

//#define	POLLING			1
#define	ISR				2

//= Zawartosc "_Sharc_Session_Include_DAC.h" START	************
#define	DSPBLOCK2_DAC		2
#define	SMPBLOCK_SIZE		320   // 240  //
#define	SMPBLOCK_SIZE44		294   // 240  //
#define	RAMSEG_QTY			2	// 8 //	
#define	DSPBLOCK_SIZE1		(SMPBLOCK_SIZE*RAMSEG_QTY)
#define	DSPBLOCK_SIZE1R		(SMPBLOCK_SIZE44*RAMSEG_QTY)
//= Zawartosc "_Sharc_Session_Include_DAC.h" END  	************		

#define		DSPBLOCK_SIZE2		(2*DSPBLOCK_SIZE1*2)
#define		DSPBLOCK_SIZE2R		(2*DSPBLOCK_SIZE1R*2)
#define		BLRAM_QTY			3
#define		DSPBUF_QTY			3

#define 	OUTBLOCK_SIZE			SMPBLOCK_SIZE
#define		OVSBLOCK_SIZE			(SMPBLOCK_SIZE* 2) //17-KWI-17  (SMPBLOCK_SIZE* OVS_RATIO/OUT_OVS)  // !! Powinno byc (SMPBLOCK_SIZE*OVS_RATIO)

#define 	STAGES_QTY		21
#define 	DSP_QTY 		17
#define 	STAGES_EXT3		(DSP_QTY)   // STAGES_EXT3-1 [14]->CTB  STAGES_EXT3-5 [10]->EXPH


#define		VLEND			800
#define 	PEAK_QTY		25  


#define			SDRAM_USED
#define			SHARC_QUATTRO			// na potrzeby konfiguracji LED


#define	GREEN_2 	7
#define	BLUE_1 		6
#define	RED_4 		13
#define	NO_COLOR	0

//#define 	"E:\\Software\\Matlab\\Filtry\\_PROD_44k\\Session\\ys.txt"

#if defined(__ADSP21000__)
#if defined(__LONG_LONG_PROCESSOR_TIME__)
typedef long long clock_t;
#else
typedef long clock_t;
#endif
#endif

#if defined(__ADSPBLACKFIN__)
typedef long long clock_t;
#endif

#if defined(__ADSPTS__)
typedef long long clock_t;
#endif

//-----------------------------------------------------------------------------------------------
//=================    C L O C K   C O N T R O L      ===========================================
//-----------------------------------------------------------------------------------------------

#define		PWMVolume		(1<<3)
#define		PWM_4CH			(1<<4)


#define		SR_MAX_xCD		4
// Sample Rates
#define 	CD44_16			0x0044 	//0x01
#define 	CD48_16			0x0048	//0x02
#define 	HD88_16			0x8816	//0x0A	
#define 	HD96_16			0x9616	//0x0B	
#define 	HD88_24			0x0088	//0x0C
#define 	HD96_24			0x0096	//0x0D
#define 	UD176_16		0x6110	//0x11	
#define 	UD192_16		0x6130	//0x12	
#define 	UD176_24		0x6110	//0x13
#define 	UD192_24		0x6130	//0x14
#define 	XD352_24		0xD220	//0x??
#define 	XD384_24		0xD258	//0x??

#define 	SR_XBUF					(((int)SR_PROC::SAMPLE_RATE & 0xFFF)/0x44)
#define 	TEST_SR_44K_DOMAIN		(!((SR_PROC::SAMPLE_RATE&0xFFF)%0X44))
#define 	CLK_BASE				TEST_SR_44K_DOMAIN
#define 	SR_44_48				(1-CLK_BASE)

#define		DWN_LEVEL_QTY			2
#define 	BMT_FILTER_X_QTY		(SR_PROC::DAC_SR/2+DWN_LEVEL_QTY)
#define 	MID_TWT_RSMP_X_QTY		(SR_PROC::DAC_SR/2+DWN_LEVEL_QTY)

extern char	SR_string_tab[22][2];


// Tryby DDS_CLK_MODE
#define 	CLK44				44
#define 	CLK48				48
#define 	DEFAULT_CLK			5  // Tryb startowy


// CORRECTION types
#define		CORRECTION_A		0
#define		CORRECTION_B		1
#define		CORRECTION_C		2
#define		CORRECTION_D		3


//==================================================================
// ----------  F U N C T I O N S  uzywane w wielokrotnie    --------
//==================================================================

	EXTERNC void	Print_Debug(int n, int format);
	
	EXTERNC void trace(char *prec, int UART_trace); 
	
	EXTERNC void I2C_EEPROM(void);	
	
	EXTERNC 	void  Send_Debug_ISR(char *tab2send, int msglen);
	
	EXTERNC void	 Clear_MODEC(void);

	EXTERNC void Print_DSPstring_Modes(void); 
	
	EXTERNC void UART_speed (int UART_kbaud, int UART_no);




EXTERNC void I2S_Init(void);


#define		RSMP_LEVEL_QTY			3
#define		CLK_BASE_QTY			2

#define		X_MASK 					0x01000000
#define		XP_MASK					0x02000000
#define		EP_MASK					0x04000000
#define		DB_MASK					0x00010000
#define		DM_MASK					0x00020000
#define		DT_MASK					0x00040000
#define		RB_MASK					0x00100000
#define		RM_MASK					0x00200000
#define		RT_MASK					0x00400000
#define		ET_MASK					0x00800000
#define		EB_MASK					0x08000000

#define		YB_MASK					0x00000100
#define		YM_MASK					0x00000200
#define		YT_MASK					0x00000400
#define		RP_MASK					0x00001000

#define		FLOW_QTY					4

#define		K_GETBUF_FLOW				0
#define		K_GET_FLOW					1
#define		k_and_kBUFS					2

#define		INPUT_BUF_QTY				2
#define		No1_INPUT_BUF				0
#define		No2_INPUT_BUF				1

#define 	MAX_YBUFS_PER_I2SLINE		4

typedef enum 
{
	DUARK_I2S=1,   SPDIF=2, RPI_I2S_TEST = 3
} I2S_OUT_TYPE;

EXTERNC void RPI_TX_ISR(int);
EXTERNC void RPI_RX_ISR(int);

   #define 		IN1_ACTIVE						1	// SPORT0A DAI18/DAI12
   
 #if (SOFT_V & SH_BASE || SOFT_V & DELI_BASE) 
	#define 		OUT1A_ACTIVE				1
	#define 		OUT1B_ACTIVE				1	
 #elif (SOFT_V & FP_BASE) 
	#define 		OUT1A_ACTIVE				1
	#define 		OUT1B_ACTIVE				1		
 #endif
 
 #if (SOFT_V == SH_XOEP_PROD) 
	#define 		OUTPI_ACTIVE 				1
 #endif
 	  
 #if ((SOFT_V & FP_TEST2RX) == FP_TEST2RX)   // For  FP_KOEP_PROD only I2S used  
	#define			INTWT_ACTIVE 				1
 #else
	#define			INTWT_ACTIVE 				0	
 #endif	
  
/* 
 #if (I2SREC_ACTIVE == 1)
	#define 		IN2_ACTIVE					1	// SPORT4A ->DAI07
 #endif
*/
 
 
EXTERNC void UART_Setup (void);
EXTERNC void  LED_init(void);

EXTERNC void Audio_Clk_IntClear(void);

EXTERNC void InitPLL_SDRAM(void);
EXTERNC void Print_DSPstring_Modes(void);
EXTERNC	void LED_SET(int red, int blue, int green,   int duration_ms);

void 	SRU_SP0RX_SPDIF(void);
 void 	 SRU_SPORT0A_SPDIF(void);
 void 	 SRU_SPORT0B(void);
 
void 	SRU_SP3TX_FPGA(void);
void	 SRU_SPORT3A_FPGA(void);
 void	 SRU_SPORT3B_FPGA(void);

void 	SRU_SP5TX_SPDIF(void);
 void	 SRU_SPORT5A_SPDIF(void);

void 	SRU_SP2RX_RPI(void);
 void 	 SRU_SPORT2A_RPI(void);
 
#define		EMPTY_STAMP			0x7FFE00 

#define		_GETX_SPDIF			1
#define		_GETX_SHARC			2
#define		_GETX_RPI			3

extern int		RPI_SR; 

#define		SPK_QTY						3
#define		RSMPOBJ_QTY					(SPK_QTY+2)
#define		FILTOBJ_QTY					(SPK_QTY+1)

#define		DSP_FILT					1
//#define		DSP_SM						1
#define		DSP_RSMP					2

#define		DSPSTRING_LEN				(14+2)  // tyle faktycznych znakow bez zera na koncu // +2 - rozszerzenie kodu kolumny do 4 znakow

#define		DAC_CODAC_MODE_0			0
#define		DAC_CODAC_MODE_1			1
#define		SPK_CODE_CH3				2
#define		SPK_CODE_CH4				3


#define		MC_BAS_MT					(2+2)
#define 	SR_2CHAR					(4+2)
#define 	PRE_EXPH_MOD				(6+2)

#define		IPL_BAS						(8+2)
#define		IPL_MID						(9+2)
#define		IPL_TWT						(10+2)
#define		FR20k_EQ_LEVEL				(11+2)
#define		IPL_EXT						(13+2)
#define		xOVS_EXT_LEVEL				(14+2)

#define 	IPL_QTY						4 // 5  - usunieta interp. G

//= DSP String, pos 2   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define		MCBAS_BIT0					0
#define		MCBAS_BIT1					1
#define		MCPRE_BIT					2
#define		MCTWT_BIT					3

#define		DSPSTR_MCMT					(MCTWT_BIT|((SR_2CHAR-2)<<4))  
#define		DSPSTR_MCLIN_TWT			(MCTWT_BIT|((SR_2CHAR-2)<<4))  
#define		DSPSTR_BAS0					(MCBAS_BIT0|((SR_2CHAR-2)<<4))  // Bit0 on String[2]
#define		DSPSTR_BAS1					(MCBAS_BIT1|((SR_2CHAR-2)<<4))  

//= DSP String, pos 6	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define		EXPH_BITPOS					0
#define		MODEC_BITPOS				1
#define		MCPRE2_BITPOS				2

#define		DSPSTR_MODEC				(MODEC_BITPOS|((SR_2CHAR+2)<<4))  // Bit1 on String[6]
#define		DSPSTR_ACOUSTIC_CORR		(MODEC_BITPOS|((SR_2CHAR+2)<<4))  // Bit1 on String[6]
#define		DSPSTR_EXPH					(EXPH_BITPOS|((SR_2CHAR+2)<<4))  
#define		DSPSTR_MCPRE				(MCPRE2_BITPOS|((SR_2CHAR+2)<<4))  


#define		DSPSTR_TWT_OVSEQ_0			(0|(FR20k_EQ_LEVEL<<4))  
#define		DSPSTR_TWT_OVSEQ_1			(1|(FR20k_EQ_LEVEL<<4))  

#define		STAGE_DESC_SIZE				12

#define		BAS_CORR_LEVEL_GET			(DSPSTRING::Get_Digitbit(DSPSTR_BAS0)|DSPSTRING::Get_Digitbit(DSPSTR_BAS1))
#define		OVSFILT_EQ_LEVEL_GET		(DSPSTRING::Get_Digitbit(DSPSTR_TWT_OVSEQ_0)|DSPSTRING::Get_Digitbit(DSPSTR_TWT_OVSEQ_1))
#endif

#define 	DWN_1						1


EXTERNC void  	InitPLL_SDRAM(void);
EXTERNC void  	Print_DSPstring_Modes(void);
EXTERNC	void  	LED_SET(int red, int blue, int green,   int duration_ms);
EXTERNC void 	LED_blink(int color1,int color2, int color3,int n, int len, int prewrap, int postwrap); 
EXTERNC void  	FlagLED_Setup(void);
EXTERNC void  	Version_LED(void);
EXTERNC void  	SDRAM_clear(void);
EXTERNC void  	Core_SETUP_SDRAM_clear(void);
EXTERNC void 	reserve(void);
EXTERNC void 	trace(char *prec, int ch_qty);
EXTERNC int 	MODEC_filt_qty(int chn);

#define			xTAIL_ITPL				5
#define			RSMP_LEV2				4*4
#define			EP_MAXPRESIZE			1550

#define			DSPBUF_SIZE				(DSPBLOCK_SIZE2+xTAIL_ITPL+RSMP_LEV2+xTAIL_ITPL+RSMP_LEV2+ EP_MAXPRESIZE)
//#define 		EP_SIZE					(DSPBUF_SIZE-xTAIL_ITPL) // (DSPBLOCK_SIZE2+1000) //  //2600 // DSPBLOCK_SIZE2  // 
//#define 		EP_SIZE2				(DSPBLOCK_SIZE2+EP_PRESIZE+ EP_POSTSIZE)


#define 		STAGE_ON_MARK			'|' 
#define 		STAGE_OFF_MARK			'x' 	

#define 		xTAIL_POLY4				3
#define 		xTAIL_POLY6				5
#define			xMAIN_CLK_COUNTING		OFF			
#define			BUFSRC_UART_TRACE		OFF			
#define			LMK_SOFTCONFIG			OFF // ON //

//= Base colors for Sample Rates
#define			Avocado					0xC7DB00
#define			Greenery				0x6FB98F
#define			Spring_Green			0x89DA59
#define			Granny_Smith			0xBBCF4A

#define			Wave					0x66A5AD


#define			Turquoise				0x5BC8AC
#define			Electric_Blue			0x4897D8
#define			Violet					0xE5A9FF
#define			Grapefruit				0xFA812F
#define			Hot_Pink				0xF52549


extern int 		DUMP_ENABLE;

#define			SPORTRX_SRCBUF			1
#define			SPORTRX_DIRECTBUF		2

#define 		UART_TX_MONITOR			0	

extern			int 					RXA[];
extern			int 					RXB[];
extern			int 					k_RXA, k_RXB;

#define			FDUMP_SIZE				17000

#define			xLEDRGB_SRTAB			(128+0x01)
#define			xFIRXO_TAB				0x02

#if (XOEP_SPDIF_SPORT5 == 1)    
 #define		I2STX_QTY				3
#else  
 #define		I2STX_QTY				2
#endif                                                                                                                                                   


EXTERNC void 			Send1Byte(int byte2send);
 EXTERNC  void Delay_ms(int ms); 
 
#define		I2S0_SPORT3A					0
#define		I2S1_SPORT3B					1
#define		I2S2_SPDIF_SPORT5A				2
 
extern "C" void LMK_OUT_ISR(int dummy);
extern "C" void SPDIF_SPORT5_ISR(int dummy);

#define 	SET_FIR				1
#define 	CLR_FIR				1
#define 	FPGAFIR_QTY			4

#define		x55_XO				0
#define		x55_TWT				1
#define		x55_EP				2
#define		x55_DD_RES			3


#ifndef MCEP_H
#define MCEP_H

#define		SPKCODE_LEN		10 

#define		MCB_MAXSECT		50
#define		MCT_MAXSECT		100	
#define		MCA_MAXSECT		100
#define		EPS_MAXSECT		20
#define 	EPFIR_SIZE		1504
#define		MAL_MAXSECT		50
#define 	MAL_LEN			980

typedef struct  {
char		spkcode[SPKCODE_LEN+1]; 
float		MCB[2][MCB_MAXSECT*4+2];
float		MCT[6][MCT_MAXSECT*4+2];
float		EPS[6][EPS_MAXSECT*4+2];
long long  	EPF[6][EPFIR_SIZE];
}  MCEP_T;

typedef struct  {
char		acucode[SPKCODE_LEN+1]; 
float		MCA[2][MCA_MAXSECT*4+2];
}  ACU_T;

typedef struct  {
float		MAL_S[5][MAL_MAXSECT*4+2];
long long	MAL_F[5][MAL_LEN];
}  MAL_T;

#endif

#define		numLED_SR			0x0100

#define 	MULTISPK		1
#define 	xINC			2

#define 	xDSPMODE 		xMULTISPK  //xINC //  

#define 	UART_OLD 		1 
#define 	UART_NEW 		2  
#define 	UART_TYPE 		UART_OLD  

#define		MODES_QTY		21

/*
typedef struct  {
char		*spkcode; //[8];
float		MCB44[2];
int 		MCB44_SECT;
float		MCB48[2];
int 		MCB48_SECT;
float		MCT44[2];
int 		MCT44_SECT;
float		MCT48[2];
int 		MCT48_SECT;
float		EPM44[2];
int			EPM44_SECT;
float		EPM48[2];
int			EPM48_SECT;
}  xMCEP_T;
*/

/*
typedef struct  {
char		spkcode[11]; //[8];
float		*MCB[2];
float		*MCT[6];
float		 *EP[6];
}  MCEP_T;
*/
