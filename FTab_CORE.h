
#include "FSections_CORE.h"
#include "FSect_TRI_TEST.h"
#include 	"FSect_XU_1st_150u_LIS0.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@   PRE stages   @@@@@@@@@@@@@@@@@@@@@@@@@@@     
//section ("seg_sdram2") struct  DSP_Steps_TAB	 S_PRE[1]; //= {}; 
//SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_PRE[]= { 
// { &FILT[PRE] ,  	0, 	IIR 	, MCPRE44 , MCPRE44_state , NCPRE44_state, 	"_*1_44*_****_**" ,' ' , 	MCPRE44_SECT,  	DWN_1,	0, "P01_IIR_MCPRE44"},
// { &FILT[PRE] , 	0, 	IIR 	, MCPx44Q3 , 	MCPx44Q3_L , 		MCPx44Q3_R,   	"_*1_44*_****_**" ,	'4',	MCPx44Q3_SECT,	DWN_1,	0, "P00_MCP KEF Q350"},
//};

// @@@@@@@@@@@@@@@@@@@@@@@@@@@   BAS stages   @@@@@@@@@@@@@@@@@@@@@@@@@@@     
SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_BAS[]= {
//= Downsampling	
// {	&FILT[BAS], 	0, 	DOSI,	DIRACx1 , 		DIRACx1_L, 			DIRACx1_R,			":*1_4**_****_**",	'D', 	DYB88B_SECT,  		4,	   	80, "Tri DN4x_DYB88"}   
 {	&FILT[BAS], 	0, 	DOSI,	DYB88B , 		DYB88B_state, 		DXB88B_state,		":*1_4**_****_**",	'D', 	DYB88B_SECT,  		4,	   	80, "DN4x_DYB88"}   
,{	&FILT[BAS], 	0, 	DOSI, 	DYB96B , 		DYB96B_state, 		DXB96B_state,		":*1_8**_****_**",	'D', 	DYB96B_SECT, 		8,		80, "DN8x_DYB96"}   
,{	&FILT[BAS], 	0, 	DOSI, 	DYB17B , 		DYB17B_state, 		DXB17B_state,		":*1_1**_****_**",	'D', 	DYB17B_SECT, 		16,		80, "DN1x_DYB17"} 
 
,{	&FILT[BAS], 	2,	FMODEC, R0B44A, 		R0B44A_state, 		R0B44A_state,		":**_**2_****_**" ,	'M',	R0B44A_SECT, 		DWN_1,	0, 	"MODC_ROB44"}  
//#########  RESAMPLER stages   #############   
,{	&RSMP[BAS], 	0, 	IIR,   	OVB19B,			OVB19B_state, 		OXB19B_state,		":**_***_****_**",	'X',	OVB19B_SECT,		DWN_1,	141,"XOB_OVB19"}  
//,{	&RSMP[BAS],		2, 	IIR,   	DIRACx2_BAS,	DIRACx2_BAS_L, 	DIRACx2_BAS_R,			":**_***_****_**",	 	'X',		DIRACx2_BAS_SECT,	DWN_1,	0, 	"Tri Bas"}
};

// @@@@@@@@@@@@@@@@@@@@@@@@@@@   MID stages   @@@@@@@@@@@@@@@@@@@@@@@@@@@   

SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_MID[]= {
 { &FILT[MID] , 	0 , 	NUL,	NULL , 		NULL , 				NULL	, 			"XX*_***_****_**" , 'I', 	0, 					DWN_1,  0, 	"NULL_Dummy" }  
 }; 
