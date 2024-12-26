
#include 	"tt.h"
#include 	<stdlib.h>

#include 	"hSTAGE_PROC.hpp"
#include 	"hBUF_MANAGER.hpp"

//================================================================================
//===========  C L A S S   D E C L A R A T I O N   S T A R T   ===================
//================================================================================
#include 	"hPOLY.hpp"

SECTION(("seg_sdram2")) 	float	cpoly[32] = {1/8, 2/8, 3/8, 4/8, 5/8, 6/8, 7/8};
SECTION(("seg_dmda"))	 	struct interpol 	 	IPL32[IPL_QTY]; 
//SECTION(("seg_dmda"))	 	struct interpol 	 	Lag;
//SECTION(("seg_dmda")) 		struct interpol 	 	He;
//SECTION(("seg_dmda")) 		struct interpol 	 	Osc;

	int				POLY::LOAD_SIZE;			
	int				POLY::EXT_IPL_TYPE;			
	int				POLY::OVS_EXT;

	//IPL_map_struct		POLY::IPL_codes[IPL_QTY+1]= { {'L',NULL}, {'G',&IPL32[0]}, {'B',&IPL32[1]}, {'H',&IPL32[2]}, {'W',&IPL32[3]}, {'O',&IPL32[4]} };
	//IPL_map_struct		POLY::IPL_codes[IPL_QTY+1]= { {'L',NULL}, {'W',&IPL32[0]}, {'H',&IPL32[1]}, {'G',&IPL32[2]}, {'I',&IPL32[3]}, {'J',&IPL32[4]} };
	
//	IPL_map_struct			POLY::IPL_codes[IPL_QTY+1]= { {'L',NULL}, {'W',&IPL32[0]}, {'H',&IPL32[1]}, {'G',&IPL32[2]}, {'I',&IPL32[3]}, {'J',&IPL32[4]} };
	IPL_map_struct			POLY::IPL_codes[IPL_QTY+1]= { {'L',NULL}, {'W',&IPL32[0]}, {'H',&IPL32[1]}, {'I',&IPL32[3]}, {'J',&IPL32[4]} };

		
//================================================================================
//===========  C L A S S   D E C L A R A T I O N  S T O P   ======================
//================================================================================


void 	POLY::Constructor(void)
{
	int k;
	
//	for(k=0; k<IPL_QTY; k++)
//		POLY::Generate(IPL_codes[k].Code, &IPL32[k], 	32); 	//Lagrange_calc( &Lag, 32);		 	
	
 	//= Interpolation INIT
	//POLY::Generate('G', &Lag, 	32); 	//Lagrange_calc( &Lag, 32);		 	
	//POLY::Generate('B', &BS, 	32); 	//BS_calc( &BS, 32);
	//POLY::Generate('H', &He, 	32); 	//Hermite_calc( &He, 32);  			
	//POLY::Generate('O',	&Osc, 	32); 	//Osc2nd_calc( &Osc, 32);
}


//struct interpol	* 
int 	POLY::Map_Poly(char  pCode)
{
	//int kpoly=0;
	int		k;
	
	//for(k=0;k<IPL_QTY;k++)
	//	if (IPL_codes[k].Code == pCode)
	//		break;
	//return(IPL_codes[k].Poly);
	if (pCode == 'S' || pCode == 'Z')
		k=0;
	else
	{
		for(k=0; k<IPL_QTY+1; k++)
		{
			if (POLY::IPL_codes[k].Code==pCode)
				break;
		}
	}			
/*	
	switch(pCode)
	{
		case 'L':	kpoly=0;
		  break;		
		case 'G':	kpoly=1;
		  break;
		case 'B':	kpoly=2;
		  break;
		case 'H':	kpoly=3;
		  break;
		case 'W':	kpoly=4;
		  break;
//		case 'O':	kpoly=5;
//		  break;		  		  		  		  		  
	}
*/	
	return(k);
}



void POLY::Linear_calc(struct interpol *poly, int xlevel)
{
	int k;
	float a;
	
	
	poly_init(xlevel);
	//xlevel=4;
	for(k=0; k< xlevel; k++)
	{ 
		a= cpoly[k];
		poly->fm2[k]= 0;
		poly->fm1[k]= a;
		poly->f0[k]	= 1-a;
		poly->fp1[k]= 0;	
	}
}

