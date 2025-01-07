#ifndef SIMSDRAM_H
#define SIMSDRAM_H

class SIM_SDRAM	{
//********************************************************************************	
public:
  //= Data members
	static		int		CH_MIX;
		
	static		int		D_LEN; 
	static		int		E_LEN;
	static		int		I_LEN;
	
  //= Methods		
	static void			MDump(float *BUF, int data_type, int dump_size, int bufsim_mask);
//********************************************************************************
private:		
  //= Data members
	static	char		fname[FILENAME_LEN];
	static	int	 		kname;
	static	int	 		js, ks; // generic purpose indices 	
  //= PRIVATE Methods				
	static	void 		MemDump(float *BUF_FDATA, int *BUF_IDATA,int data_type, int lenfull, int max_level, char *fname);
	static	void 		Dump_BUF_CHN_NamePrep(int bufname, int channel);
	static  int 		fname_DSPSTRING(void);
	static  void 		fname_MODEC(int channel);			
	static  void		fname_BUF_CHN(int jbuf, int kch, char buftype_code1, char buftype_code2) ; 

	static	 void 		Restart_Playback(void);
//	static	  void 		Flags_Reset(void); 						
	
  //= REFACTOR - this should be a service 	of the UART class							
	static	   void 	Send1Byte_U1(int byte2send);    						
};

#endif
