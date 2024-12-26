#ifndef 	PACK_MAN_H
#define		PACK_MAN_H

class PACK_MAN // : public BUF_MANAGER
{
 public:  //Attributes	
  BUF_MANAGER		*YS[MAX_YBUFS_PER_I2SLINE];
 	int				YBUFS_QTY;
 	int				YS_first;
 	int				YSTEP;
 
 	int				k_PUT; 
 	int				k_PUTBUFS;		
 	int				k_GET; 
 	int				k_GETBUFS;

 	int				k_GET_DMA; 
 	int				k_GETBUFS_DMA;
 	int				BUF_SIZE; 
 	int				MINDIST_YDSP;
 	 	
 	int 			OUT_SMPQTY;
 	int				ZEROS; 
		
static 	int			YSPACE_OK;

 public:  //  @ Methods @
//	void			Init(BUF_MANAGER *YS0, BUF_MANAGER *YS1, BUF_MANAGER	*YS2, BUF_MANAGER *YS3, int STARTDIST_PUTGET ,  int pMINDIST_YDSP,  int TCB_BlockSize, int zeros);
//	void			Init(BUF_MANAGER *YS0, BUF_MANAGER *YS1, int TCB_BlockSize, int pZeros, BUF_MANAGER	*YS2, BUF_MANAGER *YS3); 
	void			Init(int TCB_BlockSize, int pZeros); 
	
// Ekrem to call
	void 			GETX_DMA(int DATA_QTY, int *I2Sbuf);	
 	void 			GETX_SPDIF(int *I2Sbuf, int SPACE_CHECK);  //BUF_MANAGER *YS[4])
 	void 			GETX(int *I2Sbuf);
 	void 			GETX_SPDIF24(int *SPDIFbuf, int SPACE_CHECK); 	
 private:
 	void			(*pGETX)(int *I2Sbuf);
	void			Control_YSPACE(void);	
	void 			Calc_YS_PUT(void); 	 	
	void 			Update_YS_GET(int cGET);
	int 			RPIGET_OK(void); 
	void			varInit(void);
	
}; 		

#endif

