	
//-------------------------------------------------------------------------
//===  Stages   B A S   ===  
#if !defined(DYB88B_SECT) && !defined(DYB88B_TAPS)
    #define  DYB88B_SECT      12
    section  ("seg_dmda2")    float    	DYB88B_state[DYB88B_SECT*2+ 1];
    section  ("seg_dmda2")    float     DXB88B_state[DYB88B_SECT*2+ 1];
  #include "DYB88B_TUN_x4_B24_11025Hz_-3dB@5040_-60dB@8610Hz_RIPPLE_0.8dB_PRE22_POST159.h"
#endif

#if !defined(DYB96B_SECT) && !defined(DYB96B_TAPS)
    #define  DYB96B_SECT      16
    section  ("seg_dmda2")    float     DYB96B_state[DYB96B_SECT*2+ 1];
    section  ("seg_dmda2")    float     DXB96B_state[DYB96B_SECT*2+ 1];
  #include "DYB_HD_B_TUN_B32__x8_5513Hz_-3dB@2730_-60dB@4710Hz_RIPPLE_0dB_PRE44_POST467.h"
#endif

#if !defined(DYB17B_SECT_SECT) && !defined(DYB17B_SECT_TAPS)
	#define  DYB17B_SECT      30
    section  ("seg_dmda2")    float     DYB17B_state[DYB17B_SECT*2+ 1];
    section  ("seg_dmda2")    float     DXB17B_state[DYB17B_SECT*2+ 1];
	#include "DYB_UD_B_TUN_B60__x16_2756Hz_-3dB@2500_-60dB@4360Hz_RIPPLE_0.1dB_PRE89_POST1008.h"
#endif

#if !defined(R0B44A_SECT) && !defined(R0B44A_TAPS)
    #define  R0B44A_SECT      1
    section  ("seg_dmda2")    float     R0B44A_state[R0B44A_SECT*2+ 1];
  #include "R0B44A_TUN_Test1__B2_11025Hz_0dB@0_0dB@0Hz_RIPPLE_7.2dB_PRE0_POST39379.h"
#endif

#if !defined(OVB19B_SECT) && !defined(OVB19B_TAPS)
    #define  OVB19B_SECT      3
    section  ("seg_dmda2")    float     OVB19B_state[OVB19B_SECT*2+ 1];
    section  ("seg_dmda2")    float     OXB19B_state[OVB19B_SECT*2+ 1];
  #include "OVB19B_TUN_B5_BUT5_800Hz_-6dB@920_-30dB@1630Hz_RIPPLE_0dB_PRE140_POST1075.h"
#endif

//-------------------------------------------------------------------------
//===  Stages   M I D   ===  

#if !defined(OVM96D_SECT) && !defined(OVM96D_TAPS)
    #define  OVM96D_SECT      1
    section  ("seg_dmda2")    float     OVM96D_state[OVM96D_SECT*2+ 1];
    section  ("seg_dmda2")    float     OXM96D_state[OVM96D_SECT*2+ 1];
  #include "OVM96D_TUN_B0_x1_0Hz_-20dB@96000_-50dB@96000Hz_RIPPLE_120dB_PRE0_POST1.h"
#endif

// Filtry OVS B192

/*	
#if !defined(MYM48B_SECT) && !defined(MYM48B_TAPS)
    #define  MYM48B_SECT      8
    section  ("seg_dmda2")    float     MYM48B_state[MYM48B_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXM48B_state[MYM48B_SECT*2+ 1];
  #include "MYM48B_TUN_x2_B16_22050Hz_-3dB@5470_-60dB@9370Hz_RIPPLE_45.5dB_PRE11_POST111.h"
#endif

#if !defined(MYM88B_SECT) && !defined(MYM88B_TAPS)
    #define  MYM88B_SECT      12
    section  ("seg_dmda2")    float     MYM88B_state[MYM88B_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXM88B_state[MYM88B_SECT*2+ 1];    
  #include "MYM88B_TUN_x4_B24_11025Hz_-3dB@5040_-60dB@8610Hz_RIPPLE_0.8dB_PRE22_POST159.h"
#endif

#if !defined(MYM96B_SECT) && !defined(MYM96B_TAPS)
    #define  MYM96B_SECT      17
    section  ("seg_dmda2")    float     MYM96B_state[MYM96B_SECT*2+ 1];
	section  ("seg_dmda2")    float     MXM96B_state[MYM96B_SECT*2+ 1];    
  #include "MYM96B_TUN_B34__x8_5513Hz_-3dB@2740_-60dB@4700Hz_RIPPLE_0dB_PRE44_POST443.h"
#endif
*/

