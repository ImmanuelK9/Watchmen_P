/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2009-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : FUZZI
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <stdio.h>
#include "driverlib/timer.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include <os.h>
#include <test_rbTree.h>

/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/

#define ONESECONDTICK             7000000

#define TASK1PERIOD                   10
#define TASK2PERIOD                   20


#define WORKLOAD1                     3
#define WORKLOAD2                     2


#define TIMERDIV                      (BSP_CPUClkFreq() / (CPU_INT32U)OSCfg_TickRate_Hz)

#define APP_DEBUG 0

/************************************allocStaticMemWDeadline()*********************************************
 * Description 	:	Use this macro to allocate all needed structures at once
 * Argument(s)	: 	varName		name of the variables, without quotation marks, is usually just the
 * 								function name
 *					stkSize		the desired stack size for this task
 * Note(s)		:	Example usage: allocStaticMemWDeadline(exampleTask, 256);
 *********************************************************************************************************/
#define allocStaticMemWDeadline(varName, stkSize)			\
			static OS_TCB			varName##TCB;			\
			static CPU_STK			varName##Stk[stkSize];	\
			static Node				varName##RecNode;		\
			static OS_NODE_INFO		varName##RecListKey;	\
			static Node				varName##EdfNode;		\
			static OS_NODE_INFO		varName##EdfListKey;	\
			static OS_TCB_TO_NODE	varName##TcbToNode;

/************************************macroOSRecTaskCreate()*********************************************
 * Description 	:	Use this macro to create a periodic task
 * Argument(s)	: 	varName		name of the variables
 * 					name		name of the task, without quotation marks
 *					arg			pointer to arg
 *					prio		
 *					stkSize
 *					stkLimitDivider		stack limit will be calculated to stkSize/stkLimitDivider
 *					extPtr
 *					period
 * Note(s)		:	Example usage: allocStaticMemWDeadline(exampleTask, 256);
 *********************************************************************************************************/
