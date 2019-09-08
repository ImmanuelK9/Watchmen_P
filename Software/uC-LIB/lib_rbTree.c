/*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*                                            RED BLACK TREES
*
* Filename      : lib_rbTree.c
* Version       : ?
* Programmer(s) : Watchmen_T
*
* Note(s) : (1) compare https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include "lib_rbTree.h"

/********************************************LOCAL DEFINES***********************************************/

/*******************************************LOCAL CONSTANTS**********************************************/

/******************************************LOCAL DATA TYPES**********************************************/

/********************************************LOCAL TABLES************************************************/

/***************************************LOCAL GLOBAL VARIABLES*******************************************/

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/
Node*   getGrandParent  (Node* p_n);
Node*   getSibling      (Node* p_n);
Node*   getUncle        (Node* p_n);
Node*   rotLeft         (Node* p_n);
Node*   rotRight        (Node* p_n);
void	insertRec		(Node* p_root, Node* p_n);
void	repairTree		(Node* p_n);

CPU_INT08S	cmpKey		(Node* p_a, Node* p_b);

/*************************************LOCAL CONFIGURATION ERRORS*****************************************/

/********************************************insert()*****************************************************
 * Description : (1) insert a node into the tree
 * Argument(s) : p_root  pointer to root of the tree
 *               p_n     pointer to node that should be inserted
 * Return(s)   : A pointer to the (new) root of the tree
 *********************************************************************************************************/
Node* insert    (Node *p_root, Node *p_n){
	insertRec(p_root, p_n);
	repairTree(p_n);

	//find the new root of the tree
	p_root = p_n;
	while(0 != p_root->parent) p_root = p_root->parent;
	return p_root;
}

/*********************************************LOCAL FUNCTIONS*******************************************/

/******************************************* getGrandParent() *******************************************       
 * Description : (1) get grandparent of node
 * Argument(s) : p_n     pointer to node under inspection
 * Return(s)   : A pointer to the grandparent
 * Note(s)     : (a) returns null if n is the root node
 *               (b) returns null if the parent of n is the root node
 *********************************************************************************************************/
Node*   getGrandParent  (Node* p_n){
    Node* p_p = p_n->parent;

    if(0 == p_p)  return 0;
    return p_p->parent;
}

/********************************************getSibling()************************************************
 * Note(s)     : (1) returns null if there is no sibling, i.e.
 *                   (a) n has no parent
 *                   (b) n's parent has only one child (that is n)
 ******************************************************************************************************/
Node*   getSibling  (Node* p_n){
    Node* p_p = p_n->parent;

    if(0 == p_p) return 0;
    if(p_n == p_p->left)    return p_p->right;
    else                    return p_p->left;
}

/*******************************************getUncle()*************************************************
 * Note(s)     : (1) returns null if there is no uncle, e.g.
 *                   (a) n has no parent
 *                   (b) n has no grandparent
 ******************************************************************************************************/
Node*   getUncle    (Node* p_n){
    Node* p_p = p_n->parent;
    
    if(0 == p_p) return 0;
    return getSibling(p_p);
}

/********************************************rotLeft()***************************************************
 * Description : (1) rotate the right child to the node's position
 *                       N                R
 *                      / \      =>      / \
 *                     a   R            N   c
 *                        / \          / \
 *                       b   c        a   b 
 * Argument(s) : p_n     pointer to the left child that is going to be rotated
 ******************************************************************************************************/
Node*   rotLeft     (Node* p_n){
    Node* p_p = p_n->parent;
    Node* p_r = p_n->right;
    // 0 != p_r must be true because we cannot make a leaf an internal node
    
    p_n->right = p_r->left;
    if(0 != p_n->right) p_n->right->parent = p_n;

    p_r->left = p_n;
    p_n->parent = p_r;

    if( 0 != p_p ){
        //N wasnt the root
        if(p_n == p_n->left) p_n->left = p_r;
        else                 p_n->right = p_r;
    }

    p_r->parent = p_p;
}

