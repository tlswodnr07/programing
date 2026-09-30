#include "sort.h"
#include <stdlib.h>
#include <stdio.h>
static int less(Item a, Item b, Stats *s) { s->comparisons++; return a.key < b.key; }
static void swap(Item *a, Item *b, Stats *s) { Item t=*a; *a=*b; *b=t; s->writes+=3; }
void insertionSort(Item *a,size_t n,Stats *s) {
    for(size_t i=1;i<n;i++) { Item v=a[i]; s->writes++; size_t j=i;
        while(j && less(v,a[j-1],s)) { a[j]=a[j-1]; s->writes++; j--; }
        a[j]=v; s->writes++;
    }
}
static void merge_range(Item *a,Item *b,size_t lo,size_t hi,Stats *s) {
    if(hi-lo<2) return;
    size_t mid=lo+(hi-lo)/2; merge_range(a,b,lo,mid,s); merge_range(a,b,mid,hi,s);
    size_t i=lo,j=mid,k=lo;
    while(i<mid && j<hi) { b[k++]=less(a[j],a[i],s)?a[j++]:a[i++]; s->writes++; }
    while(i<mid) { b[k++]=a[i++]; s->writes++; }
    while(j<hi) { b[k++]=a[j++]; s->writes++; }
    for(k=lo;k<hi;k++) { a[k]=b[k]; s->writes++; }
}
void mergeSort(Item *a,size_t n,Stats *s) {
    if(n<2) return;
    Item *b=malloc(n*sizeof(*b));
    if(!b) { perror("malloc"); exit(1); }
    merge_range(a,b,0,n,s); free(b);
}
static void sink(Item *a,size_t root,size_t n,Stats *s) {
    while(root<n/2) { size_t child=root*2+1;
        if(child+1<n && less(a[child],a[child+1],s)) child++;
        if(!less(a[root],a[child],s)) break;
        swap(&a[root],&a[child],s); root=child;
    }
}
void heapSort(Item *a,size_t n,Stats *s) {
    for(size_t i=n/2;i>0;i--) sink(a,i-1,n,s);
    for(size_t end=n;end>1;end--) { swap(&a[0],&a[end-1],s); sink(a,0,end-1,s); }
}
