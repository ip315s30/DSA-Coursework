#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

List **radixsort(List **src, int qcnt, int (*indx)(void *value, int pos), int pos);
