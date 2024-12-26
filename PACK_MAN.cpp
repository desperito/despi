
////////////////////////////////////////////////////////////////////////////////////////
//NAME:     Buffer.cpp
//DATE:     17/10/2021
//PURPOSE:  Flexible buffer class implementation
////////////////////////////////////////////////////////////////////////////////////////
#include 	<stdio.h>
#include 	"tt.h"

#include 	"hSTAGE_PROC.hpp"
#include 	"hBUF_MANAGER.hpp"
#include	"hSR_PROC.hpp"


extern 		BUF_MANAGER			X[2]; 	
	
//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================


	#include	"hPACK_MAN.hpp"
	
	int					PACK_MAN::YSPACE_OK;

	PACK_MAN			PM[I2STX_QTY][2]; //  
	
		
//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================

#define		xSTARTDIST_PUTGET 		(BUF_SIZE/2)
#define		xMINDIST_YDSP			20000 

//#pragma optimize_off
//void	PACK_MAN::Init(BUF_MANAGER *YS0, BUF_MANAGER *YS1, int TCB_BlockSize, int pZEROS, BUF_MANAGER	*YS2, BUF_MANAGER *YS3)  //int STARTDIST_PUTGET ,  int pMINDIST_YDSP,  
void	PACK_MAN::Init(int TCB_BlockSize, int pZEROS)  //int STARTDIST_PUTGET ,  int pMINDIST_YDSP,  
{
	int k;

	//YS[0] = YS0; YS[1] = YS1;  YS[2] = YS2; YS[3] = YS3;
	YS_first = 0; YBUFS_QTY= 0;
  	for(k=0; k< sizeof(YS)/sizeof(BUF_MANAGER *); k++)		// MAX_YBUFS_PER_I2SLINE
  	{
   		if (YS[k] != NULL) 
			++YBUFS_QTY;
		else if (YBUFS_QTY ==0)
			++YS_first;
  	}			
	BUF_SIZE = YS[YS_first]->BUF_SIZE; 	
	k_PUT	= xSTARTDIST_PUTGET ;  //BUF_SIZE/2; //STARTDIST_PUTGET ;
	k_PUTBUFS = 0;
	MINDIST_YDSP = xMINDIST_YDSP;
	
 
	if (pZEROS == NULL)
	{ // automatic ZEROS value calculation when pZeros not in use
  		if (YS[1] == NULL)   
  			ZEROS = 1;
  		else
  			ZEROS = 0;
	}
	else 
		ZEROS = pZEROS-1;
  	 
  	OUT_SMPQTY = TCB_BlockSize/(YBUFS_QTY+ZEROS);		
 	k_PUT = 0;  	k_PUTBUFS= 0;		
	k_GET= 0;		k_GETBUFS= 0;
	k_GET_DMA= 0;  	k_GETBUFS_DMA= 0;  	
	
	YSTEP = 1;  // @ For future use - to check
}

//= INTEGER OVERFLOW to refactor
void 	PACK_MAN::Calc_YS_PUT(void)
{	
	int 		k; 
	
   #if	(SOFT_V & SHARC_HW)  // these variables get overflown if 32-bit so in the 32-bit Sharc processor they require special 64-bit data types
 	long long	kmax, cput;    
  #else	// for LINUX_HW 
	int			kmax, cput; 
  #endif
  
	kmax=0; 
  	for(k=0; k< YBUFS_QTY; k++)
  	{
//= INTEGER OVERFLOW to refactor  		
  		cput = YS[k]->k_IN+ YS[k]->k_INBUFS*YS[k]->BUF_SIZE;
		if (cput> kmax)
		{
			kmax = cput;
			k_PUT = YS[k]->k_IN;
			k_PUTBUFS = YS[k]->k_INBUFS;
		}						
  	}
}

/*
void 	PACK_MAN::Update_YS_GET(int cGET)
{
	
	if(cGET < BUF_SIZE)
		k_GET= cGET;
	else
	{
		k_GET= cGET%BUF_SIZE;
		++k_GETBUFS;
	}
}
*/

 #if (SOFT_V & SHARC_HW)  		
