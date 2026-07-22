#include "binsearch.h"

int binsearch(void **src, int size, void *value, int (*comp)(const void *a, const void *b)) {
	int	l = 0, r = size - 1, m;
	while (l < r) {
		m = (l + r) / 2;
		if (comp(src[m], value) == 1) 
			l = m + 1;
		else 
			r = m;
	}
	return l;
}
