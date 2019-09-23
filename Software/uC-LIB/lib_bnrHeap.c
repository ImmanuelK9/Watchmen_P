/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                            BINARY MIN HEAP
*
* Filename		: lib_bnrHeap.c
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : (1) Maybe use CPU_DATA    CPU_CntLeadZeros (CPU_DATA    val); ?
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include "lib_bnrHeap.h"
#include "lib_tree.h"

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/

/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/******************************************bhIinsert()*****************************************************
 * Description : (1) insert a node into the tree
 * Argument(s) : p_root  pointer to root of the tree
 *               p_n     pointer to node that should be inserted
 * Return(s)   : A pointer to the (new) root of the tree
 *********************************************************************************************************/
Node* bhInsert    (Node *p_root, Node *p_n){
    //TODO
    return 0;
}

/***************************************bhDeleteNode()******************************************************
 * Description	: delete a node from the tree
 * Argument(s)	: p_n	pointer to node that should be deleted
 * Return(s)    : A pointer to the (new) root of the tree
 * Note(s)		:
 *********************************************************************************************************/
Node* bhDeleteNode (Node *p_n){
    //TODO
	return 0;
}

/******************************************bhFindMin()*************************************************
 * Description : (1) finds the minimum
 * Argument(s) : p_n	node from where to start the search (should be the root)
 *********************************************************************************************************/
Node* bhFindMin (Node* p_n){
    //TODO
    return p_n;
}


/*********************************************LOCAL FUNCTIONS********************************************/