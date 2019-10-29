/*********************************************************************************************************
*                                                tests
*                                  tests for custom implementations
*
*                                       TESTS FOR AVL TREE
*
* Filename		: test_avlHeap.h
* Version		: ?
* Programmer(s)	: Watchmen_P
*
* Note(s) :
*********************************************************************************************************/

#ifndef TEST_AVLTREE_MODULE_PRESENT
#define TEST_AVLTREE_MODULE_PRESENT

/********************************************INCLUDE FILES***********************************************/
#include <lib_tree.h>

/***********************************************DEFINES**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
void testAVLScenario0(Node* a, Node* b);
Node* testAVLScenario1(Node* x, CPU_INT32U k);
void testAVLScenario2(Node* x, CPU_INT32U k);
void testAVL(Node* p_n);
void testAVLProperties(Node* p_n);
void testAVLRelationship(Node* p_n);
void print2DAVL(Node* root);

/***********************************************MODULE END***********************************************/
#endif