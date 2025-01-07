#include 	"tt.h"

#if (SOFT_V & SHARC_HW) 		
 #include 	<21369.h>
 #include 	<Cdef21369.h>
 #include 	<def21369.h>
 #include 	<filter.h> 
#else
  EXTERNC	void iir(const float *input, float *output, const float *coeffs, float *state, int samples, int sections);
#endif
 
#include 	<math.h>
#include 	<stdio.h>
//#include 	"E:\PRODUCTION\Elektronika_NEW\FILTRY\Matlab\spec2.h"

#include 	"hPlaySession.hpp"
#include 	"hSR_PROC.hpp"
#include 	"hSTAGE_PROC.hpp"
#include 	"hFILTER.hpp"
#include 	"hDSPSTRING.hpp"

#include "rt20.h"
#include "rt200.h"

//#include 	"C:\git_repos\__X1\include\test_x65.h"

int  k, k1, k2;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//===-----------    C L A S S  ===============
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#include 	"hMODEC.hpp"

#if (SOFT_V & SHARC_HW)	
 	float 	pm modes_filts[2][MODES_QTY][MODE_COEFF_QTY]; 		
#else  // for LINUX_HW = needs to be without Sharc-specific *pm* memory type qualifier
 	float 		modes_filts[2][MODES_QTY][MODE_COEFF_QTY];
#endif
//================================================================================
//===========  C L A S S   V A R I A B L E S   D E F I N I T I O N   =============
//================================================================================

  SECTION(("seg_dmda2"))  float  	MODEC::modes_state[CHN_QTY][MODES_QTY][4];
  
// char string[3][30]={"String 1","String 2","String 3"};
  SECTION(("seg_sdram2"))   char		MODEC::qlabels[2][FILTER_Q_VALUES][QLABEL_LEN] = { {"_Q90", "_Q50","_Q30", "_Q20","_Q10", "_Q05"},{"_Q90", "_Q50","_Q30", "_Q20","_Q10", "_Q05"} };
  SECTION(("seg_sdram2"))   char		MODEC::fullname[2][MODEC_FULLNAME_LEN];  
  SECTION(("seg_sdram2"))   int 		MODEC::kqtext[2][FILTER_Q_VALUES]={QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN,QNAME_LEN};
  //, MODEC::kfull[2]={0,0}; 
  int		MODEC::filt_qty[2] = {0,0};
  SECTION(("seg_sdram2"))   int		MODEC::filt_by_Q_qty[2][FILTER_Q_VALUES];
  
//================================================================================
//===========  C L A S S   F U N C T I O N S   D E F I N I T I O N   ==============
//================================================================================

void	 MODEC::Clear_MODEC(void) 
					{  			
						int kfreq, kf1;  
				#if	(SOFT_V & SHARC_HW)
						trace(MODEC::fullname[LEFT_0],0);		    		 
				#endif		
  						filt_qty[LEFT_0]= 0; filt_qty[RIGHT_0]= 0;  // Clear filters
						for (kfreq=0; kfreq< MODES_QTY; kfreq++)
						{
							for (kf1=0; kf1<4; kf1++)
							{
								modes_filts[LEFT_0][kfreq][kf1] = 0;
								modes_filts[RIGHT_0][kfreq][kf1] = 0;
							}
						}
  						
  						Clear_labels();
  						Concatenate_qlabels(LEFT_0); Concatenate_qlabels(RIGHT_0); 
  			#if	(SOFT_V & SHARC_HW)						
  						trace("ClrMODEC",0);
			#endif  						
					}
					
void	MODEC::state_clr(void)
				{
					for(k=LEFT_0; k<=RIGHT_0; k++)
						for(k1=0; k1<=MODES_QTY; k1++)
							for(k2=0; k2<=MODE_COEFF_QTY; k2++)
								modes_state[k][k1][k2] = 0;
				}	
					