// Filtry OVS C96
#if !defined(MYT44B_SECT) && !defined(MYT44B_TAPS)
    #define  MYT44B_SECT      4
    section  ("seg_dmda2")    float     MYT44B_state[MYT44B_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXT44B_state[MYT44B_SECT*2+ 1];    	
	#include "MYT44B_TUN_x1_B8_192000Hz_-3dB@10050_-60dB@17430Hz_RIPPLE_83.8dB_PRE6_POST28.h"
#endif

#if !defined(MYM48C_SECT) && !defined(MYM48C_TAPS)
    #define  MYM48C_SECT      6
    section  ("seg_dmda2")    float     MYM48C_state[MYM48C_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXM48C_state[MYM48C_SECT*2+ 1];    
  #include "MYM48C_TUN_96_x2_B12_24000Hz_-3dB@7830_-60dB@10760Hz_RIPPLE_27.3dB_PRE11_POST110.h"
#endif

#if !defined(MYM88C_SECT) && !defined(MYM88C_TAPS)
    #define  MYM88C_SECT      10
    section  ("seg_dmda2")    float     MYM88C_state[MYM88C_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXM88C_state[MYM88C_SECT*2+ 1];    
  #include "MYM88C_TUN_96_x4_B20_12000Hz_-3dB@7210_-60dB@10430Hz_RIPPLE_0.1dB_PRE23_POST249.h"
#endif

#if !defined(MYM96C_SECT) && !defined(MYM96C_TAPS)
    #define  MYM96C_SECT      16
    section  ("seg_dmda2")    float     MYM96C_state[MYM96C_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXM96C_state[MYM96C_SECT*2+ 1];    
  #include "MYM96C_TUN_B32__96_x8_22050Hz_-3dB@3900_-60dB@6430Hz_RIPPLE_0.2dB_PRE46_POST778.h"
#endif

//-------------------------------------------------------------------------
//===  Stages   T W T   ===  
// *******  Korekcje amplitudowe  *************

// Zeros MYT48 
#if !defined(MCT44D_SECT) && !defined(MCT44D_TAPS)
    #define  MCT44D_SECT      2
    section  ("seg_dmda2")    float     MCT44D_state[MCT44D_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT44D_state[MCT44D_SECT*2+ 1];
  #include "MCT44D_TUN_MYT48B_OVS_FR__B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_0.2dB_PRE0_POST37.h"
#endif

// STEP 
#if !defined(MCT48A_SECT) && !defined(MCT48A_TAPS)
    #define  MCT48A_SECT      2
    section  ("seg_dmda2")    float     MCT48A_state[MCT48A_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT48A_state[MCT48A_SECT*2+ 1];
  #include "MCT48A_TUN_NOS_FR__B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_0.5dB_PRE0_POST36.h"
#endif
// STEP + MYT48
#if !defined(MCT48C_SECT) && !defined(MCT48C_TAPS)
    #define  MCT48C_SECT      2
    section  ("seg_dmda2")    float     MCT48C_state[MCT48C_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT48C_state[MCT48C_SECT*2+ 1];    
  #include "MCT48C_TUN_ST19k0dB_B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_0.4dB_PRE0_POST41.h"
#endif
#if !defined(MCT48D_SECT) && !defined(MCT48D_TAPS)
    #define  MCT48D_SECT      2
    section  ("seg_dmda2")    float     MCT48D_state[MCT48D_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT48D_state[MCT48D_SECT*2+ 1];    
  #include "MCT48D_TUN_ST17k0dB_B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_0.4dB_PRE0_POST24.h"
#endif

// LINEAR 
#if !defined(MCT88A_SECT) && !defined(MCT88A_TAPS)
    #define  MCT88A_SECT      3
    section  ("seg_dmda2")    float     MCT88A_state[MCT88A_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT88A_state[MCT88A_SECT*2+ 1];
  #include "MCT88A_TUN_LIN4X_OVS_FR__B6_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_1.1dB_PRE0_POST37.h"
#endif
// LINEAR + MYT48
#if !defined(MCT88C_SECT) && !defined(MCT88C_TAPS)
    #define  MCT88C_SECT      2
    section  ("seg_dmda2")    float     MCT88C_state[MCT88C_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT88C_state[MCT88C_SECT*2+ 1];
  #include "MCT88C_TUN_LIN_MYT_OVS_FR__B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_0.9dB_PRE0_POST33.h"
#endif

// LAGRANGE 
#if !defined(MCT44G1_SECT) && !defined(MCT44G1_TAPS)
    #define  MCT44G1_SECT      3
    section  ("seg_dmda2")    float     MCT44G1_state[MCT44G1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT44G1_state[MCT44G1_SECT*2+ 1];
  #include "MCT44G1_TUN_LG19kNq0dB_B6_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_0.3dB_PRE0_POST61.h"
#endif
#if !defined(MCT44G2_SECT) && !defined(MCT44G2_TAPS)
    #define  MCT44G2_SECT      3
    section  ("seg_dmda2")    float     MCT44G2_state[MCT44G2_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT44G2_state[MCT44G2_SECT*2+ 1];
  #include "MCT44G2_TUN_LG16kNq1dB_B6_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_0.3dB_PRE0_POST43.h"
#endif
// LAGRANGE + MYT48



// BSPLINE xFIR MC-HIGH 19k i LOW 16k
#if !defined(MCT17A_SECT) && !defined(MCT17A_TAPS)
    #define  MCT17A_SECT      2
    section  ("seg_dmda2")    float     MCT17A_state[MCT17A_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT17A_state[MCT17A_SECT*2+ 1];    
  #include "MCT17A_TUN_BS19k1dB_B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_2.3dB_PRE0_POST50.h"
#endif

#if !defined(MCT17B_SECT) && !defined(MCT17B_TAPS)
    #define  MCT17B_SECT      2
    section  ("seg_dmda2")    float     MCT17B_state[MCT17B_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCT17B_state[MCT17B_SECT*2+ 1];    
  #include "MCT17B_TUN_BS16kNq2dB_B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_2.3dB_PRE0_POST63.h"
#endif
// BSPLINE + MYT48
#if !defined(MCT17C_SECT) && !defined(MCT17C_TAPS)
    #define  MCT17C_SECT      2
    section  ("seg_dmda2")    float     MCT17C_state[MCT17C_SECT*2+ 1];
	section  ("seg_dmda2")    float     NCT17C_state[MCT17C_SECT*2+ 1];
  #include "MCT17C_TUN_BF16kNq2dB_B4_22000Hz_-20dB@22050_-50dB@22050Hz_RIPPLE_2.1dB_PRE0_POST38.h"
#endif

//  OVERSAMPLING
#if !defined(OVT19B_SECT) && !defined(OVT19B_TAPS)
    #define  OVT19B_SECT      64
    section  ("seg_dmda2")    float     OVT19B_state[OVT19B_SECT*2+ 1];
    section  ("seg_dmda2")    float     OXT19B_state[OVT19B_SECT*2+ 1];
  #include "OVT19B_TUN_B127_BUT5_800Hz_-6dB@20_-30dB@20Hz_RIPPLE_83.7dB_PRE123_POST1090.h"
#endif

#if !defined(VVT19A_SECT) && !defined(VVT19A_TAPS)
    #define  VVT19A_SECT      1
    section  ("seg_dmda2")    float     VVT19A_state[VVT19A_SECT*2+ 1];
    section  ("seg_dmda2")    float     VXT19A_state[VVT19A_SECT*2+ 1];
  #include "VVT19A_TUN_B0_x1_0Hz_-20dB@96000_-50dB@96000Hz_RIPPLE_120dB_PRE0_POST1.h"
#endif
//  FIR CD MYT48B
#if !defined(MYT48B_SECT) && !defined(MYT48B_TAPS)
    #define  MYT48B_SECT      8
    section  ("seg_dmda2")    float     MYT48B_state[MYT48B_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXT48B_state[MYT48B_SECT*2+ 1];    
  #include "MYT48B_TUN_x2_B16_22050Hz_-3dB@5470_-60dB@9370Hz_RIPPLE_45.5dB_PRE11_POST111.h"
#endif
//  FIR HD MYT48C
#if !defined(MYT48C_SECT) && !defined(MYT48C_TAPS)
    #define  MYT48C_SECT      6
    section  ("seg_dmda2")    float     MYT48C_state[MYT48C_SECT*2+ 1];
    section  ("seg_dmda2")    float     MXT48C_state[MYT48C_SECT*2+ 1];    
  #include "MYT48C_TUN_96_x2_B12_24000Hz_-3dB@7830_-60dB@10760Hz_RIPPLE_27.3dB_PRE11_POST110.h"
#endif


#if !defined(DVT44A_SECT) && !defined(DVT44A_TAPS)
    #define  DVT44A_TAPS      2
    section  ("seg_dmda2")    float     DVT44A_state[DVT44A_TAPS+ 1];
  #include "DVT44A_FIR_B1_0Hz_0dB@10_0dB@10Hz_RIPPLE_120dB_PRE1_POST1.h"
#endif


#if !defined(MCDAC_44_b1_SECT) && !defined(MCDAC_44_b1_TAPS)
    #define  MCDAC_44_b1_SECT      8
    section  ("seg_dmda2")    float     MCDAC_44_b1_state[MCDAC_44_b1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_b1_state[MCDAC_44_b1_SECT*2+ 1];
  #include "MCDAC_44_b1_18500_1dB_19500_4dB_B17.h"
#endif

#if !defined(MCDAC_44_b2_SECT) && !defined(MCDAC_44_b2_TAPS)
    #define  MCDAC_44_b2_SECT      10
    section  ("seg_dmda2")    float     MCDAC_44_b2_state[MCDAC_44_b2_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_b2_state[MCDAC_44_b2_SECT*2+ 1];
  #include "MCDAC_44_b2_17000_1dB_18000_7dB_B20.h"
#endif

#if !defined(MCDAC_44_B1_SECT) && !defined(MCDAC_44_B1_TAPS)
    #define  MCDAC_44_B1_SECT      11
    section  ("seg_dmda2")    float     MCDAC_44_B1_state[MCDAC_44_B1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_B1_state[MCDAC_44_B1_SECT*2+ 1];
  #include "MCDAC_44_B1_18500_1dB_19500_4dB_B22.h"
#endif


#if !defined(MCDAC_44_B2_SECT) && !defined(MCDAC_44_B2_TAPS)
    #define  MCDAC_44_B2_SECT      11
    section  ("seg_dmda2")    float     MCDAC_44_B2_state[MCDAC_44_B2_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_B2_state[MCDAC_44_B2_SECT*2+ 1];
  #include "MCDAC_44_B2_17000_1dB_18000_7dB_B22.h"
#endif


#if !defined(MCDAC_44_L1_SECT) && !defined(MCDAC_44_L1_TAPS)
    #define  MCDAC_44_L1_SECT      9
    section  ("seg_dmda2")    float     MCDAC_44_L1_state[MCDAC_44_L1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_L1_state[MCDAC_44_L1_SECT*2+ 1];
  #include "MCDAC_44_L1_18500_1dB_19500_3dB_B18.h"
#endif

#if !defined(MCDAC_44_l1_SECT) && !defined(MCDAC_44_l1_TAPS)
    #define  MCDAC_44_l1_SECT      7
    section  ("seg_dmda2")    float     MCDAC_44_l1_state[MCDAC_44_l1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_l1_state[MCDAC_44_l1_SECT*2+ 1];
  #include "MCDAC_44_l1_18500_1dB_19500_3dB_B15.h"
#endif

#if !defined(MCDAC_44_g1_SECT) && !defined(MCDAC_44_g1_TAPS)
    #define  MCDAC_44_g1_SECT      7
    section  ("seg_dmda2")    float     MCDAC_44_g1_state[MCDAC_44_g1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_g1_state[MCDAC_44_g1_SECT*2+ 1];
  #include "MCDAC_44_g1_18500_1dB_19500_3dB_B15.h"
#endif

#if !defined(MCDAC_44_G1_SECT) && !defined(MCDAC_44_G1_TAPS)
    #define  MCDAC_44_G1_SECT      10
    section  ("seg_dmda2")    float     MCDAC_44_G1_state[MCDAC_44_G1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_G1_state[MCDAC_44_G1_SECT*2+ 1];
  #include "MCDAC_44_G1_18500_1dB_19500_3dB_B20.h"
#endif

#if !defined(MCDAC_44_F1_SECT) && !defined(MCDAC_44_F1_TAPS)
    #define  MCDAC_44_F1_SECT      9
    section  ("seg_dmda2")    float     MCDAC_44_F1_state[MCDAC_44_F1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_44_F1_state[MCDAC_44_F1_SECT*2+ 1];
  #include "MCDAC_44_F1_18500_1dB_19500_3dB_B19.h"
#endif

#if !defined(MCDAC_48_g1_SECT) && !defined(MCDAC_48_g1_TAPS)
    #define  MCDAC_48_g1_SECT      6
    section  ("seg_dmda2")    float     MCDAC_48_g1_state[MCDAC_48_g1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_48_g1_state[MCDAC_48_g1_SECT*2+ 1];
  #include "MCDAC_48_g1_19500_1dB_21000_1dB_B13.h"
#endif

#if !defined(MCDAC_48_G1_SECT) && !defined(MCDAC_48_G1_TAPS)
    #define  MCDAC_48_G1_SECT      10
    section  ("seg_dmda2")    float     MCDAC_48_G1_state[MCDAC_48_G1_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCDAC_48_G1_state[MCDAC_48_G1_SECT*2+ 1];
  #include "MCDAC_48_G1_19500_1dB_21000_1dB_B20.h"
#endif

#if !defined(MCBASx44_SECT) && !defined(MCBASx44_TAPS)
    #define  MCBASx44_SECT      23
    section  ("seg_dmda2")    float     MCBASx44_state[MCBASx44_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCBASx44_state[MCBASx44_SECT*2+ 1];
  #include "MCBASx44_MM_PW1_L30Hz_CUT800Hz_PR400_B47.h"
#endif

#if !defined(MCPRE44_SECT) && !defined(MCPRE44_TAPS)
    #define  MCPRE44_SECT      40
    section  ("seg_dmda2")    float     MCPRE44_state[MCPRE44_SECT*2+ 1];
    section  ("seg_dmda2")    float     NCPRE44_state[MCPRE44_SECT*2+ 1];
  #include "MCPRE44_MM_PW1_CUT830Hz_PR400_B81.h"
#endif

#if !defined(MCBx44Q3_SECT) && !defined(MCBx44Q3_TAPS)
    #define  MCBx44Q3_SECT      30
    section  ("seg_dmda2")    float dm MCBx44Q3_L[MCBx44Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCBx44Q3_R[MCBx44Q3_SECT*2+ 1];
  #include    "MCBx44Q3_KEFQ350_B61_len568.h"
#endif


#if !defined(MCBx48Q3_SECT) && !defined(MCBx48Q3_TAPS)
    #define  MCBx48Q3_SECT      30
    section  ("seg_dmda2")    float dm MCBx48Q3_L[MCBx48Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCBx48Q3_R[MCBx48Q3_SECT*2+ 1];
  #include    "MCBx48Q3_KEFQ350_B61_len725.h"
#endif


#if !defined(MCP48Q3_SECT) && !defined(MCP48Q3_TAPS)
    #define  MCP48Q3_SECT      32
    section  ("seg_dmda2")    float dm MCP48Q3_L[MCP48Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP48Q3_R[MCP48Q3_SECT*2+ 1];
  #include    "MCP48Q3_KEFQ350_B65_len3936.h"
#endif


#if !defined(MCP44Q3_SECT) && !defined(MCP44Q3_TAPS)
    #define  MCP44Q3_SECT      37
    section  ("seg_dmda2")    float dm MCP44Q3_L[MCP44Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP44Q3_R[MCP44Q3_SECT*2+ 1];
  #include    "MCP44Q3_KEFQ350_B75_len2091.h"
#endif

#if !defined(MCP88Q3_SECT) && !defined(MCP88Q3_TAPS)
    #define  MCP88Q3_SECT      37
    section  ("seg_dmda2")    float dm MCP88Q3_L[MCP88Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP88Q3_R[MCP88Q3_SECT*2+ 1];
  #include    "MCP88Q3_KEFQ350_B75_len1356.h"
#endif

#if !defined(MCP96Q3_SECT) && !defined(MCP96Q3_TAPS)
    #define  MCP96Q3_SECT      40
    section  ("seg_dmda2")    float dm MCP96Q3_L[MCP96Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP96Q3_R[MCP96Q3_SECT*2+ 1];
  #include    "MCP96Q3_KEFQ350_B81_len1372.h"
#endif

#if !defined(MCP17Q3_SECT) && !defined(MCP17Q3_TAPS)
    #define  MCP17Q3_SECT      27
    section  ("seg_dmda2")    float dm MCP17Q3_L[MCP17Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP17Q3_R[MCP17Q3_SECT*2+ 1];
  #include    "MCP17Q3_KEFQ350_B55_len3601.h"
#endif

#if !defined(MCP19Q3_SECT) && !defined(MCP19Q3_TAPS)
    #define  MCP19Q3_SECT      57
    section  ("seg_dmda2")    float dm MCP19Q3_L[MCP19Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP19Q3_R[MCP19Q3_SECT*2+ 1];
  #include    "MCP19Q3_KEFQ350_B115_len4397.h"
#endif

/*
#if !defined(MCP17Q3_SECT) && !defined(MCP17Q3_TAPS)
    #define  MCP17Q3_SECT      35
    section  ("seg_dmda2")    float dm MCP17Q3_L[MCP17Q3_SECT*2+ 1];
    section  ("seg_dmda2")    float dm MCP17Q3_R[MCP17Q3_SECT*2+ 1];
  #include    "MCP17Q3_KEFQ350_B71_len2981.h"
#endif
*/