#define macroOSRecTaskCreate(varName, name, arg, prio, stkSize, stkLimitDivider, period)	\
			OSRecTaskCreate((OS_TCB *) &##varName##TCB, (CPU_CHAR *) #varName,						\
				(OS_TASK_PTR) name, (void *) arg, (OS_PRIO) prio, (CPU_STK *) &##varName##Stk[0],	\
				(CPU_STK_SIZE) stkSize / stkLimitDivider, (CPU_STK_SIZE) stkSize,					\
				(OS_MSG_QTY) 0u, (OS_TICK) 0u, (void *) &##varName##TcbToNode,									\
				(OS_OPT)(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),								\
				(OS_ERR *) &err, &##varName##RecNode, &##varName##RecListKey,							\
				&##varName##EdfNode, &##varName##EdfListKey, period)

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_TCB       AppTaskStartTCB;
static  CPU_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  OS_TCB       AppTaskOneTCB;
static  CPU_STK      AppTaskOneStk[APP_TASK_ONE_STK_SIZE];

static  OS_TCB       AppTaskTwoTCB;
static  CPU_STK      AppTaskTwoStk[APP_TASK_TWO_STK_SIZE];

#define  APP_TASK_THREE_STK_SIZE                      128u
allocStaticMemWDeadline(AppTaskThree, APP_TASK_THREE_STK_SIZE);

#define APP_LED_BLINK_STK_SIZE	128u
allocStaticMemWDeadline(APPLED, APP_LED_BLINK_STK_SIZE);

#define APP_MOVE_F_STK_SIZE	128u
allocStaticMemWDeadline(APPMoveF, APP_MOVE_F_STK_SIZE);

#define APP_MOVE_B_STK_SIZE	128u
allocStaticMemWDeadline(APPMoveB, APP_MOVE_B_STK_SIZE);

#define APP_MOVE_L_STK_SIZE	128u
allocStaticMemWDeadline(APPMoveL, APP_MOVE_L_STK_SIZE);

#define APP_MOVE_R_STK_SIZE	128u
allocStaticMemWDeadline(APPMoveR, APP_MOVE_R_STK_SIZE);

CPU_INT32U      iCnt = 0;
CPU_INT08U      Left_tgt;
CPU_INT08U      Right_tgt;
CPU_INT32U      iToken  = 0;
CPU_INT32U      iCounter= 1;
CPU_INT32U      iMove   = 10;
CPU_INT32U      measure=0;

#define		K	20
Node			a;
Node			b;
Node			x[K];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void        AppRobotMotorDriveSensorEnable    ();
        void        IntWheelSensor                    ();
        void        RoboTurn                          (tSide dir, CPU_INT16U seg, CPU_INT16U speed);

static  void        AppTaskStart                 (void  *p_arg);
static  void        AppTaskOne                   (void  *p_arg);
static  void        AppTaskTwo                   (void  *p_arg);
static  void        AppTaskThree                 (void  *p_arg);

static	void		LEDBlink					(void *p_arg);
static	void		moveForward					(void *p_arg);
static	void		moveBackward				(void *p_arg);
static	void		leftTurn					(void *p_arg);
static	void		rightTurn					(void *p_arg);
static 	void		move						(tSide dir);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;

    BSP_IntDisAll();                                            /* Disable all interrupts.                              */
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
    //fprintf(stdout, "%#10X\n", a);
    //testRBTreeScenario0(&a, &b);
	//testRBTreeScenario2(x, K);
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,           /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10u,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) (CPU_INT32U) 0, 
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void  *p_arg)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
    (void)&p_arg;
    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */
    clk_freq = BSP_CPUClkFreq();                                /* Determine SysTick reference freq.                    */
    cnts     = clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
    CPU_TS_TmrFreqSet(clk_freq);
    
    /* Enable Wheel ISR Interrupt */
    AppRobotMotorDriveSensorEnable();
    
    /* Initialise the 2 Main Tasks to  Deleted State */

    //OSTaskCreate((OS_TCB     *)&AppTaskOneTCB, (CPU_CHAR   *)"App Task One", (OS_TASK_PTR ) AppTaskOne, (void       *) 0, (OS_PRIO     ) APP_TASK_ONE_PRIO, (CPU_STK    *)&AppTaskOneStk[0], (CPU_STK_SIZE) APP_TASK_ONE_STK_SIZE / 10u, (CPU_STK_SIZE) APP_TASK_ONE_STK_SIZE, (OS_MSG_QTY  ) 0u, (OS_TICK     ) 0u, (void       *)(CPU_INT32U) 1, (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), (OS_ERR     *)&err);
    //OSTaskCreate((OS_TCB     *)&AppTaskTwoTCB, (CPU_CHAR   *)"App Task Two", (OS_TASK_PTR ) AppTaskTwo, (void       *) 0, (OS_PRIO     ) APP_TASK_TWO_PRIO, (CPU_STK    *)&AppTaskTwoStk[0], (CPU_STK_SIZE) APP_TASK_TWO_STK_SIZE / 10u, (CPU_STK_SIZE) APP_TASK_TWO_STK_SIZE, (OS_MSG_QTY  ) 0u, (OS_TICK     ) 0u, (void       *) (CPU_INT32U) 2, (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), (OS_ERR     *)&err);
	
	//prio is (should be) ignored by the OS from now on, because periodic => OSCfg_EdfSchedPrio
	//macroOSRecTaskCreate(AppTaskThree, AppTaskThree, 0, 4, APP_TASK_THREE_STK_SIZE, 10u, 5000);
	macroOSRecTaskCreate(APPLED, LEDBlink, 0, 4, APP_LED_BLINK_STK_SIZE, 10u, 5000);
	//macroOSRecTaskCreate(APPMoveF, moveForward, 0, 4, APP_MOVE_F_STK_SIZE, 10u, 10000);
	//macroOSRecTaskCreate(APPMoveB, moveBackward, 0, 4, APP_MOVE_B_STK_SIZE, 10u, 17000);
	//macroOSRecTaskCreate(APPMoveL, leftTurn, 0, 4, APP_MOVE_L_STK_SIZE, 10u, 25000);
	//macroOSRecTaskCreate(APPMoveR, rightTurn, 0, 4, APP_MOVE_R_STK_SIZE, 10u, 47000);
    
    /* Delete this task */
    OSTaskDel((OS_TCB *)0, &err);
    
}

