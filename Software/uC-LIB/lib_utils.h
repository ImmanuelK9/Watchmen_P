/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                              UTILITIES
*
* Filename      : lib_utils.h
* Version       : ?
* Programmer(s) : Watchmen_P
*
* Note(s) : (1) Use int as keys, change to TCB later
*********************************************************************************************************/

#ifndef  LIB_UTILS_MODULE_PRESENT
#define  LIB_UTILS_MODULE_PRESENT

/********************************************INCLUDE FILES**********************************************/
#include <cpu.h>       // for the data types
#include <os.h>        // for specific key
#include <lib_tree.h>  // for data type specific comparison function

/***********************************************DEFINES*************************************************/

/*********************************************DATA TYPES************************************************/

/******************************************GLOBAL VARIABLES**********************************************/

/***********************************************MACRO'S**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
Node* getRoot(Node* p_n);
void swapNodes(Node* p_a, Node* p_b);
Node* findMinChild(Node* p_n);

/***********************************************MODULE END***********************************************/
#endif