/********************************************rotRight()****************************************************
 * Description : (1) rotate the left child to the node's position
 *                       N                L
 *                      / \      =>      / \
 *                     L   c            a   N
 *                    / \                  / \
 *                   a   b                b   c
 * Argument(s) : p_n     pointer to the left child that is going to be rotated
 *********************************************************************************************************/
Node*   rotRight    (Node* p_n){
    Node* p_p = p_n->parent;
    Node* p_l = p_n->left;
    // 0 != p_l must be true because we cannot make a leaf an internal node
    
    p_n->left = p_l->right;
    if(0 != p_n->left) p_n->left->parent = p_n;

    p_l->right = p_n;
    p_n->parent = p_l;

    if( 0 != p_p ){
        //N wasnt the root
        if(p_n == p_n->left) p_n->left = p_l;
        else                 p_n->right = p_l;
    }

    p_l->parent = p_p;
}

/********************************************insertRec()*************************************************
 * Description : (1) insert a node into the tree recursively
 *				 (2) helper function for insert()
 * Argument(s) : p_root  pointer to root of the tree
 *               p_n     pointer to node that should be inserted
 * Note(s)     : (1) tree property: left_child <= root
 *				 (2) the inserting node doesnt have to be initialized beforehands except of the key
 *********************************************************************************************************/
void insertRec	(Node* p_root, Node* p_n){
	// descend the tree to a leaf
	if(0 != p_root){
		if(cmpKey(p_n, p_root) <= 0){
			if(0 == p_root->left) p_root->left = p_n;
			else{
				insertRec(p_root->left, p_n);
				return;
			}
		} else {
			if(0 == p_root->right) p_root->right = p_n;
			else{
				insertRec(p_root->right, p_n);
				return;
			}
		}
	}

	p_n->parent = p_root;
	p_n->color = RED;
	p_n->left = 0;
	p_n->right = 0;
}

/********************************************repairTree()************************************************
 * Description : (1) restore the RBTree properties after a node has been inserted
 * Argument(s) : p_n		pointer to node that has been inserted
 * Note(s)     : (a) There are different cases that can occur
 *					(0) N was inserted into an empty tree / N is the root node / n has no parent
 *					(1) N's parent P is black
 *					(2) N's parent P is red => P is not the root => P has a parent
 *						(2a) N's uncle is red
 *						(2b) N's uncle is black or null
 *							(2bI) if n is "on the inside" of the subtree
 *							(2bII)if n is "on the outside" of the subtree
 *********************************************************************************************************/
void repairTree	(Node* p_n){
	if(0 == p_n->parent) p_n->color = BLACK;
	else if(p_n->parent->color == BLACK) return;
	else if(0 != getUncle(p_n) && RED == getUncle(p_n)->color){
		//case (2a)
		p_n->parent->color = BLACK;
		getUncle(p_n)->color = BLACK;
		getGrandParent(p_n)->color = RED;
		repairTree(getGrandParent(p_n));
	} else {
		//case (2b)
		Node* p_p = p_n->parent;
		Node* p_g = getGrandParent(p_n);
		
		//case (2bI)
		//if n is "on the inside" of the subtree under G move it to the "outside"
		if(p_n == p_p->right && p_p == p_g->left){
			rotLeft(p_p);
			p_n = p_n->left;	
			//this renaming leads to the state as if n was already
			//on the "outside" before calling repairTree
		} else if(p_n == p_p->left && p_p == p_g->right){
			rotRight(p_p);
			p_n = p_n->right;
		}

		//case (2bII)
		//proceed as if N was already on the "outside"
		//parents and grandparents could have changed during 
		p_p = p_n->parent;
		p_g = getGrandParent(p_n);

		if(p_n == p_p->left) rotRight(p_g);
		else rotLeft(p_g);

		p_p->color = BLACK;
		p_g->color = RED;
	}
}

/********************************************cmpKey()******************************************************
 * Description : (1) compares the keys of two nodes
 * Argument(s) : p_a/b     pointer to the first/second node
 * Note(s)     : (a) assuming 0!=p_a and 0!=p_b
 *********************************************************************************************************/
CPU_INT08S	cmpKey		(Node* p_a, Node* p_b){
	if(p_a->key < p_b->key) return -1;
	if(p_a->key > p_b->key) return 1;
	return 0;
}