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
#include <stdlib.h>

#define COUNT 5

/********************************************LOCAL TABLES************************************************/
CPU_INT32U keys[] = {3,13,2,8,6,46,5,2,3,7,9,17,46,13,54,97,64,84,56,45,91,76,35,42,38,24,75,34,93,33};

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/
void print2DUtil(Node *root, int space);

void    testRBTreeScenario0(Node* a, Node* b){
    //Node* a = (Node*)  malloc(sizeof(Node));
    a->left=a->parent=a->right=0;
    a->color = BLACK;
    a->key=15;
    a->key=12;

    //Node* b = (Node*)  malloc(sizeof(Node));
    b->key=5;
    insert(a, b);
    testRBTree(a);
    //free(a);
    //free(b);
}

void    testRBTreeScenario1(Node* x, CPU_INT32U k){
    Node* currentRoot = x;
    x->color = BLACK;
    x->key=15;
    for(int i=0; i<k; ++i){
        x[i+1].key=keys[i];
        currentRoot = insert(currentRoot, x+i+1);
    }
    testRBTree(currentRoot);
    //print2D(currentRoot);
}

void    testRBTree      (Node* p_n){
    testSearchTree(p_n);
    testBlackRoot(p_n);
    testNotRedRed(p_n);
    testBlackEqual(p_n);
    fprintf(stdout, "%s", "Test finished\n");
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
    if(p_n->color == RED) fprintf(stdout, "%s", "BlackRoot error\n");
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

// Function to print binary tree in 2D  
// It does reverse inorder traversal  
void print2DUtil(Node *root, int space)  
{  
    // Base case  
    if (root == NULL)  
        return;  
  
    // Increase distance between levels  
    space += COUNT;  
  
    // Process right child first  
    print2DUtil(root->right, space);  
  
    // Print current node after space  
    // count  
    fprintf(stdout, "%s", "\n");  
    for (int i = COUNT; i < space; i++)  
        fprintf(stdout, "%s", " ");;  
    fprintf(stdout, "%d", root->key);
    if(root->color == RED) fprintf(stdout, "%s\n", "-R");
    else fprintf(stdout, "%s\n", "-B");
  
    // Process left child  
    print2DUtil(root->left, space);  
}  
  
// Wrapper over print2DUtil()  
void print2D(Node *root)  
{  
    // Pass initial space count as 0  
    print2DUtil(root, 0);  
}  

/*********************************************LOCAL FUNCTIONS********************************************/