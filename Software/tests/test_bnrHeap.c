/*********************************************************************************************************
*                                                FOLDER (e.g. uC/LIB)
*                                        DESCRIPTION (e.g. CUSTOM LIBRARY MODULES)
*
*                                            CONTENT (e.g. BINARY HEAP)
*
* Filename		: test_bnrHeap.c
* Version		: ?
* Programmer(s)	: Watchmen_P
*
* Note(s) :
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include "test_bnrHeap.h"
#include <stdio.h>
#include <stdlib.h>

#define ROWCOUNT 5

/********************************************LOCAL TABLES************************************************/
CPU_INT32U keysBH[] = { 0,47,17,0,6,46,5,2,3,7,9,17,46,13,54,97,64,84,56,45,91,76,35,42,38,24,75,34,93,33 };

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/
void print2DUtilBH(Node* root, int space);

void testBHScenario0(Node* a, Node* b) {
	//Node* a = (Node*)  malloc(sizeof(Node));
    Node* root = a;
	a->left = a->parent = a->right = 0;
	a->key = 6;
    a->nodes_number = 1;
	a->tree = BNRHEAP;

	//Node* b = (Node*)  malloc(sizeof(Node));
	b->key = 5;
	b->tree = BNRHEAP;
	root = insert(a, b);
	print2DBH(a);
	testBH(a);
    fprintf(stdout, "\n a is %d \n", a->key);
    fprintf(stdout, "\n b is %d \n", b->key);

	deleteNode(b);
	print2DBH(a);
	testBH(a);
}

Node* testBHScenario1(Node* x, CPU_INT32U k) {
	Node* currentRoot = x;
	x->key = 5;
	x->nodes_number = 1;
	x->tree = BNRHEAP;
	for (int i = 0; i < k; ++i) {
		x[i + 1].key = keysBH[i];
		x[i + 1].tree = BNRHEAP;
		currentRoot = insert(currentRoot, x + i + 1);
	}
	testBH(currentRoot);
	print2DBH(currentRoot);
	return currentRoot;
}

void testBHScenario2(Node* x, CPU_INT32U k) {
	Node* currentRoot = testBHScenario1(x, k);

	fprintf(stdout, "\n deleting %d \n", (x+2)->key);

	currentRoot = deleteNode(x + 2);
	testBH(currentRoot);
	print2DBH(currentRoot);

	fprintf(stdout, "\n inserting %d \n", (x + 2)->key);

	currentRoot = insert(currentRoot, x + 2);
	testBH(currentRoot);
	print2DBH(currentRoot);

	fprintf(stdout, "\n deleting %d \n", (x + 4)->key);

	currentRoot = deleteNode(x + 4);
	testBH(currentRoot);
	print2DBH(currentRoot);

	fprintf(stdout, "\n deleting %d \n", (x + 1)->key);

	currentRoot = deleteNode(x+1);
	testBH(currentRoot);
	print2DBH(currentRoot);

	//currentRoot = insert(currentRoot, x + 2);
}

void testBH(Node* p_n) {
	testBHProperties(p_n);
	testBHRelationship(p_n);
	fprintf(stdout, "%s", "Test finished\n");
}

/******************************************testSearchTree()**********************************************
 * Description	: Tests the Min Binary Heap properties that left, right > root
 * Notes        : 
 *********************************************************************************************************/
void testBHProperties (Node* p_n) {
	if (p_n != 0) {
		if (0 != p_n->left) {
			if (cmpKey(p_n, p_n->left) > 0) fprintf(stdout, "%s", "BH Property error\n");
			testBHProperties(p_n->left);
		}
		if (0 != p_n->right) {
			if (cmpKey(p_n, p_n->right) > 0) fprintf(stdout, "%s", "BH Property error\n");
			testBHProperties(p_n->right);
		}
	}
}

void testBHRelationship(Node* p_n) {
	if (p_n == 0) return;
	if (p_n->left != 0) {
		if (p_n->left->parent != p_n) {
			fprintf(stdout, "%s", "Relationship error\n");
		}
		testBHRelationship(p_n->left);
	}
	if (p_n->right != 0) {
		if (p_n->right->parent != p_n) {
			fprintf(stdout, "%s", "Relationship error\n");
		}
		testBHRelationship(p_n->right);
	}
	if (p_n->parent != 0 && p_n->parent->left != p_n && p_n->parent->right != p_n) {
		fprintf(stdout, "%s", "Relationship error\n");
	}

}

// Function to print binary heap in 2D  
// It does reverse inorder traversal  
void print2DUtilBH(Node* root, int space)
{
	// Base case  
	if (root == NULL)
		return;

	// Increase distance between levels  
	space += ROWCOUNT;

	// Process right child first  
	print2DUtilBH(root->right, space);

	// Print current node after space  
	// count  
	fprintf(stdout, "%s", "\n");
	for (int i = ROWCOUNT; i < space; i++)
		fprintf(stdout, "%s", " ");;
	fprintf(stdout, "%d", root->key);

	// Process left child  
	print2DUtilBH(root->left, space);
}

// Wrapper over print2DUtilBH()  
void print2DBH(Node* root)
{
	// Pass initial space count as 0  
	print2DUtilBH(root, 0);
}

/*********************************************LOCAL FUNCTIONS********************************************/