void MODEC::Concatenate_qlabels(int CHANNEL)
{
	int kq, kfull= 0;
	
 	for(kq=0; kq<6; kq++)
 	{
 		k= 0; 
 		while ( qlabels[CHANNEL][kq][k] && filt_by_Q_qty[CHANNEL][kq]>0)
 			fullname[CHANNEL][kfull++]= qlabels[CHANNEL][kq][k++];
 	} 		
} 

 #if (SOFT_V & SHARC_HW)  
	#pragma optimize_off
 #endif		
//void	Init(void)
void 	MODEC::Add_labels(int CHANNEL, float freq, int Q, int dB)
{
	char 	cfreq[5], cdB[3]; 
	int		kq, ftest, ftest2;
	float 	freq1, freq10;
	
	CHANNEL-=1; //LEFT/RIGHT coding starts from 1
   //= Convert numbers to strings 
   
//   	freq10 = (freq*10);
//   	ftest = (int) freq10;
//   	freq10 = 42.9*10;  
//   	ftest2 = (int) freq10;

	sprintf(cfreq,"%d",(int) freq );
	sprintf(cdB,"%d",(int) dB);
	
	switch (Q) 
	{	case 90: 	kq = 0; break;			
       	case 50:	kq = 1;	break;	
       	case 30:	kq = 2;	break;
       	case 20:	kq = 3; break;
       	case 10:	kq = 4; break;
       	case  5:	kq = 5; break;
 	};
 	qlabels[CHANNEL][kq][kqtext[CHANNEL][kq]++]= '_';
  //= Append the frequency number to mode@Q description 
 	
 	for(k=0;cfreq[k]; k++) 
 		qlabels[CHANNEL][kq][kqtext[CHANNEL][kq]++]= cfreq[k];
 		
 	qlabels[CHANNEL][kq][kqtext[CHANNEL][kq]++]= '@'; 
  //= Append the decibel reduction number to mode@Q description
 	for(k=0;cdB[k]; k++)  	
 		qlabels[CHANNEL][kq][kqtext[CHANNEL][kq]++]= cdB[k];	
   	qlabels[CHANNEL][kq][kqtext[CHANNEL][kq]+1]= 0;
 	++filt_by_Q_qty[CHANNEL][kq];  // Count filter qty by Q to create optimal description 
   	
 	Concatenate_qlabels(CHANNEL);	 	 	
}
 #if (SOFT_V & SHARC_HW) 
	#pragma optimize_on
 #endif	

void  MODEC::Clear_labels(void)  //Clear all qlabels and fullname
{
	char *ptab, *pfullname;
	int  *pkqtext, *p_filt_by_Q_qty ;
					/*	 
						for(k=0; k<2; k++)
							for(k1=0;k1<6; k1++)
								for(k2=4; k2<40; k2++)
									qlabels[		
					*/	
					 //= Set start of tables for quicker mass-reset			
	ptab = 				(char *) qlabels;
	pkqtext= 			(int *)  kqtext;
	p_filt_by_Q_qty = 	(int *)  filt_by_Q_qty;
	pfullname = 		(char *) fullname;
						
	for(k=0; k<CHN_QTY*FILTER_Q_VALUES; k++)
	{
		*(pkqtext+k) 			= QNAME_LEN; // Set first pos after constant Q name prefix in each qlabel string 
		*(p_filt_by_Q_qty+k) 	= 0;	// Reset qlabels qty 
	}
  	for (k=0; k< MODEC_FULLNAME_LEN*CHN_QTY; k++) 	
  		pfullname[k]= 0;  // Clear full label
							
	for(k=QNAME_LEN; k<CHN_QTY*FILTER_Q_VALUES*QLABEL_LEN; k+=QLABEL_LEN)  // Insert 0 right after first four Qnn_ characters - equivalent of clearing all q-label table 
		*(ptab+k) = 0;					
}


