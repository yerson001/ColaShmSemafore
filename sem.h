#ifndef _SEM_H_
#define _SEM_H_

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>                                                             
#include<sys/sem.h>                                                             
                                                                                 
                                                                                
#define PATHNAME "."                                                            
#define PROJ_ID 0x7777                                                          
                                                                                
union semun{                                                                    
     int              val;   /* Value for SETVAL */                              
    struct semid_ds *buf;   /* Buffer for IPC_STAT,IPC_SET */                   
    unsigned short  *array; /* Array for GETALL,SETALL */                       
    struct seminfo  *__buf; /* Buffer for IPC_INFO */                           
};                                                                              
                                                                                 
int createSem(int num);                                                         
int initSem(int semid,int nums,int initval);                                    
int getSem(int num);                                                            
int P(int semid,int nums);                                                      
int V(int semid,int nums);
int destroySem(int semid);
#endif 

int createSem(int num)
{

    key_t key = ftok(PATHNAME,PROJ_ID);
    if(key < 0){
        perror("ftok");
        return -1;
    }
    int semid = semget(key,num,IPC_CREAT|IPC_EXCL|0666);
    if(semid < 0){
        perror("semget");
        return -2;
    }
    return semid;
}
int initSem(int semid,int nums,int initval)
{
    union semun sem;
    sem.val = initval;
    if(semctl(semid,nums,SETVAL,sem) < 0){
        perror("semctl");
        return -1;
    }
    return 0;
}
int getSem(int num)
{
    key_t key = ftok(PATHNAME,PROJ_ID);
    if(key < 0){
        perror("ftok");
        return -1;
    }
    int semid = semget(key,num,IPC_CREAT);
    if(semid < 0){
        perror("semget");
        return -2;
    }
    return semid;
}
static int PV(int semid,int nums,int op)
{
    struct sembuf sf;
    sf.sem_num = nums;
    sf.sem_op = op;
    sf.sem_flg = 0;
    if(semop(semid,&sf,1)<0){
        perror("semop");
        return -1;
    }
    return 0;
}
int P(int semid,int nums)
{
    return PV(semid,nums,-1);
}
int V(int semid,int nums)
{
    return PV(semid,nums,1);
}
int destroySem(int semid)
{
    if(semctl(semid,0,IPC_RMID)==-1){
        perror("semctl");
        return -1;
    }
     return 0;
}
