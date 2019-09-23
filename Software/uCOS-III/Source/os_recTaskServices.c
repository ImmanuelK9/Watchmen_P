/*********************************************************************************************************
*                                                uC/OS-III
*                                        The Real-Time Kernel
*
*                                      RECURSIVE TASK MANAGEMENT
*
* Filename		: os_recTaskServices.c
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : 
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include <os.h>
#include <lib_tree.h>
#include <stdio.h>
#include <os_edf.h>

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/
CPU_STK      *OSRecTaskStkInit          (OS_TASK_PTR            p_task,
                                         void                  *p_arg,
                                         CPU_STK               *p_stk_base,
                                         CPU_STK               *p_stk_limit,
                                         CPU_STK_SIZE           stk_size,
                                         OS_OPT                 opt);

void          OSTaskCreateMod           (OS_TCB                *p_tcb,
                                         CPU_CHAR              *p_name,
                                         OS_TASK_PTR            p_task,
                                         void                  *p_arg,
                                         OS_PRIO                prio,
                                         CPU_STK               *p_stk_base,
                                         CPU_STK_SIZE           stk_limit,
                                         CPU_STK_SIZE           stk_size,
                                         OS_MSG_QTY             q_size,
                                         OS_TICK                time_quanta,
                                         void                  *p_ext,
                                         OS_OPT                 opt,
                                         OS_ERR                *p_err);

void		OSRecTaskFinishHelp(void);


/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/********************************************************************************************************
 *                                                    CREATE A PERIODIC TASK
 *
 * Description	: Similar to OSTaskCreate, but sets the extension pointer to point to the period
 *
 * Arguments	: 	...            	see OSTaskCreate
 *                	//additional data for recursion
 * 					p_recListNode		pointer to Node that is used for the RecList
 * 				  	p_recListKey		pointer to key datastructure that is used as an info in the node
 * 					p_edfRdyListNode	pointer to Node that is used for the EdfRdyList
 * 					p_edfRdyListKey		pointer to key datastructure that is used as an info in the node
 * 				  	period				periodicity of this task
 *
 *
 * Returns		: A pointer to the TCB of the task created.  This pointer must be used as an ID (i.e handle) to the task.
 * Notes		: TODO add functionality to schedule together
***********************************************************************************************************/
void OSRecTaskCreate 	(OS_TCB                *p_tcb,
                         CPU_CHAR              *p_name,
                         OS_TASK_PTR            p_task,
                         void                  *p_arg,
	                     OS_PRIO                prio,
                         CPU_STK               *p_stk_base,
                    	 CPU_STK_SIZE           stk_limit,
                         CPU_STK_SIZE           stk_size,
                         OS_MSG_QTY             q_size,
                         OS_TICK                time_quanta,
                         void                  *p_ext,
                         OS_OPT                 opt,
                         OS_ERR                *p_err,
						 //additional data for recursion
						 Node				   *p_recListNode,
						 OS_NODE_INFO		   *p_recListKey,
						 Node				   *p_edfRdyListNode,
						 OS_NODE_INFO		   *p_edfRdyListKey,
						 CPU_INT32U				period	 			 
						 ){
	//TODO
	//create task similar to OSTaskCreate?
	p_recListKey->period=period;
	p_recListKey->tcbPtr=p_tcb;
	//should be changed later when schedule together functionality is implemented
	p_recListKey->TickCtrMatch = OSTickCtr + period;
	p_recListKey->recNode = p_recListNode;
	p_recListKey->edfNode = p_edfRdyListNode;

	p_edfRdyListKey->tcbPtr=p_tcb;
	p_edfRdyListKey->recNode = p_recListNode;
	p_edfRdyListKey->edfNode = p_edfRdyListNode;

	p_recListNode->info=p_recListKey;
	p_recListNode->key = p_recListNode->info->TickCtrMatch;
	p_recListNode->tree = RECURSIONTREE;

	OSRecList = insert(OSRecList, p_recListNode);

	OSTaskCreateMod (	p_tcb,
                    p_name,
                    p_task,
                  	p_arg,
                    prio,
                    p_stk_base,
                    stk_limit,
                    stk_size,
                    q_size,
                    time_quanta,
                    p_ext,
                    opt,
                    p_err);
}

