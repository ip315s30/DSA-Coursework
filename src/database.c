#include "database.h" 


static int __comp2perbydep(const void *a, const void *b)
{
	Person arg1 = *(Person *)a;
	Person arg2 = *(Person *)b;

	if (arg1.depnum < arg2.depnum) return 1;
	if (arg1.depnum > arg2.depnum) return -1;
	return 0;
}

static int __comp2freqnd(const void *a, const void *b)
{
	PersFreq arg1 = *(PersFreq *)a;
	PersFreq arg2 = *(PersFreq *)b;

	if (arg1.size < arg2.size) return 1;
	if (arg1.size > arg2.size) return -1;
	return 0;
}

static int __comp2btrndbydep(BTreeNode *a, BTreeNode *b)
{
	int arg1 = ((PersFreq *)(a->value))->depnum;
	int arg2 = ((PersFreq *)(b->value))->depnum;

	if (arg1 < arg2) return -1;
	if (arg1 > arg2) return 1;
	return 0;
}

static int __comp4binsearch(const void *a, const void *b)
{
	int arg1 = dbprsnyear((Person*) a);
	int arg2 = *(int*)b;

	if (arg1 < arg2) return 1;
	if (arg1 > arg2) return -1;
	return 0;
}

static int __yearindex(void *value, int pos)
{
	Person arg1 = *(Person*)value;
	return arg1.birday[pos] - '0';
}

static int __btraction(BTreeNode *node)
{
	PersFreq	*freqnd = (PersFreq*)node->value;
	ListNode	*tmp;
	printf("Depart: %i, count: %i\n", freqnd->depnum, freqnd->size);
	lsmap(freqnd->persons->front, freqnd->persons->back, freqnd->size, dbprtprsnlsnd);
	return 0;
}

Person *dbnewprsn(char *name, short depnum, char *job, char *birday)
{
	Person *retval = (Person*)malloc(sizeof(Person));
	memcpy(retval->name, name, 30 * sizeof(char));
	retval->depnum = depnum;
	memcpy(retval->job, job, 22 * sizeof(char));
	memcpy(retval->birday, birday, 10 * sizeof(char));
	return retval;
}

void dbprtperbtr(BTree *tree)
{
	bfsact(tree->root, __btraction);
}

void dbprtprsn(Person *person)
{
	printf("%s\t%03hi\t%s\t%s", person->name, person->depnum, person->job, person->birday);
}

void dbprtprsnlsnd(ListNode *node)
{
	Person *prsn = ((Person*)node->value);
	dbprtprsn(prsn);
	printf("\n");
}

int dbprsnyear(Person* person) {
	return (person->birday[6] - '0') * 10 + person->birday[7] - '0';
}

List *readdb(const char *filename)
{
	List	*retval = lsnew();
	FILE	*fp = fopen(filename, "r");
	char	buffer[64];
	char	c;
	short	pos;
	while (fread(buffer, sizeof(char), 64, fp) == 64) 
		lspushback(retval, dbnewprsn(buffer, *((short*)(buffer + 30)), buffer + 32, buffer + 54));
	fclose(fp);
	return retval;
}

List *dbyearfltr(void **src, int size, int year)
{
	List	*retval	= lsnew();
	int		pos;
	int		i;
	pos = binsearch(src, size, &year, __comp4binsearch);
	for (; pos < size && dbprsnyear(src[pos]) == year; pos++)
		lspushback(retval, src[pos]);
	return retval;
}

List *dbdatesort(List **src)
{
	int i, j;
	for (i = 0; i < 3; i++) 
		for (j = 0; j < 2; j++) 
			radixsort(src, 10, __yearindex, 3 * i + 1 - j);
	return *src;
}

