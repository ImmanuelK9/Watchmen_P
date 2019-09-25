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

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/



/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/******************************************bhIinsert()*****************************************************
 * Description : insert a node into the heap
 * Argument(s) : h      the heap in which the new node will be inserted
 *               p_n    pointer to node that should be inserted
 * Return(s)   : A heap in which the desired node was inserted
 *********************************************************************************************************/
/*
Heap* bhInsert    (Heap *h, heap_node *p_n){
    //TODO
    return 0;
}
*/

/***************************************bhDeleteNode()******************************************************
 * Description	: delete a node from the heap
 * Argument(s)	: p_n	pointer to node that should be deleted
 * Return(s)    : A heap from which the desired node was deleted
 *********************************************************************************************************/
/*
Heap* bhDeleteNode (heap_node *p_n){
    //TODO
	return 0;
}
*/

/******************************************bhFindMin()*************************************************
 * Description : finds the minimum key in the heap
 * Argument(s) : h      the heap
 * Return(s)   : The node with the minimum key in the heap
 *********************************************************************************************************/
/*
heap_node* bhFindMin (Heap *h){
    //TODO
    return p_n;
}
*/

//hacky way

/******************************************bhIinsert()*****************************************************
 * Description : insert a node into the tree
 * Argument(s) : p_root  pointer to root of the tree
 *               p_n     pointer to node that should be inserted
 * Return(s)   : A pointer to the (new) root of the tree
 *********************************************************************************************************/
Node* bhInsert(Node* p_root, Node* p_n) {

	//edge case - heap is empty

	if (p_root == NULL) {
		p_n->parent = NULL;
		p_root = p_n;
		return p_root;
	}

	Node* insertionParentNode = getInsertionLocation(p_root);
	Node* insertionNode;

	//check whiich child is the insertion location and insert it
	if ((insertionParentNode->left) == NULL) {
		insertionParentNode->left = p_n;
	}
	else {
		insertionParentNode->right = p_n;
	}

	p_n->parent = insertionParentNode;

	p_root = bubbleUp(p_root, p_n);

	p_root->nodes_number = p_root->nodes_number + 1;

	return p_root;
}

/***************************************bhDeleteNode()******************************************************
 * Description	: delete a node from the tree
 * Argument(s)	: p_n	pointer to node that should be deleted
 * Return(s)    : A pointer to the (new) root of the tree
 * Note(s)		:
 *********************************************************************************************************/
Node* bhDeleteNode(Node* p_n) {

	//find last node that will replace the deleted one
	Node* lastNode = getLast()

	//replace node to be deletd with lastNode



	//bubble down lastNode



	p_root->nodes_number = p_root->nodes_number - 1;

	return p_root;
}

/******************************************bhFindMin()*************************************************
 * Description : finds the node with the minimum key
 * Argument(s) : p_root	the root node, which is also the minimum element
 * Return(s)   : The node with the minimum key in the heap
 *********************************************************************************************************/
Node* bhFindMin(Node* p_root) {

	//edge case - heap is empty

	if (p_root == NULL) {
		return 0;
	}

	return p_root;
}

/*********************************************LOCAL FUNCTIONS********************************************/

/******************************************getLast()*************************************************
 * Description : finds the last element in the heap (in the array representation) -
				 - the rightmost node on the last level of the heap
 * Argument(s) : h      the heap
 * Return(s)   : The last element in the heap
 * Note(s)     : In order to obtain this node, we use a trick that works on complete binary trees:

	Based on the binary represenatation of the number of nodes in the tree, we can decide 
	which way to go through the tree so that we find the desired node in O(log n) time

	Method: 
	We take into account all digits except the leftmost 1, such as by starting from left to right:
				if the digit is 0, we move from the current down to the left node in the tree
				if the digit is 1, we move to the right node

	Example: number of nodes = 5 <=> 1 0 1
			- first digit taken into account is 0 => from root (0) we move left to 1
			- second (and last digit) is 1 => from current node (1) we move right to 4
			=> we arrived at the final node

		                0
                       / \        
                      1   2           
                     / \                
                    3   4
 *********************************************************************************************************/
/*
heap_node* getLast(Heap* h) {
	
	CPU_INT32U nodes_number = h -> nodes_number;

	// stores the binary representation of the number of nodes in heap, 
	// such that nodes_number_binary[0] stores the least significant digit of nodes_number
	CPU_INT32U[32] nodes_number_binary;			
	

	for (int i = 0; i < 32; i++) {
		nodes_number_binary[i] = nodes_number % 2;
		nodes_number = nodes_number >> 1;
	}

	//move through tree

	return p_n;
}
*/

//continue hacky way

