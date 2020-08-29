#pragma once
#include <stdlib.h>

enum RBColor
{
	RED,
	BLACK
};


typedef struct RBNode
{
	int key;
	struct RBNode* parent;
	struct RBNode* left;
	struct RBNode* right;
	enum RBColor color;
} RBNode;

typedef struct RBTree
{
	RBNode* root;
}RBTree;

RBNode* newRBNode(int key);
RBTree* newRBTree();
void freeRBTree(RBTree* tree);

int rb_check(RBTree* tree);

void rb_init(RBTree* tree,int arr[],int count);
void rb_insert(RBTree* tree, int key);
void rb_delete(RBTree* tree, int key);