////////////////////////////////////////////////////////////////////////////////////////
//NAME:     	Buffer.cpp
//CREATE DATE:  17/10/2021
//PURPOSE:  	Flexible buffer class implementation
////////////////////////////////////////////////////////////////////////////////////////
#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<math.h>
#include 	"tt.h"
//#include    "_Sharc_EXPH_DEF.h"
#include  	"SIM.h"
#include  	"hBUF_SRC.hpp"
#include  	"hSR_PROC.hpp"
#include  	"hSR_PROC.hpp"
#include  	"hDSP_MANAGER.hpp"
#include  	"hFILTER.hpp"
#include  	"hRESAMPLER.hpp"
#include  	"hPlaySession.hpp"
#include  	"hDSPSTRING.hpp"


#define		BUFFER_DELAY_SPACE		300

#define		FLOW_BAS					0
#define		FLOW_MID					1
#define		FLOW_TWT					2
#define		FLOW_XP_EP_RPI				3


EXTERNC  void 			BUF_SRC_INIT(void);
extern BUF_SRC 			X_SRC, PIX1, PIX2;

extern 	FILTER  		FILT[FILTOBJ_QTY];
extern 	RESAMPLER  		RSMP[RSMPOBJ_QTY];
	
//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================
#include	"hBUF_MANAGER.hpp"
	
BUF_MANAGER 	*BUFSIM_List[10];

#include  	"hSIM_SDRAM.hpp"

//#if (SOFT_V == FP_TAB)
 #if  (SOFT_V == SH_XOEP_PROD)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
 	// int		tri[]= {-8000000, -500000, -32000, -10000, -5000, -1000, -400, -100, -10, -1, 1, 10,  100, 400, 1000, 5000, 10000, 32000, 500000, 8000000 };  
 	int		tri[]= {-1700000005,-60000003, -8000008, -500001, -32003, -10004, -4998, -1004, -403, -103, -7, -4, 1, 10,  100, 400, 1000, 5000, 10000, 32000, 500000, 8000000, 60000000,  1700000000};

 	void 	test_fill(float *buf, int bufsize, int kb)
 	{
 		int k;
 		
 		while ( kb< bufsize )
 		{
 			for(k=0;k< sizeof(tri) && kb< bufsize; k++)
 				buf[kb++]= tri[k];
 		} 		
 	} 	
#elif (SOFT_V == FP_DSP  || (SOFT_V & FP_TEST2RX) == FP_TEST2RX )   

	#include 	"_FP_TWT2RX_TABLIST.h"
 #define		HP_FPGA_SIZE	2000		

 #define		FPTEST_HPDIRAC			1
 #define		FPTEST_SINUS			2
 #define		FPTEST_DATATYPE			FPTEST_HPDIRAC
 
 extern int HP_DIRAC_TAB24_40k[]; 
 extern int  SPKIR_44[];

 	#if (FPTEST_DATATYPE == FPTEST_HPDIRAC)  
 		void 	test_fill(float *buf, int *SOURCE, int SOURCE_SIZE,  int bufsize, int scaler)
 	#else
 	 	void 	test_fill(float *buf, float *SOURCE, int SOURCE_SIZE,  int bufsize, int scaler) 	
 	#endif 	
 		{
 			int k, kb=0;
 		
 			while ( kb< bufsize )
 			{
  				for(k=0; k< SOURCE_SIZE && kb< bufsize; k++)			
 					buf[kb++]= (float) SOURCE[k]/(float) scaler; 			 				
 			} 		
 		}  	
 	 	
#endif

//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================
#define				X_SIZE			100000
#define				RY_SIZE			40000
#define				TEMP_SIZE		30000

 #if  (	(WORK_CONFIG & RUNMODE_MASK) == PROD_MODE )    	 
//	SECTION(("seg_sdram4"))	int 	 tXL[100000], 			 					tXR[100000]; 	
	SECTION(("seg_sdram4"))	float 	tXL[X_SIZE], 			 					tXR[X_SIZE]; 
	SECTION(("seg_sdram4"))	float 	tXPL[TEMP_SIZE], 							tXPR[TEMP_SIZE];
	SECTION(("seg_sdram4"))	float 	tETL[RY_SIZE], 								tETR[RY_SIZE]; 											
	SECTION(("seg_sdram4"))	float 	tDBL[TEMP_SIZE*2], 							tDBR[TEMP_SIZE*2];
	SECTION(("seg_sdram4"))	float 	tDML[RY_SIZE], 								tDMR[RY_SIZE]; 
	SECTION(("seg_sdram4"))	float 	tDTL[TEMP_SIZE], 							tDTR[TEMP_SIZE]; 									
//	SECTION(("seg_sdram4"))	float 	tYML[TEMP_SIZE], 							tYMR[TEMP_SIZE]; 
//	SECTION(("seg_sdram4"))	int 	tRPL[96000], 								tRPR[96000];  		
//	SECTION(("seg_sdram4"))	float 	tRPL[30000], 								tRPR[30000];  		
	SECTION(("seg_sdram4"))	float 	tRBL[RY_SIZE], 								tRBR[RY_SIZE]; 
	//SECTION(("seg_sdram4"))	float 	tRML[RY_SIZE*2], 							tRMR[RY_SIZE*2]; 
//	SECTION(("seg_sdram4"))	float 	tRML[RY_SIZE], 								tRMR[RY_SIZE]; 	
	SECTION(("seg_sdram4"))	float 	tRTL[RY_SIZE], 								tRTR[RY_SIZE]; 
	SECTION(("seg_sdram4"))	float 	tEBL[RY_SIZE], 								tEBR[RY_SIZE]; 											
 #elif (	(WORK_CONFIG & RUNMODE_MASK) == TEST_MODE )
