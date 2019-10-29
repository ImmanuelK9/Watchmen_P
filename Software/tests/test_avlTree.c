/*********************************************************************************************************
*                                                FOLDER (e.g. uC/LIB)
*                                        DESCRIPTION (e.g. CUSTOM LIBRARY MODULES)
*
*													TESTS FOR AVL TREE
*
* Filename		: test_bnrHeap.c
* Version		: ?
* Programmer(s)	: Watchmen_P
*
* Note(s) :
*********************************************************************************************************/


/********************************************INCLUDE FILES***********************************************/
#include "test_avlTree.h"
#include <stdio.h>
#include <stdlib.h>

#define ROWCOUNT 5

/********************************************LOCAL TABLES************************************************/
CPU_INT32U keysBH[] = { 0,47,17,0,6,46,5,2,3,7,9,17,46,13,54,97,64,84,56,45,91,76,35,42,38,24,75,34,93,33 };

/**************************************LOCAL FUNCTION PROTOTYPES*****************************************/
void print2DUtilAVL(Node* root, int space);

void testAVLScenario0(Node* a, Node* b) {
	Node* root = a;
	a->left = a->parent = a->right = 0;
	a->key = 6;
	a->nodes_number = 1;
	a->tree = AVLTREE;

	b->key = 5;
	b->tree = AVLTREE;
	root = insert(a, b);
	print2DAVL(a);
	testAVL(a);
	fprintf(stdout, "\n a is %d \n", a->key);
	fprintf(stdout, "\n b is %d \n", b->key);

	deleteNode(b);
	print2DAVL(a);
	testAVL(a);
}

Node* testBHScenario1(Node* x, CPU_INT32U k) {
	Node* currentRoot = x;
	x->key = 5;
	x->nodes_number = 1;
	x->tree = AVLTREE;
	for (int i = 0; i < k; ++i) {
		x[i + 1].key = keysBH[i];
		x[i + 1].tree = AVLTREE;
		currentRoot = insert(currentRoot, x + i + 1);
	}
	testAVL(currentRoot);
	print2DAVL(currentRoot);
	return currentRoot;
}

void testAVLScenario2(Node* x, CPU_INT32U k) {
	Node* currentRoot = testAVLScenario1(x, k);

	fprintf(stdout, "\n deleting %d \n", (x + 2)->key);

	currentRoot = deleteNode(x + 2);
	testAVL(currentRoot);
	print2DAVL(currentRoot);

	fprintf(stdout, "\n inserting %d \n", (x + 2)->key);

	currentRoot = insert(currentRoot, x + 2);
	testAVL(currentRoot);
	print2DAVL(currentRoot);

	fprintf(stdout, "\n deleting %d \n", (x + 4)->key);

	currentRoot = deleteNode(x + 4);
	testAVL(currentRoot);
	print2DAVL(currentRoot);

	fprintf(stdout, "\n deleting %d \n", (x + 1)->key);

	currentRoot = deleteNode(x + 1);
	testAVL(currentRoot);
	print2DAVL(currentRoot);

	//currentRoot = insert(currentRoot, x + 2);
}

void testAVL(Node* p_n) {
	testAVLProperties(p_n);
	testAVLRelationship(p_n);
	fprintf(stdout, "%s", "Test finished\n");
}

/******************************************testSearchTree()**********************************************
 * Description	: Tests the AVL tree properties that .... (to be filled in)
 * Notes        :
 *********************************************************************************************************/
void testAVLProperties(Node* p_n) {
	if (p_n != 0) {
		if (0 != p_n->left) {
			if (cmpKey(p_n, p_n->left) > 0) fprintf(stdout, "%s", "BH Property error\n");
			testAVLProperties(p_n->left);
		}
		if (0 != p_n->right) {
			if (cmpKey(p_n, p_n->right) > 0) fprintf(stdout, "%s", "BH Property error\n");
			testAVLProperties(p_n->right);
		}
	}
}

void testAVLRelationship(Node* p_n) {
	if (p_n == 0) return;
	if (p_n->left != 0) {
		if (p_n->left->parent != p_n) {
			fprintf(stdout, "%s", "Relationship error\n");
		}
		testAVLRelationship(p_n->left);
	}
	if (p_n->right != 0) {
		if (p_n->right->parent != p_n) {
			fprintf(stdout, "%s", "Relationship error\n");
		}
		testAVLRelationship(p_n->right);
	}
	if (p_n->parent != 0 && p_n->parent->left != p_n && p_n->parent->right != p_n) {
		fprintf(stdout, "%s", "Relationship error\n");
	}

}

// Function to print AVL tree in 2D  
// It does reverse inorder traversal  
void print2DUtilAVL(Node* root, int space)
{
	// Base case  
	if (root == NULL)
		return;

	// Increase distance between levels  
	space += ROWCOUNT;

	// Process right child first  
	print2DUtilAVL(root->right, space);

	// Print current node after space  
	// count  
	fprintf(stdout, "%s", "\n");
	for (int i = ROWCOUNT; i < space; i++)
		fprintf(stdout, "%s", " ");;
	fprintf(stdout, "%d", root->key);

	// Process left child  
	print2DUtilAVL(root->left, space);
}

// Wrapper over print2DUtilBH()  
void print2DAVL(Node* root)
{
	// Pass initial space count as 0  
	print2DUtilAVL(root, 0);
}

/*********************************************LOCAL FUNCTIONS********************************************/