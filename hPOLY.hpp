#ifndef POLY_H
#define POLY_H

struct	interpol	
{
	float	fm3[32];	
	float	fm2[32];
	float	fm1[32];
	float	f0[32];
	float	fp1[32];	
	float	fp2[32];		
};
extern 	struct interpol 	 	BS, Lag, He, Osc;


struct 	IPL_map_struct
{
	char			Code;
struct interpol		*Poly;
};

class POLY	{
public:    //= Data members

static struct interpol	*CF;
static	int				LOAD_SIZE;			
static	int				EXT_IPL_TYPE;			
static	int				OVS_EXT;
static	struct 	IPL_map_struct	IPL_codes[IPL_QTY+1];
		
//*****************************************	

 public:	// Methods
static void 			Constructor(void);
static int 				Map_Poly(char  pCode);
static void 			ValCalc(char POLY_TYPE, float a, float *fm2, float *fm1, float *f0, float *fp1, float *fm3, float *fp2);
static	void 			LIN2x(STAGE_PROC *SP);
static	void 			IPL_H6(STAGE_PROC *SP);

 private:  // Internal calc methods	
static	void			poly_init(int xlevel);
static	void 			Linear_calc(struct interpol *poly, int xlevel);
static	void 			Lagrange_calc(struct interpol *poly, int xlevel);
static	void 			BS_calc(struct interpol *poly, int xlevel);
static	void 			Osc2nd_calc(struct interpol *poly, int xlevel);
static	void 			Hermite_calc(struct interpol *poly, int xlevel);
static	void 			poly_ipl(STAGE_PROC *SP, int IPL_RATE, int IPL_TYPE);

};

#endif
