//================================================================================
//===========  		S I M   C O N F I G   	======================================
//================================================================================
	

#define		HP_DIRAC 				1
#define		SPK_IR					2 


#define 	xCH_MIX					(LEFT+RIGHT) //  LEFT  //  RIGHT //   CHN_WORKMODE // 
//#define 	xDSP_SPK_MIX			BAS_B // BAS_B // BAS  //   BAS //     BAS //  PRE  //      PRE  //  -1 //    BAS_B + MID_B + TWT_B
//#define 	xEXT_SPK_MIX			MID_B  // 0 // -1 //  //    BAS_B + MID_B + TWT_B
//#define 	xINBUF_MIX				0 //  XIN_B // //XIN_B // XIN_B // XIN_B + PRE_B + EXP_B

#if	(SOFT_V == SH_DSP)
	#define		xSIM_TAB			SPK_IR	// 	HP_DIRAC // 
	#define		DUMP_MIX			(DM_MASK)  // | RM_MASK  )  // RT_MASK  // DB_MASK //// (DM_MASK) //  (ET_MASK ) //RT_MASK  //  (RT_MASK) // | RB_MASK)  // EB_MASK  // (X_MASK)  // RT_MASK | RB_MASK |  DM_MASK)  // (RB_MASK | ET_MASK | RT_MASK | DM_MASK ) //  | ET_MASK) // RB_MASK  (DM_MASK ) // (RB_MASK  | RT_MASK | DM_MASK) // DT_MASK  //   (RB_MASK  | RT_MASK) //   //| XP_MASK //// DB_MASK // RB_MASK // DB_MASK // YM_MASK)  // 
#else
	#define		xSIM_TAB			SPK_IR	// HP_DIRAC // 	
	#define		DUMP_MIX			RM_MASK // RT_MASK  // (DM_MASK)  //  (RT_MASK | RB_MASK)  //(DM_MASK) //  // EB_MASK  // (X_MASK)  // RT_MASK | RB_MASK |  DM_MASK)  // (RB_MASK | ET_MASK | RT_MASK | DM_MASK ) //  | ET_MASK) // RB_MASK  (DM_MASK ) // (RB_MASK  | RT_MASK | DM_MASK) // DT_MASK  //    
#endif	
//#endif

#define 	xINB_REC_LEN			30000
#define 	xDSP_REC_LEN			25000  // 2.8E+6 	//7E+4
#define 	xRSMP_REC_LEN			30000

#define 	xEXT_REC_LEN			xDSP_REC_LEN