void POLY::Lagrange_calc(struct interpol *poly, int xlevel)
{
	int k;
	float a;
	
	poly_init(xlevel);
	//xlevel=4;	
	for(k=1; k<= xlevel; k++)
	{ 
			a			= cpoly[k-1];
		poly->fm2[k-1]	= (	 -   a			+   a*a*a)/ 6;
		poly->fm1[k-1]	= (    2*a 	+   a*a -   a*a*a)/ 2;
		 poly->f0[k-1]	= (2 -   a	- 2*a*a +   a*a*a)/ 2;
		poly->fp1[k-1]	= (   -2*a 	+ 3*a*a -   a*a*a)/ 6;	
	}
}


void POLY::BS_calc(struct interpol *poly, int xlevel)
{
	int k;
	float a;
	
	poly_init(xlevel);
	//xlevel=4;
	for(k=1; k<= xlevel; k++)
	{ 
					a	= cpoly[k-1];
		poly->fm2[k-1]	= (					    a*a*a)/ 6;
		poly->fm1[k-1]	= (1 + 3*a 	+ 3*a*a - 3*a*a*a)/ 6;
		 poly->f0[k-1]	= (4 		- 6*a*a + 3*a*a*a)/ 6;
		poly->fp1[k-1]	= (1 - 3*a 	+ 3*a*a -   a*a*a)/ 6;	
	}
}

void POLY::Osc2nd_calc(struct interpol *poly, int xlevel)
{
	int k;
	float a;
	
	poly_init(xlevel);
	for(k=1; k<= xlevel; k++)
	{ 
				   	a	= cpoly[k-1];
		 poly->fm2[k-1]	= (				   - 3*a*a*a+ 5*a*a*a*a - 2*a*a*a*a*a)/ 2;
		 poly->fm1[k-1]	= (      a +   a*a + 9*a*a*a -15*a*a*a*a +6*a*a*a*a*a)/ 2;  //Ostre granie: - 9*a*a*a
		  poly->f0[k-1]	= (2 	   - 2*a*a - 9*a*a*a +15*a*a*a*a -6*a*a*a*a*a)/ 2;	//Ostre granie: + 9*a*a*a
		 poly->fp1[k-1]	= (    - a +   a*a + 3*a*a*a - 5*a*a*a*a +2*a*a*a*a*a)/ 2;	
	}
}

void POLY::Hermite_calc(struct interpol *poly, int xlevel)
{
	int k;
	float a;
	
	poly_init(xlevel);
	for(k=1; k<= xlevel; k++)
	{ 
				a		= cpoly[k-1];
		poly->fm2[k-1]	= (			-	  a*a +   a*a*a)/ 2;
		poly->fm1[k-1]	= (	      a + 	4*a*a - 3*a*a*a)/ 2;
		 poly->f0[k-1]	= (2 	 	- 	5*a*a + 3*a*a*a)/ 2;
		poly->fp1[k-1]	= (	     -a + 	2*a*a -   a*a*a)/ 2;	
	}
}

void	POLY::poly_init(int xlevel)
{
	int 	k;
	float	kf, xlevelf;
	
	xlevelf = (float) xlevel; 
	k=1;
	for(kf=1; kf<xlevel; kf=kf+1) 
	{
		cpoly[k-1]= kf/xlevelf ;
		k=k+1;
	}
		
	cpoly[31]= (float) 0;	
}


