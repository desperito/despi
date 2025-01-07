

#include "filter_sections.h"

//=========   SIM CONTROL CONSTANTS  ==========
//#define 	RECORDING_ONOFF
#define		DAC					'D'
#define		CODAC				'K'
#define		SPEAKER_DEVICE		'S'

#define		INIT_REC_MODE		SIM_I2SOFF_D_ONLY

#define 	DWN_1				1


#define P44A
extern struct StageManager SMPre;
section ("seg_sdram2") struct  DSP_Steps_TAB	 S_PRE[1]; //= {}; 
//SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_PRE[]= { 
// { &FILT[PRE] ,  	0, 	IIR 	, MCPRE44 , MCPRE44_state , NCPRE44_state, 	"_*1_44*_****_**" ,' ' , 	MCPRE44_SECT,  	DWN_1,	0, "P01_IIR_MCPRE44"},
// { &FILT[PRE] , 	0, 	IIR 	, MCPx44Q3 , 	MCPx44Q3_L , 		MCPx44Q3_R,   	"_*1_44*_****_**" ,	'4',	MCPx44Q3_SECT,	DWN_1,	0, "P00_MCP KEF Q350"},
//};

#define B44A
extern struct StageManager SMBas;
SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_BAS[]= {
//= Downsampling	
 {	&FILT[BAS], 	0, 	DOSI,	DYB88B , 		DYB88B_state, 		DXB88B_state,	"_K*_4**_****_**",	'D', 	DYB88B_SECT,  		4,	   	90, 	"AB01_DOSI_DYB88B"}   //4*23    //-52
//,{ &SMBas , 1 	, DOSF 	, DYB96B , 	DYB96B_state , 		DXB96B_state 	,0 , 2 , DSPBLOCK_SIZE1 	,"%s", 	"_K*_88*_****_**" ,"D", 	DYB96B_SECT, 0xFFFF, 8,	0, "B01_DOSF_DYB96B"}  // DOWNSMP/4     
,{	&FILT[BAS], 	0, 	DOSI, 	DYB96B , 		DYB96B_state, 		DXB96B_state,	"_K*_8**_****_**",	'D', 	DYB96B_SECT, 		8,		90, 	"BB01_DOSI_DYB96B"}   //2*46     
,{	&FILT[BAS], 	0, 	DOSI, 	DYB17B , 		DYB17B_state, 		DXB17B_state,	"_K*_1**_****_**",	'D', 	DYB17B_SECT, 		16,		90, 	"CB01_DOSI_DYB17B"}  
,{	&FILT[BAS], 	1, 	IIR,   	MCBx44Q3,		MCBx44Q3_L, 		MCBx44Q3_R,		"_K3_-**_****_**",	 3,		MCBx44Q3_SECT,		DWN_1,	0, 		"Dx44MCB02_EQ_MCBAS"}  //MODEX 100   
,{	&FILT[BAS], 	1, 	IIR,   	MCBx48Q3,		MCBx48Q3_L, 		MCBx48Q3_R,		"_K3_=**_****_**",	 3,		MCBx48Q3_SECT,		DWN_1,	0, 		"Dx48MCB02_EQ_MCBAS"}  //MODEX 100   

//,{	&FILT[BAS], 	1, 	IIR,   	MCBASx44,		MCBASx44_state, 	NCBASx44_state,	"_K2_***_****_**",		' ',MCBASx44_SECT,		DWN_1,	0, 		"DMCB02_EQ_MCBASx44"}  //MODEX 100   
,{	&FILT[BAS], 	2,	FMODEC, R0B44A, 		R0B44A_state, 		R0B44A_state,	"_K*_**2_****_**" ,	'M',	R0B44A_SECT, 		DWN_1,	0, 		"B03_MODC_ROB44A" }  //MODEX 100

//#########  RESAMPLER stages   #############   
,{	&RSMP[BAS], 	0, 	IIR,   	OVB19B,			OVB19B_state, 		OXB19B_state,	"_K*_***_****_**",	'X',	OVB19B_SECT,		DWN_1,	141, 	"EXO_LP RSMP"}  //MODEX 100   
//,{&SMBas, 	15,	OVSI, 	OVB19B, 	OVB19B_state, 	OXB19B_state, 	0, 	2, 	DSPBLOCK_SIZE2,	 "%s", 	"_K*_***_****_**" ,	"O", 	OVB19B_SECT, 0xFFFF, 0,	'Z'<<16 |'X'<<8, "B15_OVSI_OVB19B"}  // LP XO BUT5 800Hz@UHD
};
#define M44A
extern struct StageManager SMMid;
SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_MID[]= {

 // Konfig filtry OVS CD - wyciecie image-ow dla wlaczonego FIR EXT	
 { &FILT[MID] , 	0 , 	NUL,	NULL , 			NULL , 				NULL	, 	"XX*_***_****_**" , 	'I', 0, 			DWN_1,  0, 	"M_M17_OVSE_OVM96D" }  
// { &FILT[MID] , 	0 , 	IIR,	OVM96D , 		OVM96D_state , 		OXM96D_state , 		"_*_***_****_**" , 	'I', 			OVM96D_SECT, 	0,  	0, 	"M17_OVSE_OVM96D" }  
  
//  ,{ &SMMid , 18 , IIR 	, MYM48B , MYM48B_state , MXM48B_state 	, 0 , 1 , DSPBLOCK_SIZE2 	,"%s", 	"_*_**_****_F2" ,"*", 	MYM48B_SECT, 0xFFFF, 0, 0, "M18_IIR_MYM48B" }  // @@@ RSMP // Filtr /4 F_Craven_B dla 44k: -3db@5k, -23dB@7k, -100dB@10k 
//  { &SMMid , 19 , IIR 	, MYM88B , MYM88B_state , MXM88B_state 	, 0 , 1 , DSPBLOCK_SIZE2 	,"%s", 	"_*_**_****_F4" ,"*", 	MYM88B_SECT, 0xFFFF, 0, 0, "M19_IIR_MYM88B" }       
//  ,{ &SMMid , 20 , IIR 	, MYM96B , MYM96B_state , MXM96B_state 	, 0 , 1 , DSPBLOCK_SIZE2 	,"%s", 	"_*_**_****_F8" ,"*", 	MYM96B_SECT, 0xFFFF, 0, 0, "M20_IIR_MYM96B" }     
  }; 
  
