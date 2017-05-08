#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/types.h>
#include <unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>

const int SELECTION_SORT_LIMIT = 5;


int * arr;
int sz;
int * a1;
int * a2;

void selection_sort(int * arr, int l, int r)
{
  int mi,temp;
  for(int i=l; i<=r; i++)
  {
    mi = i;
    for(int j=i+1; j<=r; j++)
      if(arr[mi]>arr[j])
        mi = j;

    temp = arr[mi];
    arr[mi] = arr[i];
    arr[i] = temp;
  }
}

void merge(int * arr, int l, int m, int r)
{
  int n1 = m-l+1;
  int n2 = r-m;
  int i,j,k;

  for(i=0; i<n1; i++)
    a1[i] = arr[l+i];
  for(j=0; j<n2; j++)
    a2[j] = arr[m+1+j];

  i=0;j=0;k=l;
  while(i<n1 && j<n2)
  {
    if(a1[i] < a2[j])
      arr[k++] = a1[i++];
    else
      arr[k++] = a2[j++];
  }
  while(i<n1)
    arr[k++] = a1[i++];
  while(j<n2)
    arr[k++] = a2[j++];
}

void mergeSort(int * arr, int l, int r)
{
  if(l>=r)return;

  if((r-l)<=SELECTION_SORT_LIMIT)
  {
    selection_sort(arr, l, r);
    return;
  }

  int m=(r-l)/2+l;
  mergeSort(arr, l, m);
  mergeSort(arr, m+1, r);
  merge(arr, l, m, r);
}

int main()
{
  scanf("%d",&sz);

  arr = malloc(sizeof(int)*(sz+5));
  a1 = malloc(sizeof(int)*(sz+5));
  a2 = malloc(sizeof(int)*(sz+5));

  for(int i=0; i<sz; i++)
    scanf("%d",&arr[i]);

  //printf("calling...\n" );
  mergeSort(arr, 0, sz-1);
  //printf("done...\n" );

  /*
  for(int i=0; i<sz; i++)
    printf("%d\n",arr[i]);
  printf("\n");
  */

  return 0;
}
