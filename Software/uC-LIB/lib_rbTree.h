/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                            RED BLACK TREES
*
* Filename      : lib_rbTree.h
* Version       : ?
* Programmer(s) : Watchmen_T
*
* Note(s) : (1) Use int as keys, change to TCB later
*********************************************************************************************************/

#ifndef  LIB_RBTREE_MODULE_PRESENT
#define  LIB_RBTREE_MODULE_PRESENT

/********************************************INCLUDE FILES**********************************************/
#include <cpu.h>       // for the data types

/***********************************************DEFINES*************************************************/
enum color {RED, BLACK};

/*********************************************DATA TYPES************************************************/
typedef struct node {
    Node        *parent;
    Node        *left;
    Node        *right;
    enum color  color;
    CPU_INT32U  key;
} Node;

/******************************************GLOBAL VARIABLES**********************************************/

/***********************************************MACRO'S**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
Node*       insert              (Node *p_root, Node *p_n);
void        delete              (Node *p_n);

/***********************************************MODULE END***********************************************/
#endif