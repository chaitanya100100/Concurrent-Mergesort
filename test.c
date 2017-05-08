#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int main()
{
  FILE * fp = fopen("input.txt", "w");
  int n=10000000;
  int limit = 1e7;

  fprintf(fp, "%d\n", n);
  for(int i=0; i<n; i++)
    fprintf(fp, "%d\n", rand()%limit);
  fclose(fp);
  return 0;
}