/*******************************************OSRecTaskFinish()*********************************************
 * Description 	:	A recursive task should call this function when it finishes (at least indirectly)
 * Argument(s)	: 	p_tcb      is the TCB of the tack to delete
 *					p_err      is a pointer to an error code returned by this function:
 *                             OS_ERR_NONE                  if the call is successful
 * Note(s)		:	NOT IN USE
 *********************************************************************************************************/
void OSRecTaskFinish (OS_TCB *p_tcb1, OS_ERR *p_err){
	//reset stack: reset StkPtr, clear stack
	//copied from OSTaskCreate
	CPU_SR_ALLOC();
	OS_TCB *p_tcb = p_tcb1;
	if (p_tcb == (OS_TCB *)0) {                             /* Finish 'Self'?                                         */
        CPU_CRITICAL_ENTER();
        p_tcb  = OSTCBCurPtr;                               /* Yes.                                                   */
        CPU_CRITICAL_EXIT();
    }
	OS_CRITICAL_ENTER();
	OS_OPT 		opt = p_tcb->Opt;
	CPU_STK 	*p_sp = p_tcb->StkPtr;
	CPU_STK 	*p_stk_base = p_tcb->StkBasePtr;
	CPU_STK_SIZE	stk_size = p_tcb->StkSize;
	CPU_STK_SIZE	i;

	OS_RdyListRemove(p_tcb);
															/* --------------- CLEAR THE TASK'S STACK --------------- */
	if ((opt & OS_OPT_TASK_STK_CHK) != (OS_OPT)0) {         /* See if stack checking has been enabled                 */
		if ((opt & OS_OPT_TASK_STK_CLR) != (OS_OPT)0) {     /* See if stack needs to be cleared                       */
			p_sp = p_stk_base;
			for (i = 0u; i < stk_size; i++) {               /* Stack grows from HIGH to LOW memory                    */
				*p_sp = (CPU_STK)0;                         /* Clear from bottom of stack and up!                     */
				p_sp++;
			}
		}
	}
	
	p_sp = OSRecTaskStkInit(	p_tcb->TaskEntryAddr,     /* Initialize the stack, i.e. init the CPU regs */
								p_tcb->TaskEntryArg,         
								p_tcb->StkBasePtr,
								p_tcb->StkLimitPtr,
								p_tcb->StkSize,
								p_tcb->Opt);
	
	//CPU_CRITICAL_ENTER();
	p_tcb  = OSTCBCurPtr;	
	//CPU_CRITICAL_EXIT();

	p_tcb->StkPtr        = p_sp;                            /* set the new sp */

	//stack should be successfully reset now
	//TODO
	//what to do from here?? ----------------------------
	p_tcb->TaskState = (OS_STATE)OS_TASK_STATE_DEL;
	#if APP_DEBUG
		fprintf(stdout, "%s", "In OSTaskRecFinish\n");
	#endif
	OS_CRITICAL_EXIT_NO_SCHED();
	OSSched();

	*p_err = OS_ERR_NONE;
}


/****************************************OSRecTaskListUpdate()*********************************************
 * Description 	:	This should be run at every tick
 * Note(s)		:	orient at os_tick-OS - OS_TickListUpdate, especially at case OS_TASK_STATE_DLY
 *********************************************************************************************************/