// @@@@@@@@@@@@@@@@@@@@@@@@@@@   TWT stages   @@@@@@@@@@@@@@@@@@@@@@@@@@@     
SECTION(("seg_sdram2")) struct  DSP_Steps_TAB  S_TWT[]= {
 
  { &FILT[TWT],  	0 , IIR, 	MCT48A, 		MCT48A_state 	, 	NCT48A_state 	, 	"***_4**_**s1_**" ,	1, 		MCT48A_SECT, 		DWN_1, 	0, 	"s144_MCT-A" } // Korekcja Step 2,7dB pik@19k do 2dB Nyquist
, { &FILT[TWT],  	0 , IIR, 	MCT48C, 		MCT48C_state 	, 	NCT48C_state 	, 	"***_4**_**S1_**" ,	1, 		MCT48C_SECT, 		DWN_1,	0, 	"S144_MCT-C" } // Korekcja Step+F_Craven 4.2dB pik@19k do 0dB Nyquist
, { &FILT[TWT],  	0 , IIR, 	MCT48D,			MCT48D_state 	, 	NCT48D_state 	, 	"***_4**_**S2_**" ,	2 , 	MCT48D_SECT, 		DWN_1, 	0, 	"S244_MCT-D" } // Korekcja Step+F_Craven 2,3dB pik@17k do 0dB Nyquist

 //= 44kHz Korekcje amplitudowe przetwarzania D/A
 	//= Linear
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_l1,	MCDAC_44_l1_state, 	NCDAC_44_l1_state,	"***_44*_**l1_**", 	1,		MCDAC_44_l1_SECT,	DWN_1, 	0, 	"l144_MCDAC" }   
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_L1, 	MCDAC_44_L1_state, 	NCDAC_44_L1_state, 	"***_44*_**L1_**", 	1,	 	MCDAC_44_L1_SECT,	DWN_1, 	0, 	"L144_MCDAC" } 
	//= Lagrange
//, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_g1, 	MCDAC_44_g1_state, 	NCDAC_44_g1_state, 	"***_44*_**g1_**",	1,	 	MCDAC_44_g1_SECT, 	DWN_1, 	0, 	"g144_MCDAC" } 
, { &FILT[TWT],  	0 , IIR, 	MCT48A, 		MCT48A_state 	, 	NCT48A_state 	, 	"***_44*_**g1_**",	1,	 	MCDAC_44_g1_SECT, 	DWN_1, 	0, 	"g144_MCDAC" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_G1, 	MCDAC_44_G1_state, 	NCDAC_44_G1_state, 	"***_44*_**G1_**",	1,	 	MCDAC_44_G1_SECT, 	DWN_1, 	0, 	"G144_MCDAC" } //LAG 4.2dB pik@19k do 0dB Nyquist   

, { &FILT[TWT],  	0 , IIR, 	MCOVS44_h4, 	MCOVS44_h4_L, 		MCOVS44_h4_R, 		"***_44*_**h1_**",	1,	 	MCOVS48_h4_SECT, 	DWN_1, 	0, 	"h044_MCOVS" } 
, { &FILT[TWT],  	0 , IIR, 	MCOVS48_h4, 	MCOVS48_h4_L, 		MCOVS48_h4_R, 		"***_48*_**h1_**",	1,	 	MCOVS48_h4_SECT, 	DWN_1, 	0, 	"h048_MCOVS" } 
, { &FILT[TWT],  	0 , IIR, 	MCOVS44_w, 		MCOVS44_w_L, 		MCOVS44_w_R, 		"***_44*_**w1_**",	1,	 	MCOVS48_w_SECT, 	DWN_1, 	0, 	"w44_MCOVS" } 
, { &FILT[TWT],  	0 , IIR, 	MCOVS48_w, 		MCOVS48_w_L, 		MCOVS48_w_R, 		"***_48*_**w1_**",	1,	 	MCOVS48_w_SECT, 	DWN_1, 	0, 	"w48_MCOVS" } 
, { &FILT[TWT],  	0 , IIR, 	MCOVS44_h63, 	MCOVS44_h63_L, 		MCOVS44_h63_R, 		"***_44*_**i1_**",	1,	 	MCOVS44_h63_SECT, 	DWN_1, 	0, 	"h344_MCOVS" } 
, { &FILT[TWT],  	0 , IIR, 	MCOVS48_h63, 	MCOVS48_h63_L, 		MCOVS48_h63_R, 		"***_48*_**i1_**",	1,	 	MCOVS48_h63_SECT, 	DWN_1, 	0, 	"h344_MCOVS" } 
, { &FILT[TWT],  	0 , IIR, 	MCOVS44_h65, 	MCOVS44_h65_L, 		MCOVS44_h65_R, 		"***_44*_**j1_**",	1,	 	MCOVS44_h65_SECT, 	DWN_1, 	0, 	"h544_MCOVS" } 
, { &FILT[TWT],  	0 , IIR, 	MCOVS48_h65, 	MCOVS48_h65_L, 		MCOVS48_h65_R, 		"***_48*_**j1_**",	1,	 	MCOVS48_h65_SECT, 	DWN_1, 	0, 	"h544_MCOVS" } 

