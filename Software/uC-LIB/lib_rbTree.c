/*
*********************************************************************************************************
*                                                uC/LIB
*                                        CUSTOM LIBRARY MODULES
*
*
*                                            RED BLACK TREES
*
* Filename      : lib_rbTree.h
* Version       : ?
* Programmer(s) : Watchmen_T
*********************************************************************************************************
* Note(s) : none
*
*********************************************************************************************************/


/*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************/
#include "lib_rbTree.h"

/*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************/

/*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************/

/*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************/

/*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************/

/*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************/

/*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************/
Node*   getGrandParent  (Node* p_n);
Node*   getSibling      (Node* p_n);
Node*   getUncle        (Node* p_n);
Node*   rotLeft         (Node* p_n);
Node*   rotRight        (Node* p_n);

/*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************/

/*********************************************************************************************************
*                                           getGrandParent()
*
* Description : (1) get grandparent of node
*
* Argument(s) : p_n     pointer to node under inspection
*
* Return(s)   : A pointer to the grandparent
*
* Note(s)     : (a) returns null if n is the root node
*               (b) returns null if the parent of n is the root node
*********************************************************************************************************/
Node*   getGrandParent  (Node* p_n){
    Node* p_p = p_n->parent;

    if(0 == p_p)  return 0;
    return p_p->parent;
}

/*********************************************************************************************************
*                                           getSibling()
*
* Note(s)     : (1) returns null if there is no sibling, i.e.
*                   (a) n has no parent
*                   (b) n's parent has only one child (that is n)
*********************************************************************************************************/
Node*   getSibling  (Node* p_n){
    Node* p_p = p_n->parent;

    if(0 == p_p) return 0;
    if(p_n == p_p->left)    return p_p->right;
    else                    return p_p->left;
}

/*********************************************************************************************************
*                                           getUncle()
*
* Note(s)     : (1) returns null if there is no uncle, e.g.
*                   (a) n has no parent
*                   (b) n has no grandparent
*********************************************************************************************************/
Node*   getUncle    (Node* p_n){
    Node* p_p = p_n->parent;
    
    if(0 == p_p) return 0;
    return getSibling(p_p);
}

/*********************************************************************************************************
*                                           rotLeft()
*
* Description : (1) rotate the right child to the node's position
*                       N                R
*                      / \      =>      / \
*                     a   R            N   c
*                        / \          / \
*                       b   c        a   b 
*
* Argument(s) : p_n     pointer to the left child that is going to be rotated
*********************************************************************************************************/
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

/*********************************************************************************************************
*                                           rotRight()
*
* Description : (1) rotate the left child to the node's position
*                       N                L
*                      / \      =>      / \
*                     L   c            a   N
*                    / \                  / \
*                   a   b                b   c
*
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