void OSRecTaskListUpdate (void){
	//TODO critical section??? maybe borrow from os_tick-OS_TickListUpdate?
	Node* p_min = findMin(OSRecList);

	//use >= for safety, if intr missed
	while(p_min!=0 && OSTickCtr >= p_min->info->TickCtrMatch){	/* Process each TCB that expires               */

		//Step 1 Make task ready to run
		
		p_min->info->tcbPtr->TaskState = OS_TASK_STATE_RDY;
		
		//orientate at OSTaskCreate, many (unused) parts got left out for better overview
		//=====================
			CPU_STK_SIZE   i;
			CPU_STK       *p_sp;
			CPU_STK       *p_stk_limit;
			CPU_SR_ALLOC();

			OS_TCB        *p_tcb=p_min->info->tcbPtr;
			CPU_CHAR      *p_name=p_tcb->NamePtr;
			OS_TASK_PTR    p_task=p_tcb->TaskEntryAddr;
			void          *p_arg=p_tcb->TaskEntryArg;
			OS_PRIO        prio=p_tcb->Prio;
			CPU_STK       *p_stk_base=p_tcb->StkBasePtr;
			CPU_STK_SIZE   stk_limit=(CPU_STK_SIZE) 128u / 10u;
			CPU_STK_SIZE   stk_size=p_tcb->StkSize;
			OS_MSG_QTY     q_size=0;
			OS_TICK        time_quanta=p_tcb->TimeQuanta;
			void          *p_ext=p_tcb->ExtPtr;
			OS_OPT         opt=p_tcb->Opt;
			OS_TaskInitTCB(p_tcb);                                  /* Initialize the TCB to default values                   */
																	/* --------------- CLEAR THE TASK'S STACK --------------- */
			if ((opt & OS_OPT_TASK_STK_CHK) != (OS_OPT)0) {         /* See if stack checking has been enabled                 */
				if ((opt & OS_OPT_TASK_STK_CLR) != (OS_OPT)0) {     /* See if stack needs to be cleared                       */
					p_sp = p_stk_base;
					for (i = 0u; i < stk_size; i++) {               /* Stack grows from HIGH to LOW memory                    */
						*p_sp = (CPU_STK)0;                         /* Clear from bottom of stack and up!                     */
						p_sp++;
					}
				}
			}
																		/* ------- INITIALIZE THE STACK FRAME OF THE TASK ------- */
			#if (CPU_CFG_STK_GROWTH == CPU_STK_GROWTH_HI_TO_LO)
				p_stk_limit = p_stk_base + stk_limit;
			#else
				p_stk_limit = p_stk_base + (stk_size - 1u) - stk_limit;
			#endif
			//different
			p_sp = OSRecTaskStkInit(p_task,
								p_arg,
								p_stk_base,
								p_stk_limit,
								stk_size,
								opt);

																	/* -------------- INITIALIZE THE TCB FIELDS ------------- */
			p_tcb->TaskEntryAddr = p_task;                          /* Save task entry point address                          */
			p_tcb->TaskEntryArg  = p_arg;                           /* Save task entry argument                               */

			p_tcb->NamePtr       = p_name;                          /* Save task name                                         */

			p_tcb->Prio          = prio;                            /* Save the task's priority                               */

			p_tcb->StkPtr        = p_sp;                            /* Save the new top-of-stack pointer                      */
			p_tcb->StkLimitPtr   = p_stk_limit;                     /* Save the stack limit pointer                           */

			p_tcb->TimeQuanta    = time_quanta;                     /* Save the #ticks for time slice (0 means not sliced)    */
		
			p_tcb->ExtPtr        = p_ext;                           /* Save pointer to TCB extension                          */
			p_tcb->StkBasePtr    = p_stk_base;                      /* Save pointer to the base address of the stack          */
			p_tcb->StkSize       = stk_size;                        /* Save the stack size (in number of CPU_STK elements)    */
			p_tcb->Opt           = opt;                             /* Save task options                                      */


			#if OS_CFG_TASK_Q_EN > 0u
				OS_MsgQInit(&p_tcb->MsgQ,                               /* Initialize the task's message queue                    */
							q_size);
			#endif

				OSTaskCreateHook(p_tcb);                                /* Call user defined hook                                 */
																		/* --------------- ADD TASK TO READY LIST --------------- */
				OS_CRITICAL_ENTER();
				OS_PrioInsert(p_tcb->Prio);
				OS_RdyListInsertTail(p_tcb);
				
				//substitute in order to use edf scheduler
				//OS_EdfRdyListInsert(p_min);

			#if OS_CFG_DBG_EN > 0u
				OS_TaskDbgListAdd(p_tcb);
			#endif

			OSTaskQty++;                                            /* Increment the #tasks counter                           */

			if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Return if multitasking has not started                 */
				OS_CRITICAL_EXIT();
				return;
			}

			OS_CRITICAL_EXIT_NO_SCHED();
		//=============

		//Step 2 Manage the RecList
		//delete old entry and add next entry in RecList
		OSRecList = deleteNode(p_min);
		p_min->info->TickCtrMatch = OSTickCtr + p_min->info->period;
		p_min->key = p_min->info->TickCtrMatch;
		OSRecList = insert(OSRecList, p_min);

		p_min = findMin(OSRecList);
	}
}