//	SECTION(("seg_sdram4"))	int 	 tXL[xINB_REC_LEN], 			 			tXR[xINB_REC_LEN]; 	
	SECTION(("seg_sdram4"))	float 	 tXL[100000], 			 					tXR[100000]; 			
	SECTION(("seg_sdram4"))	float 	tXPL[xINB_REC_LEN], 						tXPR[xINB_REC_LEN];
	SECTION(("seg_sdram4"))	float 	tDBL[xDSP_REC_LEN], 						tDBR[xDSP_REC_LEN]; 
	SECTION(("seg_sdram4"))	float	tDML[xDSP_REC_LEN*2], 						tDMR[xDSP_REC_LEN*2]; 
	SECTION(("seg_sdram4"))	float	tDTL[xDSP_REC_LEN], 						tDTR[xDSP_REC_LEN]; 
	SECTION(("seg_sdram4"))	float 	tYML[xEXT_REC_LEN], 						tYMR[xEXT_REC_LEN];  
//	SECTION(("seg_sdram4"))	int 	tRPL[xEXT_REC_LEN], 						tRPR[xEXT_REC_LEN];  	
//	SECTION(("seg_sdram4"))	float  	tRPL[xEXT_REC_LEN], 						tRPR[xEXT_REC_LEN];  	
	SECTION(("seg_sdram4"))	float 	tRBL[xRSMP_REC_LEN], 						tRBR[xRSMP_REC_LEN]; 
	SECTION(("seg_sdram4"))	float 	tRML[xRSMP_REC_LEN], 						tRMR[xRSMP_REC_LEN]; 
	SECTION(("seg_sdram4"))	float 	tRTL[xRSMP_REC_LEN], 						tRTR[xRSMP_REC_LEN]; 	
	SECTION(("seg_sdram4"))	float 	tETL[xRSMP_REC_LEN], 						tETR[xRSMP_REC_LEN]; 		
	SECTION(("seg_sdram4"))	float 	tEBL[xRSMP_REC_LEN], 						tEBR[xRSMP_REC_LEN]; 			
 #endif   	 
	

	BUF_MANAGER			XP[2]= { BUF_MANAGER((float *) tXPL, sizeof(tXPL)/sizeof(DSPdata),  0, 				  0), BUF_MANAGER((float *) tXPR, sizeof(tXPR)/sizeof(DSPdata), 0, 0)};  
	BUF_MANAGER			DB[2]= { BUF_MANAGER((float *) tDBL, sizeof(tDBL)/sizeof(DSPdata),  BUFFER_DELAY_SPACE, 0), BUF_MANAGER((float *) tDBR, sizeof(tDBR)/sizeof(DSPdata), BUFFER_DELAY_SPACE, 0) }; 
	BUF_MANAGER			DM[2]= { BUF_MANAGER((float *) tDML, sizeof(tDML)/sizeof(DSPdata),  BUFFER_DELAY_SPACE, 0), BUF_MANAGER((float *) tDMR, sizeof(tDMR)/sizeof(DSPdata), BUFFER_DELAY_SPACE, 0) };
	BUF_MANAGER			DT[2]= { BUF_MANAGER((float *) tDTL, sizeof(tDTL)/sizeof(DSPdata),  BUFFER_DELAY_SPACE, 0), BUF_MANAGER((float *) tDTR, sizeof(tDTR)/sizeof(DSPdata), BUFFER_DELAY_SPACE, 0) };
	
	BUF_MANAGER 		X[2], 
						YM[2],
						RP[2],
						RB[2], RM[2], RT[2], ET[2], EB[2]; 		
						
	SECTION(("seg_sdram2"))	 BUF_MANAGER *TBUFS[] = {X, XP, DB, DM, DT, RB, RT, EB, ET }; 		
	//SECTION(("seg_sdram2"))	 char BUFNAMES_L[9][4] = {"XL", "XPL", "DBL", "DML", "DTL", "RBL", "RTL", "EBL", "ETL" }; 		
	SECTION(("seg_sdram2"))	 char BUFNAMES_L[9][3] = {"X", "XP", "DB", "DM", "DT", "RB", "RT", "EB", "ET" }; 		
	
	//SECTION(("seg_sdram2"))	 char BUFNAMES_R[9][4] = {"XR", "XPR", "DBR", "DMR", "DTR", "RBR", "RTR", "EBR", "ETR" }; 		
	
	//SECTION(("seg_sdram2"))	char (*BUFNAMES[])[4]={BUFNAMES_L, BUFNAMES_R};				
	
							
	void	PRINT_kBUFS(void)
	{
		int				k, kf ;
		float			ktot, kftot;
		
		for(k=0; k< sizeof(TBUFS); k++)
		{
			ktot = TBUFS[k][LEFT_0].k_INBUFS* TBUFS[k][LEFT_0].BUF_SIZE+ TBUFS[k][LEFT_0].k_IN ;  
			if (TBUFS[k][LEFT_0].k_INBUFS> 0)
			{
				sprintf(dbgtab,"%s:%.1f", BUFNAMES_L[k], ktot/1000);
				//trace(dbgtab, 0);
			}
			for (kf=0; kf<FLOW_QTY; kf++)
			{
				kftot = TBUFS[k][LEFT_0].Flow[LEFT_0][kf][K_GETBUF_FLOW]* TBUFS[k][LEFT_0].BUF_SIZE+ TBUFS[k][LEFT_0].Flow[LEFT_0][kf][K_GET_FLOW]; 
				if (TBUFS[k][LEFT_0].Flow[LEFT_0][kf][K_GETBUF_FLOW]> 0)
				{
					sprintf(dbgtab,"%s%c:%.1f", BUFNAMES_L[k], kf+48, kftot/1000);
					//trace(dbgtab, 0);
				}				
			}
		}			
	}
	
	
BUF_MANAGER::BUF_MANAGER(void)
{
	BUF_SIZE=   DSPBLOCK_SIZE2;
}

BUF_MANAGER::BUF_MANAGER(float *pBUF, int pBUF_SIZE, int START_PUT, int START_GET)
{
	Buf 		= pBUF; 
	BUF_SIZE	= pBUF_SIZE; 
	k_IN 		= START_PUT; 
	k_OUT 		= START_GET; 
}		

int ksim_list;


