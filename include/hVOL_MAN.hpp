#ifndef VOL_MAN_H
#define VOL_MAN_H

class VOL_MAN // : public BUF_MANAGER
{
 public:  //Attributes
static	int			LSEG, RSEG;
		
 public:  //Methods
static 	void		Constructor(void);
static 	void 		VOL_Stages_Setup(void);
static	void		Proc_Peaks(BUF_MANAGER *B, float *ROB, int stage_size); 	
static	int 		Peak_Correction1(float *ROB, int stage_size);
static	int 		Peak_Correction2(float *ROB, int stage_size);
private:
static	void		VOL_TYPE(FILTER	*F, int Stage_Qty);
//	void 			VOL_MCB(STAGE_PROC *SP, int kStage);
//	void 			VOL_MCMT(STAGE_PROC *SP, int kStage); 	
};

void 	VOL_MCB(STAGE_PROC *SP, int kStage);
void 	VOL_MCMT(STAGE_PROC *SP, int kStage);
void 	VOL_SCALER(STAGE_PROC *SP, int kStage);
void 	VOL_LIMITER(STAGE_PROC *SP, int kStage);
void 	VOL_LIMSCALER(STAGE_PROC *SP, int kStage);
		
#endif