/*********************************************LOCAL FUNCTIONS********************************************/

/********************************************************************************************************
 *                               INITIALIZE A PERIODIC TASK'S STACK
 *
 * Description: This function is called by ... to initialize the stack
 *              frame of the task being created. This function is highly processor specific.
 *
 * Arguments  : see OSTaskStkInit
 *
 * Returns    : Always returns the location of the new top-of-stack' once the processor registers have
 *              been placed on the stack in the proper order.
 **********************************************************************************************************/
CPU_STK  *OSRecTaskStkInit (OS_TASK_PTR    p_task,
							void          *p_arg,
							CPU_STK       *p_stk_base,
							CPU_STK       *p_stk_limit,
							CPU_STK_SIZE   stk_size,
							OS_OPT         opt){
    CPU_STK  *p_stk;


    (void)opt;                                              /* Prevent compiler warning                               */

    p_stk = &p_stk_base[stk_size];                          /* Load stack pointer                                     */
                                                            /* Registers stacked as if auto-saved on exception        */
    *--p_stk = (CPU_STK)0x01000000u;                        /* xPSR                                                   */
    *--p_stk = (CPU_STK)p_task;                             /* Entry Point                                            */
    *--p_stk = (CPU_STK)OSRecTaskFinishHelp;                /* R14 (LR) --- different!!                                             */
    *--p_stk = (CPU_STK)0x12121212u;                        /* R12                                                    */
    *--p_stk = (CPU_STK)0x03030303u;                        /* R3                                                     */
    *--p_stk = (CPU_STK)0x02020202u;                        /* R2                                                     */
    *--p_stk = (CPU_STK)p_stk_limit;                        /* R1                                                     */
    *--p_stk = (CPU_STK)p_arg;                              /* R0 : argument                                          */
                                                            /* Remaining registers saved on process stack             */
    *--p_stk = (CPU_STK)0x11111111u;                        /* R11                                                    */
    *--p_stk = (CPU_STK)0x10101010u;                        /* R10                                                    */
    *--p_stk = (CPU_STK)0x09090909u;                        /* R9                                                     */
    *--p_stk = (CPU_STK)0x08080808u;                        /* R8                                                     */
    *--p_stk = (CPU_STK)0x07070707u;                        /* R7                                                     */
    *--p_stk = (CPU_STK)0x06060606u;                        /* R6                                                     */
    *--p_stk = (CPU_STK)0x05050505u;                        /* R5                                                     */
    *--p_stk = (CPU_STK)0x04040404u;                        /* R4                                                     */

    return (p_stk);
}

/**********************************************************************************************************
 * Description 	:	Creates a periodic task
 * Note(s)		:	largely copied from OSTaskCreate but this
 * 						(1) does not put the task into ready Q -> does not get scheduled directly
 * 						(2) uses another TaskStkInit function / this may be omitted
 * 						(3) ALWAYS uses OSCfg_EdfSchedPrio as prio !! TODO
 *********************************************************************************************************/