void BUF_MANAGER::Init(float *pfBUF, int *piBUF, long long pBUF_SIZE, int START_PUT, int START_GET, int pSIM_MASK, FILTER *Delay_FILTER)
{ 
	int	k;

	BUF_SIZE=  pBUF_SIZE;
	if (pfBUF != NULL)
	{	BufType	=1;
		Buf 	= pfBUF; 
		for(k=0; k<BUF_SIZE; k++)
			Buf[k] =0;
	}
	else
	{	BufType	=2;		
		Bufi 	= piBUF; 
		for(k=0; k<BUF_SIZE; k++)
			Bufi[k] =0;
	}		
	
	 
	diff_INOUT = 0;
	
	k_INBUFS 		= 0; 
	k_OUTBUFS	 	= 0;
  
  //= For X[] buffers the algo inside BUF_SRC Init depndent on SR overwrites the starting position
	if (PlaySession::RECORDING_MODE != AUDIO_NOREC) //== SIM_I2SOFF_D_ONLY)		
		k_IN = 	START_PUT; 
	else 
		k_IN =  START_PUT; //OUTBUF_DIST*3;	 		
		
	k_OUT 		= START_GET; 
	GETPUT_DIST = START_PUT;
		
	for (k=0; k<FLOW_QTY; k++)
	{
		Flow[LEFT_0][k][K_GETBUF_FLOW]= k_OUTBUFS; 		Flow[RIGHT_0][k][K_GETBUF_FLOW]= 	k_OUTBUFS; 
		Flow[LEFT_0][k][K_GET_FLOW]= 	k_OUT; 			Flow[RIGHT_0][k][K_GET_FLOW]= 		k_OUT; 
	}
   
  	
	SIM_MASK = pSIM_MASK;
  //= If buffer used for stream delay handling then link the buffer to the FILT/RSMP block	
  //= Then in RESAMPLER::Rsmp_Size_Vol_Delay() this code adjusts buffer's k_IN:
  // 		if (RSMP[kSpk].DelayBuf != NULL) 
  //			RSMP[kSpk].DelayBuf[LEFT_0]->k_IN -= RSMP[kSpk].Delay;
	if (Delay_FILTER != NULL)
	{
		k= SIM_MASK&0x0F;
   		Delay_FILTER->DelayBuf[(SIM_MASK&0x0F)-1] = this;
	}
	
    	if( ((SIM_MASK& 0x0F) & xCH_MIX)  && ( (SIM_MASK& 0xFFFFFFF00) & DUMP_MIX) )    	
    	{
    		SIM_FLAG = FALSE; 
        	BUFSIM_List[ksim_list++] = this;
    	}	
    	else 
    		SIM_FLAG = NONREC;  	
}

int  BUF_MANAGER::Flow_select(int BUF_MASK_ID)
{
	int jF; 
	
	switch(BUF_MASK_ID & 0xFFFFFF00)
	{
		case  DB_MASK: case  YB_MASK:	case  RB_MASK: case  EB_MASK: //case   X_MASK: 
			jF = FLOW_BAS; 
		  break;		
		case  DM_MASK: 	case  YM_MASK: 	case  RM_MASK:
			jF = FLOW_MID; 
		  break;		
		case  DT_MASK:	case  YT_MASK:	case  RT_MASK: case  ET_MASK:
			jF = FLOW_TWT; 		
		  break;	
		case  XP_MASK: case  EP_MASK: 
			jF = FLOW_XP_EP_RPI; 				  
		  break;				
		default: 
			//printf("Bad mask %d %x ",BUF_MASK_ID, this);	
			printf("Bad mask %d %p ", BUF_MASK_ID, this);
	}
	return(jF);
}

extern	float 	fDUMP[2][FDUMP_SIZE];			

#if (xRUN_MODE == TEST_MODE)
 #if (SOFT_V == FP_DSP || (SOFT_V & FP_TEST2RX) == FP_TEST2RX )   

 int		BUF_MANAGER::SIM_process(int PUT_TYPE)
 {
 	int k;
 	
 	//DUMP_ENABLE= 1; 
  	if ( DUMP_ENABLE )  // || k_INBUFS> 0) // k_INBUFS> xSTART_INBUFS)  
  	//if ( k_INBUFS> xSTART_INBUFS)    	
	{  	
		if( SIM_FLAG == FALSE)
		{	
  			DUMP_ENABLE = 0 ; //&= ~( 1<<( (SIM_MASK&0xF)-1) );			
 			//SIM_FLAG = TRUE;
			for(k=0;k<FDUMP_SIZE; k++)
			{
				fDUMP[0][k]= X[0].Buf[k];
				fDUMP[1][k]= X[1].Buf[k];
			}
			//SIM_SDRAM::MDump(Buf, DUMP_FLOAT, BUF_SIZE, SIM_MASK);
			SIM_SDRAM::MDump(fDUMP[0], DUMP_FLOAT, FDUMP_SIZE, 	X_MASK | LEFT);
			for(k=0;k<5E+6;k++) ; 

			SIM_SDRAM::MDump(fDUMP[1], DUMP_FLOAT, FDUMP_SIZE,	X_MASK | RIGHT);

			//X[LEFT_0].Init (tXL, NULL,  sizeof(tXL)/sizeof(DSPdata),  	10000				,0, X_MASK  |LEFT,	0);	 
	 		//X[RIGHT_0].Init(tXR, NULL,  sizeof(tXR)/sizeof(DSPdata),  	10000				,0, X_MASK  |RIGHT, 0);	  		  	 		 	
			
			//for (k=0; k<1E+5; k++) ;
  			//exit(0);									
  			//if (k_INBUFS> 0) 
  			//	exit(0); 
  			k= 0;
		}
  	}
 }
 #else  // other Sharc modes and RPI_V

 int		BUF_MANAGER::SIM_process(int DATA_QTY)
 {
  	if (PlaySession::RECORDING_MODE > AUDIO_NOREC)   // SIM Mode
  	{
  		if ( (k_IN +DATA_QTY) > BUF_SIZE)  // If buffer full mark the sim data ready flag
		{  	
			if( SIM_FLAG == FALSE)
			{	
				SIM_FLAG = TRUE;  		
				SIM_SDRAM::MDump(Buf, DUMP_FLOAT, BUF_SIZE, SIM_MASK);
			}
		}
  	}
  	return(0);						
 }
 #endif
