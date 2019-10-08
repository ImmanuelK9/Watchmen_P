/*********************************************************************************************************
*                                                tests
*                                  tests for custom implementations
*
*                                       TESTS FOR BINARY HEAP
*
* Filename		: test_bnrHeap.h
* Version		: ?
* Programmer(s)	: Watchmen_P
*
* Note(s) :
*********************************************************************************************************/

#ifndef TEST_BNRHEAP_MODULE_PRESENT
#define TEST_BNRHEAP_MODULE_PRESENT

/********************************************INCLUDE FILES***********************************************/
#include <lib_tree.h>

/***********************************************DEFINES**************************************************/

/******************************************FUNCTION PROTOTYPES*******************************************/
void testBHScenario0(Node* a, Node* b);
Node* testBHScenario1(Node* x, CPU_INT32U k);
void testBHScenario2(Node* x, CPU_INT32U k);
void testBH(Node* p_n);
void testBHProperties(Node* p_n);
void testBHRelationship(Node* p_n);
void print2DBH(Node* root);

/***********************************************MODULE END***********************************************/
#endif