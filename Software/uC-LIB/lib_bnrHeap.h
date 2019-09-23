/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                            BINARY MIN HEAP
*
* Filename		: lib_bnrHeap.h
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : 
*********************************************************************************************************/

#ifndef BNRHEAP_MODULE_PRESENT
#define BNRHEAP_MODULE_PRESENT

/********************************************INCLUDE FILES***********************************************/
#include <cpu.h>
#include <os.h>

/***********************************************EXTERNS**************************************************/

/***********************************************DEFINES**************************************************/

/*********************************************DATA TYPES*************************************************/

/******************************************GLOBAL VARIABLES**********************************************/

/***********************************************MACRO'S**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
Node*       bhInsert                (Node *p_root, Node *p_n);
Node*       bhDeleteNode            (Node *p_n);
Node*	    bhFindMin			    (Node* p_n);

/******************************************CONFIGURATION ERRORS******************************************/

/***********************************************MODULE END***********************************************/
#endif