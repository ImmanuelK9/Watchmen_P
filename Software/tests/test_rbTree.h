/*********************************************************************************************************
*                                                tests
*                                  tests for custom implementations
*
*                                       TESTS FOR RED BLACK TREES
*
* Filename		: test_rbTree.h
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : 
*********************************************************************************************************/

#ifndef TEST_RBTREE_MODULE_PRESENT
#define TEST_RBTREE_MODULE_PRESENT

/********************************************INCLUDE FILES***********************************************/
#include <lib_rbTree.h>

/***********************************************DEFINES**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
void    testSearchTree  (Node* p_n);
void    testBlackRoot   (Node* p_n);
void    testNotRedRed   (Node* p_n);
void    testBlackEqual  (Node* p_n);

/***********************************************MODULE END***********************************************/
#endif