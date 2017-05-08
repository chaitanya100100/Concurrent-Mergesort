#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/types.h>
#include <unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<math.h>

const int SELECTION_SORT_LIMIT = 5;


int * arr;
int sz;
int sm_id;
int le;
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

void mergeSortNormal(int * arr, int l, int r)
{
  if(l>=r)return;

  if((r-l)<=SELECTION_SORT_LIMIT)
  {
    selection_sort(arr, l, r);
    return;
  }

  int m=(r-l)/2+l;
  mergeSortNormal(arr, l, m);
  mergeSortNormal(arr, m+1, r);
  merge(arr, l, m, r);
}

void mergeSort(int * arr, int l, int r, int level)
{
  if(l>=r)return;

  if((r-l)<=SELECTION_SORT_LIMIT)
  {
    selection_sort(arr, l, r);
    return;
  }
  if(level == le)
  {
    mergeSortNormal(arr, l, r);
    return;
  }

  int m = (r-l)/2+l;
  pid_t pid_l, pid_r;

  if((pid_l = fork())==-1)
  {
    perror("fork");
    _exit(EXIT_FAILURE);
  }
  else if(pid_l==0)
  {
    mergeSort(arr, l, m, level+1);
    _exit(EXIT_SUCCESS);
  }
  else
  {
    if((pid_r = fork())==-1)
    {
      perror("fork");
      _exit(EXIT_FAILURE);
    }
    else if(pid_r==0)
    {
      mergeSort(arr, m+1, r, level+1);
      _exit(EXIT_SUCCESS);
    }
    else
    {
      int status;
      waitpid(pid_l, &status, 0);
      waitpid(pid_r, &status, 0);
      merge(arr, l, m, r);
    }
  }
}

int main()
{
  scanf("%d",&sz);

  a1 = malloc(sizeof(int)*(sz+5));
  a2 = malloc(sizeof(int)*(sz+5));

  if((sm_id = shmget(IPC_PRIVATE, (sz+5) * sizeof(int),  IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) < 0)
  {
    perror("shmget");
    _exit(EXIT_FAILURE);
  }

  if( !(arr = shmat(sm_id, NULL, 0)) )
  {
      perror("shmat");
      _exit(EXIT_FAILURE);
  }

  for(int i=0; i<sz; i++)
    scanf("%d",&arr[i]);

  le = (log(sz))/1.4;
  //printf("calling...\n" );
  mergeSort(arr, 0, sz-1, 0);
  //printf("done...\n" );

  /*
  for(int i=0; i<sz; i++)
    printf("%d\n",arr[i]);
  printf("\n");
  */


  if (shmdt(arr) == -1)
  {
       perror("shmdt");
       _exit(EXIT_FAILURE);
  }
  if (shmctl(sm_id, IPC_RMID, NULL) == -1)
  {
       perror("shmctl");
       _exit(EXIT_FAILURE);
  }
  return 0;
}