#define T44A
extern struct StageManager SMTwt;
SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_TWT[]= {
// { &FILT[TWT],  	3 , IIR, 	MCT44D 	, 	MCT44D_state 	, NCT44D_state , 		"_**_4**_xxz*_**" , ' ', 	MCT44D_SECT, 		DWN_1, 	0, 	"a_MCT44D" } // Test impulse response, zeby nie bylo tylko jednej wartosci 1 Dirac
//TEST ,  { &SMTwt,  3 , IIR, 	MCT44D 	, MCT44D_state 	, NCT44D_state 	, 0 , 1 , DSPBLOCK_SIZE1 ,	"%s", 	"_*_4*_Z1F_**" ,"*", 	MCT44D_SECT, 0xFFFF, 0, 0, "T03_IIR_MCT44D" } // "**Z*F*1*" Korekcja Z Craven, 1,7dB pik@19k do 1dB Nyquist
 
 { &FILT[TWT],  	0 , IIR, 	MCT48A, 	MCT48A_state 	, NCT48A_state 	, 		"_**_4**_**s1_**" ,	1, 		MCT48A_SECT, 		DWN_1, 	0, 	"b_MCT48A" } // Korekcja Step 2,7dB pik@19k do 2dB Nyquist
, { &FILT[TWT],  	0 , IIR, 	MCT48C, 	MCT48C_state 	, NCT48C_state 	, 		"_**_4**_**S1_**" ,	1, 		MCT48C_SECT, 		DWN_1,	0, 	"c_MCT48C" } // Korekcja Step+F_Craven 4.2dB pik@19k do 0dB Nyquist
, { &FILT[TWT],  	1 , IIR, 	MCT48D, 	MCT48D_state 	, NCT48D_state 	, 		"_**_4**_**S2_**" ,	2 , 	MCT48D_SECT, 		DWN_1, 	0, 	"d_MCT48D" } // Korekcja Step+F_Craven 2,3dB pik@17k do 0dB Nyquist

 //= 44kHz Korekcje amplitudowe przetwarzania D/A
 	//= Linear
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_l1, MCDAC_44_l1_state, NCDAC_44_l1_state,	"_**_44*_**l1_**", 	1,		MCDAC_44_l1_SECT,	DWN_1, 	0, 	"e_MCDAC_44_l1" }   
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_L1, MCDAC_44_L1_state, NCDAC_44_L1_state, "_**_44*_**L1_**", 	1,	 	MCDAC_44_L1_SECT,	DWN_1, 	0, 	"f_MCDAC_44_L1" } 
	//= Lagrange
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_g1, MCDAC_44_g1_state, NCDAC_44_g1_state, "_**_44*_**g1_**",	1,	 	MCDAC_44_g1_SECT, 	DWN_1, 	0, 	"g_MCDAC_44_g1" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_G1, MCDAC_44_G1_state, NCDAC_44_G1_state, "_**_44*_**G1_**",	1,	 	MCDAC_44_G1_SECT, 	DWN_1, 	0, 	"h_MCDAC_44_G1" } //LAG 4.2dB pik@19k do 0dB Nyquist   
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_g1, MCDAC_44_g1_state, NCDAC_44_g1_state, "_**_44*_**h1_**",	1,	 	MCDAC_44_g1_SECT, 	DWN_1, 	0, 	"i_MCDAC_44_g1->h1" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_G1, MCDAC_44_G1_state, NCDAC_44_G1_state, "_**_44*_**H1_**",	1,	 	MCDAC_44_G1_SECT, 	DWN_1, 	0, 	"j_MCDAC_44_G1->H1" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_g1, MCDAC_44_g1_state, NCDAC_44_g1_state, "_**_44*_**o1_**",	1,	 	MCDAC_44_g1_SECT, 	DWN_1, 	0, 	"i_MCDAC_44_g1->h1" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_G1, MCDAC_44_G1_state, NCDAC_44_G1_state, "_**_44*_**O1_**",	1,	 	MCDAC_44_G1_SECT, 	DWN_1, 	0, 	"j_MCDAC_44_G1->H1" } 

	//= B-Spline
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_b1, MCDAC_44_b1_state, NCDAC_44_b1_state, "_**_44*_**b1_**",	1,	 	MCDAC_44_b1_SECT, 	DWN_1, 	0, 	"k_MCDAC_44_b1" } 
, { &FILT[TWT],  	1 , IIR, 	MCDAC_44_b2, MCDAC_44_b2_state, NCDAC_44_b2_state, "_**_44*_**b2_**",	2,	 	MCDAC_44_b2_SECT, 	DWN_1, 	0, 	"l_MCDAC_44_b1" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_B1, MCDAC_44_B1_state, NCDAC_44_B1_state, "_**_44*_**B1_**",	1, 		MCDAC_44_B1_SECT, 	DWN_1, 	0, 	"m_MCDAC_44_B1" } 
, { &FILT[TWT],  	1 , IIR, 	MCDAC_44_B2, MCDAC_44_B2_state, NCDAC_44_B2_state, "_**_44*_**B2_**", 	2, 		MCDAC_44_B2_SECT, 	DWN_1, 	0, 	"n_MCDAC_44_B1" } 
 	//= FIR alone
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_F1, MCDAC_44_F1_state, NCDAC_44_F1_state,	"_**_44*_**Z1_**",	1,		MCDAC_44_F1_SECT, 	DWN_1, 	0, 	"o_MCDAC_44_F1" } 

