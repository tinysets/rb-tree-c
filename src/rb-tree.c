#include <stdio.h>
#include "rb-tree.h"

static void post_order_free(RBNode* root) {
	if (root) {
		post_order_free(root->left);
		post_order_free(root->right);
		free(root);
	}
}

void freeRBTree(RBTree* tree) {
	if (tree) {
		post_order_free(tree->root);
		free(tree);
	}
}

static int _rb_check(RBTree* tree, RBNode* root, int blackNodeCount, int currBlackNodeCount) {
	if (root == NULL) {
		return 1;
	}

	RBNode* p = root->parent;

	if (root->color == RED && p->color == RED) {
		printf("存在连续红结点\n");
		return 0;// 连续红结点
	}

	if (p) {
		if (root == p->left) {
			if (!(root->key < p->key)) {
				printf("不是bst\n");
				return 0;
			}
		}
		else {
			if (!(root->key > p->key)) {
				printf("不是bst\n");
				return 0;
			}
		}
	}

	if (root->color == BLACK) {
		currBlackNodeCount++;
	}

	if (root->left == NULL || root->right == NULL) {
		if (currBlackNodeCount != blackNodeCount) {
			printf("黑色节点数不一致\n");
			return 0;
		}
	}

	return _rb_check(tree, root->left, blackNodeCount, currBlackNodeCount)
		&& _rb_check(tree, root->right, blackNodeCount, currBlackNodeCount);
}

int rb_check(RBTree* tree) {
	if (tree->root == NULL) {
		return 1;
	}

	if (tree->root->color == RED) {
		printf("根节点为红色\n");
		return 0;
	}

	RBNode* curr = tree->root;
	int blackNodeCount = 0;
	while (curr)
	{
		if (curr->color == BLACK) {
			blackNodeCount++;
		}
		curr = curr->left;
	}

	return _rb_check(tree, tree->root, blackNodeCount, 0);
}


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
	int node_is_left_node = p ? is_left_node(node) : 0;

	RBNode* right = node->right;
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
							p->color = BLACK;
							pp->color = RED;
							right_rotate(tree, pp);
							return;
						}
						else {// case 2
							left_rotate(tree, p);
							insert_balance(tree, p);
							return;
						}
					}
					else {
						if (!is_left_node(node)) {// case 3
							p->color = BLACK;
							pp->color = RED;
							left_rotate(tree, pp);
							return;
						}
						else {// case 4
							right_rotate(tree, p);
							insert_balance(tree, p);
							return;
						}
					}
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
		else {// 已经存在
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

static void delete_balance(RBTree* tree, RBNode* x, RBNode* p) {
	if (x && x == tree->root) {
		x->color = BLACK;
		return;
	}

	if (x == p->left) {
		RBNode* s = p->right;
		RBNode* sl = s->left;
		RBNode* sr = s->right;
		if (s->color == BLACK) {
			if (p->color == RED) {
				// case 1
				if (is_black_or_null(sl) && is_black_or_null(sr)) {
					// case 1.1
					p->color = BLACK;
					s->color = RED;
					return;
				}
				else if(sr && sr->color == RED){
					// case 1.2
					p->color = BLACK;
					sr->color = BLACK;
					s->color = RED;
					left_rotate(tree,p);
					return;
				}
				else {
					// case 1.3
					sl->color = BLACK;
					s->color = RED;
					right_rotate(tree,s);// to case 1.2
					delete_balance(tree, x, p);
					return;
				}
			}
			else {
				// case 2
				if (is_black_or_null(sl) && is_black_or_null(sr)) {
					// case 2.1
					s->color = RED;
					delete_balance(tree,p,p->parent); // one more time delete_balance
					return;
				}
				else if(sr && sr->color == RED) {
					// case 2.2
					p->color = BLACK;
					sr->color = BLACK;
					s->color = BLACK;
					left_rotate(tree, p);
					return;
				}
				else {
					// case 2.3
					sl->color = BLACK;
					s->color = RED;
					right_rotate(tree, s);// to case 2.2
					delete_balance(tree, x, p);
					return;
				}
			}
		}
		else
		{
			// case 3
			p->color = RED;
			s->color = BLACK;
			left_rotate(tree, p);// to case 1
			delete_balance(tree, x, p);
			return;
		}
	}
	else {
		RBNode* s = p->left;
		RBNode* sl = s->left;
		RBNode* sr = s->right;

		if (s->color == BLACK) {
			if (p->color== RED) {
				// case 1
				if (is_black_or_null(sl) && is_black_or_null(sr)) {
					// case 1.1
					p->color = BLACK;
					s->color = RED;
					return;
				}
				else if(sl && sl->color == RED) {
					// case 1.2
					p->color = BLACK;
					sl->color = BLACK;
					s->color = RED;
					right_rotate(tree, p);
					return;
				}
				else {
					// case 1.3
					sr->color = BLACK;
					s->color = RED;
					left_rotate(tree,s);// to case 1.2
					delete_balance(tree,x,p);
					return;
				}
			}
			else {
				// case 2
				if (is_black_or_null(sl) && is_black_or_null(sr)) {
					// case 2.1
					s->color = RED;
					delete_balance(tree,p,p->parent);// one more time delete_balance
					return;
				}
				else if (sl && sl->color == RED) {
					// case 2.2
					p->color = BLACK;
					s->color = BLACK;
					sl->color = BLACK;
					right_rotate(tree, p);
					return;
				}
				else {
					// case 2.3
					sr->color = BLACK;
					s->color = RED;
					left_rotate(tree, s);// to case 2.2
					delete_balance(tree, x, p);
					return;
				}
			}
		}
		else {
			// case 3
			p->color = RED;
			s->color = BLACK;
			right_rotate(tree, p);// to case 1
			delete_balance(tree, x, p);
			return;
		}
	}
}

static void rb_delete_node(RBTree* tree, RBNode* root) {
	RBNode* p = root->parent;
	if (!root->left && !root->right) {
		if (root->color == BLACK) {
			// case 1.1
			if (!p) {
				free(root);
				tree->root = NULL;
				return;
			}
			else {
				// parent must be exist,and sibling must be exist
				if (is_left_node(root)) {
					free(root);
					p->left = NULL;
					delete_balance(tree, p->left, p);
					return;
				}
				else {
					free(root);
					p->right = NULL;
					delete_balance(tree, p->right, p);
					return;
				}
			}
		}
		else {
			// case 1.2
			// parent must be exist
			if (is_left_node(root)) {
				free(root);
				p->left = NULL;
			}
			else {
				free(root);
				p->right = NULL;
			}
			return;
		}
	}
	else if ((root->left && !root->right) || (!root->left && root->right)) {
		if (root->left) { // case 2.1
			root->key = root->left->key;
			free(root->left);
			root->left = NULL;
		}
		else {// case 2.2
			root->key = root->right->key;
			free(root->right);
			root->right = NULL;
		}
		return;
	}
	else {
		// case 3
		RBNode* prev_node = prev(root);
		root->key = prev_node->key;
		rb_delete_node(tree, prev_node);
		return;
	}
}

void rb_delete(RBTree* tree, int key) {
	RBNode* root = tree->root;

	while (root)
	{
		if (key < root->key) {
			root = root->left;
		}
		else if (key > root->key) {
			root = root->right;
		}
		else {
			rb_delete_node(tree, root);
			break;
		}
	}
	if (tree->root) {
		tree->root->color = BLACK;
	}
}