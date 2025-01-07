#include 	"tt.h"
#if (SOFT_V & SHARC_HW)
 #include 	<21369.h>
 #include 	<Cdef21369.h>
 #include 	<def21369.h>
 #define  	SRUDEBUG  // Check SRU Routings for errors.
 #include 	<SRU.h>
#endif


#include 	<time.h> 
#include 	<stdio.h>

#include 	<string.h>

#define			LED_RED_ON				{SRU(HIGH,DPI_PB13_I);}
#define			LED_RED_OFF				{SRU(LOW,DPI_PB13_I);}
#define			LED_BLUE_ON				{SRU(HIGH,DPI_PB06_I);}
#define			LED_BLUE_OFF			{SRU(LOW,DPI_PB06_I);}
#define			LED_GREEN_ON			{SRU(HIGH,DPI_PB07_I);}
#define			LED_GREEN_OFF			{SRU(LOW,DPI_PB07_I);}

#define         LED_YELLOW_ON          	{SRU(HIGH,DAI_PB05_I);}
#define        	LED_YELLOW_OFF        	{SRU(LOW,DPAI_PB05_I);}
#define        	LED_WHITE_ON          	{SRU(HIGH,DAI_PB08_I);}
#define        	LED_WHITE_OFF         	{SRU(LOW,DAI_PB08_I);}


//SECTION(("seg_sdram3")) 	char				dbgtab[DBGTAB_SIZE];	
clock_t  	clock1; 

//= The only function used by both SHARC HW and LINUX HW, the other below are only used with SHARC_HW 
 EXTERNC void	Print_Debug(int n, int format)
 {
  	int 		i; //, cPlaySession_DEBUG_MODE;
    double 		secs; 
    char		tmp_str[MODEC_FULLNAME_LEN];
    long long	clock0;
  
	
/*
//	if ( TIME_INFO )
//	clock1 = clock();

//	{
//		for(i=0; i<n; i++)
//		{
//			tmp_str[i]= dbgtab[i];
//		}
//		tmp_str[n]=0;	
//		secs=  (double) clock1  / CLOCKS_PER_SEC; 
		//sprintf(tmp_str,  "", ) ;
		
//		switch ( format )
//		{
//			case NOTIME:		// Sam napis bez Time i Newline
//				sprintf(dbgtab, "%s", tmp_str) ;
//			  break;			  
*/
/*			
			case TIME:			// Standardowy format Time, bez Newlines
				sprintf(dbgtab, "[%.4f]%s", secs, tmp_str) ;		
			  break;	
			case TIME_NL:		// Time + Newline 
				sprintf(dbgtab, "[%.4f]%s\n\r", secs, tmp_str) ;
			  break;
			case NL_TIME:		// Newline + Time 
				sprintf(dbgtab, "\n\r[%.4f]%s", secs, tmp_str) ;
			  break;	
			case NL_TIME_NL:		// Newline + Time + Newline	
				sprintf(dbgtab, "\n\r[%.4f]%s\n\n\r", secs, tmp_str) ;
			  break;
			case NOTIME_NL:		// Sam napis bez Time + Newline
				sprintf(dbgtab, "%s\n\r", tmp_str) ;
			  break;
			case NL_NOTIME:		// Newline + Sam napis bez Time
				sprintf(dbgtab, "\n\r%s", tmp_str) ;
			  break;
			case NL_NOTIME_NL:	// Newline + Sam napis bez Time + Newline	
				sprintf(dbgtab, "\n\r%s\n\r", tmp_str) ;
			  break;
			  
		}
*/			
//	cPlaySession_DEBUG_MODE = PlaySession_DEBUG_MODE();
	switch (PlaySession_DEBUG_MODE())
	{
		case UART_DBG:	
 #if	(SOFT_V & SHARC_HW)			
			Debug_Send(n, dbgtab);
 #endif						
			break;
		case UART_PC_PRINTF:	
			printf(dbgtab);		
 #if	(SOFT_V & SHARC_HW)			
			trace(dbgtab, 0);
 #endif			
		  break;	
		case UART_PC:	
			//Smok_Reply(n, dbgtab);
 #if	(SOFT_V & SHARC_HW)			
			trace(dbgtab, 0);
 #endif			
			//Send_Debug_ISR(dbgtab,n);
			break;			
		case PRINTF_DBG:
			printf(dbgtab);
			break;			
		case UART_PRINTF_DBG:
			Debug_Send(n, dbgtab);		
			printf(dbgtab);
			break;			
		case NO_DBG:
			break;
	}	
	for (i=0; i<sizeof(dbgtab); i++)
		dbgtab[i] =0;

}



//=======================================
// SHARC_HW functions only till the end of file
//=======================================


