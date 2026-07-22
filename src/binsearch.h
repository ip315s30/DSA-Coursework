#pragma once
#include <stdio.h>
#include <stdlib.h>

int binsearch(void **src, int size, void *value, int (*comp)(const void *a, const void *b));
