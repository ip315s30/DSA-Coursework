#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <conio.h>
#include "list.h"
#include "bintree.h"
#include "binsearch.h"
#include "compress.h"
#include "radixsort.h"

#define scanf_s scanf

typedef struct cperson	Person;
typedef struct cperfreq PersFreq;

typedef struct cperson {
	char		name[30];
	short		depnum;
	char		job[22];
	char		birday[10];
} Person;

typedef struct cperfreq{
	short depnum;
	int		size;
	List *persons;
} PersFreq;


BTree			*dbls2btrdep(List *list);
List			*dbdatesort(List **src);
int				dbaction(int value, const char *dbname);
int				dbmenu1();
int				dbmenu2();
int				dbmenu3();
int				dbmenu4();
Person		*dbnewprsn(char *name, short depnum, char *job, char *birday);
int				dbprsnyear(Person *person);
int				dbprtcodetb(List *fqtb);
void			dbprtperbtr(BTree	*tree);
void			dbprtprsn(Person *person);
void			dbprtprsnlsnd(ListNode *node);
List			*readdb(const char *filename);
List			*dbyearfltr(void **src, int size, int year);

