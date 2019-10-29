/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*												AVL TREE
*
* Filename		: lib_avlTree.c
* Version		: ?
* Programmer(s)	: Watchmen_P
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include "lib_avlTree.h"
#include "lib_tree.h"
#include "lib_utils.h"

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/*
struct heap_node {
	struct	heap_node	*parent;
	struct	heap_node	*left;
	struct	heap_node	*right;
	//enum	tree	tree;
	CPU_INT32U		key;
	//OS_NODE_INFO 	*info;
};

struct Heap {
	struct        heap_node       *root;
	CPU_INT32U          nodes_number;
};
*/

/*
struct node {
	struct	node	*parent;
	struct	node	*left;
	struct	node	*right;
	enum	color	color;
	enum	tree	tree;
	CPU_INT32U		key;
	CPU_INT32U		nodes_number;			//the nodes_number is used to store the height of a node for now
	CPU_BOOLEAN		overflowState;
	CPU_BOOLEAN		inTree;
	OS_NODE_INFO 	*info;
};

*/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/



/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

   /******************************************avlIinsert()*****************************************************
	* Description : insert a node into the tree
	* Argument(s) : p_root  pointer to root of the tree
	*               p_n     pointer to node that should be inserted
	* Return(s)   : A pointer to the (new) root of the tree
	*********************************************************************************************************/
Node* avlInsert(Node* p_root, Node* p_n) {


}

/***************************************avlDeleteNode()******************************************************
 * Description	: delete a node from the tree
 * Argument(s)	: p_n	pointer to node that should be deleted
 * Return(s)    : A pointer to the (new) root of the tree
 * Note(s)		:
 *********************************************************************************************************/
Node* avlDeleteNode(Node* p_n) {


}

/******************************************avlFindMin()*************************************************
 * Description : finds the node with the minimum key
 * Argument(s) : p_root	the root node
 * Return(s)   : The node with the minimum key in the tree
 *********************************************************************************************************/
Node* avlFindMin(Node* p_root) {


}

/*********************************************LOCAL FUNCTIONS********************************************/

