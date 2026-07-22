#include "list.h"

ListNode *lsnewnd(void *value)
{
	ListNode	*retval = (ListNode*)malloc(sizeof(ListNode));
	retval->value = value;
	retval->prev = NULL;
	retval->next = NULL;
	return retval;
}

List *lsnew()
{
	List	*retval = (List*)malloc(sizeof(List));
	retval->size = 0;
	retval->front = NULL;
	retval->back = NULL;
	return retval;
}

List *lscopy(List *src)
{
	List	*retval;
	ListNode *cur;
	retval	= lsnew();
	for (cur = src->front; cur; cur = cur->next) 
		lspushback(retval, cur->value);
	return retval;
}

List **lsconnect(List **a, List **b)
{
	if (!(*a)->size) {
		free(*a);
		*a = *b;
		printf("%i %i\n", (*a)->size, (*b)->size);
		return a;
	}
	if (!(*b)->size) {
		free(*b);
		return a;
	}
	(*a)->size += (*b)->size;
	(*a)->back->next = (*b)->front;
	(*b)->front->prev = (*a)->back;
	(*a)->back = (*b)->back;
	return a;
}

void **ls2indarr(List *src)
{
	void **retval;
	ListNode *cur;
	int i;
	retval = calloc(src->size, sizeof(void*));
	for (i = 0, cur = src->front; cur; i++, lsnxnd(&cur))
		retval[i] = cur->value;
	return retval;
}

void lsdelete(List *src)
{
	while (lspopfront(src));
	free(src);
}

ListNode *lspushbf(List *list, ListNode *node, void *value)
{
	ListNode *retval = lsnewnd(value);
	if (node) {
		retval->next = node;
		if (node->prev) {
			retval->prev = node->prev;
			node->prev->next = retval;
		}
		if (node == list->front)
			list->front = retval;
		node->prev = retval;
	}
	return retval;
}

ListNode *lspushaf(List *list, ListNode *node, void *value)
{
	ListNode *retval = lsnewnd(value);
	if (node) {
		retval->prev = node;
		if (node->next) {
			retval->next = node->next;
			node->next->prev = retval;
		}
		if (node == list->back)
			list->back = retval;
		node->next = retval;
	}
	return retval;
}

ListNode *lspushback(List *list, void *value)
{
	list->size++;
	if (list->front) 
		return lspushaf(list, list->back, value);
	else {
		list->front = lsnewnd(value);
		list->back = list->front;
		return list->front;
	}
}

ListNode *lspushfront(List* list, void *value)
{
	list->size++;
	if (list->front) 
		return lspushbf(list, list->front, value);
	else {
		list->front = lsnewnd(value);
		list->back = list->front;
		return list->front;
	}
}
ListNode* lspopback(List* list)
{
	if (!list->back)
		return 0;
	if (list->back->prev) {
		list->back = list->back->prev;
		free(list->back->next);
		list->back->next = NULL;
	} else {
		free(list->back);
		list->front = NULL;
		list->back = NULL;
	}
	list->size--;
	return list->back;
}

ListNode* lspopfront(List* list)
{
	if (!list->front)
		return 0;
	if (list->front->next) {
		list->front = list->front->next;
		free(list->front->prev);
		list->front->prev = NULL;
	} else {
		free(list->front);
		list->front = NULL;
		list->back = NULL;
	}
	list->size--;
	return list->front;
}

ListNode *lsnxnd(ListNode **node)
{
	*node = (*node)->next;
	return *node;
}

ListNode *lsprnd(ListNode **node)
{
	*node = (*node)->prev;
	return *node;
}

ListNode *lsmap(ListNode *beg, ListNode *end, int count, void (*func)(ListNode *node))
{
	int i;
	for (i = 0; beg && i < count; lsnxnd(&beg), i++) {
		func(beg);
		if (beg == end)
		 break;
	}
	return beg;
}

ListNode *lsoffset(ListNode *beg, int offset)
{
	int i;
	for (i = 0; beg && i < offset; i++)
		lsnxnd(&beg);
	return beg;
}