BTree *dbls2btrdep(List *list)	// list to binary tree by dep
{
	BTree			*retval	= btrnew();
	Person		*perarr	= calloc(list->size, sizeof(Person));
	ListNode	*cur;
	PersFreq	*freqarr;
	PersFreq	*newfreq;
	int	depcnt = 1;
	int i, j;
	cur = list->front;
	if (!list->size)
		return retval;
	for (i = 0; cur; i++, lsnxnd(&cur)) 
		memcpy(&perarr[i], ((Person*)(cur->value)), sizeof(Person));
	qsort(perarr, list->size, sizeof(Person), __comp2perbydep);
	for (i = 0; i < list->size - 1; i++)
		if (perarr[i].depnum - perarr[i + 1].depnum)
			depcnt++;
	freqarr = calloc(depcnt, sizeof(PersFreq));
	newfreq = malloc(sizeof(PersFreq));
	newfreq->depnum = -1;
	newfreq->size = 0;
	memcpy(&freqarr[0], newfreq, sizeof(PersFreq));
	for (i = 0, j = -1; i < list->size; i++) {
		if (newfreq->depnum - perarr[i].depnum) {
			newfreq->depnum		= perarr[i].depnum;
			newfreq->persons	= lsnew();
			j++;
			memcpy(&freqarr[j], newfreq, sizeof(PersFreq));
		} 
		lspushback(freqarr[j].persons, &perarr[i]);
		freqarr[j].size++;
	}
	qsort(freqarr, depcnt, sizeof(PersFreq), __comp2freqnd);
	retval->comp = __comp2btrndbydep;
	for (i = 0; i < depcnt; i++) 
		btrinsertnd(retval, btrnewnd(&freqarr[i]));
	free(newfreq);
	return retval;
}

int dbmenu1()
{
	char usrin;
	printf("\033[2J\033[1;1H");
	printf("To perform an action, enter its code and press Enter\n");
	printf("1.\tLoad database to list\n");
	printf("2.\tPrint Hilber-Moore code table for db\n");
	printf("3.\tQuit\n");
	scanf_s("%c", &usrin);
	getchar();
	usrin = ((usrin == '3') ? 0 : (usrin - '0' + 10));
	return usrin;
}

int dbmenu2()
{
	char usrin;
	printf("\033[2J\033[1;1H");
	printf("To perform an action, enter its code and press Enter\n");
	printf("1.\tPrint all raw table\n");
	printf("2.\tPrint all ordered table\n");
	printf("3.\tPrint raw table per 20 person\n");
	printf("4.\tPrint ordered table per 20 person\n");
	printf("5.\tFind person by year of birth and make queue\n");
	printf("6.\tQuit\n");
	scanf_s("%c", &usrin);
	getchar();
	usrin = usrin - '0' + 20;
	return usrin;
}

int dbmenu3()
{
	char usrin;
	printf("\033[2J\033[1;1H");
	printf("To perform an action, enter its code and press Enter\n");
	printf("1.\tPrint person list\n");
	printf("2.\tBuild tree by depart number\n");
	printf("3.\tQuit\n");
	scanf_s("%c", &usrin);
	getchar();
	usrin = usrin - '0' + 30;
	return usrin;
}

int dbmenu4()
{
	char usrin;
	printf("\033[2J\033[1;1H");
	printf("To perform an action, enter its code and press Enter\n");
	printf("1.\tPrint tree\n");
	printf("2.\tFind depart\n");
	printf("3.\tQuit\n");
	scanf_s("%c", &usrin);
	getchar();
	usrin = usrin - '0' + 40;
	return usrin;
}


int dbprtcodetb(List *fqtb)
{
	double sumbefore = 0.0;
	double sumafter = 0.0;
	double entropy = 0.0;
	double avgword = 0.0;
	printf("Char\tCount\tFrequency\tCode size\tCode\n");
	printf("-----------------------------------------------------------------\n");
	for (ListNode *tmp = fqtb->front; tmp; lsnxnd(&tmp)) {
		SymbFreq sq = *(SymbFreq *)(tmp->value);
		sumbefore += sq.count;
		sumafter += sq.count * sq.code[0];
		avgword += sq.freq * sq.code[0];
		entropy -= sq.freq * log2(sq.freq);
		if (sq.symb == '\n')
			continue;
		printf("%c\t%i\t%lf\t%i\t\t", sq.symb, sq.count, sq.freq, sq.code[0]);
		for (int j = 0; j < sq.code[0]; j++) {
			printf("%c", sq.code[j + 1]);
		}
		printf("\n");
	}
	printf("-----------------------------------------------------------------\n");
	printf("size before encoding:\t%i\n", (int)sumbefore);
	printf("size after encoding:\t%i\n", (int)(sumafter / 8));
	printf("compression ratio:\t%lf\n", sumbefore / (sumafter / 8.0));
	printf("message entropy:\t%lf\n", entropy);
	printf("avg word size:\t\t%lf\n", avgword);
	printf("symbols count:\t\t%i\n", fqtb->size);
	printf("-----------------------------------------------------------------\n");
	getchar();
	return 10;
}