//==   #pragma optimize_off
 #endif 

 #if (SOFT_V & SHARC_HW)		
	#pragma optimize_for_speed
 #endif 		
 
 int *q1, kk; 
  
 void PACK_MAN::GETX_SPDIF24(int *I2Sbuf, int SPACE_CHECK) //, int DOWN_RATE)  //BUF_MANAGER *YS[4])
// void PACK_MAN::GETX_SPDIF(int *I2Sbuf, int SPACE_CHECK) //, int DOWN_RATE)  //BUF_MANAGER *YS[4]) 
 {
 	
 int			k_YS, jI2S, jsmp, yGET, YSTEP; 
  	int			BUF_LEFT;
 
 BUF_MANAGER	*B;			
		
 		q1= I2Sbuf; 
		Calc_YS_PUT();
		
		YSTEP = YBUFS_QTY + ZEROS; 
	for(k_YS=YS_first; k_YS< YS_first+YBUFS_QTY; k_YS++)
	{	
		B = YS[k_YS]; 
	 	yGET= 		k_GET;	 	
		switch(B->BufType)
		{ 
	  	  case 1:  // Float buffer data	 	  	  		
			if ( (yGET+ OUT_SMPQTY) <BUF_SIZE )
			{
				for(jsmp=0, jI2S= k_YS; jsmp< OUT_SMPQTY; jsmp++, jI2S+=YSTEP, yGET++)
					I2Sbuf[jI2S] = B->Buf[yGET];  
			}
			else 
			{
				BUF_LEFT = BUF_SIZE- yGET;
				for(jsmp=0, jI2S= k_YS; jsmp< BUF_LEFT ; jsmp++, jI2S+=YSTEP, yGET++)
					I2Sbuf[jI2S] = B->Buf[yGET];  
				yGET = 0 ; 	//++k_GETBUFS;   						  			
				for(  ; jsmp< OUT_SMPQTY ; jsmp++, jI2S+=YSTEP, yGET++)
					I2Sbuf[jI2S] = B->Buf[yGET];  
			}
	 	    break;
	  	  case 2:  // int buffer data	  	  
			if ( (yGET+ OUT_SMPQTY) <BUF_SIZE )
			{			
				for(jsmp=0, jI2S= k_YS; jsmp< OUT_SMPQTY; jsmp++, jI2S+=YSTEP, yGET++)
					I2Sbuf[jI2S] = B->Bufi[yGET];  
			}
			else 
			{
				BUF_LEFT = BUF_SIZE- yGET;
				for(jsmp=0, jI2S= k_YS; jsmp< BUF_LEFT ; jsmp++, jI2S+=YBUFS_QTY, yGET++)
					I2Sbuf[jI2S] = B->Bufi[yGET];  
				yGET = 0 ; 	//++k_GETBUFS;   						  			
				for(  ; jsmp< OUT_SMPQTY ; jsmp++, jI2S+=YBUFS_QTY, yGET++)
					I2Sbuf[jI2S] = B->Bufi[yGET];  
			}			
	 	   break;
 		}	 	    		
	}
		
	if (yGET < k_GET)
		++k_GETBUFS;
	k_GET = yGET;
	++kk;
	if (kk>300)
		kk =0; 	
	if (SPACE_CHECK) 
		Control_YSPACE();	
	
 #if (SOFT_V & SHARC_HW)  			
	#pragma optimize_for_speed
 #endif 			
}
//#endif

 
#define 	SCALER_32BIT 		80.0 // 64.0 // 		

