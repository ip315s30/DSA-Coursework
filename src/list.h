#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct clist		List;
typedef struct clinode	ListNode;

typedef struct clinode {
	void				*value;
	ListNode		*prev;
	ListNode		*next;
} ListNode;

typedef struct clist {
	int					size;
	ListNode		*front;
	ListNode		*back;
} List;

ListNode			*lsnewnd			(void *value);
List					*lsnew				();
List					*lscopy				(List *src);
List					**lsconnect		(List **a, List **b);
void					**ls2indarr		(List *src);
void					lsdelete			(List *src);
ListNode			*lspushbf			(List *list, ListNode *node, void *value);
ListNode			*lspushaf			(List *list, ListNode *node, void *value);
ListNode			*lspushback		(List *list, void *value);
ListNode			*lspushfront	(List *list, void *value);
ListNode			*lspopback		(List *list);
ListNode			*lspopfront		(List *list);
ListNode			*lsnxnd				(ListNode **node);
ListNode			*lsprnd				(ListNode **node);
ListNode			*lsmap				(ListNode *beg, ListNode *end, int count, void (*func)(ListNode *node));
ListNode			*lsoffset			(ListNode *beg, int offset);
