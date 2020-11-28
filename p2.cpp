#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h> 
#include <stdio.h> 
#include <iostream>  
#include "sem.h"

using namespace std;

#define SHMSZ 27

int main() 
{   
    int shmid;
    int *shm, *s,*ss;
    key_t key;
    key = ftok(".",'A');

    if(key == -1)
    {
        perror("ftok");
        exit(1);
    }

    if ((shmid = shmget(key, SHMSZ, 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    if ((shm = (int*)shmat(shmid, NULL, 0)) == (int *) -1) {
        perror("shmat");
        exit(1);
    }

     s = shm;
    
     //comienza los procesos
    int semid = createSem(1);
    initSem(semid,0,1);
    int n1 = fork(); 
    if(n1<0) {
        cout<<"error al crear proceso"<<endl;
    }  
    else if(n1==0) {
    int _semid = getSem(0);
    int t=1;  
      while(t--){
        P(_semid,0);
        if((*s)%2==0)
        {
            cout<<"["<<*s<<"]";
            cout << "\033[1;43m Proceso 2 The Number is EVEN\033[0m" << endl;
        }
        fflush(stdout);
        usleep(321456);
        V(_semid,0);    
       }
    }
    else{
        int t=1;
        while(t--){
         P(semid,0);
         if((*s)%2!=0)
         {
             cout<<"["<<*s<<"]";
             cout << "\033[1;45mProceso 3 The number is ODD\033[0m" << endl;
         }
         fflush(stdout);
         usleep(121456);
         V(semid,0);
        }
      }
    destroySem(semid);
    
    return 0; 
}
