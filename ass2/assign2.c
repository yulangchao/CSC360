/*
 * assign2.c
 *
 * Name:
 * Student Number:
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include "train.h"

/*
 * If you uncomment the following line, some debugging
 * output will be produced.
 *
 * Be sure to comment this line out again before you submit 
 */

/* #define DEBUG	1 */



void ArriveBridge (TrainInfo *train);
void CrossBridge (TrainInfo *train);
void LeaveBridge (TrainInfo *train);



pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_q= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
/*
 * This function is started for each thread created by the
 * main thread.  Each thread is given a TrainInfo structure
 * that specifies information about the train the individual 
 * thread is supposed to simulate.
 */
 
int westt[1000];
int eastt[1000]; 
int i=0;
int j=0;
int k=0;
int l=0;
int count;
int count_w,count_e;
int train_direction = 0; 

void * Train ( void *arguments )
{
	TrainInfo	*train = (TrainInfo *)arguments;

	/* Sleep to simulate different arrival times */
	usleep (train->length*SLEEP_MULTIPLE);

	ArriveBridge (train);
	CrossBridge  (train);
	LeaveBridge  (train); 

	/* I decided that the paramter structure would be malloc'd 
	 * in the main thread, but the individual threads are responsible
	 * for freeing the memory.
	 *
	 * This way I didn't have to keep an array of parameter pointers
	 * in the main thread.
	 */
	free (train);
	return NULL;
}

/*
 * You will need to add code to this function to ensure that
 * the trains cross the bridge in the correct order.
 */
 

 
int judge() {
 
	if( train_direction == 1 ) {
		return westt[k];
	} else {
		return eastt[l];
	}
 	return -1;
}
 
void ArriveBridge ( TrainInfo *train )
{
	printf ("Train %2d arrives going %s\n", train->trainId, 
			(train->direction == DIRECTION_WEST ? "West" : "East"));
	/* Your code here... */
        pthread_mutex_lock(&mutex_q);
        if( train->direction == 1){
           westt[i]=train->trainId;
           i++;
           count_w=i;
        }else
        if( train->direction == 2){
           eastt[j]=train->trainId;
           j++;
           count_e=j;
        }else{
           printf("wrong");
        }
        pthread_mutex_unlock(&mutex_q);
        
        
        pthread_mutex_lock(&mutex_b);
  
	while( train_direction != 0 && train->trainId != judge() ) {
	    	pthread_cond_wait(&cv, &mutex_b);
	}
}

/*
 * Simulate crossing the bridge.  You shouldn't have to change this
 * function.
 */
void CrossBridge ( TrainInfo *train )
{
	printf ("Train %2d is ON the bridge (%s)\n", train->trainId,
			(train->direction == DIRECTION_WEST ? "West" : "East"));
	fflush(stdout);
	
	/* 
	 * This sleep statement simulates the time it takes to 
	 * cross the bridge.  Longer trains take more time.
	 */
	usleep (train->length*SLEEP_MULTIPLE);

	printf ("Train %2d is OFF the bridge(%s)\n", train->trainId, 
			(train->direction == DIRECTION_WEST ? "West" : "East"));
	fflush(stdout);
}

/*
 * Add code here to make the bridge available to waiting
 * trains...
 */
void LeaveBridge ( TrainInfo *train )
{
	// change the direction for next train

	// lock the queue mutex
	// remove the train from the queue
	pthread_mutex_lock(&mutex_q);
	if( train->direction == 1 ) {
		k++;
             //   count_w=count_w-1;
	} else if( train->direction == 2) {
		l++;
             //   count_e=count_e-1;
                if(count_w>k){
//count for east when west exists
                count++;
                }
	} else {
		// This should never be reached...
		printf("wrong");
	}
        // printf("%d,east: %d, west: %d,eeee:%d,wwww:%d\n",count,l,k,count_e,count_w);
	pthread_mutex_unlock(&mutex_q);
       //  printf("%d,east: %d, west: %d,eeee:%d,wwww:%d\n",count,l,k,count_e,count_w);
        if(count<2 && count_e>l && count_w>=k){
	train_direction = DIRECTION_EAST;
        }else if(count<2 && count_e==l && count_w>k){
        train_direction = DIRECTION_WEST;
        }else if(count>=2 && count_e>l && count_w==k){
	train_direction = DIRECTION_EAST;
        
        }else if(count>=2 && count_e>l && count_w>k){
	train_direction = DIRECTION_WEST;
        count=0;
        }else if(count>=2 && count_e==l && count_w>k){
        train_direction = DIRECTION_WEST;
        } else if(count<2 && count_e==l && count_w==k){
        train_direction = DIRECTION_WEST;
        }


	// unlock mutex
	pthread_mutex_unlock(&mutex_b);


    //    printf("%d,east: %d, west: %d\n",count,count_e,count_w);

	pthread_cond_broadcast(&cv);







}

int main ( int argc, char *argv[] )
{
	int		trainCount = 0;
	char 		*filename = NULL;
	pthread_t	*tids;
	int		i;

		
	/* Parse the arguments */
	if ( argc < 2 )
	{
		printf ("Usage: part1 n {filename}\n\t\tn is number of trains\n");
		printf ("\t\tfilename is input file to use (optional)\n");
		exit(0);
	}
	
	if ( argc >= 2 )
	{
		trainCount = atoi(argv[1]);
	}
	if ( argc == 3 )
	{
		filename = argv[2];
	}	
	
	initTrain(filename);

	
	/*
	 * Since the number of trains to simulate is specified on the command
	 * line, we need to malloc space to store the thread ids of each train
	 * thread.
	 */
	tids = (pthread_t *) malloc(sizeof(pthread_t)*trainCount);
	
	/*
	 * Create all the train threads pass them the information about
	 * length and direction as a TrainInfo structure
	 */
	for (i=0;i<trainCount;i++)
	{
		TrainInfo *info = createTrain();
		
		printf ("Train %2d headed %s length is %d\n", info->trainId,
			(info->direction == DIRECTION_WEST ? "West" : "East"),
			info->length );

		if ( pthread_create (&tids[i],0, Train, (void *)info) != 0 )
		{
			printf ("Failed creation of Train.\n");
			exit(0);
		}
	}

	/*
	 * This code waits for all train threads to terminate
	 */
	for (i=0;i<trainCount;i++)
	{
		pthread_join (tids[i], NULL);
	}
	
	free(tids);
	return 0;
}