void MODEC::Msg101(int *msg) //msg[UARTMSG_MAX_SIZE])
{
	float 	freq;
	int 	Q, dB, CHANNEL, k;
	float	hbo12, hbo13, hbo22, hbo23; 

	
	
	//freq= 	(msg[2] -UART101_OFFSET) + (float) (msg[4]- UART101_OFFSET)/100 ;
	//Q= 		msg[6] 	- UART101_OFFSET;
	//dB= 	msg[8]	- UART101_OFFSET;
	CHANNEL = msg[5]; 

	freq= 	((float ) msg[1]) *10 + ((float) msg[2])/10;

	dB = 	msg[3]/10;
	Q =  	msg[4];
	
	if(freq>0) 
	{
		k=0;
		k=k*2;
	} 
	
//		++x101;
//	if (!(x101%5))
//		k=1;
	//if 		(freq == 0)		
	//Clear_MODEC();
 	//else 					
 		Generate_filter(CHANNEL, freq, Q, dB, 0 );
 		//Print_DSPstring_Modes();	
 	

}


/*
void MODEC::Msg101(int *msg) //msg[UARTMSG_MAX_SIZE])
{
	float 	freq;
	int 	Q, dB, CHANNEL, k, Boost;
	float	hbo12, hbo13, hbo22, hbo23; 

	
	for (k=0; k< MODES_QTY; k++)
	{
		freq= 	((float ) (msg[k*3+3] & 0x0F) *256 + (float) msg[k*3+4])/10;
		dB = 	msg[k*3+5] & 0x0F;
		Q =  	(msg[k*3+3] & 0xF0)>>4;
		Boost = (msg[k*3+5] & 0x80)>>7;
		if (freq == 0)
			break;
 		Generate_filter(CHANNEL, freq, Q, dB, Boost);				
	}
	DELI_CMD::LED_Color(0x0010, (msg[2]>>4)-0x0A );
	DELI_CMD::LED_Color(0x0001, (msg[2]& 0x0F)-0x0A) );	
}
*/



void MODEC::Generate_filter(int CHANNEL, float freq, int Q, int dB, int Boost)
{
	float  	rgen, rdiff, rxt, t;
	float	hby11, hbs11, hby12, hbs12, hby13, hby14;
	float	hbo12, hbo13, hbo22, hbo23; 
	int		kq, kfreq, kf1, xdwn = 4, apos=2, bpos = 0, sign=1 ;
	float	SR_BASE4; 


	if (TEST_SR_44K_DOMAIN)
		SR_BASE4 = 10025;
	else 
		SR_BASE4 = 12000;
		
		
  //= Obsluga komunikatu kasujacego aktualne filtry 
	if (freq == 0)
	{
		Clear_MODEC(); 
/*		
		for (kfreq=0; kfreq< MODES_QTY; kfreq++)
		{
			for (kf1=0; kf1<4; kf1++)
			{
				modes_Lfilts[kfreq][kf1] = 0;
				modes_Rfilts[kfreq][kf1] = 0;
			}
		}
*/		
  		//trace("Gen",0);
		return;
	}
		
/*			
 //== Wyliczenie indeksu z uwzglednieniem progu freq 1/2Hz
 	if (freq >= FREQ_2Hz)
 		kfreq = floor( (freq- FREQ_2Hz)/2)+ FREQ_2Hz- LOW_FREQ;
 	else 
 		kfreq = floor(freq)-LOW_FREQ; 		
*/
 
	
	if (dB >10)
	{
		apos =0; bpos =2; sign=-1; 
		dB-=10;
	}	

/*
	if (Boost)
	{
		apos =0; bpos =2; sign=-1; 
	}			
*/	
	dB= dB-4;
	
 	if (dB >= -4)
 	{		
 		
		switch (Q) 
		{		case 90: 	kq = 0; break;
            	case 50:	kq = 1;	break;	
            	case 30:	kq = 2;	break;
            	case 20:	kq = 3; break;
            	case 10:	kq = 4; break;
            	case  5:	kq = 5; break;
	    }   
	    
		//kq = 6-Q; 
	    
  #if	(SOFT_V & SHARC_HW ) 			    
		asm("bit set MODE1 RND32;");	
  #endif	        
		rxt = 	(rt20[kq][dB][3]* rt20[kq][dB][4] + rt200[kq][dB][3]* rt200[kq][dB][4] )/2; 
		rgen = 	rt20[kq][dB][3]+(rt200[kq][dB][3]-rt20[kq][dB][3])* (freq-20)/(200-20);
		t 	= 	rxt/rgen/100;

   		hbs11= 	freq/(SR_BASE4/2)*3.14;
  		rdiff = rgen*1E-5;
    	hbs12= 	1-rdiff;	
	
    	hby11 = hbs11; hby12 = hbs12; hby13 = -3/log10(hbs12)/SR_BASE4    ; // zamiast SR_BASE4 Bylo 44100*xdwn; 
    	hby14 = pow(10, -3/(t*SR_BASE4));

		hbo12 = -2*hby12*cos(hby11);  hbo13 = hby12* hby12;
		hbo22 = -2*hby14*cos(hby11);  hbo23 = hby14* hby14; 
  #if	(SOFT_V & SHARC_HW ) 			    		
		asm("bit clr MODE1 RND32;");
  #endif		
 	  //== Zapis filtru
 	  	if (CHANNEL & LEFT)
 	  	{  	  		
			modes_filts[LEFT_0][filt_qty[LEFT_0]][bpos] =  	-hbo23	*sign;	
			modes_filts[LEFT_0][filt_qty[LEFT_0]][bpos+1] = -hbo22	*sign;
			modes_filts[LEFT_0][filt_qty[LEFT_0]][apos] =  	 hbo13	*sign;
			modes_filts[LEFT_0][filt_qty[LEFT_0]][apos+1] =  hbo12	*sign;
			++filt_qty[LEFT_0];			
			
			MODEC::Add_labels(LEFT, freq*10, Q, dB+4);
		}
 	  	if (CHANNEL & RIGHT)
 	  	{
			modes_filts[RIGHT_0][filt_qty[RIGHT_0]][bpos] = -hbo23;
			modes_filts[RIGHT_0][filt_qty[RIGHT_0]][bpos+1] = -hbo22;
			modes_filts[RIGHT_0][filt_qty[RIGHT_0]][apos] =  hbo13;
			modes_filts[RIGHT_0][filt_qty[RIGHT_0]][apos+1] =  hbo12;
			++filt_qty[RIGHT_0];  	  		
						
			MODEC::Add_labels(RIGHT, freq*10, Q, dB+4);
		}		
 	}	 
}


