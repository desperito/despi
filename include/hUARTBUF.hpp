#ifndef UARTBUF_H
#define UARTBUF_H

//*****************************************	
//*****************************************	
#define 	UARTMSG_STD_SIZE			 DELI_FRAMESIZE

class UARTBUF
{
public:
  //= Data members	
	int				msg[UARTMSG_STD_SIZE];  	
	int				msg_len;	
	int				chr_left;		
	int				(*Buf)[21]; 						
	int				wline;
	int				rline;	
	int				BUFSIZE;		
static 	UARTBUF 	* PTRS[2];

	
  //= USER Methods Public interface		
static	void 		Constructor(void);  
  	void			Init(int pBUF_SIZE); 
  	void			PUT_CHAR(char uart_char);	  	
  	void			PUT_MSG(int *msg, int LEN);	
static  	void			UART_Monitor_NEW(void);  	
void			UART_Monitor_NEWx(void);  	

private:
	  	
  	  		
};
#endif
