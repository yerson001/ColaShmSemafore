#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <iostream>
#include <queue>
#include <thread>
#include <unistd.h>
using namespace std;

#define SHMSZ 27

//************************************
//g++ p1.cpp -o p1 -std=c++11 -pthread
//************************************

queue<int> myqueue;
int shmid,num,cont=0;
int *shm, *s;


void FullQueue(int n)
{
    for(int i=0; i<n; ++i)
    {
      int num = rand()%(1000);
      myqueue.push(num);
    }

}

void Shm(key_t key)
{
  if(key == -1)
  {
    perror("ftok");
    exit(1);
  }

    //asgigna segmento
   if((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    exit(1);
   }

   if ((shm = (int*)shmat(shmid, NULL, 0)) == (int*) -1) {
    perror("shmat");
    exit(1);
   }
   // Zona para compartir
    s = shm;
    while (!myqueue.empty()) { 
      *s = myqueue.front();
      cout<<" * Value SHM [ "<<*s<<" ] *"<<endl; 
      myqueue.pop(); 
      usleep(4000000);
    }cout<<endl;
}

main()
{
    srand(time(NULL));
    key_t key;
    key = ftok(".",'A');
    thread primero(FullQueue,10);
    thread segundo(Shm,key);
    primero.join();
    segundo.join();
    //cout<<"funciona"<<endl;

    exit(0);
}
