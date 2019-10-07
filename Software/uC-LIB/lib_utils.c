/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                              UTILITIES
*
* Filename		: lib_utils.c
* Version		: ?
* Programmer(s)	: Watchmen_p
*
* Note(s) :
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include "lib_utils.h"
#include <stdio.h>

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/

/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/*********************************************LOCAL FUNCTIONS********************************************/

/********************************************getRoot()**********************************************
 * Description	: get the root of the tree including p_n
 * Argument(s)	: p_n		any node in the heap
 * Return(s)	: The root of the heap
 * Note(s)      : same as lib_rbTree.c
 *********************************************************************************************************/
Node* getRoot(Node* p_n) {

	if (p_n == 0) return 0;

	Node* p_root = p_n;

	while (0 != p_root->parent) p_root = p_root->parent;

	return p_root;
}

/********************************************swapNodes()**********************************************
* Description	: equivalent to a change of keys of a and b but maintains identity
* 				  The following properties hold:
* 					(1) the keys (of a and b) change position in the tree
* 					(2) for each position in the tree:
* 							- parent, left, right remain the same
*********************************************************************************************************/
void swapNodes(Node* p_a, Node* p_b) {

	//a is child of b?
	if (p_a->parent == p_b) {
		//reverse them so that it will always be the case that the first argument (p_a) is the parent
		swapNodes(p_b, p_a);
		return;
	}

	CPU_INT08U bIsChildOfA = 0;
	if (p_b->parent == p_a) bIsChildOfA = 1;

	Node help;
	help.parent = p_a->parent;
	help.left = p_a->left;
	help.right = p_a->right;
	help.key = p_a->key;

	CPU_INT08U aIsLeftChild = 0;
	CPU_INT08U bIsLeftChild = 0;

	if (p_a->parent != 0 && p_a == p_a->parent->left) aIsLeftChild = 1;
	if (p_b->parent != 0 && p_b == p_b->parent->left) bIsLeftChild = 1;

	if (bIsChildOfA)	p_a->parent = p_b;
	else 				p_a->parent = p_b->parent;

	if (!bIsChildOfA && p_a->parent != 0) {
		if (bIsLeftChild) p_a->parent->left = p_a;
		else p_a->parent->right = p_a;
	}

	p_a->left = p_b->left;
	if (p_a->left != 0) p_a->left->parent = p_a;

	p_a->right = p_b->right;
	if (p_a->right != 0) p_a->right->parent = p_a;
	p_a->color = p_b->color;

	p_b->parent = help.parent;
	if (p_b->parent != 0) {
		if (aIsLeftChild) p_b->parent->left = p_b;
		else p_b->parent->right = p_b;
	}

	if (bIsChildOfA && bIsLeftChild)	p_b->left = p_a;
	else 								p_b->left = help.left;
	if (p_b->left != 0) p_b->left->parent = p_b;

	if (bIsChildOfA && !bIsLeftChild) 	p_b->right = p_a;
	else								p_b->right = help.right;
	if (p_b->right != 0) p_b->right->parent = p_b;
	p_b->color = help.color;
}

/********************************************findMaxChild()**********************************************
* Description	: Finding the child with the largest key based on cmpKey() in lib_tree.c
* Argument(s)	: p_n		the node whose children are compared
* Return(s)		: The child with the max key value
* Note(s)		: in case at least one of the children are 0, returns 0
*********************************************************************************************************/
Node* findMaxChild(Node* p_n) {

	Node* leftChild = p_n->left;
	Node* rightChild = p_n->right;

	if (leftChild == 0 || rightChild == 0) return 0;
	if (cmpKey(leftChild, rightChild))		return leftChild;
	else									return rightChild;

}