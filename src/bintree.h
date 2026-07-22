#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct cbtrnode	BTreeNode;
typedef struct cbtree		BTree;

typedef struct cbtrnode {
	void			*value;
	BTreeNode	*left;
	BTreeNode	*right;
} BTreeNode;

typedef struct cbtree {
	BTreeNode	*root;
	int				(*comp)(BTreeNode *a, BTreeNode *b);
} BTree;

BTree			*btrnew			();
BTreeNode *btrnewnd		(void *value);
void			bfsact			(BTreeNode *root, int (*action)(BTreeNode *node));
BTreeNode *bfsfind		(BTree *tree, BTreeNode *node);
BTreeNode *btrinsertnd(BTree *tree, BTreeNode *node);