void POLY::ValCalc(char POLY_TYPE, float a, float *fm2, float *fm1, float *f0, float *fp1, float *fm3, float *fp2)
{
	float c; 
	
	switch(POLY_TYPE)
	{
		
		case 'G':
			*fm2	= (	 -   a			+   a*a*a)/ 6;
			*fm1	= (    2*a 	+   a*a -   a*a*a)/ 2;
		 	*f0		= (2 -   a	- 2*a*a +   a*a*a)/ 2;
			*fp1	= (   -2*a 	+ 3*a*a -   a*a*a)/ 6;			
		  break;
		  
		case 'H':
			*fm2	= (			-	  a*a +   a*a*a)/ 2;
			*fm1	= (	      a + 	4*a*a - 3*a*a*a)/ 2;
		 	*f0		= (2 	 	- 	5*a*a + 3*a*a*a)/ 2;
			*fp1	= (	     -a + 	2*a*a -   a*a*a)/ 2;			
		  break;
/*		case 'B':
			*fm2	= (					    a*a*a)/ 6;
			*fm1	= (1 + 3*a 	+ 3*a*a - 3*a*a*a)/ 6;
		 	*f0		= (4 		- 6*a*a + 3*a*a*a)/ 6;
			*fp1	= (1 - 3*a 	+ 3*a*a -   a*a*a)/ 6;			
		  break;		  
*/		  
		case 'O':
			*fm2	= (				   - 3*a*a*a+ 5*a*a*a*a - 2*a*a*a*a*a)/ 2;
			*fm1	= (      a +   a*a + 9*a*a*a -15*a*a*a*a +6*a*a*a*a*a)/ 2;  
		 	*f0		= (2 	   - 2*a*a - 9*a*a*a +15*a*a*a*a -6*a*a*a*a*a)/ 2;	
			*fp1	= (    - a +   a*a + 3*a*a*a - 5*a*a*a*a +2*a*a*a*a*a)/ 2;			
		  break;		  		  
		case 'W':		  
            *fm2	= (      -1*a   +1*a*a  )/ 2;   
            *fm1	= (       3*a 	-1*a*a  )/ 2;     
            *f0		= (2     -1*a   -1*a*a  )/ 2; 
            *fp1	= (      -1*a   +1*a*a  )/ 2;			  
		  break;		  		                       
		case 'I':  // acting as Hermite 6p3o		  
           	*fm3	= (                  	+1.0/12*a*a  	-1.0/12*a*a*a); 
          	*fm2	= (        -1.0/12*a   	-1.0/2*a*a   	+7.0/12*a*a*a); 
           	*fm1	= (          2.0/3*a  	+5.0/3*a*a  	-4.0/3*a*a*a);
          	*f0		= ( 1        +0*a     -7.0/3*a*a   	+4.0/3*a*a*a);  
          	*fp1	= (         -2.0/3*a   	+5.0/4*a*a   	-7.0/12*a*a*a);
         	*fp2	= (        1.0/12*a    	-1.0/6*a*a  	+1.0/12*a*a*a);    		  
		  break;		  		                       
		case 'J':	// acting as Hermite 6p5o		  
        	*fm3	= (	0	+	0	*a+	1.0/12	*a*a+	-1.0/24	*a*a*a+	-1.0/12	*a*a*a*a+	1.0/24	*a*a*a*a*a	);
        	*fm2	= (	0 	+	-1.0/12 	*a+	-11.0/24	*a*a+	7.0/24 	*a*a*a+	11.0/24	*a*a*a*a+	-5.0/24	*a*a*a*a*a	); 
         	*fm1	= (	0 	+	2.0/3	*a+	3.0/2	*a*a+	-7.0/12	*a*a*a+	-1 	*a*a*a*a+	5.0/12	*a*a*a*a*a	);
         	*f0		= (	1 	+	0 	*a+	-25.0/12	*a*a+	5.0/12	*a*a*a+	13.0/12	*a*a*a*a+	-5.0/12	*a*a*a*a*a	);  
         	*fp1	= (	0	+	-2.0/3	*a+	13.0/12	*a*a+	-1.0/24	*a*a*a+	-7.0/12	*a*a*a*a+	5.0/24	*a*a*a*a*a	);
        	*fp2	= ( 0 	+	1.0/12  	*a+	-1.0/8	*a*a+	-1.0/24	*a*a*a+	1.0/8	*a*a*a*a+	-1.0/24	*a*a*a*a*a); 		  
		  break;		  		                       		  
	}
}


	
void POLY::poly_ipl(STAGE_PROC *SP, int IPL_RATE, int IPL_TYPE)
{
// @@@@@@  POLY::LOAD_SIZE Needs to be properly set when this func used		
	int j, k, m, OVS_Size;
	float poly; 
	struct interpol *CF; 
	float *Roboczy, *VRob; 
	
	OVS_Size = POLY::LOAD_SIZE;
	
	if (!IPL_TYPE)   //selection between CH (IPL_TYPE from Resampler) and EXT interpolation 
		IPL_TYPE = POLY::EXT_IPL_TYPE;
	CF		= &IPL32[Map_Poly(IPL_TYPE)];
		
	VRob 	= SP->stage_ptr[(SP->stage_cnt+1)%2];
	Roboczy = SP->stage_ptr[SP->stage_cnt]-3;  // One extra preceding sample - the last sample of the previous block
	
	for (j=0; j< 3; j++)
		Roboczy[j]= SP->tail[j];
		
	switch (IPL_RATE)
	{
		case 2:
 			for(k=0, j=1; j<OVS_Size+3 -2; k+=IPL_RATE, j++)
			{
				VRob[k]= 	Roboczy[j+2]* CF->fm2[31]+ Roboczy[j+1]* CF->fm1[31]+ Roboczy[j]* CF->f0[31]+ Roboczy[j-1]* CF->fp1[31];				
				VRob[k+1] = Roboczy[j+2]* CF->fm2[15]+ Roboczy[j+1]* CF->fm1[15]+ Roboczy[j]* CF->f0[15]+ Roboczy[j-1]* CF->fp1[15];				
			}
		 break;
	}
		
	for (j=0; j< 3; j++)
		SP->tail[j] = Roboczy[OVS_Size+j];
}

 //	OUT[k]= IN[kX0+5]* T->fp3[k]+ IN[kX0+4]* T->fp2[k]+ IN[kX0+3]* T->fp1[k]+ IN[kX0+2]* T->f_0[k]+ IN[kX0+1]* T->fm1[k]+ IN[kX0]* T->fm2[k];