static  void  AppTaskOne (void  *p_arg)
{ 
    OS_ERR      err;
    CPU_INT32U  k, i, j;
    
    if(iMove > 0)
    {
      if(iMove%2==0)
      {  
      RoboTurn(FRONT, 16, 50);
      iMove--;
      }
      else{
        RoboTurn(BACK, 16, 50);
        iMove++;
      }
    }
    
    for(k=0; k<WORKLOAD1; k++)
    {
      for(i=0; i <ONESECONDTICK; i++){
        j=2*i;
      }
     }
    
    OSTaskDel((OS_TCB *)0, &err);   

}


static  void  AppTaskTwo (void  *p_arg)
{   
    OS_ERR      err;
    CPU_INT32U  i,k,j=0;
   
    for(i=0; i <(ONESECONDTICK); i++)
    {
      j = ((i * 2) + j);
    }
    
    BSP_LED_Off(0u);
    for(k=0; k<5; k++)
    {
      BSP_LED_Toggle(0u);
      for(i=0; i <ONESECONDTICK/2; i++)
         j = ((i * 2)+j);
    }
    
    BSP_LED_Off(0u);
   OSTaskDel((OS_TCB *)0, &err);

}

static  void  AppTaskThree (void  *p_arg)
{
    OS_ERR      err;
    CPU_INT32U  i,k,j=0;
   
    /*for(i=0; i <(ONESECONDTICK); i++)
    {
      j = ((i * 2) + j);
    }*/
    
    BSP_LED_Off(0u);
    for(k=0; k<3; k++)
    {
      BSP_LED_Toggle(0u);
      for(i=0; i <ONESECONDTICK/2; i++)
         j = ((i * 2)+j);
    }
    
    BSP_LED_Off(0u);
    //OSRecTaskFinish(&AppTaskThreeTCB, &err);
    //fprintf(stdout, "%s", "hallo\n");
    //while(OSTickCtr < 100);
    #if APP_DEBUG
        fprintf(stdout, "%s", "In AppTaskThree\n");
    #endif
    OSTaskDel((OS_TCB *)0, &err);
    //OS tried to delete because there is an "accidental return"
}

static	void	LEDBlink	(void *p_arg){
	OS_ERR      err;
	CPU_INT32U i,j=0;

	BSP_LED_Off(0u);	
	BSP_LED_Toggle(0u);
	//wait for 0,25 sec
    for(i=0; i <ONESECONDTICK/4; i++) j = ((i * 2)+j);
	BSP_LED_Off(0u);
	OSTaskDel((OS_TCB *)0, &err);
}

static	void	moveForward	(void *p_arg){
	move(FRONT);
}

static	void	moveBackward	(void *p_arg){
	move(BACK);
}

static	void	leftTurn	(void *p_arg){
	move(LEFT_SIDE);
}

static	void	rightTurn	(void *p_arg){
	move(RIGHT_SIDE);
}

static 	void	move	(tSide dir){
	OS_ERR      err;
    CPU_INT32U  k, i, j; 

    RoboTurn(dir, 7, 50);

	// copied from AppTaskOne
	// probably used as delay, so that this becomes a blocking operation
    for(k=0; k<WORKLOAD1; k++)
    	for(i=0; i <ONESECONDTICK/2; i++)
        	j=2*i;
    
    OSTaskDel((OS_TCB *)0, &err);
}

