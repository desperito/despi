
#if !defined(DIRACx1_SECT) 
    #define  DIRACx1_SECT      1
    section  ("seg_dmda2")    float    	DYB88B_state[DIRACx1_SECT*2+ 1];
    section  ("seg_dmda2")    float     DXB88B_state[DIRACx1_SECT*2+ 1];
	#include ".\Filtry_2021\DYB88B.h"
#endif

#if !defined(DYB96B_SECT) 
    #define  DYB96B_SECT      16
    section  ("seg_dmda2")    float     DYB96B_state[DIRACx1_SECT*2+ 1];
    section  ("seg_dmda2")    float     DXB96B_state[DIRACx1_SECT*2+ 1];
    #include ".\Filtry_2021\DYB96B.h"
#endif

#if !defined(DYB17B_SECT_SECT) 
	#define  DYB17B_SECT      30
    section  ("seg_dmda2")    float     DYB17B_state[DIRACx1_SECT*2+ 1];
    section  ("seg_dmda2")    float     DXB17B_state[DIRACx1_SECT*2+ 1];
	#include ".\Filtry_2021\DYB17B.h"    
#endif


#if !defined(R0B44A_SECT) 
    #define  R0B44A_SECT      1
    section  ("seg_dmda2")    float     R0B44A_state[R0B44A_SECT*2+ 1];
  #include ".\Filtry_2021\R0B44A_TUN_Test1__B2_11025Hz_0dB@0_0dB@0Hz_RIPPLE_7.2dB_PRE0_POST39379.h"
#endif

#if !defined(OVB19B_SECT) 
    #define  OVB19B_SECT      3
    section  ("seg_dmda2")    float     OVB19B_state[OVB19B_SECT*2+ 1];
    section  ("seg_dmda2")    float     OXB19B_state[OVB19B_SECT*2+ 1];
#endif
