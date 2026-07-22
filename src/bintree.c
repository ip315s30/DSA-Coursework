#include "bintree.h"

BTree *btrnew()
{
	BTree *retval = malloc(sizeof(BTree));
	retval->root = NULL;
	retval->comp = NULL;
	return retval;
}

BTreeNode *btrnewnd(void *value)
{
	BTreeNode *retval = malloc(sizeof(BTreeNode));
	retval->value = value;
	retval->left	=	NULL;
	retval->right	= NULL;
	return retval;
}

void bfsact(BTreeNode *root, int (*action)(BTreeNode *node))
{
	if (root) {
		bfsact(root->left, action);
		action(root);
		bfsact(root->right, action);
	}
}

BTreeNode *bfsfind(BTree *tree, BTreeNode *node)
{
	BTreeNode *cur = tree->root;
	if (!cur)
		return 0;
	while (1) {
		if (tree->comp(node, cur) == 1) {
			if (cur->right)
				cur = cur->right;
			else 
				return 0;
		} else if (tree->comp(node, cur) == -1) {
			if (cur->left)
				cur = cur->left;
			else 
				return 0;
		} else 
			return cur;
	}
}


BTreeNode *btrinsertnd(BTree *tree, BTreeNode *node)
{
	BTreeNode *cur = tree->root;
	if (!cur) {
		tree->root = node;
		return node;
	}
	while (1) {
		if (tree->comp(node, cur) == 1) {
			if (cur->right)
				cur = cur->right;
			else {
				cur->right = node;
				break;
			}
		} else if (tree->comp(node, cur) == -1) {
			if (cur->left)
				cur = cur->left;
			else {
				cur->left = node;
				break;
			}
		}
	}
	return node;
}