static  void  AppRobotMotorDriveSensorEnable ()
{
    BSP_WheelSensorEnable();
    BSP_WheelSensorIntEnable(RIGHT_SIDE, SENSOR_A, (CPU_FNCT_VOID)IntWheelSensor);
    BSP_WheelSensorIntEnable(LEFT_SIDE, SENSOR_A, (CPU_FNCT_VOID)IntWheelSensor);
}


void IntWheelSensor()
{
	CPU_INT32U         ulStatusR_A;
	CPU_INT32U         ulStatusL_A;

	static CPU_INT08U CountL = 0;
	static CPU_INT08U CountR = 0;

	static CPU_INT08U data = 0;

	ulStatusR_A = GPIOPinIntStatus(RIGHT_IR_SENSOR_A_PORT, DEF_TRUE);
	ulStatusL_A = GPIOPinIntStatus(LEFT_IR_SENSOR_A_PORT, DEF_TRUE);

        if (ulStatusR_A & RIGHT_IR_SENSOR_A_PIN)
        {
          GPIOPinIntClear(RIGHT_IR_SENSOR_A_PORT, RIGHT_IR_SENSOR_A_PIN);           /* Clear interrupt.*/
          CountR = CountR + 1;
        }

        if (ulStatusL_A & LEFT_IR_SENSOR_A_PIN)
        {
          GPIOPinIntClear(LEFT_IR_SENSOR_A_PORT, LEFT_IR_SENSOR_A_PIN);
          CountL = CountL + 1;
        }

	if((CountL >= Left_tgt) && (CountR >= Right_tgt))
        {
          data = 0x11;
          Left_tgt = 0;
          Right_tgt = 0;
          CountL = 0;
          CountR = 0;
          BSP_MotorStop(LEFT_SIDE);
          BSP_MotorStop(RIGHT_SIDE);
        }
        else if(CountL >= Left_tgt)
        {
          data = 0x10;
          Left_tgt = 0;
          CountL = 0;
          BSP_MotorStop(LEFT_SIDE);
        }
        else if(CountR >= Right_tgt)
        {
          data = 0x01;
          Right_tgt = 0;
          CountR = 0;
          BSP_MotorStop(RIGHT_SIDE);
        }
        return;
}

void RoboTurn(tSide dir, CPU_INT16U seg, CPU_INT16U speed)
{
	Left_tgt = seg;
        Right_tgt = seg;

	BSP_MotorStop(LEFT_SIDE);
	BSP_MotorStop(RIGHT_SIDE);

        BSP_MotorSpeed(LEFT_SIDE, speed <<8u);
	BSP_MotorSpeed(RIGHT_SIDE,speed <<8u);

	switch(dir)
	{
            case FRONT :
                    BSP_MotorDir(RIGHT_SIDE,FORWARD);
                    BSP_MotorDir(LEFT_SIDE,FORWARD);
                    BSP_MotorRun(LEFT_SIDE);
                    BSP_MotorRun(RIGHT_SIDE);
                    break;
                    
            case BACK :
                    BSP_MotorDir(LEFT_SIDE,REVERSE);
                    BSP_MotorDir(RIGHT_SIDE,REVERSE);
                    BSP_MotorRun(RIGHT_SIDE);
                    BSP_MotorRun(LEFT_SIDE);
                    break;
                    
            case LEFT_SIDE :
                    BSP_MotorDir(RIGHT_SIDE,FORWARD);
                    BSP_MotorDir(LEFT_SIDE,REVERSE);
                    BSP_MotorRun(LEFT_SIDE);
                    BSP_MotorRun(RIGHT_SIDE);
                    break;
                    
            case RIGHT_SIDE:
                    BSP_MotorDir(LEFT_SIDE,FORWARD);
                    BSP_MotorDir(RIGHT_SIDE,REVERSE);
                    BSP_MotorRun(RIGHT_SIDE);
                    BSP_MotorRun(LEFT_SIDE);
                    break;
                    
            default:
                    BSP_MotorStop(LEFT_SIDE);
                    BSP_MotorStop(RIGHT_SIDE);
                    break;
	}

	return;
}
