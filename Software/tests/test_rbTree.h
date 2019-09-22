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
#include <lib_tree.h>

/***********************************************DEFINES**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
void    testRBTreeScenario0(Node* a, Node* b);
Node*   testRBTreeScenario1(Node* x, CPU_INT32U k);
Node*    testRBTreeScenario2(Node* x, CPU_INT32U k);
void    testRBTree      (Node* p_n);
void    testSearchTree  (Node* p_n);
void    testBlackRoot   (Node* p_n);
void    testNotRedRed   (Node* p_n);
void    testBlackEqual  (Node* p_n);
void    testRelationship    (Node* p_n);
void    print2D         (Node *root);

/***********************************************MODULE END***********************************************/
#endif