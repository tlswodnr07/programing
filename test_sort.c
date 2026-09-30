#define main comparisonMain
#include "../src/main.c"
#undef main
int main(void) { char *args[]={"test_sort","--test"}; return comparisonMain(2,args); }