//= 48kHz Korekcje amplitudowe przetwarzania D/A
	//= Lagrange
, { &FILT[TWT],  	0 , IIR, 	MCDAC_48_g1, MCDAC_48_g1_state, NCDAC_48_g1_state , "_**_48*_**g1_**" ,	1, 		MCDAC_48_g1_SECT, 	DWN_1, 	0, 	"p_MCDAC_48_g1" } 
, { &FILT[TWT],  	0 ,	IIR, 	MCDAC_48_G1, MCDAC_48_G1_state,	NCDAC_48_G1_state , "_**_48*_**G1_**" , 1,	 	MCDAC_48_G1_SECT,	DWN_1, 	0, 	"r_MCDAC_48_G1" } 
//, { &SMTwt , 2 	, IIR , MCPRE44 	, MCPRE44_state 	, NCPRE44_state		, 0	, 1 , DSPBLOCK_SIZE1 	,"%s", 	"_*_44_****_**" ,"*", 	MCPRE44_SECT, 0xFFFF, 4,	0, "T02_TEST_IIR_MCPRE44"}  // DOWNSMP/4    
, { &FILT[TWT], 	2, 	IIR , 	MCP44Q3 , 	MCP44Q3_L , 		MCP44Q3_R,   		"_*8_44*_****_**",	'C',	MCP44Q3_SECT,		DWN_1,	0, "MCP44 KEF Q350"}
, { &FILT[TWT], 	2, 	IIR , 	MCP48Q3 , 	MCP48Q3_L , 		MCP48Q3_R,   		"_*8_48*_****_**",	'C',	MCP48Q3_SECT,		DWN_1,	0, "MCP48 KEF Q350"}
, { &FILT[TWT], 	2, 	IIR , 	MCP88Q3 , 	MCP88Q3_L , 		MCP88Q3_R,   		"_*8_88*_****_**",	'C',	MCP88Q3_SECT,		DWN_1,	0, "MCP88 KEF Q350"}
, { &FILT[TWT], 	2, 	IIR , 	MCP96Q3 , 	MCP96Q3_L , 		MCP96Q3_R,   		"_*8_96*_****_**",	'C',	MCP96Q3_SECT,		DWN_1,	0, "MCP96 KEF Q350"}
, { &FILT[TWT], 	2, 	IIR , 	MCP17Q3 , 	MCP17Q3_L , 		MCP17Q3_R,   		"_*8_17*_****_**",	'C',	MCP17Q3_SECT,		DWN_1,	0, "MCP17 KEF Q350"}
, { &FILT[TWT], 	2, 	IIR , 	MCP19Q3 , 	MCP19Q3_L , 		MCP19Q3_R,   		"_*8_19*_****_**",	'C',	MCP19Q3_SECT,		DWN_1,	0, "MCP19 KEF Q350"}

 //==== CODAC   ====
   //=  Filtr high-pass + oversampling dla wszystkich oprocz UD
