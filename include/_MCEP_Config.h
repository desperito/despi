#ifndef MCEPCONFIG_H
 #define MCEPCONFIG_H

  extern float pm OVB19B[]; 
  extern  SECTION  ("seg_dmda2") 	float OVB19B_state[]; 
  extern  SECTION  ("seg_dmda2")	float OXB19B_state[]; 
 
  extern float pm OVT19B[]; 
  extern  SECTION  ("seg_dmda2") 	float OVT19B_state[]; 
  extern  SECTION  ("seg_dmda2") 	float OXT19B_state[]; 
    
  extern float pm R0B44A[]; 
  extern float  SECTION  ("seg_dmda2") R0B44A_state[]; 

  extern float pm DYB96B[];    
  extern float  SECTION  ("seg_dmda2") DXB96B_state[];
  extern float  SECTION  ("seg_dmda2") DYB96B_state[];  

  extern float pm DYB88B[];    
  extern float  SECTION  ("seg_dmda2") DXB88B_state[];
  extern float  SECTION  ("seg_dmda2") DYB88B_state[];  

  extern float pm DYB17B[];    
  extern float  SECTION  ("seg_dmda2") DXB17B_state[];
  extern float  SECTION  ("seg_dmda2") DYB17B_state[];  
      
  	#define 		EP_HEX		16
  	#define 		MC_HEX		8
  	
    char  			SECTION  ("seg_sdram2")  	MCEP::IPL_TAB[] = {'S','W','W'}; 
  	unsigned int	SECTION  ("seg_sdram2")		MCEP::FlagsDSP =  (0x0C <<MC_HEX) | (0x00 <<EP_HEX) // | 0x0A     
    																							; 
 
/*    	
 #if  (SOFT_V == SH_DSP || SOFT_V == SH_XOEP_PROD || SOFT_V ==  DELI_RSMP)

  	//#include   "FTab_D4TC_TC2x_NR10k_15k.h"
  	//char DSPSTRING_default[DSPSTRING_LEN+2] = 	"D4TCB_442_zlw1_l1";
    
    #include  "FSect_RESONANCE.h"
	//#include  "FSect_Jeze_wskiego.h"
	
  #if UART_TYPE == UART_OLD 
   #if  (SOFT_V == SH_DSP || SOFT_V ==  DELI_RSMP)   	
	#include     "FTab_CORE_SPK4CHR.h"  
	//#include     "FTab_CORE_TWT_DIRAC.h"	
   	#include     "FTab_CORE_BAS_XOSH.h"   	
   #elif  (SOFT_V == SH_XOEP_PROD)
  	#include     "FTab_FP_XOEP_CORE_SPK4CHR.h"
   	//#include     "FTab_CORE_BAS_XOFP.h"   	
   #elif  (SOFT_V == FP_FORD)
  	#include     "FTab_FP_XOEP_CORE_SPK4CHR.h"       
    #include     "FTab_CORE_MID.h"    
   #endif 	
  #elif  UART_TYPE == UART_NEW
  
 #endif
*/

#endif

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   
