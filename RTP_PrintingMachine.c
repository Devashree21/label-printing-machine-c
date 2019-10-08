//* Project Name: Real time model for Box Label Printing Machine *   //

// Program Description: Pthreads for box and machine implementation //
//------------------------------------------------------------------//
#include <stdio.h>      /* standard I/O routines    */
#include <stdlib.h>     /* standard I/O routines    */
#include <unistd.h>     /* standard I/O routines    */
#include <sys/time.h>	/* getting thread execution times */
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>	/*Semaphore functions	    */
#include <pthread.h>    /* pthread functions and data structures */

// Semaphores
sem_t mutex;
sem_t full;
sem_t empty;

struct Data_D
{
    struct timeval timestamp;
};
struct timeval mstart, mstop; //for machine thread tasks 
struct timeval bstart, bstop; //for box thread tasks

double secs = 0;

typedef struct
{
	sem_t empty;
	sem_t full;
	sem_t mutex;
}buf;
buf shared;

#define NumThreads 2

/* #Define functions to be executed by the new threads */

void* Machine()    //Machine Thread
{
	// Part 1: Machine informs its availability//
	{
		sem_wait(&shared.empty); 	//Means Machine is free and Box can arrive 
		sem_wait(&shared.mutex); 	//Mutex lock acquired for the thread

		gettimeofday(&mstart, NULL); 
		printf("Machine is free..\n");
		printf("Waiting for Box to arrive\t\t");
		sleep(2);
		gettimeofday(&mstop, NULL);
		secs = (double)(mstop.tv_usec-mstart.tv_usec) / 1000000 + (double)(mstop.tv_sec-mstart.tv_sec);
		printf("Execution Time: %fs\n\n",secs);
	}	
	
	//	Part 2: Box Arrival and Placement
	{
		//Mutex is still with Machine Thread
		printf("Box arriving...Machine is full!\n"); 
		printf("Box arrival time is 2s\t\t\t");
		sleep(2);
		gettimeofday(&mstop, NULL);
		secs = (double)(mstop.tv_usec-mstart.tv_usec) / 1000000 + (double)(mstop.tv_sec-mstart.tv_sec);
		printf("Execution Time: %fs\n",secs);
		sleep(2);

		printf("\nBox loaded for printing\t\t\t");
		printf("\nBox loading time is 2s\t\t\t");
		gettimeofday(&mstop, NULL);
		secs = (double)(mstop.tv_usec-mstart.tv_usec) / 1000000 + (double)(mstop.tv_sec-mstart.tv_sec);
		printf("Execution Time: %fs\n\n",secs);
		printf("Machine Thread now waits ...\t\t");
		sleep(1);
		gettimeofday(&mstop, NULL);
		secs = (double)(mstop.tv_usec-mstart.tv_usec) / 1000000 + (double)(mstop.tv_sec - mstart.tv_sec);
		printf("Execution Time: %fs\n",secs);
		printf("........................\n\n\n");

		sem_post(&shared.mutex); //mutex unlock
		sem_post(&shared.full); //Means Machine is locked and box thread can perform
	} 
}
 void* Box() //Box thread
{
	//	Part 1: Label Printing
	{
		sem_wait(&shared.full); // Acquiring semaphore for Box label printing 
		sem_wait(&shared.mutex); //Mutex lock for the thread
		gettimeofday(&bstart, NULL);
		printf("\nBox locked for printing.\nWait.. printing on\t\t\t");
		sleep(5);
		gettimeofday(&bstop, NULL);
		secs = (double)(bstop.tv_usec-bstart.tv_usec) / 1000000 + (double)(bstop.tv_sec-bstart.tv_sec);
		printf("Execution Time: %fs\n",secs);
	} 

	//	Part 2: Box unloading and sending the next box in queue
	{
		printf("\nLabel Printing complete.\nWait..box unloading..\t\t\t");
		sleep(2);
		gettimeofday(&bstop, NULL);
		secs = (double)(bstop.tv_usec-bstart.tv_usec) / 1000000 + (double)(bstop.tv_sec-bstart.tv_sec);
		printf("Execution Time: %fs\n\n",secs);

		printf("Box Unloaded.Machine is free again...\t");
		sleep(1); 
		gettimeofday(&bstop, NULL);
		secs = (double)(bstop.tv_usec-bstart.tv_usec) / 1000000 + (double)(bstop.tv_sec-bstart.tv_sec);
		printf("Execution Time: %fs\n\n",secs);

		printf("Box Thread now waits...\t\t\t");
		sleep(2);
		gettimeofday(&bstop, NULL);
		secs = (double)(bstop.tv_usec-bstart.tv_usec) / 1000000 + (double)(bstop.tv_sec-bstart.tv_sec);
		printf("Execution Time: %fs\n",secs);

		sem_post(&shared.mutex); //mutex unlock
		sem_post(&shared.empty); //Back to Machine thread
	}
} 
	
int main(int argc, char *argv[]) 
{
	//Information about the Author and Program//
	printf("RTP-C Program for Label Printing Machine\n");
	printf("For RTP II Project WS2017-18\n");
	printf("Submission Date: 15.03.2018\n");
	printf("Author:Devashree Madhugiri");

	printf("\n\n\n");
	printf("Program started\n..............................................");
	printf("\n\n");
	
	//Part 1: Initialize values for Semaphores//
	sem_init(&shared.empty,0,1);
	sem_init(&shared.full,0,0);
	
	//Part 2: Create pthreads 
   { 
        /* create a new thread that will execute 'Machine function' */
        pthread_t  MT;    	/* thread's ID */
    	pthread_create(&MT,NULL,Machine, NULL); 
        printf("This is the Machine Thread..(%u) ... \n\n",MT);
    	pthread_join(MT,NULL);
    }
        
	{
	 /* create a new thread that will execute 'Box Function' */
		pthread_t  BT;     	/* thread ID */
		pthread_create(&BT,NULL,Box,NULL);  
		printf("This is the Box Thread..(%u)... \n\n",BT);
		pthread_join(BT,NULL);
	}
return 0;		
}