#endif
 
//template<typename TP>		
void 		BUF_MANAGER::PUTf(int DATA_QTY, float *Dataf, int DATA_STEP)
{
	int j, j1; 

#if (xRUN_MODE == TEST_MODE)		
  #if (SOFT_V == FP_DSP || (SOFT_V & FP_TEST2RX) == FP_TEST2RX)
	SIM_process(1);  
  #else	
	SIM_process(DATA_QTY);  
  #endif
#endif
  
	for(j=0, j1=0; j1< DATA_QTY && k_IN < BUF_SIZE;j1++, j+=DATA_STEP) 
	{ 
		Buf[k_IN++] = Dataf[j] ;
	} 
	if (j1< DATA_QTY)
	{
		k_IN = 0 ; 	++k_INBUFS;   						  			
		for(; j1< DATA_QTY ; j1++, j+=DATA_STEP)
		{
			Buf[k_IN++] = Dataf[j] ;
		}						  		
	}	  		
}


int			k_m=0, marking;

void 		BUF_MANAGER::PUTi(int DATA_QTY, int *Datai, int DATA_STEP)
{
	int j, j1; 

#if (xRUN_MODE == TEST_MODE)		
  #if (SOFT_V == FP_DSP || (SOFT_V & FP_TEST2RX) == FP_TEST2RX)
	SIM_process(2);  
  #else	
	SIM_process(DATA_QTY);  
  #endif
#endif		
	switch(BufType)
	{ 
	  case 1:
		for(j=0, j1=0; j1< DATA_QTY && k_IN < BUF_SIZE; j1++, j+=DATA_STEP)  
			Buf[k_IN++] = Datai[j] ;
		if (j1< DATA_QTY)
		{
			k_IN = 0 ; 	++k_INBUFS;   						  			
			for(; j1< DATA_QTY ; j1++, j+=DATA_STEP)
				Buf[k_IN++] = Datai[j] ;    		
		} 
		break;
	  case 2:
		for(j=0, j1=0; j1< DATA_QTY && k_IN < BUF_SIZE;j1++, j+=DATA_STEP, k_m++) 
		{   
			if (this == &X[0])
			{ 
				++marking;
				marking%=256; 
				Bufi[k_IN++] =  Datai[j] | marking;
			}
			else 
				Bufi[k_IN++] =  Datai[j] ;			
		}
		if (j1< DATA_QTY)
		{
			k_IN = 0 ; 	++k_INBUFS;   						  			
			for(; j1< DATA_QTY ; j1++, j+=DATA_STEP)
				Bufi[k_IN++] = Datai[j] ;    		
		}  
		break;
	}	  			  		
}

void 		BUF_MANAGER::PUTiX(int DATA_QTY, int *Datai, int DATA_STEP, int rep)
{
	int j, j1; 

#if (xRUN_MODE == TEST_MODE)			
	SIM_process(DATA_QTY); 
#endif	

	for(j=0, j1=0; j1< DATA_QTY && k_IN < BUF_SIZE;j1++, j+=DATA_STEP)  
	{
		Buf[k_IN++] = Datai[j] ;
		Buf[k_IN++] = Datai[j] ;
		if(rep>2)
		{	Buf[k_IN++] = Datai[j] ;
			Buf[k_IN++] = Datai[j] ;
		}		
	}
		
	if (j1< DATA_QTY)
	{
		k_IN = 0 ; 	++k_INBUFS;   						  			
		for(; j1< DATA_QTY ; j1++, j+=DATA_STEP)
		{
			Buf[k_IN++] = Datai[j] ;    	
			Buf[k_IN++] = Datai[j] ;    		
			if(rep>2)
			{	Buf[k_IN++] = Datai[j] ;
				Buf[k_IN++] = Datai[j] ;
			}					
		}				
	}	  		
}


//template<typename TG>		
int BUF_MANAGER::GETF(int DATA_QTY, float *Dataf, int pBUF_MASK_ID)
{
	int 	j; 
	int		jF; 
  	int		*ck_GET, *ck_GETBUFS, BUF_LEFT;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	jF = Flow_select(pBUF_MASK_ID);	
				
	ck_GETBUFS= (&Flow[0][jF][K_GETBUF_FLOW]);  ck_GET= (&Flow[0][jF][K_GET_FLOW]);
		
	if ( (*ck_GET+DATA_QTY) <BUF_SIZE )
	{			
		for(j=0; j< DATA_QTY ;j++)  
			Dataf[j] = Buf[(*ck_GET)++];
	}
	else 
	{
		BUF_LEFT = BUF_SIZE- *ck_GET;
		for(j=0; j< BUF_LEFT ;j++)  
			Dataf[j] = Buf[(*ck_GET)++];
		(*ck_GET) = 0 ; 	++(*ck_GETBUFS);   						  			
		for(; j< DATA_QTY ; j++)   
			Dataf[j] = Buf[(*ck_GET)++];    		
	}			
  	return(1);
}

// Data load from interleaved X buffer 
int BUF_MANAGER::GETF2(int DATA_QTY, float *Dataf, int pBUF_MASK_ID, int chn)
{
	int j, j1; 
	int 	jF; 
  	int			*ck_GET, BUF_LEFT;
		
	
  //= Check if there is data in X buf to Get	
	jF = Flow_select(pBUF_MASK_ID);  
 	//if (!DATA_AVAILABLE(jF, chn) )
	//	return (0); 
			
	ck_GET= (&Flow[chn][jF][K_GET_FLOW]);
	 		
	if ( (*ck_GET+DATA_QTY*2) <BUF_SIZE )
	{			
		for(j=0; j< DATA_QTY ;j++, (*ck_GET)+=2)  
			Dataf[j] = Buf[*ck_GET];
	}
	else 
	{
		BUF_LEFT = (BUF_SIZE- *ck_GET)/2; //- (2-chn))
		//for(j=0; j< BUF_LEFT ;j++, (*ck_GET)+=2)  
		for(j=0; (*ck_GET)< BUF_SIZE ;j++, (*ck_GET)+=2)
			Dataf[j] = Buf[*ck_GET];
		(*ck_GET) = chn ;  ++Flow[chn][jF][K_GETBUF_FLOW];    						  			
		for(; j< DATA_QTY ; j++, (*ck_GET)+=2)   
			Dataf[j] = Buf[*ck_GET];    		
	}			  		
	 return(1);
}

