#include "rb-tree.h"

RBNode* newRBNode(int key) {
	RBNode* node = (RBNode*)malloc(sizeof(RBNode));
	node->key = key;
	node->color = RED;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	return node;
}

RBTree* newRBTree() {
	RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
	tree->root = NULL;
	return tree;
}


static RBNode* sibling(RBNode* node) {
	if (node) {
		RBNode* p = node->parent;
		if (p) {
			if (p->left == node) {
				return p->right;
			}
			else if (p->right == node) {
				return p->left;
			}
			else {
				return NULL;
			}
		}
	}
	return NULL;
}

static int is_left_node(RBNode* node) {
	return node->parent->left == node ? 1 : 0;
}

static int is_black_or_null(RBNode* node) {
	return (node == NULL || node->color == BLACK) ? 1 : 0;
}

static RBNode* prev(RBNode* node) {
	RBNode* prev = node->left;
	while (prev->right) {
		prev = prev->right;
	}
	return prev;
}

static RBNode* left_rotate(RBTree* tree, RBNode* node) {
	RBNode* p = node->parent;
	int node_is_left_node = p ? is_left_node(p) : 0;

	RBNode* right = p->right;
	RBNode* right_left = right->left;

	right->left = node;
	right->left->parent = right;

	node->right = right_left;
	if (node->right) {
		node->right->parent = node;
	}

	if (p) {
		if (node_is_left_node) {
			p->left = right;
			p->left->parent = p;
		}
		else {
			p->right = right;
			p->right->parent = p;
		}
	}
	else {
		tree->root = right;
		tree->root->parent = NULL;
	}

	return right;
}

static RBNode* right_rotate(RBTree* tree, RBNode* node) {
	RBNode* p = node->parent;
	int node_is_left_node = p ? is_left_node(node) : 0;

	RBNode* left = node->left;
	RBNode* left_right = left->right;

	left->right = node;
	left->right->parent = left;

	node->left = left_right;
	if (node->left) {
		node->left->parent = node;
	}

	if (p) {
		if (node_is_left_node) {
			p->left = left;
			p->left->parent = p;
		}
		else {
			p->right = left;
			p->right->parent = p;
		}
	}
	else {
		tree->root = left;
		tree->root->parent = NULL;
	}
	return left;
}


void rb_init(RBTree* tree, int arr[], int count) {
	for (int i = 0; i < count; i++)
	{
		rb_insert(tree, arr[i]);
	}
}

static void insert_balance(RBTree* tree, RBNode* node) {
	if (node->color == RED) {
		RBNode* p = node->parent;
		if (p) {
			if (p->color == RED) {
				RBNode* pp = p->parent;
				RBNode* uncle = sibling(p);
				if (is_black_or_null(uncle)) {
					if (is_left_node(p)) {
						if (is_left_node(node)) {// case 1

						}
						else {// case 2

						}
					}
					else {
						if (!is_left_node(node)) {// case 3

						}
						else {// case 4

						}
					}
					// case 1
				}
				else {
					pp->color = RED;
					p->color = uncle->color = BLACK;
					insert_balance(tree, pp);
					return;
				}
			}
		}
		else {// root
			node->color = BLACK;
		}
	}
}

static void _rb_insert(RBTree* tree, int key) {
	RBNode* root = tree->root;
	while (1)
	{
		if (key < root->key) {
			if (root->left) {
				root = root->left;
			}
			else {
				root->left = newRBNode(key);
				root->left->parent = root;
				insert_balance(tree, root->left);
				return;
			}
		}
		else if (key > root->key) {
			if (root->right) {
				root = root->right;
			}
			else {
				root->right = newRBNode(key);
				root->right->parent = root;
				insert_balance(tree, root->right);
				return;
			}
		}
		else {// �Ѿ�����
			return;
		}
	}
}

void rb_insert(RBTree* tree, int key) {
	if (tree->root == NULL) {
		tree->root = newRBNode(key);
	}
	else {
		_rb_insert(tree, key);
	}
	tree->root->color = BLACK;
}

void rb_delete(RBTree* tree, int key) {

}