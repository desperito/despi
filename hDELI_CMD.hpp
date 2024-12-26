#ifndef DELI_CMD_H
#define DELI_CMD_H

//*****************************************	
//*****************************************	



class DELI_CMD
{
public:
  //= Data members	

	static 	int 			MSG[DELI_FRAMESIZE+1];
	static 	unsigned int	frame_no, cRXQTY, tRXQTY, FIRQTY[FPGAFIR_QTY], tFIRQTY[FPGAFIR_QTY], LED[4];
	static	long long		FIR_SUM;					
	
  //= USER Methods Public interface		
  	static void 			x55RX(int *msg);  	
  	static void 			Proc_SR(int *msg);
  	static void 			Send(int cmd);  
  	static void 			LED_Color(int LED_NO, int color_rgb);
  	static void 			Send_FIR(long long (*FIR), int mode, int FIR_SIZE, int code);
  	static void 			Send_EP(int mode);
  	static void 			Send_MAL(void);
  	static void 			Send_VOL(int *msg);  	
  	
private:
  	static int 				CRC_calc(int *MSG);  
  	static unsigned int 	CalcCRC32(unsigned int crc, char *buffer, unsigned int length);
  	static int 				Setup_ACKFIR(int FIR_NO);  
  	static int 				Check_ACKFIR(void); 
  	static void 			SEND_EXEC(void);  
  	
  	
};
#endif