//=  GETX_SPDIF: Standard Sharc sending to SPDIF in both cases:
// 1) when Sharc does standalone DSP and 
// 2) when testing what comes from Rpi - then comes to PIX1 buffer and then is moved to PIX2 buffer from where it is sent via SPDIF
 void PACK_MAN::GETX_SPDIF(int *I2Sbuf, int SPACE_CHECK) //, int DOWN_RATE)  //BUF_MANAGER *YS[4])
 {
 	
 int			k_YS, jI2S, jsmp, yGET, YSTEP; 
  	int			BUF_LEFT;
 
 BUF_MANAGER	*B;			
	
		Calc_YS_PUT();
		
		YSTEP = YBUFS_QTY + ZEROS; 
	for(k_YS=YS_first; k_YS< YS_first+YBUFS_QTY; k_YS++)
	{	
		B = YS[k_YS]; 
	 	yGET= 		k_GET;	 	
		switch(B->BufType)
		{ 
	  	  case 1:  // Float buffer data	 	  	  		
			if ( (yGET+ OUT_SMPQTY) <BUF_SIZE )
			{			
				for(jsmp=0, jI2S= k_YS; jsmp< OUT_SMPQTY; jsmp++, jI2S+=YSTEP, yGET++)
				  #if (SOFT_V == SH_XOEP_PROD)	
				  	I2Sbuf[jI2S] = (int) (SCALER_32BIT* B->Buf[yGET]);
				  #else
					I2Sbuf[jI2S] = B->Buf[yGET];  
				  #endif	
			}
			else 
			{
				BUF_LEFT = BUF_SIZE- yGET;
				for(jsmp=0, jI2S= k_YS; jsmp< BUF_LEFT ; jsmp++, jI2S+=YSTEP, yGET++)
				  #if (SOFT_V == SH_XOEP_PROD)	
				  	I2Sbuf[jI2S] = (int) (SCALER_32BIT* B->Buf[yGET]);
				  #else				
					I2Sbuf[jI2S] = B->Buf[yGET];  
				  #endif	
				yGET = 0 ; 	//++k_GETBUFS;   						  			
				for(  ; jsmp< OUT_SMPQTY ; jsmp++, jI2S+=YSTEP, yGET++)
				  #if (SOFT_V == SH_XOEP_PROD)	
				  	I2Sbuf[jI2S] = (int) (SCALER_32BIT* B->Buf[yGET]);
				  #else				
					I2Sbuf[jI2S] = B->Buf[yGET];  
				  #endif	
			}			
	 	    break;
	  	  case 2:  // int buffer data	  	  
			if ( (yGET+ OUT_SMPQTY) <BUF_SIZE )
			{			
				for(jsmp=0, jI2S= k_YS; jsmp< OUT_SMPQTY; jsmp++, jI2S+=YSTEP, yGET++)
				  #if (SOFT_V == SH_XOEP_PROD)	
				  	I2Sbuf[jI2S] = (int) (SCALER_32BIT* B->Buf[yGET]);
				  #else				
					I2Sbuf[jI2S] = B->Bufi[yGET];  
                 #endif					
			}
			else 
			{
				BUF_LEFT = BUF_SIZE- yGET;
				for(jsmp=0, jI2S= k_YS; jsmp< BUF_LEFT ; jsmp++, jI2S+=YBUFS_QTY, yGET++)
				  #if (SOFT_V == SH_XOEP_PROD)	
				  	I2Sbuf[jI2S] = (int) (SCALER_32BIT* B->Buf[yGET]);
				  #else				
					I2Sbuf[jI2S] = B->Bufi[yGET];  
				  #endif	
				yGET = 0 ; 	//++k_GETBUFS;   						  			
				for(  ; jsmp< OUT_SMPQTY ; jsmp++, jI2S+=YBUFS_QTY, yGET++)
				  #if (SOFT_V == SH_XOEP_PROD)	
				  	I2Sbuf[jI2S] = (int) (SCALER_32BIT* B->Buf[yGET]);
				  #else				
					I2Sbuf[jI2S] = B->Bufi[yGET];  
                  #endif					
			}			
	 	    break;
 		}	 	    		
	}
		
	if (yGET < k_GET)
		++k_GETBUFS;
	k_GET = yGET;
		
	if (SPACE_CHECK) 
		Control_YSPACE();	
	
 #if (SOFT_V & SHARC_HW)  			
	#pragma optimize_for_speed
 #endif 			
}

		 			
 #if (SOFT_V & SHARC_HW)		
//	#pragma optimize_for_speed
 #endif 		

 	float	PACK_DIFF; 
 	int bfs; 
 
void	PACK_MAN::Control_YSPACE(void)
{
	bfs = BUF_SIZE; 
	PACK_DIFF = (k_PUTBUFS-k_GETBUFS)* BUF_SIZE+ k_PUT - k_GET;  
	if ( PACK_DIFF < (BUF_SIZE-MINDIST_YDSP) )	
		YSPACE_OK = 1;
	else
		YSPACE_OK = 0;	
}