, { &FILT[TWT],  	0 , IIR, 	MCT48A, 		MCT48A_state 	, 	NCT48A_state 	, 	"***_44*_**b1_**",	1,	 	MCDAC_44_g1_SECT, 	DWN_1, 	0, 	"o144_MCDAC" } 

, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_b2, 	MCDAC_44_b2_state, 	NCDAC_44_b2_state, 	"***_44*_**b2_**",	2,	 	MCDAC_44_b2_SECT, 	DWN_1, 	0, 	"b244_MCDAC" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_B1, 	MCDAC_44_B1_state, 	NCDAC_44_B1_state, 	"***_44*_**B1_**",	1, 		MCDAC_44_B1_SECT, 	DWN_1, 	0, 	"B144_MCDAC" } 
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_B2,	MCDAC_44_B2_state, 	NCDAC_44_B2_state, 	"***_44*_**B2_**", 	2, 		MCDAC_44_B2_SECT, 	DWN_1, 	0, 	"B244_MCDAC" } 
 	//= FIR alone
, { &FILT[TWT],  	0 , IIR, 	MCDAC_44_F1, 	MCDAC_44_F1_state, 	NCDAC_44_F1_state,	"***_44*_**Z1_**",	1,		MCDAC_44_F1_SECT, 	DWN_1, 	0, 	"Z144_MCDAC" } 

//= 48kHz Korekcje amplitudowe przetwarzania D/A
	//= Lagrange
, { &FILT[TWT],  	0 , IIR, 	MCDAC_48_g1, 	MCDAC_48_g1_state, 	NCDAC_48_g1_state , "***_48*_**g1_**" ,	1, 		MCDAC_48_g1_SECT, 	DWN_1, 	0, 	"g148_MCDAC" } 
, { &FILT[TWT],  	0 ,	IIR, 	MCDAC_48_G1, 	MCDAC_48_G1_state,	NCDAC_48_G1_state , "***_48*_**G1_**" , 1,	 	MCDAC_48_G1_SECT,	DWN_1, 	0, 	"G148_MCDAC" } 

 //==== CODAC   ====
		// #########  RESAMPLER stages   #############   
