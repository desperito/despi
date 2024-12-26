#ifndef DSP_MCEP_H
#define DSP_MCEP_H


class MCEP
{
 public:	
  //= Data members
  static	int			SpkNum; 
  static   unsigned int	FlagsDSP;   
  static	char		IPL_TAB[5];   
  
 //= Methods    	
  static void 			Init(void); 
  static void			Config(void);
  static void 			Index(char *cSPK); 
  static void 			Stage_Switches(void);  

 private:
};
#endif 
