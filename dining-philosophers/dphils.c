//
//  dphils.c
//  OS - HW4
//

#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#define N 5
#define m 1
#define LEFT (i + N - 1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
typedef sem_t semaphore;
int state[N];
semaphore mutex;
semaphore s[N];
semaphore *forks;
semaphore knives;
int number_of_philosophers;
int TIME;
int num_knives;
void take_forks(int);
void put_forks(int);
void test(int);
void think(int);
void eat(int);
void quit(int);

void philosopher(int idx)
{
    for(int i = 0; i < 5; i++)
    {
        think(idx);
        take_forks(idx);
        eat(idx);
        put_forks(idx);
    }
}

void take_forks(int i)
{
    sem_wait(&mutex);
    state[i] = HUNGRY;
    test(i);
    sem_post(&mutex);
    sem_wait(&forks[i]);
    sem_wait(&knives);
}

void put_forks(int i)
{
    sem_wait(&mutex);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);
}

void test(int i)
{
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        sem_post(&forks[i]);
        sem_post(&knives);
    }
}

void think(int i)
{
    printf("Philosopher %d thinking\n", i);
    sleep(rand() % (TIME + 1 - 0) + 0);
    printf ("Philosopher %d done thinking\n", i);
}

void eat(int i)
{
    printf ("Philosopher %d: yum yum spagett! (%d, %d)\n", i, state[LEFT], state[RIGHT]);
    sleep(rand() % (TIME + 1 - 0) + 0);
    printf ("Philosopher %d done eating\n", i);
}

void quit(int signo)
{
    switch(signo)
    {
        case SIGINT: case SIGTSTP:
            munmap(forks, number_of_philosophers * sizeof(semaphore));
            sem_destroy(&mutex);
            sem_destroy(forks);
            exit(0);
        default:
            printf("signal unhandled\n");
            break;
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT,quit);    //catch signals
    signal(SIGTSTP,quit);
    
    if(argc < 4)
    {
        printf("Please enter the correct number of parameters.\n");
    }
    else
    {
        number_of_philosophers = atoi(argv[1]);
        num_knives = atoi(argv[2]);
        TIME = atoi(argv[3]);
    }
    if(num_knives == 0)
    {
        //map shared memory for processes
        forks = mmap(0, number_of_philosophers * sizeof(semaphore), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        int i;
        //initialize semaphores
        for(i = 0; i < number_of_philosophers;i++)
        {
            sem_init(&forks[i], 0, 1);
        }
        sem_init(&mutex, 0, 1);
        //fork off philosophers
        for(i = 0;i < number_of_philosophers;i++)
        {
            if(fork() == 0)
            {
                philosopher(i);
            }
        }
    }
    else if (num_knives > 0)
    {
        //map shared memory for processes
        forks = mmap(0, number_of_philosophers * sizeof(semaphore), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        int i;
        //initialize semaphores
        for(i = 0; i < number_of_philosophers;i++)
        {
            sem_init(&forks[i], 0, 1);
        }
        sem_init(&knives, 0, num_knives);
        sem_init(&mutex, 0, 1);
        //fork off philosophers
        for(i = 0;i < number_of_philosophers;i++)
        {
            if(fork() == 0)
            {
                philosopher(i);
            }
        }
    }
    wait(NULL);
    quit(SIGTSTP);
    return 0;
}


