#pragma once
#include <stdio.h>
#include <math.h>
#include "list.h"

typedef struct csymbfreq SymbFreq;

typedef struct csymbfreq {
	char		symb;
	int			count;
	double	freq;
	char		*code; // code view: {code size}code. example: 501001
} SymbFreq;


char *double2bin(double a, char size);
List *freqtable(const char *filename);
