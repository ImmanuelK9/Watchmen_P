/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                            TREE INTERFACE
*
* Filename		: lib_tree.c
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : 
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include <lib_tree.h>
#include <lib_rbTree.h>
#include <lib_bnrHeap.h>
#include <stdio.h>

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/

/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/********************************************insert()**********************************************
 * Description	: Simulates an interface for tree operations
 * Argument(s)	: see the actual implementations
 * Return(s)	: see the actual implementations
 * Note(s)		: (1) p_n != 0 should be guaranteed
 *********************************************************************************************************/
Node*       insert              (Node *p_root, Node *p_n){
    if(0 == p_n){
        fprintf(stdout, "%s", "Tree error - node is null\n");
        return 0; 
    }
    switch(p_n->tree){
        case    RBTREE:		return rbInsert(p_root,p_n);
        case    BNRHEAP:	return bhInsert(p_root, p_n); 
		default:			fprintf(stdout, "%s", "Tree error - unitialized tree type\n");
                            return 0;
    }
}

/********************************************deleteNode()**********************************************
 * Description	: Simulates an interface for tree operations
 * Argument(s)	: see the actual implementations
 * Return(s)	: see the actual implementations
 * Note(s)		: (1) p_n != 0 should be guaranteed
 *********************************************************************************************************/
Node*       deleteNode          (Node *p_n){
	if(0 == p_n){
        fprintf(stdout, "%s", "Tree error - node is null\n");
        return 0; 
    }
    switch(p_n->tree){
        case    RBTREE:		return rbDeleteNode(p_n);
        case    BNRHEAP:	return bhDeleteNode(p_n); 
		default:			fprintf(stdout, "%s", "Tree error - unitialized tree type\n");
                            return 0;
    }
}

/********************************************findMin()**********************************************
 * Description	: Simulates an interface for tree operations
 * Argument(s)	: see the actual implementations
 * Return(s)	: see the actual implementations
 * Note(s)		: (1) p_n == 0 is allowed
 *********************************************************************************************************/
Node*	    findMin			    (Node* p_n){
	if(0 == p_n) return 0;      //this could happen if findMin is called on an empty tree
    switch(p_n->tree){
        case    RBTREE:		return rbFindMin(p_n);
        case    BNRHEAP:	return bhFindMin(p_n); 
		default:			fprintf(stdout, "%s", "Tree error - unitialized tree type\n");
                            return 0;
    }
}

/********************************************cmpKey()******************************************************
 * Description : (1) compares the keys of two nodes
 * 				 (2) returns a-b
 * Argument(s) : p_a/b     pointer to the first/second node
 * Note(s)     : (a) assuming 0!=p_a and 0!=p_b
 *********************************************************************************************************/
CPU_INT08S	cmpKey		(Node* p_a, Node* p_b){
	if(p_a->key < p_b->key) return -1;
	if(p_a->key > p_b->key) return 1;
	return 0;
}

/*********************************************LOCAL FUNCTIONS********************************************/