//void  MODEC::FILTER (struct StageManager *SMAct, int StageAct, float *stage_ptr[2], int *pstage_cnt)
void  MODEC::FILTER (STAGE_PROC *SP, int kStage)
{
	
	int 		k, m=0;
	int 		CHANNEL;
	
//	float pm	(*modes_filts)[4];
//	float 		(*modes_state)[3];
	
  //= Wybor filtrow i buforow LEFT/RIGHT - s� rozne w zaleznosci od kanalu lewy czy prawy	
/*  
	if 		(LR_state == LEFT)
	{	modes_state= modes_Lstate; modes_filts = modes_Lfilts; }
	else if (LR_state == RIGHT)
	{	modes_state= modes_Rstate; modes_filts = modes_Rfilts; }	
*/	
	//if(!PlaySession::ACOUSTIC_CORR)
	if(!DSPSTRING::Get_Digitbit(DSPSTR_ACOUSTIC_CORR))
		return;  //MODEC_Clear_MODEC();
		
	//CHANNEL= LR_state-1;   // LEFT/RIGHT Channel
	CHANNEL= SP->Chn;
	
  //=  @@@@@@@@@@@@  REFACTOR AS DERIVED FROM DSP_BASE		  
	for (k=0; k< MODES_QTY; k++)
	{
		if (modes_filts[CHANNEL][k][0] != 0)
		{
			// iir(stage_ptr[(*pstage_cnt)%2], stage_ptr[(*pstage_cnt+1)%2], modes_filts[CHANNEL][k], modes_state[CHANNEL][k], SMAct->size[StageAct], 1);
			// ++(*pstage_cnt); (*pstage_cnt)%=2;
			
			//iir(SP->stage_ptr[SP->stage_cnt%2], SP->stage_ptr[(SP->stage_cnt+1)%2], stages[kStage].coeffs+2, stages[kStage].LR_state[SP->Chn], stages[kStage].size, *(SP->FiltRsmp->stages[kStage].coeffs) );  			
			iir(SP->stage_ptr[SP->stage_cnt%2], SP->stage_ptr[(SP->stage_cnt+1)%2], modes_filts[CHANNEL][k], modes_state[CHANNEL][k],  SP->FiltRsmp->stages[kStage].size, 1);  						
			++SP->stage_cnt; SP->stage_cnt%=2;				
			++m;
		}
	}	
  //=  @@@@@@@@@@@@  REFACTOR AS DERIVED FROM DSP_BASE	
}


