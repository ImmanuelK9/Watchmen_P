/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                            TREE INTERFACE
*
* Filename		: lib_tree.h
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : 
*********************************************************************************************************/

#ifndef TREE_MODULE_PRESENT
#define TREE_MODULE_PRESENT

/********************************************INCLUDE FILES***********************************************/
#include <cpu.h>
#include <os.h>

/***********************************************EXTERNS**************************************************/

/***********************************************DEFINES**************************************************/

/*********************************************DATA TYPES*************************************************/

/******************************************GLOBAL VARIABLES**********************************************/

/***********************************************MACRO'S**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
Node*       insert              (Node *p_root, Node *p_n);
Node*       deleteNode          (Node *p_n);
Node*	    findMin			    (Node* p_n);
CPU_INT08S	cmpKey		        (Node* p_a, Node* p_b);

/******************************************CONFIGURATION ERRORS******************************************/

/***********************************************MODULE END***********************************************/
#endif