//#########  RESAMPLER stages   #############   
, { &RSMP[TWT],  	0 , IIR, 	OVT19B, 	OVT19B_state 	, 	OXT19B_state 	, 	"_K*_4**_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	128,	"s_OVT19B" } // 76// usuniete z przedostatniego pola 'L'<<16 |'1'<<8 | 'F'// Filtr High Pass XO w trybie korektora, chyba z OVS
 //=  Filtr high-pass bez oversamplingu
, { &RSMP[TWT], 	0 , IIR, 	OVT19B, 	OVT19B_state 	, 	OXT19B_state 	, 	"_K*_88*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	128,  "t_OVT19B" } 
, { &RSMP[TWT], 	0 , IIR, 	OVT19B, 	OVT19B_state 	, 	OXT19B_state 	, 	"_K*_96*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	137,  "u_OVT19B" } 
, { &RSMP[TWT], 	0 , IIR,  	OVT19B 	, 	OVT19B_state 	, 	OXT19B_state 	, 	"_K*_17*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	126,  "v_OVT19B" } 
, { &RSMP[TWT], 	0 , IIR,  	OVT19B 	, 	OVT19B_state 	, 	OXT19B_state 	, 	"_K*_19*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	134,  "w_OVT19B" } 
 //===  FIR_CH  ====== 
, { &RSMP[TWT], 	1,  IIR, 	MYT48B 	, 	MYT48B_state 	, 	MXT48B_state 	, 	"_K*_4**_**:*_**" ,	'G', 	MYT48B_SECT,  		DWN_1,	 11, "x_MYT48B" } //-11 *RPI* UWAGA - to moze nie byc odpowidnik wczesniejszej konfiguracji LP-DAC rekonstrukcyjny na 20k      
, { &RSMP[TWT], 	1,  IIR,  	MYT48C 	, 	MYT48C_state 	, 	MXT48C_state 	, 	"_K*_8**_**:*_**" , 'G', 	MYT48C_SECT, 		DWN_1,	 12,"FIIR RT15D_HD_OVSI_MYT48C"}  // -12 IPL + FIR-C lagodny


 //===  DAC  ======
, { &RSMP[TWT], 	0, IIR, 	MYT48B 	, 	MYT48B_state 	, 	MXT48B_state 	,	"_D*_4**_**:*_**" ,	'F', 	MYT48B_SECT, 		DWN_1, 	0, 	"z_FIRCH_MYT48B" }  // *RPI* UWAGA - to moze nie byc odpowidnik wczesniejszej konfiguracji Filtr /4 F_Craven_B - Filtr TWT, na ktory sa oddzielne korekcje MCT
, { &RSMP[TWT], 	0, IIR, 	MYT48C 	, 	MYT48C_state 	, 	MXT48C_state 	,	"_D*_88*_**:*_**" , 'F', 	MYT48C_SECT, 		DWN_1, 	0, 	"Z_MYT48C"}  // IPL + FIR-C lagodny
, { &RSMP[TWT], 	0, IIR, 	MYT48C 	, 	MYT48C_state 	, 	MXT48C_state 	,	"_D*_96*_**:*_**" , 'F', 	MYT48C_SECT, 		DWN_1, 	0, 	"X_MYT48C"}  // IPL + FIR-C lagodny

 //, { &SMTwt, 15 , OVSI, VVT19A 	, VVT19A_state 	, VXT19A_state 	, 0 , 2 , DSPBLOCK_SIZE2R , "%s", 	"_D_4*_**.*_**" ,"", 	VVT19A_SECT, 0xFFFF,  0, 0,  "T15D_4x_OVSI_VVT19A" }  // *RPI* UWAGAFiltr jedynkowy Dirac - Wstawianie zer - jezeli nie jest wlaczony FIR_CH 
//@RSMP , { &bSMTwt, 15 , OVSI, VVT19A 	, 		VVT19A_state 	, VXT19A_state 		, 0 , 2 , DSPBLOCK_SIZE2 , "%s", 	"_D*_4**_**.*_**" ,"", 		VVT19A_SECT, 0xFFFF,  0, 0,  "T15D_4x_OVSI_VVT19A" }  // *RPI* UWAGAFiltr jedynkowy Dirac - Wstawianie zer - jezeli nie jest wlaczony FIR_CH 
//@RSMP , { &bSMTwt, 15 , OVSI, VVT19A 	, 		VVT19A_state 	, VXT19A_state 		, 0 , 2 , DSPBLOCK_SIZE2 , "%s", 	"_D*_9E*_**.*_**" ," ", 	VVT19A_SECT, 0xFFFF,  0, 0,  "T15D_88_OVSI_VVT19A" }  // *RPI* UWAGAFiltr jedynkowy Dirac - Wstawianie zer - jezeli nie jest wlaczony FIR_CH  
// , { &SMTwt, 15 , OVSI, MYT48C 	, 		MYT48C_state 	, MXT48C_state 		, 0 , 2 , DSPBLOCK_SIZE2 ,	"%s", 	"_D*_96*_**:*_**" ,"", 	MYT48C_SECT, 0xFFFF,  0, 0, "T15D_96_OVSI_MYT48C"}  // IPL + FIR-C lagodny
 //, { &SMTwt, 15 , OVSI, VVT19A 	, 		VVT19A_state 	, VXT19A_state 		, 0 , 2 , DSPBLOCK_SIZE2R , "%s", 	"_D*_96*_**.*_**" ,"", 	VVT19A_SECT, 0xFFFF,  0, 0,  "T15D_96_OVSI_VVT19A" }  // *RPI* UWAGAFiltr jedynkowy Dirac - Wstawianie zer - jezeli nie jest wlaczony FIR_CH  
 }; 

/*

_D_44_Xbl1_b8
012345678901234
_D_44_Z1F_F8
00-01) 	_D		1) DAC_CODAC_MODE_0    // za nazwa zestawu idzie korekcja amplitudowa MC i zwrotnica
02)     _
03-04) 	44		3-4) SR_2CHAR 
05)     _

06-08) 	Z1F   	6) IPL_CHN_TYPE_2= SMTwt.SPK_XO_IPL_TYPE; 
				7) FR20k_EQ_LEVEL_3= PlaySession.FR20k_EQ+48; 
				8) FIR_CH_BAS_TWT_4 = 'F'/'X' <-PlaySession.FIR_CH-> 
09)     _
10) 	F		10) FIR_EXT_MID_5= SMMid.SPK_XO_IPL_TYPE; 
11) 	8		11) xOVS_EXT_LEVEL_7= PlaySession.OVS_EXT+48, 
//============================================================
Value set for IPL: b/g/l/B/G/L/F/X 
06) 	b   	6)  IPL_BAS = SMBas.SPK_XO_IPL_TYPE; 
07)     _		
08)     g		8)  IPL_MID = SMMid.SPK_XO_IPL_TYPE; 
09)     _		
10-11)  B1		10) IPL_TWT = SMTwt.SPK_XO_IPL_TYPE; 
				11) FR20k_EQ_LEVEL_3= PlaySession.FR20k_EQ+48; 
12)     _
10) 	F		10) IPL_EXT = PlaySession.EXT_IPL_TYPE; 
11) 	8		11) xOVS_EXT_LEVEL_7= PlaySession.OVS_EXT+48, 

*/
