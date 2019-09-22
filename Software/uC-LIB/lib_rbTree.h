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
#include <os.h>        // for specific key

/***********************************************DEFINES*************************************************/
//outsourced to os.h
//enum color {RED, BLACK};

/*********************************************DATA TYPES************************************************/
/*struct node {
    struct node *parent;
    struct node *left;
    struct node *right;
    enum color  color;
    OS_REC_LIST_KEY  *key;
};
typedef struct node Node;*/

/******************************************GLOBAL VARIABLES**********************************************/

/***********************************************MACRO'S**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
Node*       insert              (Node *p_root, Node *p_n);
Node*       deleteNode          (Node *p_n);
Node*	    findMin			    (Node* p_n);
CPU_INT08S	cmpKey		        (Node* p_a, Node* p_b);

/***********************************************MODULE END***********************************************/
#endif