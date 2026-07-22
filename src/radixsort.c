#include "radixsort.h"

List **radixsort(List **src, int qcnt, int (*indx)(void *value, int pos), int pos)	// radix sort {source, size, queue count, index function}
{
	List **queues = malloc(sizeof(List*) * qcnt);
	int i, j, k;
	for (i = 0; i < qcnt; i++)
		queues[i] = lsnew();
	for (;(*src)->size;) {
		lspushback(queues[indx((*src)->front->value, pos)], (*src)->front->value);
		lspopfront(*src);
	}
	for (i = 0, j = 0; i < qcnt; i++) 
		lsconnect(src, &queues[i]);
	return src;
}
