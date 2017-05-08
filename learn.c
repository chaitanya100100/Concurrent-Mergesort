#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include <unistd.h>

int sz = 0x6400;

int main()
{
  int id;
  int * arr;

  id = shmget(IPC_PRIVATE, sz, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
  arr = (int*)shmat(id, 0, 0);

  arr[0] = 17;


  pid_t pid1,pid2;


  printf("Before fork : %d\n",arr[0]);
  if ((pid1 = fork()) == -1){
    perror("fork");
    exit(EXIT_FAILURE);
  }
  else if(pid1==0){
    arr = (int*)shmat(id, 0, 0);
    
    printf("Child1 before increment %d\n", arr[0]);
    arr[0]++;
    printf("Child1 after increment %d\n", arr[0]);
    shmdt(arr);
  }
  else{
    if ((pid2 = fork()) == -1){
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if(pid2==0){
      arr = (int*)shmat(id, 0, 0);
      printf("Child2 before increment %d\n", arr[0]);
      arr[0]++;
      printf("Child2 after increment %d\n", arr[0]);
      shmdt(arr);
    }
    else{
      int status;
      printf("Parent before wait %d\n", arr[0]);
      waitpid(pid1, &status, 0);
      waitpid(pid2, &status, 0);
      printf("Parent after wait %d\n", arr[0]);
    }
  }
  return 0;
}