void POLY::LIN2x(STAGE_PROC *SP)
{
	int		j, k1; 
	float 	*Roboczy, *VRob; 

	VRob 	= SP->stage_ptr[(SP->stage_cnt+1)%2];
	Roboczy = SP->stage_ptr[SP->stage_cnt]-1;  // One extra preceding sample - the last sample of the previous block
	Roboczy[0] = SP->tail[0];
	for(k1=0, j=0; j< DSPBLOCK_SIZE2; k1++, j+=2)
	{
		VRob[j]= (Roboczy[k1]+Roboczy[k1+1])/2;
		VRob[j+1]= Roboczy[k1+1];
	}
	SP->tail[0] = Roboczy[k1];
	
	++SP->stage_cnt; SP->stage_cnt%=2;
	SP->BufD->PUTf(DSPBLOCK_SIZE2, SP->stage_ptr[SP->stage_cnt], 1);	
}

void POLY::IPL_H6(STAGE_PROC *SP)
{
	int		j, k; 
	float 	*Roboczy, *VRob, *Rob5;
	float 	fm2, fm1, f_0, fp1, fm3, fp2, fp3;

		
	VRob 	= SP->stage_ptr[(SP->stage_cnt+1)%2];
	Roboczy = SP->stage_ptr[SP->stage_cnt]- xTAIL_POLY6;  
	Rob5 = Roboczy+ xTAIL_POLY6;
	
 	for (j=0; j< xTAIL_POLY6; j++)
		Roboczy[j]= SP->tail[j]; 		
			//&R_IPLTab[j][kbase][kSR][klevel].fp2[kY], &R_IPLTab[j][kbase][kSR][klevel].fp1[kY], &R_IPLTab[j][kbase][kSR][klevel].f_0[kY], &R_IPLTab[j][kbase][kSR][klevel].fm1[kY], &R_IPLTab[j][kbase][kSR][klevel].fp3[kY], &R_IPLTab[j][kbase][kSR][klevel].fm2[kY]
	POLY::ValCalc('J', 0.5, &fp2, &fp1, &f_0, &fm1, &fp3, &fm2); 			// fm2, fm1, f0, fp1, fm3, fp2);			
	for(j=0, k=0; k< DSPBLOCK_SIZE2; k+=2, j++)	  //	for(k=0, j=1; j<YSIZE+3 -2; k++, j++)	
	{	
		VRob[k]= Roboczy[j+5]* fp3+ Roboczy[j+4]* fp2+ Roboczy[j+3]* fp1+ Roboczy[j+2]* f_0+ Roboczy[j+1]* fm1+ Roboczy[j]* fm2; 						
		VRob[k+1]= Roboczy[j+3]; 		
	}		
	
 	for (k=0; k< xTAIL_POLY6; k++)
		SP->tail[k]= Rob5[DSPBLOCK_SIZE2/2-xTAIL_POLY6+k]; 					

	++SP->stage_cnt; SP->stage_cnt%=2;
	SP->BufD->PUTf(DSPBLOCK_SIZE2, SP->stage_ptr[SP->stage_cnt], 1);		
}
