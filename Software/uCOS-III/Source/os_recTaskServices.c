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
#include <lib_rbTree.h>

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/

/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/********************************************************************************************************
 *                                                    CREATE A PERIODIC TASK
 *
 * Description	: Similar to OSTaskCreate, but sets the extension pointer to point to the period
 *
 * Arguments	: ...            see OSTaskCreate
 *                //additional data for recursion
 *
 *
 * Returns		: A pointer to the TCB of the task created.  This pointer must be used as an ID (i.e handle) to the task.
 * Notes		:
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
						 CPU_INT32U				period			 
						 ){
	//TODO
	//create task similar to OSTaskCreate?
	//set period
}

/*******************************************OSRecTaskFinish()*********************************************
 * Description 	:	A recursive task should call this function when it finishes
 * Argument(s)	: 	p_tcb      is the TCB of the tack to delete
 *					p_err      is a pointer to an error code returned by this function:
 *                             OS_ERR_NONE                  if the call is successful
 * Note(s)		:
 *********************************************************************************************************/
void OSRecTaskFinish (OS_TCB *p_tcb, OS_ERR *p_err){
	//reset stack: reset StkPtr, clear stack
	//copied from OSTaskCreate
	OS_OPT 		opt = p_tcb->Opt;
	CPU_STK 	*p_sp = p_tcb->StkPtr;
	CPU_STK 	*p_stk_base = p_tcb->StkBasePtr;
	CPU_STK_SIZE	stk_size = p_tcb->StkSize;
	CPU_STK_SIZE	i;
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
	
	CPU_STK *p_sp = OSTaskStkInit(	p_tcb->TaskEntryAddr,     /* Initialize the stack, i.e. init the CPU regs */
									p_tcb->TaskEntryArg,         
									p_tcb->StkBasePtr,
									p_tcb->StkLimitPtr,
									p_tcb->StkSize,
									p_tcb->Opt);
	
	p_tcb->StkPtr        = p_sp;                            /* set the new sp */

	//stack should be successfully reset now
	//TODO
	//what to do from here?? ----------------------------

	*p_err = OS_ERR_NONE;
}


/****************************************OSRecTaskListUpdate()*********************************************
 * Description 	:	This should be run at every tick
 * Note(s)		:
 *********************************************************************************************************/
void OSRecTaskListUpdate (void){
	//TODO critical section??? maybe borrow from os_tick-OS_TickListUpdate?
	Node* p_min = findMin(OSRecList);

	while(OSTickCtr == p_min->key->TickCtrMatch){	/* Process each TCB that expires               */
			//make task ready to run
			p_min->key->tcbPtr->TaskState = OS_TASK_STATE_RDY;
			OS_TaskRdy(p_min->key->tcbPtr);                            /* Make task ready to run                      */

			//delete old entry and add next entry in RecList
			deleteNode(p_min);
			p_min->key->TickCtrMatch = OSTickCtr + p_min->key->period;
			OSRecList = insert(OSRecList, p_min);

			p_min = findMin(OSRecList);
	}
}

/*********************************************LOCAL FUNCTIONS********************************************/