int BUF_MANAGER::GETF_EX(STAGE_PROC *SP, int stage_no, int STEP)
{
	int 	j, k, ktop, DATA_QTY; 
	int 	jF; 
	float	*Dataf, *RobDSPx ; 
	
  	int		*ck_GET, BUF_LEFT;

	RobDSPx= SP->stage_ptr[SP->stage_cnt]; 	
  #if   (EP_TAILMODE == EP_PRETAIL)  			
  	for(k=0; k<EP_PRESIZE; k++)	
		RobDSPx[k]= 0;	
	Dataf = RobDSPx+ EP_PRESIZE;	
  #elif (EP_TAILMODE == EP_XPRETAIL)
	Dataf = RobDSPx;	
  #endif
	jF = Flow_select(SP->BufD->SIM_MASK);  
  //= Initialize to the current Flow pointer for the processed SP->Chn channel
	ck_GET= (&Flow[SP->Chn][jF][K_GET_FLOW]);
	
   //= Set the virtual buffer start using the FTab param config
	k= (*ck_GET+ SP->FiltRsmp->stages[stage_no].EP_OFFSET)%BUF_SIZE;		
	DATA_QTY = SP->FiltRsmp->stages[stage_no].size;  			
   //= Load the actual audio block size plua EP POST size
	if (  k - STEP*DATA_QTY > 0 )  // Taking data will not cross the buf edge
	{	for(j=0; j< DATA_QTY ;j++, k-=STEP)  
			Dataf[j] = Buf[k];
	}
	else // Taking data cross the buf edge
	{
		for(j=0; k>= 0;j++, k-=STEP)
			Dataf[j] = Buf[k];
			
		if (STEP ==1)
			k= BUF_SIZE-1;
		else if (STEP ==2) 
			k = BUF_SIZE-(STEP-SP->Chn);      						  			
		for(; j< DATA_QTY ; j++, k-=STEP)   
			Dataf[j] = Buf[k];
	}			
   //= Update the actual physical buffer pointer with the actual output block size at the last, always present EP proc stage
	if (SP->FiltRsmp->stages[stage_no].param_code =='E')
	{
		*ck_GET+= STEP* SP->FiltRsmp->SAVE_SIZE; 	// LOAD_QTY; 
		if (*ck_GET >= BUF_SIZE)
		{	*ck_GET%= BUF_SIZE;
			++Flow[SP->Chn][jF][K_GETBUF_FLOW];		
		}
	}						
	return(1);
}


#pragma optimize_for_speed
extern  float 										*stage_ptr[2];

int BUF_MANAGER::GET2(int DATA_QTY, float *Dataf, float *Data2, BUF_MANAGER *IN2, int BUFD_MASK_ID)
{
	int j;
int 	buf1_dataOK, buf2_dataOK;
	
	buf1_dataOK = GETF(DATA_QTY, Dataf,  BUFD_MASK_ID); 
	buf2_dataOK = IN2->GETF(DATA_QTY, Data2, BUFD_MASK_ID); 
	
	if ( buf1_dataOK && buf2_dataOK )  //SIM_MASK);
	{
		for(j=0; j< DATA_QTY; j++)  
			Dataf[j] = Dataf[j]+Data2[j];
		return(1);
	} 
	else return(0);
}



#define			xB				2000
#define			xT				4*xB
#define			xM				5000

#define			xRBUFT			500   	// for EP_MID /EP_BT there has to be at least 140 samples dist otherwise the dump will have zero segments
#define			xRBUFP			6000

#define			xEBUFT			1000
#define			xEBUFP			16000

#define			xMGETP			20000
#define			xMGETT			0000