//,{	&RSMP[PRE],		2, 	IIR,   		EPP19XU,	EPP19XU_L, 		EPP19XU_R,		"XU*_**1_****_**",	'E',	EPP19XU_SECT,	DWN_1,	0, 	"XU192EP"}		
//,{	&RSMP[TWT],		0, 	IIR,   	DIRACx3_TWT,	DIRACx3_TWT_L, 		DIRACx3_TWT_R,		":**_44*_****_**" ,	'L',	DIRACx3_TWT_SECT,	DWN_1,	139, 	"Tri TWT"}
, { &RSMP[TWT],  0, IIR, 	OVT19B, 		OVT19B_state 	, 	OXT19B_state 	, 	":**_44*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	140,"XOT4x_OVT19"} // 76// usuniete z przedostatniego pola 'L'<<16 |'1'<<8 | 'F'// Filtr High Pass XO w trybie korektora, chyba z OVS		
, { &RSMP[TWT],  0 , IIR, 	OVT19B, 		OVT19B_state 	, 	OXT19B_state 	, 	":**_48*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	145,"XOT4x_OVT19"} // 76// usuniete z przedostatniego pola 'L'<<16 |'1'<<8 | 'F'// Filtr High Pass XO w trybie korektora, chyba z OVS

//, { &RSMP[PRE],  	3 , IIR, 	OVT19B, 		OVT19B_state 	, 	OXT19B_state 	, 	":**_443_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	115,"XOP4x_OVT19"} // 76// usuniete z przedostatniego pola 'L'<<16 |'1'<<8 | 'F'// Filtr High Pass XO w trybie korektora, chyba z OVS		
//, { &RSMP[PRE],  	3 , IIR, 	OVT19B, 		OVT19B_state 	, 	OXT19B_state 	, 	":**_483_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	122,"XOP4x_OVT19"} // 76// usuniete z przedostatniego pola 'L'<<16 |'1'<<8 | 'F'// Filtr High Pass XO w trybie korektora, chyba z OVS
 //=  Filtr high-pass bez oversamplingu
, { &RSMP[TWT], 	0 , IIR, 	OVT19B  , 		OVT19B_state 	, 	OXT19B_state 	, 	":**_88*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	133,"XOT88_OVT19B" } 
, { &RSMP[TWT], 	0 , IIR, 	OVT19B  , 		OVT19B_state 	, 	OXT19B_state 	, 	":**_96*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	139,"XOT96_OVT19B" } 
, { &RSMP[TWT], 	0 , IIR, 	OVT19B 	, 		OVT19B_state 	, 	OXT19B_state 	, 	":**_17*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	129,"XOT17_OVT19B" } 
, { &RSMP[TWT], 	0 , IIR, 	OVT19B 	, 		OVT19B_state 	, 	OXT19B_state 	, 	":**_19*_****_**" ,	'L', 	OVT19B_SECT, 		DWN_1, 	136,"XOT19_OVT19B" } 
 //===  FIR_CH separate for CODAC mode (big first letter) due to delay needed ====== 
, { &RSMP[TWT], 	1,  IIR, 	MYT48B 	, 		MYT48B_state 	, 	MXT48B_state 	, 	":**_4**_**:*_**" ,	'G', 	MYT48B_SECT,  		DWN_1,	11,	"F4K_MYT48B"  } //-11 *RPI* UWAGA - to moze nie byc odpowidnik wczesniejszej konfiguracji LP-DAC rekonstrukcyjny na 20k      
, { &RSMP[TWT], 	1,  IIR,	MYT48C 	, 		MYT48C_state 	, 	MXT48C_state 	, 	":**_8**_**:*_**" , 'G', 	MYT48C_SECT, 		DWN_1,	12,	"F8K_MYT48C"  }  
 //===  FIR_CH for DAC mode (small first letter) - no delay needed ====== 
, { &RSMP[TWT], 	0, IIR, 	MYT48B 	, 		MYT48B_state 	, 	MXT48B_state 	,	".**_4**_**:*_**" ,	'F', 	MYT48B_SECT, 		DWN_1, 	0, 	"F4D_MYT48B"  }  // *RPI* UWAGA - to moze nie byc odpowidnik wczesniejszej konfiguracji Filtr /4 F_Craven_B - Filtr TWT, na ktory sa oddzielne korekcje MCT
, { &RSMP[TWT], 	0, IIR, 	MYT48C 	, 		MYT48C_state 	, 	MXT48C_state 	,	".**_8**_**:*_**" , 'F', 	MYT48C_SECT, 		DWN_1, 	0, 	"F8D_MYT48C"  }  // IPL + FIR-C lagodny
 };