void  OSTaskCreateMod (OS_TCB        *p_tcb,
                    CPU_CHAR      *p_name,
                    OS_TASK_PTR    p_task,
                    void          *p_arg,
                    OS_PRIO        prio,
                    CPU_STK       *p_stk_base,
                    CPU_STK_SIZE   stk_limit,
                    CPU_STK_SIZE   stk_size,
                    OS_MSG_QTY     q_size,
                    OS_TICK        time_quanta,
                    void          *p_ext,
                    OS_OPT         opt,
                    OS_ERR        *p_err){
		CPU_STK_SIZE   i;
	#if OS_CFG_TASK_REG_TBL_SIZE > 0u
		OS_OBJ_QTY     reg_nbr;
	#endif
		CPU_STK       *p_sp;
		CPU_STK       *p_stk_limit;
		CPU_SR_ALLOC();



	#ifdef OS_SAFETY_CRITICAL
		if (p_err == (OS_ERR *)0) {
			OS_SAFETY_CRITICAL_EXCEPTION();
			return;
		}
	#endif

	#ifdef OS_SAFETY_CRITICAL_IEC61508
		if (OSSafetyCriticalStartFlag == DEF_TRUE) {
		*p_err = OS_ERR_ILLEGAL_CREATE_RUN_TIME;
			return;
		}
	#endif

	#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
		if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* ---------- CANNOT CREATE A TASK FROM AN ISR ---------- */
			*p_err = OS_ERR_TASK_CREATE_ISR;
			return;
		}
	#endif

	#if OS_CFG_ARG_CHK_EN > 0u                                  /* ---------------- VALIDATE ARGUMENTS ------------------ */
		if (p_tcb == (OS_TCB *)0) {                             /* User must supply a valid OS_TCB                        */
			*p_err = OS_ERR_TCB_INVALID;
			return;
		}
		if (p_task == (OS_TASK_PTR)0) {                         /* User must supply a valid task                          */
			*p_err = OS_ERR_TASK_INVALID;
			return;
		}
		if (p_stk_base == (CPU_STK *)0) {                       /* User must supply a valid stack base address            */
			*p_err = OS_ERR_STK_INVALID;
			return;
		}
		if (stk_size < OSCfg_StkSizeMin) {                      /* User must supply a valid minimum stack size            */
			*p_err = OS_ERR_STK_SIZE_INVALID;
			return;
		}
		if (stk_limit >= stk_size) {                            /* User must supply a valid stack limit                   */
			*p_err = OS_ERR_STK_LIMIT_INVALID;
			return;
		}
		if (prio >= OS_CFG_PRIO_MAX) {                          /* Priority must be within 0 and OS_CFG_PRIO_MAX-1        */
			*p_err = OS_ERR_PRIO_INVALID;
			return;
		}
	#endif

	#if OS_CFG_ISR_POST_DEFERRED_EN > 0u
		if (prio == (OS_PRIO)0) {
			if (p_tcb != &OSIntQTaskTCB) {
				*p_err = OS_ERR_PRIO_INVALID;                   /* Not allowed to use priority 0                          */
				return;
			}
		}
	#endif

		if (prio == (OS_CFG_PRIO_MAX - 1u)) {
			if (p_tcb != &OSIdleTaskTCB) {
				*p_err = OS_ERR_PRIO_INVALID;                   /* Not allowed to use same priority as idle task          */
				return;
			}
		}

		OS_TaskInitTCB(p_tcb);                                  /* Initialize the TCB to default values                   */

		*p_err = OS_ERR_NONE;
																/* --------------- CLEAR THE TASK'S STACK --------------- */
		if ((opt & OS_OPT_TASK_STK_CHK) != (OS_OPT)0) {         /* See if stack checking has been enabled                 */
			if ((opt & OS_OPT_TASK_STK_CLR) != (OS_OPT)0) {     /* See if stack needs to be cleared                       */
				p_sp = p_stk_base;
				for (i = 0u; i < stk_size; i++) {               /* Stack grows from HIGH to LOW memory                    */
					*p_sp = (CPU_STK)0;                         /* Clear from bottom of stack and up!                     */
					p_sp++;
				}
			}
		}
																/* ------- INITIALIZE THE STACK FRAME OF THE TASK ------- */
	#if (CPU_CFG_STK_GROWTH == CPU_STK_GROWTH_HI_TO_LO)
		p_stk_limit = p_stk_base + stk_limit;
	#else
		p_stk_limit = p_stk_base + (stk_size - 1u) - stk_limit;
	#endif
	//different
		p_sp = OSRecTaskStkInit(p_task,
							p_arg,
							p_stk_base,
							p_stk_limit,
							stk_size,
							opt);

																/* -------------- INITIALIZE THE TCB FIELDS ------------- */
		p_tcb->TaskEntryAddr = p_task;                          /* Save task entry point address                          */
		p_tcb->TaskEntryArg  = p_arg;                           /* Save task entry argument                               */

		p_tcb->NamePtr       = p_name;                          /* Save task name                                         */

		p_tcb->Prio          = prio;              /* Save the task's priority  -------- different           */

		p_tcb->StkPtr        = p_sp;                            /* Save the new top-of-stack pointer                      */
		p_tcb->StkLimitPtr   = p_stk_limit;                     /* Save the stack limit pointer                           */

		p_tcb->TimeQuanta    = time_quanta;                     /* Save the #ticks for time slice (0 means not sliced)    */
	#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
		if (time_quanta == (OS_TICK)0) {
			p_tcb->TimeQuantaCtr = OSSchedRoundRobinDfltTimeQuanta;
		} else {
			p_tcb->TimeQuantaCtr = time_quanta;
		}
	#endif
		p_tcb->ExtPtr        = p_ext;                           /* Save pointer to TCB extension                          */
		p_tcb->StkBasePtr    = p_stk_base;                      /* Save pointer to the base address of the stack          */
		p_tcb->StkSize       = stk_size;                        /* Save the stack size (in number of CPU_STK elements)    */
		p_tcb->Opt           = opt;                             /* Save task options                                      */

	#if OS_CFG_TASK_REG_TBL_SIZE > 0u
		for (reg_nbr = 0u; reg_nbr < OS_CFG_TASK_REG_TBL_SIZE; reg_nbr++) {
			p_tcb->RegTbl[reg_nbr] = (OS_REG)0;
		}
	#endif

	#if OS_CFG_TASK_Q_EN > 0u
		OS_MsgQInit(&p_tcb->MsgQ,                               /* Initialize the task's message queue                    */
					q_size);
	#endif

		OSTaskCreateHook(p_tcb);                                /* Call user defined hook                                 */
	#if 0
																/* --------------- ADD TASK TO READY LIST --------------- */
		OS_CRITICAL_ENTER();
		OS_PrioInsert(p_tcb->Prio);
		OS_RdyListInsertTail(p_tcb);

	#if OS_CFG_DBG_EN > 0u
		OS_TaskDbgListAdd(p_tcb);
	#endif

		OSTaskQty++;                                            /* Increment the #tasks counter                           */

		if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Return if multitasking has not started                 */
			OS_CRITICAL_EXIT();
			return;
		}

		OS_CRITICAL_EXIT_NO_SCHED();
	#endif
		OSSched();
}

/**********************************************************************************************************
 * Description 	:	This function was used as a substitute for OS_TaskReturn, this needs to be specified
 * 					during task stack init
 * Note(s)		:	NOT IN USE
 *********************************************************************************************************/
void OSRecTaskFinishHelp(void){
	OS_ERR err;

	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	OS_TCB *p_tcb;                                     
	CPU_CRITICAL_ENTER();
	p_tcb  = OSTCBCurPtr;                             
	CPU_CRITICAL_EXIT();
	p_tcb->TaskState = (OS_STATE)OS_TASK_STATE_DEL;
	OS_RdyListRemove(p_tcb);
	OS_CRITICAL_EXIT_NO_SCHED();
    OSSched();

	//OSRecTaskFinish ((OS_TCB *)0 , (OS_ERR *)&err);
}