/*
EXTERNC void MODEC(struct StageManager *SMAct, int StageAct, float *stage_ptr[2], int *pstage_cnt)
{
	MODEC::FILTER(SMAct, StageAct, stage_ptr, pstage_cnt);
}
*/

EXTERNC void FMODEC(STAGE_PROC *SP, int kStage)
{
	MODEC::FILTER (SP, kStage); 
//	iir(SP->stage_ptr[SP->stage_cnt%2], SP->stage_ptr[(SP->stage_cnt+1)%2], stages[kStage].coeffs+2, stages[kStage].LR_state[SP->Chn], stages[kStage].size, *(SP->FiltRsmp->stages[kStage].coeffs) );  //SP->FiltRsmp->DSP_SIZE,
}

EXTERNC void MODEC_Msg101(int *msg) //msg[UARTMSG_MAX_SIZE]) 
{
	MODEC::Msg101(msg);
}

EXTERNC float (* MODEC_modes_state(void) )[MODES_QTY][4] 
{
	return MODEC::modes_state;
}

EXTERNC void MODEC_state_clr(void) 
{
	return MODEC::state_clr();
}

EXTERNC char * MODEC_CHN_fullname(int channel) 
{
	return MODEC::Get_CHN_fullname(channel);
}

EXTERNC int MODEC_filt_qty(int chn) 
{
	return MODEC::filt_qty[chn];
}

EXTERNC void MODEC_Generate_filter(int CHANNEL, float freq, int Q, int dB, int Boost)
{
	MODEC::Generate_filter(CHANNEL, freq, Q, dB, Boost);
}

EXTERNC void	 MODEC_Clear_MODEC(void)
{
	MODEC::Clear_MODEC();
} 

//================================================================================
//===========  		  E N D   O F   C L A S S   C O D E   			 =============
//================================================================================
/*
SECTION ("seg_sdram2") 	float  spec1[MODES_QTY][3]= 
{
  { 20e+00 ,	90 ,	10   },
  { 30e+00 ,	90 ,	8   },
  { 100e+00 ,	90 ,	6   }

};
//int kfreq=0;
*/
/*
void generate_MODEC2(float freq, float hbo12, float hbo13, float hbo22, float hbo23)
{
	int		kq, kfreq1, xdwn = 4;

 //== Wyliczenie indeksu z uwzglednieniem progu freq 1/2Hz
/// 	if (freq >= FREQ_2Hz)
// 		kfreq = floor( (freq- FREQ_2Hz)/2)+ FREQ_2Hz- LOW_FREQ;
// 	else 
// 		kfreq = floor(freq)-LOW_FREQ; 		
// /		
 	  //== Zapis filtru
		if (MODCFG_CH & LEFT)
		{
			modes_Lfilts[kfreq][0] = -hbo23;
			modes_Lfilts[kfreq][1] = -hbo22;
			modes_Lfilts[kfreq][2] =  hbo13;
			modes_Lfilts[kfreq][3] =  hbo12;
		}
		if (MODCFG_CH & RIGHT)
		{
			modes_Rfilts[kfreq][0] = -hbo23;
			modes_Rfilts[kfreq][1] = -hbo22;
			modes_Rfilts[kfreq][2] =  hbo13;
			modes_Rfilts[kfreq][3] =  hbo12;
		}			
		++kfreq;				 
}
*/


