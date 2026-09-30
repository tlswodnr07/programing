#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
static Sort sorts[]={insertionSort,mergeSort,heapSort};
static const char *names[]={"insertion","merge","heap"};
static uint32_t state=20260930;
static uint32_t rng(void) { state^=state<<13; state^=state>>17; state^=state<<5; return state; }
static void input(Item *a,size_t n,int shape) {
    state=20260930u+(uint32_t)n;
    for(size_t i=0;i<n;i++) { a[i].key=shape==0?(int)(rng()%100000):shape==1?(int)i:shape==2?(int)(n-i):shape==3?(int)(rng()%10):7; a[i].tag=(int)i; }
}
static int cmp(const void *a,const void *b) { int x=((const Item*)a)->key,y=((const Item*)b)->key; return (x>y)-(x<y); }
static int valid(Item *a,Item *reference,size_t n) { for(size_t i=0;i<n;i++) if(a[i].key!=reference[i].key) return 0; return 1; }
static int stable(Item *a,size_t n) { for(size_t i=1;i<n;i++) if(a[i-1].key==a[i].key && a[i-1].tag>a[i].tag) return 0; return 1; }
static double now(void) { return (double)clock()/CLOCKS_PER_SEC; }
int main(int argc,char **argv) {
    if(argc>1 && strcmp(argv[1],"--test")==0) {
        int checks=0;
        for(size_t n=0;n<=200;n++) for(int shape=0;shape<5;shape++) {
            Item a[201],b[201],ref[201]; input(a,n,shape); memcpy(ref,a,n*sizeof(Item)); qsort(ref,n,sizeof(Item),cmp);
            for(int k=0;k<3;k++) { memcpy(b,a,n*sizeof(Item)); Stats s={0}; sorts[k](b,n,&s); checks++;
                if(!valid(b,ref,n) || (k<2 && !stable(b,n))) { fprintf(stderr,"FAIL n=%zu shape=%d sort=%s\n",n,shape,names[k]); return 1; }
            }
        }
        printf("%d checks, 0 failures\n",checks); return 0;
    }
    const char *shapes[]={"random","sorted","reverse","duplicates","equal"};
    size_t sizes[]={1000,2000,4000,8000};
    printf("n,shape,algorithm,median_ms,comparisons,moves,stable_observed,buffer_bytes\n");
    for(int z=0;z<4;z++) for(int shape=0;shape<5;shape++) {
        size_t n=sizes[z]; Item *a=malloc(n*sizeof(Item)),*b=malloc(n*sizeof(Item)),*ref=malloc(n*sizeof(Item));
        if(!a||!b||!ref) return 1;
        input(a,n,shape); memcpy(ref,a,n*sizeof(Item)); qsort(ref,n,sizeof(Item),cmp);
        for(int k=0;k<3;k++) { double times[5]; Stats s={0}; int st=1;
            for(int r=-1;r<5;r++) { memcpy(b,a,n*sizeof(Item)); s=(Stats){0}; double t=now(); sorts[k](b,n,&s); double elapsed=(now()-t)*1000;
                if(!valid(b,ref,n)) return 2;
                st=stable(b,n); if(r>=0) times[r]=elapsed;
            }
            for(int i=1;i<5;i++) for(int j=i;j>0 && times[j]<times[j-1];j--) { double t=times[j];times[j]=times[j-1];times[j-1]=t; }
            printf("%zu,%s,%s,%.6f,%llu,%llu,%d,%zu\n",n,shapes[shape],names[k],times[2],s.comparisons,s.writes,st,k==1?n*sizeof(Item):0);
        }
        free(a);free(b);free(ref);
    }
    return 0;
}