int dbaction(int value, const char *dbname)
{
	List		*dbls;
	List		*orddbls;
	Person	**orddbarr;
	List		*yearls;
	Person	**indarr;
	BTree		*depbtr;


	dbls = lsnew();
	orddbls = lsnew();
	yearls = lsnew();
	depbtr = btrnew();
	while (value) {
		if (value == 10) {
			lsdelete(dbls);
			lsdelete(orddbls);
			dbls = readdb(dbname);
			orddbls = lscopy(dbls);
			dbdatesort(&orddbls);
			orddbarr = (Person**)ls2indarr(orddbls);
			value = dbmenu1();
		} else if (value == 11) {
			value = dbmenu2();
		} else if (value == 12) {
			List *hilmootb = freqtable(dbname);
			value = dbprtcodetb(hilmootb);
		} else if (value == 21) {
			printf("\033[2J\033[1;1H");
			printf("Name\t\t\t\tDepart\tJob\t\t\tBirthday\n");
			lsmap(dbls->front, dbls->back, dbls->size, dbprtprsnlsnd);
			getchar();
			value = dbmenu2();
		} else if (value == 22) {
			printf("\033[2J\033[1;1H");
			printf("Name\t\t\t\tDepart\tJob\t\t\tBirthday\n");
			lsmap(orddbls->front, orddbls->back, orddbls->size, dbprtprsnlsnd);
			getchar();
			value = dbmenu2();
		} else if (value == 23) {
			ListNode *tmp;
			char usrin;
			tmp = dbls->front;
			while (1) {
				printf("\033[2J\033[1;1H");
				printf("Name\t\t\t\tDepart\tJob\t\t\tBirthday\n");
				tmp = lsmap(tmp, dbls->back, 20, dbprtprsnlsnd);
				printf("For exit enter: q\n");
				scanf_s("%c", &usrin);
				if (usrin == 'q') {
					getchar();
					break;
				}
			}
			value = dbmenu2();
		} else if (value == 24) {
			ListNode *tmp;
			char usrin;
			tmp = orddbls->front;
			while (1) {
				printf("\033[2J\033[1;1H");
				printf("Name\t\t\t\tDepart\tJob\t\t\tBirthday\n");
				tmp = lsmap(tmp, orddbls->back, 20, dbprtprsnlsnd);
				printf("For exit enter: q\n");
				scanf_s("%c", &usrin);
				if (usrin == 'q') {
					getchar();
					break;
				}
			}
			value = dbmenu2();
		} else if (value == 25) {
			int			usrin;
			printf("\033[2J\033[1;1H");
			printf("Enter year of birth and press Enter: ");
			scanf_s("%i", &usrin);
			getchar();
			indarr = (Person**)ls2indarr(orddbls);
			yearls = dbyearfltr((void**)indarr, orddbls->size, usrin);
			if (!yearls->size) {
				printf("\033[2J\033[1;1H");
				printf("No person with this year of birth was found\n");
				lsdelete(yearls);
				getchar();
			} else {
				value = dbmenu3();
			}
		} else if (value == 26) {
			value = dbmenu1();
		} else if (value == 31) {
			printf("\033[2J\033[1;1H");
			printf("Name\t\t\t\tDepart\tJob\t\t\tBirthday\n");
			lsmap(yearls->front, yearls->back, yearls->size, dbprtprsnlsnd);
			getchar();
			value = dbmenu3();
		} else if (value == 32) {
			depbtr = dbls2btrdep(yearls);
			value = dbmenu4();
		} else if (value == 33) {
			value = dbmenu2();
		} else if (value == 41) {
			dbprtperbtr(depbtr);
			getchar();
			value = dbmenu4();
		} else if (value == 42) {
			int usrin;
			BTreeNode *findelem;;
			printf("\033[2J\033[1;1H");
			printf("Enter number of depart and press Enter: ");
			scanf_s("%i", &usrin);
			getchar();
			findelem = bfsfind(depbtr, &(BTreeNode){&(PersFreq){ usrin, 0, 0 }, 0, 0});
			if (findelem) {
				printf("\033[2J\033[1;1H");
				printf("Name\t\t\t\tDepart\tJob\t\t\tBirthday\n");
				List *prsns = ((PersFreq *)findelem->value)->persons;
				lsmap(prsns->front, prsns->back, prsns->size, dbprtprsnlsnd);
				getchar();
			} else {
				printf("\033[2J\033[1;1H");
				printf("Depart not found\n");
				getchar();
			}
			value = dbmenu4();
		} else if (value == 43) {
			lsdelete(yearls);
			value = dbmenu3();
		}
	}
}
