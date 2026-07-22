#include "compress.h"

char *double2bin(double a, char size)
{
	char* retval = malloc(sizeof(char) * (size + 1));
	for (int i = 0; i < size + 1; i++) {
		if (a > 1) {
			retval[i] = '1';
			a -= 1;
		} else 
			retval[i] = '0';
		a *= 2.0;
	}
	retval[0] = size;
	return retval;
}

List *freqtable(const char *filename)
{
	List			*retval = lsnew();
	FILE			*fp			= fopen(filename, "r");
	ListNode	*tmplnd;
	SymbFreq	*nwsym;
	int			sz		= 0;
	double	sumfq	=	0;
	char		c;
	double	symfq;
	double	q;
	char		*nwcode;
	while ((c = getc(fp)) != EOF) {
		tmplnd = retval->front;
		while (tmplnd && ((SymbFreq* )tmplnd->value)->symb != c)
			lsnxnd(&tmplnd);
		if (tmplnd)
			((SymbFreq* )tmplnd->value)->count++;
		else {
			nwsym = malloc(sizeof(SymbFreq));
			nwsym->count = 1;
			nwsym->symb = c;
			nwsym->freq = -1.f;
			nwsym->code = NULL;
			lspushback(retval, nwsym);
		}
		sz++;
	}
	for (tmplnd = retval->front; tmplnd; lsnxnd(&tmplnd)) {
		symfq = (double)((SymbFreq*)tmplnd->value)->count / (double)sz;
		q = sumfq + symfq / 2.0;
		nwcode = double2bin(q, (char)ceil(-log2(symfq)) + 1);
		((SymbFreq*)tmplnd->value)->code = nwcode;
		((SymbFreq*)tmplnd->value)->freq = symfq;
		sumfq += symfq;
	}
	return retval;
}