#if (SOFT_V & SHARC_HW)


 EXTERNC  void FlagLED_Setup(void)
 {
  //= LED-y sygnalizacyjne	
 	SRU(HIGH, 	DPI_PBEN06_I);			
	SRU(HIGH, 	DPI_PBEN07_I);		
	SRU(HIGH, 	DPI_PBEN13_I);
	SRU(HIGH,	PBEN05_I);
	SRU(HIGH,	PBEN08_I);	
	LED_init();	
 }

  //= ZASADY SYGNALIZACJI LED:
  //  1) Krotkie migniecie wszsytkimi
  //  2) Migniecie sama niebieska tyle razy ile glowana wersja
  //  3-DUARK) Migniecie BLUE + RED tyle razy jaka jest podwersja   
  //  3-SPDIF) Migniecie BLUE + RED + GREEN tyle razy jaka jest podwersja   

 EXTERNC  void Version_LED(void)
 {    
  #if 	( (WORK_CONFIG &  0xF0) == PROD_MODE )
    LED_SET(0, 0, 0,   0);  // version 4 - sama zielona    
    
    LED_blink( NO_COLOR,   NO_COLOR, 	NO_COLOR, 		1, 			100, 100, 100);                    
    
    LED_blink( BLUE_1,     NO_COLOR, 	NO_COLOR, 	VERSION_LED, 	300, 150, 150);            
    LED_blink( BLUE_1,     GREEN_2,  	RED_4, 		SUBVERSION_LED, 300, 300, 300);
  #endif    
 }			
 
 

	unsigned int		*sd;
	#define			xsdram2_END			0x08050000 //0x08028001			

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 EXTERNC void  SDRAM_clear(void)
 {
	int  k, m, a;
	
	sd = (unsigned int *) xsdram2_END; //REC_TEST3; 
	for (k=0;k<0x83FFFFF-xsdram2_END; k++)
		sd[k] = 0;	
 }

 EXTERNC void  Core_SETUP_SDRAM_clear(void)
 {
	
   	InitPLL_SDRAM();
	asm("bit clr MODE1 RND32;");
//	asm("bit clr MODE1 TRUNCATE;");	
	SDRAM_clear();	
 }

 
 
 EXTERNC  void Delay_ms(int ms)
 {
 	int i;
 	
 	i= ms* 3E+4;
    for (;i>0;--i)
        asm ("nop;") ;
 }

 void LED(int LED_color, int action)
 {
	switch (LED_color)
	{
		case BLUE_1:
			switch(action)
			{
				case SWITCH:
					if (*pDPI_PIN_STAT  & (1<<(LED_color-1)))
					{ SRU(LOW, 	DPI_PB06_I); }
					else 
					{SRU(HIGH, 	DPI_PB06_I) }		
					break;
				case ON:
					{	SRU(HIGH, 	DPI_PB06_I)}						
								
					break;
				case OFF:
					{	SRU(LOW, 	DPI_PB06_I)}		
					break;
			}			
			break;		
			
		case GREEN_2:				//Codec SW3-3 ustawic na OFF !!!
			switch(action)
			{
				case SWITCH:
					if (*pDPI_PIN_STAT  & (1<<(LED_color-1)))
					{ 	SRU(LOW, 	DPI_PB07_I);}
					else 
					{ 	SRU(HIGH, 	DPI_PB07_I);}		
					break;
				case ON:
					{ 	SRU(HIGH, 	DPI_PB07_I);}			
					break;
				case OFF:
					{ 	SRU(LOW, 	DPI_PB07_I);}					
					break;
			}			
			break;
			
		case RED_4:
			switch(action)
			{
				case SWITCH:
					if (*pDPI_PIN_STAT  & (1<<(LED_color-1)))
					{ 	SRU(LOW, 	DPI_PB13_I); }
					else 
					{ 	SRU(HIGH, 	DPI_PB13_I); }					
					break;
				case ON:
					{ 	SRU(HIGH, 	DPI_PB13_I); }
					break;
				case OFF:
					{ 	SRU(LOW, 	DPI_PB13_I); }		
					break;
			}			
			break;
	}
 }			
 

 EXTERNC void LED_blink(int color1,int color2, int color3,int n, int len, int prewrap, int postwrap)
 {
	int i, idelay;
	
	for(i=0; i< n; i++)
	{
		Delay_ms(prewrap);	
		
		if(color1) 
			LED(color1, SWITCH);
		if(color2) 
			LED(color2, SWITCH);
		if(color3) 
			LED(color3, SWITCH);
		Delay_ms(len);
		if(color1) 
			LED(color1, SWITCH);
		if(color2) 
			LED(color2, SWITCH);
		if(color3) 
			LED(color3, SWITCH);
		Delay_ms(len);
					
		Delay_ms(postwrap);
	}
 }


 EXTERNC	void LED_SET(int red, int blue, int green,   int duration_ms)
	{
		LED_RED_OFF; LED_BLUE_OFF; LED_GREEN_OFF;
		if (red) 
			LED_RED_ON;
		  // bit zero version
		if (blue) 
			LED_BLUE_ON;  // bit jeden version
		if (green) 
			LED_GREEN_ON;  // bit dwa version
		
		Delay_ms(duration_ms); 
	}
		 
		
 void  LED_init(void)
 {
	
 #ifdef	SHARC_QUATTRO
	SRU(HIGH, 	DPI_PBEN06_I);
	SRU(HIGH, 	DPI_PBEN07_I);   
	SRU(HIGH, 	DPI_PBEN13_I);      
 #endif	
 }

#endif

