#ifndef FTCONFIG_H
#define FTCONFIG_H

  #if	(SOFT_V & SH_BASE || SOFT_V == FP_XOEP_TEST))
   	#include 	"FTab_CORE_SPK4CHR.h"   
 //	char const DSPSTRING_default[DSPSTRING_LEN+2] = "TIB_440_zlj1_l1"; 	 
 //	char const DSPSTRING_default[DSPSTRING_LEN+2] = "Xd42B_443_zlj1_l1"; 
 //	char const DSPSTRING_default[DSPSTRING_LEN+2] = "XSA2B_483_zlj1_l1"; 	
	char DSPSTRING_default[DSPSTRING_LEN+2] = "XdA2B_443_zlj1_l1"; 			 	 	
  #elif	(SOFT_V & FP_BASE)
  	#include 	"FTab_CORE_FPGA.h"
  	#include 	"FTab_DG_PAZ.h"
  #endif	
  
   #include		"FTab_XdA2_XT_d6inch4R_B4mH_M220ux2.h"

#endif   // FTCONFIG_H