/*

void  Init_MODEC (void)
{
	int k;
	
	for (k=0; k< MODES_QTY; k++)
	{
		if (spec1[k][0] > 0 )
			generate_MODEC(spec1[k][0], spec1[k][1], spec1[k][2]);
	}
}				

void  Clear_MODEC (void)
{
	int k;
	
	if (MODCFG_CH & LEFT)
		for (k=0; k< MODES_QTY; k++)
		{	modes_Lfilts[k][0] = 0; modes_Lfilts[k][1] = 0; modes_Lfilts[k][2] = 0; modes_Lfilts[k][3] = 0; }
	if (MODCFG_CH & RIGHT)
		for (k=0; k< MODES_QTY; k++)
		{	modes_Rfilts[k][0] = 0; modes_Rfilts[k][1] = 0; modes_Rfilts[k][2] = 0; modes_Rfilts[k][3] = 0; }
}

void test_MODEC(void)
{
	int 	j, k;
	char	msg[10];
	
	//- Symulacja komunikatow startowych UART
		//PlaySession.CORRECTION_TYPE		= 	( msg[2] & (0x03<<1) )>>1;  	//	CORRECTION_TYPE 	[0..3]
		//PlaySession.TEST_SETUP			= 	( msg[2] & (0x03<<3) )>>3;  	//	TEST SETUP 			[0..3]				
		//PlaySession.TEST_SETUP2			= 	( msg[2] & (0x03<<3) )>>3;  	//	TEST SETUP 			[0..3]						
		//PlaySession.NOISE_SHAPING		=	( msg[2] & (0x01<<5) )>>5;		// 	Noise-Shaping 		0/1	
		//PlaySession.PHASE_CORRECTION	= 	  msg[2] & (0x01<<0);			// 	Phase-Correction 	0/1
	
//	msg[0]= 0; msg[1]= 0; 
//	msg[2]= 1<<6;   //Singletone 
//	msg[3]= 0; msg[4]= 0; msg[5]= 0; msg[6]= 0; msg[7]= 0; msg[8]= 0; msg[9]= 0; 
//	Msg0x50_Proc(msg);
	
//	msg[2] == CD44_16;	
//	msg[3]	= CABLE_LINK;	
//	Msg0x92_Proc(msg);

	Init_MODEC();
	spec1[0][0]= 10;
	PlaySession.CORRECTION_TYPE = CORRECTION_A; 
	PlaySession.SAMPLE_RATE = CD44_16; 
	PlaySession.DATAPROC_MODE = I2S_STEREO;
	PlaySession.DAC_SR = 1;
	PlaySession.TEST_SETUP = BAS; // FULL_SPK; //TWT;
	Vol.start_vol = 1;  
	PlaySession.MCB_TYPE = 1;
	
	DSP_Stage_config();
	
	AudioBufPtr_Init();
	
	for (k=0; k<CIRCULAR_LEN*4+1; k++)
	{
		AudioIN_ISR(0);
    	if	(BLRAM_Ready )
    	{	
    	  #ifdef	DAC    	  	
    	  	LR_state = 1;
    		DSP_BlockProc(BLRAM_ptr, 	block_bas, 	block_twt, 	block_mid); 
    		    		
//    		LR_state = 2;
//			DSP_BlockProc(BL2RAM_ptr, 	block_bas2, block_twt2, block_mid2);    		
			j=0;
		  #endif
    	}
//    	EXC0_ISR();
	}

	//generate_MODEC(30, 50, 10);	
}

*/


/*
	generate_IIR(20, 50, 10);
	test_IIR();
*/


/*
int 	modes_blocksize;

int		MODCFG_CH= 3;	


void test_IIR(void)
{
	int j, k; 
	
	asm("bit clr MODE1 RND32;");
	for	(k=0; k< DSPBLOCK_SIZE2; k++)
		RoboDSP0[k] = 0;
	RoboDSP0[0] = 1;
	j=DSPBLOCK_SIZE2;	

	
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);
	RoboDSP0[0] = 0;
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);	
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);		

	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);	
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);
	iir(RoboDSP0, RoboDSP1, mod_filt, mod_state, DSPBLOCK_SIZE2, 1);	
}
*/