/******************************************getLast()*************************************************
 * Description : finds the last element in the heap (in the array representation) -
				 - the rightmost node on the last level of the heap
 * Argument(s) : p_root		root node
 * Return(s)   : The last element in the heap
 * Note(s)     : In order to obtain this node, we use a trick that works on complete binary trees:

	Based on the binary represenatation of the number of nodes in the tree, we can decide
	which way to go through the tree so that we find the desired node in O(log n) time

	Method:
	We take into account all digits except the leftmost 1, such as by starting from left to right:
				if the digit is 0, we move from the current down to the left node in the tree
				if the digit is 1, we move to the right node

	Example: number of nodes = 5 <=> 1 0 1
			- first digit taken into account is 0 => from root (0) we move left to 1
			- second (and last digit) is 1 => from current node (1) we move right to 4
			=> we arrived at the final node

						0
					   / \
					  1   2
					 / \
					3   4
 *********************************************************************************************************/
 Node* getLast(Node* p_root) {

	 CPU_INT32U nodes_number = p_root -> nodes_number;
	 Node* p_n = p_root;

	 // stores the binary representation of the number of nodes in heap,
	 // such that nodes_number_binary[0] stores the least significant digit of nodes_number
	 CPU_INT32U[32] nodes_number_binary;

	 int position = -1;

	 while (nodes_number > 1) {
		 nodes_number_binary[++position] = nodes_number % 2;
		 nodes_number = nodes_number >> 1;
	 };

	 //move through tree by making decision that start at nodes_number_binary[position]

	 while (position > -1) {
		 if (nodes_number_binary[position]) {
			 //move right
			 p_n = p_n->right;
		 }
		 else {
			 //move left
			 p_n = p_n->left;
		 }
		 position--;
	 }

	 return p_n;
 }


 /******************************************getInsertionLocation()****************************************
 * Description : finds the first empty position in the heap (in the array representation) -
				 - the leftmost empty node on the last level of the heap
 * Argument(s) : p_root		root node
 * Return(s)   : The parent of the first empty position in the heap
 * Note(s)     : In order to obtain this location, we use the same method as for getLast(),
				with the observation that the first empty location is the following node after the last,
				so we look for the node corresponding to nodes_number + 1 

	Based on the binary represenatation of the number of nodes in the tree, we can decide
	which way to go through the tree so that we find the desired node in O(log n) time

	Method:
	We take into account all digits except the leftmost 1, such as by starting from left to right:
				if the digit is 0, we move from the current down to the left node in the tree
				if the digit is 1, we move to the right node
	Nonetheless, the least significant digit is not acted upon as that would lead to a NULL location

	Example: number of nodes = 5 => we look at the position of the next (6th) element <=> 1 1 0
			- first digit taken into account is 1 => from root (0) we move right to 2
			- as we have only one digit left, we return 2 as the parent of the next insertion location

						0
					   / \
					  1   2
					 / \
					3   4
 *********************************************************************************************************/

 Node* getInsertionLocation(Node* p_root) {

	 CPU_INT32U nodes_number = (p_root->nodes_number) + 1;
	 Node* p_n = p_root;

	 // stores the binary representation of the number of nodes in heap,
	 // such that nodes_number_binary[0] stores the least significant digit of nodes_number
	 CPU_INT32U[32] nodes_number_binary;

	 int position = -1;

	 while (nodes_number > 1) {
		 nodes_number_binary[++position] = nodes_number % 2;
		 nodes_number = nodes_number >> 1;
	 };

	 //move through tree by making decision that start at nodes_number_binary[position]

	 while (position > 0) {
		 if (nodes_number_binary[position]) {
			 //move right
			 p_n = p_n->right;
		 }
		 else {
			 //move left
			 p_n = p_n->left;
		 }
		 position--;
	 }

	 return p_n;
 }

 /******************************************bubbleUp()****************************************************
 * Description : helper function for bhInsert() that moves up the newly inserted node in its right place
 * Argument(s) : p_root  pointer to root of the tree
 *               p_n     pointer to node that should be bubbled up
 * Return(s)    : A pointer to the (new) root of the tree
 *********************************************************************************************************/
 Node* bubbleUp(Node* p_root, Node* p_n) {

	 Node* currentParent = p_n->parent;
	 Node* currentParentParent;
	 Node* tempNode;

	 while ((currentParent != NULL) && (!cmpKey(p_n, currentParent))) {

		 currentParentParent = currentParent->parent;

		 //swap p_n with its parent and rearrange tree
		 p_n->parent = currentParentParent;
		 currentParent->parent = p_n;

		 if (currentParentParent != NULL) {
			 //if there exists the need to change the currentParent parent's child pointer

			 if (cmpKey(currentParent, currentParentParent->left) == 0) {
				 //if currentParent is to the left of its parent
				 currentParentParent->left = p_n;
			 }
			 else {
				 //if currentParent is to the right of its parent
				 currentParentParent->right = p_n;
			 }
		 }

		 //take care of the other child of the currentParent

		 if (cmpKey(p_n, currentParent->left) == 0) {
			 // if p_n is left child

			 tempNode = p_n->left;
			 p_n->left = currentParent;
			 currentParent->left = tempNode;

			 tempNode = p_n->right;
			 p_n->right = currentParent->right;
			 currentParent->right = tempNode;
		 }
		 else {
			 // if p_n is right child

			 tempNode = p_n->left;
			 p_n->left = currentParent->left;
			 currentParent->left = tempNode;

			 tempNode = p_n->right;
			 p_n->right = currentParent;
			 currentParent->right = tempNode;
		 }

		 //keep moving up
		 currentParent = p_n->parent;
	 }

	 return p_root;
 }

 /******************************************bubbleDown()****************************************************
 * Description : helper function for bhDelete() that moves down the placeholder node in its right place
 * Argument(s) : p_root  pointer to root of the tree
 *               p_n     pointer to node that has to be bubbled down
 * Return(s)    : A pointer to the (new) root of the tree
 *********************************************************************************************************/
 Node* bubbleDown(Node* p_root, Node* p_n) {



 }