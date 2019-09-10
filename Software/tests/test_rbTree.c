/*********************************************************************************************************
*                                                FOLDER (e.g. uC/LIB)
*                                        DESCRIPTION (e.g. CUSTOM LIBRARY MODULES)
*
*                                            CONTENT (e.g. RED BLACK TREES)
*
* Filename		: test_rbTree.c
* Version		: ?
* Programmer(s)	: Watchmen_T
*
* Note(s) : 
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include "test_rbTree.h"
#include <stdio.h>

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/


void    testRBTree      (Node* p_n){
    testSearchTree(p_n);
    testBlackRoot(p_n);
    testNotRedRed(p_n);
    testBlackEqual(p_n);
}

/******************************************testSearchTree()**********************************************
 * Description	: Tests the binary search tree property for the given RBTree
 * Notes        : Assuming left <= root < right
 *********************************************************************************************************/
void    testSearchTree  (Node* p_n){
    if(0 != p_n->left){
        if(cmpKey(p_n, p_n->left) < 0) fprintf(stdout, "%s", "SearchTree error\n");
        testSearchTree(p_n->left);
    }
    if(0 != p_n->right){
        if(cmpKey(p_n, p_n->right) >= 0) fprintf(stdout, "%s", "SearchTree error\n");
        testSearchTree(p_n->right);
    }
}

void    testBlackRoot   (Node* p_n){
    if(p_n->color != RED) fprintf(stdout, "%s", "BlackRoot error\n");
}

void    testNotRedRed   (Node* p_n){
    if(p_n->color == RED){
        if(0 != p_n->left){
            if(p_n->left->color == RED) fprintf(stdout, "%s", "NotRedRed error\n");
            testNotRedRed(p_n->left);
        }
        if(0 != p_n->right){
            if(p_n->right->color == RED) fprintf(stdout, "%s", "NotRedRed error\n");
            testNotRedRed(p_n->right);
        }
    }
    
}

int     countBlack(Node* p_n){
    if(p_n == 0) return 0;
    int l = countBlack(p_n->left);
    int r = countBlack(p_n->right);
    if(l != r) fprintf(stdout, "%s", "countBlack error\n");
    
    if(p_n->color == RED) return l;
    else return l=1;
}

void    testBlackEqual  (Node* p_n){
    countBlack(p_n);
}

/*********************************************LOCAL FUNCTIONS********************************************/