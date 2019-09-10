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
*				https://en.wikipedia.org/w/index.php?title=Red%E2%80%93black_tree&oldid=914471956
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
void	rotLeft         (Node* p_n);
void	rotRight        (Node* p_n);
void	insertRec		(Node* p_root, Node* p_n);
void	repairTree		(Node* p_n);
void	replaceNode		(Node* p_del, Node* p_new);
Node*	findMin			(Node* p_n);
void	helpDelete123	(Node* p_n);
void	helpDelete4		(Node* p_n);
void	helpDelete56	(Node* p_n);
Node*	getRoot			(Node* p_n);

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
	return getRoot(p_n);
}

/*****************************************deleteNode()******************************************************
 * Description	: delete a node from the tree
 * Argument(s)	: p_n	pointer to node that should be deleted
 * Note(s)		: (a) p_n should not be the root of the tree! //TODO
 * 				  (b) there are different cases that can occur
 * 					(b1) n has two children -> replace key with successor's key, delete successor
 * 					(b2) n has at most one children
 * 						(b2a) n is red
 * 						(b2b) n is black but its child is red
 * 						(b2c) n is black its "child" is a leaf/nullptr, i.e. n was a black "leaf"
 * 							  (a leaf in the sense of a binary tree without usage of null-leafs)
 * 							(+) all helper case descriptions will be of the form P S S_L S_R
 * 							(+) P - Parent, S - Sibling, S_L/R - Sibling's left/right child
 * 							(+) b - black, r - red, x - black or red
 *********************************************************************************************************/
