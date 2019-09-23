/*********************************************************************************************************
*                                                uC/OS-III
*                                           The Real-Time Kernel
*
*                                               EDF-SCHEDULER
*
* Filename		: os_edf.c
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : 
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include <os.h>
#include <lib_tree.h>

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/

/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/********************************************OSEdfSched()**********************************************
 * Description	: Scheduler that manages all tasks having a deadline
 * Note(s)		: (1) should only be called inside of OS_CRITICAL sections
 *********************************************************************************************************/
void    OSEdfSched              (void){
    Node *p_toSchedule = findMin(OSEdfRdyList);
    OSTCBHighRdyPtr = p_toSchedule->info->tcbPtr;
}

void    OS_EdfRdyListInsert     (Node *p_n){
    OS_PrioInsert(OSCfg_EdfSchedPrio);
    OSEdfRdyList = insert(OSEdfRdyList, p_n);
}

void    OS_EdfRdyListRemove     (Node *p_n){
    OSEdfRdyList = delete(p_n);
    if (0 == OSEdfRdyList) OS_PrioRemove(OSCfg_EdfSchedPrio);
}

/*********************************************LOCAL FUNCTIONS********************************************/