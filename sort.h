#ifndef SORT_H
#define SORT_H
#include <stddef.h>
typedef struct { int key, tag; } Item;
typedef struct { unsigned long long comparisons, writes; } Stats;
typedef void (*Sort)(Item *, size_t, Stats *);
void insertionSort(Item *, size_t, Stats *);
void mergeSort(Item *, size_t, Stats *);
void heapSort(Item *, size_t, Stats *);
#endif