// BUF_MANAGER::BUF_MANAGER(float *pBUF, int pBUF_SIZE, int START_PUT, int START_GET)
void BUF_MANAGER::BUF_MANAGER_INIT(void)
{
	
			//Init(float *pBUF, int pBUF_SIZE, int START_PUT, int START_GET, int pSIM_MASK)
		//X[LEFT_0].SRC = &smon; 
		
     if (PlaySession::RECORDING_MODE == AUDIO_NOREC)  
	 {	
	 	//X[LEFT_0].Init (NULL,  tXL, sizeof(tXL)/sizeof(DSPdata),  	10000				,0, X_MASK  |LEFT);	 
	 	//X[RIGHT_0].Init(NULL,  tXR, sizeof(tXR)/sizeof(DSPdata),  	10000				,0, X_MASK  |RIGHT);	  		  	 	
  //= For X[] buffers the algo inside BUF_SRC Init depndent on SR overwrites the starting k_IN position, so 1000 is ineffective	 	
	 	X[LEFT_0].Init (tXL, NULL,  sizeof(tXL)/sizeof(DSPdata),  	10000				,0, X_MASK  |LEFT,	0);	 
	 	X[RIGHT_0].Init(tXR, NULL,  sizeof(tXR)/sizeof(DSPdata),  	10000				,0, X_MASK  |RIGHT, 0);	  		  	 		 	
		XP[LEFT_0].Init (tXPL, NULL, sizeof(tXPL)/sizeof(DSPdata), 	xB					,0,	XP_MASK  |LEFT,	0); 		
		XP[RIGHT_0].Init(tXPR, NULL, sizeof(tXPR)/sizeof(DSPdata), 	xB			    	,0,	XP_MASK  |RIGHT,0); 
		
/*	 #if 	(SOFT_V == FP_DSP || SOFT_V == FP_TWT2RX) 	
		test_fill(tXPL, sizeof(tXPL),1);			
		test_fill(tXPR, sizeof(tXPR),2);
	 #endif		
*/		
		DB[LEFT_0].Init( tDBL, NULL, sizeof(tDBL)/sizeof(DSPdata), 	xB					,0,			DB_MASK  |LEFT,	&FILT[BAS]);  
		DB[RIGHT_0].Init(tDBR, NULL, sizeof(tDBR)/sizeof(DSPdata), 	xB					,0,			DB_MASK  |RIGHT,&FILT[BAS]); 
		
		DT[LEFT_0].Init (tDTL, NULL, sizeof(tDTL)/sizeof(DSPdata), 	xT*SR_PROC::DAC_SR	,0,	DT_MASK  |LEFT,	&FILT[TWT]); 
		DT[RIGHT_0].Init(tDTR, NULL, sizeof(tDTR)/sizeof(DSPdata), 	xT*SR_PROC::DAC_SR	,0,	DT_MASK  |RIGHT,&FILT[TWT]); 	//BUFFER_DELAY_SPACE	
/*		
	#if  (SOFT_V == FP_XOEP_TEST)		
		test_fill(tXPL, sizeof(tXPL),0);
		test_fill(tXPR, sizeof(tXPR),0); 	
	#elif (SOFT_V == FP_TAB)		
		test_fill(tDBR, sizeof(tDBR),0);
		test_fill(tDBL, sizeof(tDBL),0);
		test_fill(tDTR, sizeof(tDTR),4);
		test_fill(tDTL, sizeof(tDTL),4);		
	#endif		
*/		
		DM[LEFT_0].Init (tDML, NULL, sizeof(tDML)/sizeof(DSPdata), 	xM					,xMGETP,	DM_MASK  |LEFT,	&FILT[MID]);
		DM[RIGHT_0].Init(tDMR, NULL, sizeof(tDMR)/sizeof(DSPdata), 	xM					,xMGETP,	DM_MASK  |RIGHT,&FILT[MID]);   //BUFFER_DELAY_SPACE	
		//if (BAS_CORR_LEVEL_GET >0)
  		//{
  		//}
 		
  		//else
  		//{
		//	DT[LEFT_0].Init (tDTL, NULL, sizeof(tDTL)/sizeof(DSPdata), 	0,				0,	DT_MASK  |LEFT,	&FILT[TWT]); 
		//	DT[RIGHT_0].Init(tDTR, NULL, sizeof(tDTR)/sizeof(DSPdata), 	0,				0,	DT_MASK  |RIGHT,&FILT[TWT]); 	//BUFFER_DELAY_SPACE	
  		//}	
//		YM[LEFT_0].Init (tYML, NULL, sizeof(tYML)/sizeof(DSPdata), 	10000				,0,	YM_MASK  |LEFT, 0);	
//		YM[RIGHT_0].Init(tYMR, NULL, sizeof(tYMR)/sizeof(DSPdata), 	10000				,0,	YM_MASK  |RIGHT,0);		 	
//		RP[LEFT_0].Init (NULL, tRPL, sizeof(tRPL)/sizeof(DSPdata), 	10000				,0, XP_MASK  |LEFT, 0);  //0x00000200|LEFT);	
//		RP[RIGHT_0].Init(NULL, tRPR, sizeof(tRPR)/sizeof(DSPdata), 	10000				,0, XP_MASK  |RIGHT, 0);	// 0x00000200|RIGHT);

							
		if(	DSPSTRING::Get_Digitbit(DSPSTR_EXPH) )	 					
		{
	  	  #if (EP_MODE == EP_TWT)						
			RB[LEFT_0].Init ( tRBL, NULL, sizeof(tRBL)/sizeof(DSPdata), xRBUFP +xEBUF+(EP_SIZE-DSPBLOCK_SIZE2)			,0, RB_MASK |LEFT, &RSMP[BAS]);  
			RB[RIGHT_0].Init(tRBR,	NULL, sizeof(tRBR)/sizeof(DSPdata), xRBUFP+xEBUF+(EP_SIZE-DSPBLOCK_SIZE2)			,0, RB_MASK |RIGHT, &RSMP[BAS]);	

		  #elif	(EP_MODE == EP_MID || EP_MODE == EP_BT)
			RB[LEFT_0].Init ( tRBL, NULL,sizeof(tRBL)/sizeof(DSPdata), 	xRBUFP			,0, RB_MASK |LEFT, &RSMP[BAS]);  //0x00000200|LEFT);	
			RB[RIGHT_0].Init(tRBR,	NULL,  sizeof(tRBR)/sizeof(DSPdata),xRBUFP			,0, RB_MASK |RIGHT, &RSMP[BAS]);	// 0x00000200|RIGHT);			
		  #endif	
		}
		else
		{
			RB[LEFT_0].Init ( tRBL, NULL,sizeof(tRBL)/sizeof(DSPdata),	xRBUFP			,0, RB_MASK |LEFT, &RSMP[BAS]);  //0x00000200|LEFT);	
			RB[RIGHT_0].Init(tRBR,	NULL,  sizeof(tRBR)/sizeof(DSPdata),xRBUFP			,0, RB_MASK |RIGHT, &RSMP[BAS]);	// 0x00000200|RIGHT);
		}					
		RT[LEFT_0].Init (tRTL, NULL, sizeof(tRTL)/sizeof(DSPdata), 	xRBUFP				,0, RT_MASK |LEFT, &RSMP[TWT]);  //0x00000200|LEFT);	
		RT[RIGHT_0].Init(tRTR, NULL, sizeof(tRTR)/sizeof(DSPdata), 	xRBUFP				,0, RT_MASK |RIGHT, &RSMP[TWT]);	// 0x00000200|RIGHT);		 							
		ET[LEFT_0].Init (tETL, NULL, sizeof(tETL)/sizeof(DSPdata), 	xEBUFP				,0, ET_MASK |LEFT, &RSMP[PRE]);  //0x00000200|LEFT);	
		ET[RIGHT_0].Init(tETR, NULL, sizeof(tETR)/sizeof(DSPdata), 	xEBUFP				,0, ET_MASK |RIGHT, NULL); //&RSMP[PRE]);	// 0x00000200|RIGHT);			  				
		EB[LEFT_0].Init (tEBL, NULL, sizeof(tEBL)/sizeof(DSPdata), 	xEBUFP				,0, EB_MASK |LEFT, 	NULL) ; //&RSMP[PRE+1]);  //0x00000200|LEFT);	
		EB[RIGHT_0].Init(tEBR, NULL, sizeof(tEBR)/sizeof(DSPdata), 	xEBUFP				,0, EB_MASK |RIGHT, NULL) ; //&RSMP[PRE+1]);	// 0x00000200|RIGHT);			  				

	#if  (SOFT_V == FP_XOEP_TEST)		
		//test_fill(tXPL, HP_DIRAC_TAB24_40k, HP_FPGA_SIZE, sizeof(tXPL),1);
		//test_fill(tXPR, HP_DIRAC_TAB24_40k, HP_FPGA_SIZE, sizeof(tXPR),1); 	
		
		test_fill(tXPL, SPKIR_44, HP_FPGA_SIZE, sizeof(tXPL),1);
		test_fill(tXPR, SPKIR_44, HP_FPGA_SIZE, sizeof(tXPR),1); 	
		
		
	#elif 	(SOFT_V == FP_DSP || SOFT_V == FP_TWT2RX) 	
/*		test_fill(tDBL, SIN200Hz_192, sizeof(SIN200Hz_192), sizeof(tDBL),1);			
		test_fill(tDBR, SIN300Hz_192, sizeof(SIN300Hz_192), sizeof(tDBR),1);			
		test_fill(tDTL, SIN2kHz_192,  sizeof(SIN2kHz_192), sizeof(tDTL),1);			
		test_fill(tDTR, SIN3kHz_192,  sizeof(SIN3kHz_192), sizeof(tDTR),1);			
*/		
		test_fill(tDBL, HP_DIRAC_TAB24_40k, HP_FPGA_SIZE, sizeof(tDBL),	1);			
		test_fill(tDBR, HP_DIRAC_TAB24_40k, HP_FPGA_SIZE, sizeof(tDBR),	1);			
		test_fill(tDTL, HP_DIRAC_TAB24_40k,	HP_FPGA_SIZE, sizeof(tDTL),	1);			
		test_fill(tDTR, HP_DIRAC_TAB24_40k, HP_FPGA_SIZE, sizeof(tDTR),	1);			
		
	 #endif		

/*	 #if  (SOFT_V == SH_XOEP_PROD)	 	 	
		test_fill(tRTL, sizeof(tRTL),0);
		test_fill(tRBL, sizeof(tRBL),0);
		test_fill(tRTR, sizeof(tRTR),0);
		test_fill(tRBR, sizeof(tRBR),0);

		test_fill(tDTL, sizeof(tDTL),0);
		test_fill(tDBL, sizeof(tDBL),0);
		test_fill(tDTR, sizeof(tDTR),0);
		test_fill(tDBR, sizeof(tDBR),0);				
	 #endif			 		
*/				
	 }		
	 else   // TEST_MODE
	 {
	 	//X[LEFT_0].Init (NULL, tXL,  sizeof(tXL)/sizeof(DSPdata),  	0					,0, X_MASK  |LEFT);  //0x01000000|LEFT);	
	 	//X[RIGHT_0].Init(NULL, tXR,  sizeof(tXR)/sizeof(DSPdata),  	0					,0, X_MASK  |RIGHT);	// 0x01000000 |RIGHT);	  		  	 		 		 		
	   #if (EP_MODE == EP_PRE)
	 	X[LEFT_0].Init (tXL,  NULL, sizeof(tXL)/sizeof(DSPdata),  	0					,0, X_MASK  |LEFT, 0);  //0x01000000|LEFT);	
	 	X[RIGHT_0].Init(tXR,  NULL, sizeof(tXR)/sizeof(DSPdata),  	0					,0, X_MASK  |RIGHT, 0);	// 0x01000000 |RIGHT);	  		  	 		 		 	
	   #else	   
	 	X[LEFT_0].Init (tXL,  NULL, sizeof(tXL)/sizeof(DSPdata),  	0					,0, X_MASK  |LEFT, 0);  //0x01000000|LEFT);	
	 	X[RIGHT_0].Init(tXR,  NULL, sizeof(tXR)/sizeof(DSPdata),  	0					,0, X_MASK  |RIGHT, 0);	// 0x01000000 |RIGHT);	  		  	 		 		 		   
	   #endif	
		XP[LEFT_0].Init (tXPL, NULL, sizeof(tXPL)/sizeof(DSPdata), 	0					,0, XP_MASK |LEFT, 0);  //0x02000000|LEFT); 
		XP[RIGHT_0].Init(tXPR, NULL, sizeof(tXPR)/sizeof(DSPdata), 	0					,0, XP_MASK |RIGHT, 0);	// 0x02000000|RIGHT); 
		DB[LEFT_0].Init( tDBL, NULL, sizeof(tDBL)/sizeof(DSPdata), 	0					,0, DB_MASK |LEFT, &FILT[BAS]);  //0x00010000|LEFT);  
		DB[RIGHT_0].Init(tDBR, NULL, sizeof(tDBR)/sizeof(DSPdata), 	0					,0, DB_MASK |RIGHT, &FILT[BAS]);	// 0x00010000|RIGHT); 
	
	  // @@@@@   !!! CHECK 1000 !!! @@@@
		//DM[LEFT_0].Init (tDML, NULL, sizeof(tDML)/sizeof(DSPdata), 	0				   	,1000, DM_MASK |LEFT, &FILT[MID]);  //0x00020000|LEFT);
		//DM[RIGHT_0].Init(tDMR, NULL, sizeof(tDMR)/sizeof(DSPdata), 	0				   	,1000, DM_MASK |RIGHT, &FILT[MID]);	// 0x00020000|RIGHT);
	  // @@@@@   !!! CHECK 1000 !!! @@@@	
		DM[LEFT_0].Init (tDML, NULL, sizeof(tDML)/sizeof(DSPdata), 	0				   	,0, DM_MASK |LEFT, &FILT[MID]);  //0x00020000|LEFT);
		DM[RIGHT_0].Init(tDMR, NULL, sizeof(tDMR)/sizeof(DSPdata), 	0				   	,0, DM_MASK |RIGHT, &FILT[MID]);	// 0x00020000|RIGHT);
		
		DT[LEFT_0].Init (tDTL, NULL, sizeof(tDTL)/sizeof(DSPdata), 	0					,0, DT_MASK |LEFT, &FILT[TWT]);  //0x00040000|LEFT); 
		DT[RIGHT_0].Init(tDTR, NULL, sizeof(tDTR)/sizeof(DSPdata), 	0					,0, DT_MASK |RIGHT, &FILT[TWT]);	// 0x00040000|RIGHT); 	
		
//		YM[LEFT_0].Init (tYML, NULL, sizeof(tYML)/sizeof(DSPdata), 	0					,0, YM_MASK |LEFT, 0);  //0x00000200|LEFT);	
//		YM[RIGHT_0].Init(tYMR, NULL, sizeof(tYMR)/sizeof(DSPdata), 	0					,0, YM_MASK |RIGHT, 0);	// 0x00000200|RIGHT);		 			
//		RP[LEFT_0].Init (NULL, tRPL, sizeof(tRPL)/sizeof(DSPdata), 	0					,0, XP_MASK |LEFT, 0);  //0x00000200|LEFT);	
//		RP[RIGHT_0].Init(NULL, tRPR, sizeof(tRPR)/sizeof(DSPdata), 	0					,0, XP_MASK |RIGHT, 0);	// 0x00000200|RIGHT);		 					
		if(	DSPSTRING::Get_Digitbit(DSPSTR_EXPH) )	 					
		{	  
	  	  #if (EP_MODE == EP_TWT)			
			RB[LEFT_0].Init (tRBL, NULL, sizeof(tRBL)/sizeof(DSPdata), xRBUFT+xEBUF+(EP_SIZE-DSPBLOCK_SIZE2)	,0, RB_MASK |LEFT, &RSMP[BAS]);  //0x00000200|LEFT);	
			RB[RIGHT_0].Init(tRBR, NULL, sizeof(tRBR)/sizeof(DSPdata), xRBUFT+xEBUF+(EP_SIZE-DSPBLOCK_SIZE2)	,0, RB_MASK |RIGHT, &RSMP[BAS]);	// 0x00000200|RIGHT);			
          #elif	(EP_MODE == EP_MID || EP_MODE == EP_BT)			
			RB[LEFT_0].Init (tRBL, NULL, sizeof(tRBL)/sizeof(DSPdata), xRBUFT									,0, RB_MASK |LEFT, &RSMP[BAS]);  //0x00000200|LEFT);	
			RB[RIGHT_0].Init(tRBR, NULL, sizeof(tRBR)/sizeof(DSPdata), xRBUFT									,0, RB_MASK |RIGHT, &RSMP[BAS]);	// 0x00000200|RIGHT);		 									
		  #endif					 									
		}
		else
		{
			RB[LEFT_0].Init (tRBL, NULL, sizeof(tRBL)/sizeof(DSPdata), xRBUFT				,0, RB_MASK |LEFT, &RSMP[BAS]);  //0x00000200|LEFT);	
			RB[RIGHT_0].Init(tRBR, NULL, sizeof(tRBR)/sizeof(DSPdata), xRBUFT				,0, RB_MASK |RIGHT, &RSMP[BAS]);	// 0x00000200|RIGHT);		 									
		}			
		RT[LEFT_0].Init (tRTL, NULL, sizeof(tRTL)/sizeof(DSPdata), xRBUFT					,0, RT_MASK |LEFT, 	&RSMP[TWT]);  //0x00000200|LEFT);	
		RT[RIGHT_0].Init(tRTR, NULL, sizeof(tRTR)/sizeof(DSPdata), xRBUFT					,0, RT_MASK |RIGHT, &RSMP[TWT]);	// 0x00000200|RIGHT);			  
//		RM[LEFT_0].Init (tRML, NULL, sizeof(tRML)/sizeof(DSPdata), xRBUFT					,0, RM_MASK |LEFT, 	&RSMP[MID]);  //0x00000200|LEFT);	
//		RM[RIGHT_0].Init(tRMR, NULL, sizeof(tRMR)/sizeof(DSPdata), xRBUFT					,0, RM_MASK |RIGHT, &RSMP[MID]);	// 0x00000200|RIGHT);			  
		
		//ET[LEFT_0].Init (tETL, NULL, sizeof(tETL)/sizeof(DSPdata), xEBUFT					,1000, ET_MASK |LEFT, 	&RSMP[PRE]);  //0x00000200|LEFT);	
		//ET[RIGHT_0].Init(tETR, NULL, sizeof(tETR)/sizeof(DSPdata), xEBUFT					,1000, ET_MASK |RIGHT, &RSMP[PRE]);	// 0x00000200|RIGHT);			  				
		ET[LEFT_0].Init (tETL, NULL, sizeof(tETL)/sizeof(DSPdata), xEBUFT					,0, ET_MASK |LEFT, 	&RSMP[PRE]);  //0x00000200|LEFT);	
		ET[RIGHT_0].Init(tETR, NULL, sizeof(tETR)/sizeof(DSPdata), xEBUFT					,0, ET_MASK |RIGHT, &RSMP[PRE]);	// 0x00000200|RIGHT);			  				
		
		EB[LEFT_0].Init (tEBL, NULL, sizeof(tEBL)/sizeof(DSPdata), xEBUFT					,0, EB_MASK |LEFT, 	&RSMP[PRE+1]);  //0x00000200|LEFT);	
		EB[RIGHT_0].Init(tEBR, NULL, sizeof(tEBR)/sizeof(DSPdata), xEBUFT					,0, EB_MASK |RIGHT, &RSMP[PRE+1]);	// 0x00000200|RIGHT);			  				
		
//*/      
	 }
  #if (SOFT_V != FP_TWT2RX && SOFT_V != FP_XOEP_TEST)  
	 X_SRC.Init(&X[0], 15000);
  #endif	 
}