Node* deleteNode (Node *p_n){
	if(0 != p_n->left && 0 !=p_n->right){
		// case (b1)
		Node* p_suc = findMin(p_n->right);
		p_n->key = p_suc->key;
		return deleteNode(p_suc);
	} else{
		// case (b2)
		Node* p_child = (0 == p_n->right) ? p_n->left : p_n->right;
		if(p_n->color == BLACK){
			//TODO p_child should always be != 0 but it seems that this was a bugfix???
			if(p_child != 0 && p_child->color==RED) p_child->color=BLACK;	//case (b2b)
			else helpDelete123(p_n);				//case (b2c)
		}
		Node* newRoot = getRoot(p_n);
		replaceNode(p_n, p_child);
		return newRoot;
	}
	//TODO eventually need to free the memory of n?
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
 * Argument(s) : p_n     pointer to node which is going to be rotated with its right child
 ******************************************************************************************************/
void   rotLeft     (Node* p_n){
    Node* p_p = p_n->parent;
    Node* p_r = p_n->right;
    // 0 != p_r must be true because we cannot make a leaf an internal node
    
    p_n->right = p_r->left;
    if(0 != p_n->right) p_n->right->parent = p_n;

    p_r->left = p_n;
    p_n->parent = p_r;

    if( 0 != p_p ){
        //N wasnt the root
        if(p_n == p_p->left) 		p_p->left = p_r;
        else if(p_n == p_p->right)	p_p->right = p_r;
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
 * Argument(s) : p_n     pointer to node which is going to be rotated with its left child
 *********************************************************************************************************/
void   rotRight    (Node* p_n){
    Node* p_p = p_n->parent;
    Node* p_l = p_n->left;
    // 0 != p_l must be true because we cannot make a leaf an internal node
    
    p_n->left = p_l->right;
    if(0 != p_n->left) p_n->left->parent = p_n;

    p_l->right = p_n;
    p_n->parent = p_l;

    if( 0 != p_p ){
        //N wasnt the root
        if(p_n == p_p->left) 		p_p->left = p_l;
        else if(p_n == p_p->right)	p_p->right = p_l;
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

/********************************************replaceNode()**********************************************
 * Description : (1) replaces a node with another new node
 * Argument(s) : p_del	pointer to the node to be deleted and replaced
 *               p_new	pointer to the new node
 * Note(s)     : (1) p_del == 0 is not allowed
 * 				 (2) p_del should not be the root of the tree!
 * 				 (3) p_new == 0 is allowed
 *********************************************************************************************************/
void replaceNode (Node* p_del, Node* p_new){
	if(0 != p_new) p_new->parent = p_del->parent;
	if(p_del == p_del->parent->left){
		p_del->parent->left = p_new;
	} else {
		p_del->parent->right = p_new;
	}
}

/********************************************findMin()*************************************************
 * Description : (1) finds the minimum for a given node
 * Argument(s) : p_n	node from where to start the search
 *********************************************************************************************************/
Node* findMin (Node* p_n){
	while(0 != p_n->left) p_n = p_n->left;
	return p_n;
}

/********************************************helpDelete123()**********************************************
 * Description	: cases (b2c1), (b2c2): b r x x and (b2c3): b b b b of the deleting function
 *********************************************************************************************************/
void	helpDelete123	(Node* p_n){
	if(0 == p_n->parent){
		// case (b2c1) n is the new root
		// nothing to do
	} else {
		//case (b2c2): b r x x
		Node* p_s = getSibling(p_n);
		if(p_s->color == RED){
			p_n->parent->color = RED;
			p_s->color = BLACK;
			if(p_n == p_n->parent->left) rotLeft(p_n->parent);
			else rotRight(p_n->parent);
		}
		//case (b2c3): b b b b
		p_s = getSibling(p_n);
		if((p_n->parent->color == BLACK) && (p_s->color == BLACK) &&
			((p_s->left == 0) || (p_s->left->color == BLACK)) && 
			((p_s->right == 0) || (p_s->right->color == BLACK)) ){
				p_s->color = RED;
				helpDelete123(p_n->parent);
		} else{
			helpDelete4(p_n);
		}
	}
}

/********************************************helpDelete4()**********************************************
 * Description	: case (b2c4): r b b b of the deleting function
 *********************************************************************************************************/
void	helpDelete4		(Node* p_n){
	//case (b2c4): r b b b
	Node* p_s = getSibling(p_n);

	if((p_n->parent->color == RED) && (p_s->color == BLACK) &&
		((p_s->left == 0) || (p_s->left->color == BLACK)) && 
		((p_s->right == 0) || (p_s->right->color == BLACK)) ){
			p_s->color = RED;
			p_n->parent->color = BLACK;
	} else {
		helpDelete56(p_n);
	}
}

/********************************************helpDelete56()**********************************************
 * Description	: cases (b2c5): x b r b and (b2c6): x b x r of the deleting function
 *********************************************************************************************************/
void	helpDelete56	(Node* p_n){
	//case (b2c5): x b r b
	Node* p_s = getSibling(p_n);

	if(p_s->color == BLACK){
		if ((p_n == p_n->parent->left) && 
			//TODO p_s->right should never be == 0 but it seems that this was a bugfix???
			((p_s->right == 0) || (p_s->right->color == BLACK)) &&
			(p_s->left->color == RED)){
				// This last test is trivial too due to cases 2-4.
				p_s->color = RED;
				p_s->left->color = BLACK;
				rotRight(p_s);
    } else if ((p_n == p_n->parent->right) && 
				//TODO p_s->left should never be == 0 but it seems that this was a bugfix???
				((p_s->left == 0) || (p_s->left->color == BLACK)) &&
				(p_s->right->color == RED)) {
				// This last test is trivial too due to cases 2-4.
				p_s->color = RED;
				p_s->right->color = BLACK;
				rotLeft(p_s);
		}
	}
	
	//case (b2c6): x b x r
	p_s = getSibling(p_n);

	p_s->color = p_n->parent->color;
	p_n->parent->color = BLACK;

	if (p_n == p_n->parent->left) {
		p_s->right->color = BLACK;
		rotLeft(p_n->parent);
	} else {
		p_s->left->color = BLACK;
		rotRight(p_n->parent);
  }
}

/********************************************getRoot()**********************************************
 * Description	: get the root of the tree including p_n
 *********************************************************************************************************/
Node*	getRoot			(Node* p_n){
	if(p_n == 0) return 0;
	Node* p_root = p_n;
	while(0 != p_root->parent) p_root = p_root->parent;
	return p_root;
}