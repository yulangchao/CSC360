/*
 * train.c
 */
 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "train.h"
 
/* A global to assign IDs to our trains */ 
int idNumber = 0;

/* If this value is set to 1, trains lengths
 * etc will be generated randomly.
 * 
 * If it is set to 0, the lengths etc will be
 * input from a file.
 */
int doRandom = 0;
int line;
/* The file to input train data from */
FILE *inputFile;

/* You can assume that no more than 80 characters
 * will be on any line in the input file
 */
#define MAXLINE		80

void	initTrain ( char *filename )
{
	doRandom = 0;
	
	/* If no filename is specified, generate randomly */
	if ( !filename )
	{
		doRandom = 1;
		srandom(getpid());
	}
	else
	{
		/* remove this line and add your code here */
		
                inputFile=fopen(filename,"r");
  		if( inputFile == NULL )
 		{
 		     perror("Error while opening the file.\n");
 		     exit(EXIT_FAILURE);
 		}
                
	}
}
 
/*
 * Allocate a new train structure with a new trainId, trainIds are
 * assigned consecutively, starting at 0
 *
 * Either randomly create the train structures or read them from a file
 *
 * This function malloc's space for the TrainInfo structure.  
 * The caller is responsible for freeing it.
 */
TrainInfo *createTrain ( void )
{
	TrainInfo *info = (TrainInfo *)malloc(sizeof(TrainInfo));

	/* I'm assigning the random values here in case
	 * there is a problem with the input file.  Then
	 * at least we know all the fields are initialized.
	 */	 
	info->trainId = idNumber++;
	info->arrival = 0;
	info->direction = (random() % 2 + 1);
	info->length = (random() % MAX_LENGTH) + MIN_LENGTH;
         
	if (!doRandom)
	{
		/* Your code here to read a line of input
		 * from the input file 
		 */
                
      	char buffer[1024];
        int pos;
        int c=0;
        int num;
      	pos = 0;
      	do{ 
        c = fgetc(inputFile);
        if(c != EOF) {
        buffer[pos++] = (char)c;
        }		
        }while(c != EOF && c != '\n');
        buffer[pos] = 0;
        printf("%s", buffer);
        char* p = &buffer[1];
     //   printf("%s", p);
        num=atoi(p);


        if(buffer[0]=='W' ||buffer[0]=='w'){
        info->direction=1;
	      }else{
        info->direction=2;
        }
        
        info->length=num;
        bzero(buffer,1024);

	
   }
	return info;

}


