/*
Yingchao Yu
V00830514
csc360 Ass1

*/




/*
 * main.c
 *
 * A simple program to implement?a?simple?shell?program?similar?to?the?Unix?shell?bash
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

//ls function

void lsex(char *arg[])
{
 pid_t ls=fork();
 if(ls<0){
       perror("fork");
       exit(0);
 }else if(ls==0){
       if(execvp(arg[0],arg)<0){
           printf("fail of command");
       }
       exit(1);
   }else{
   wait(NULL);
    // printf ("ls done\n");
   }
}



//cd function

void cd(char *arg[],char *path){    
        if(strcmp(arg[1],"..")==0){
            char *p;

            p=rindex(path,'/');

            int m=p-path;
            char current[m+1];

            strncpy(current,path, m );
            
            chdir(current);
            path=getcwd(0, 0);

        }else if(strcmp(arg[1],"~")==0){
            chdir("/home");
            path=getcwd(0, 0);     
        }else{
            if(chdir(arg[1])==-1){
            perror("Wrong");
            }
            path=getcwd(0,0);
        }
}


//background execute

void bg(char *arg[],pid_t pidnum[],char *dir[],int l,char *st[])
{
 pid_t ls=fork();
 int status;
 pid_t pr;
 if(ls<0){
       perror("fork");
       exit(0);
 }else if(ls==0){
       if(execvp(arg[0],arg)<0){
          // printf("%s",arg[0]);
           printf("fail of command\n");
       }
     //  sleep(2);
       exit(1);  
   }else{
 //  wait(NULL);
   
   pr=waitpid(ls,&status,WNOHANG);
   pidnum[l]=ls;
   st[l]="R";
   dir[l]=getcwd(0,0);
  // wait(&status);
   if(pr<0){
      printf("Wrong\n");
      exit(0);
      
   }else if(pr==0){
      sleep(1);
  }
   
  //   printf ("bg done\n");
   }
}

//background list
void bglist(pid_t pidnum[],char *dir[],int l,char *st[]){
    int k=0;
    while(k<l && k<5){
       printf("%d[%s]: %s   (pid:%d)\n",k,st[k],dir[k],pidnum[k]);
       k++;
    }  
    printf("Total Background jobs: %d\n",k);
}

//kill background process
int bgkill(pid_t pidnum[],char *dir[],int l,char *argv[],char *st[]){
    int a=0;
    l=l-1;
    kill(pidnum[atoi(argv[1])],SIGKILL);
    for(a=atoi(argv[1]);a<l;a++){
           pidnum[a]=pidnum[a+1];
           dir[a]=dir[a+1]; 
           st[a]=st[a+1];
           }
           pidnum[a]='\0';
           dir[a]='\0';   
           st[a]='\0';   
       
    
    return l;   
}


//stop process
void stop(pid_t pidnum[],char *argv[],char *st[]){
         kill(pidnum[atoi(argv[1])],SIGSTOP);
         st[atoi(argv[1])]="S";

}

//start process
void start(pid_t pidnum[],char *argv[],char *st[]){
         kill(pidnum[atoi(argv[1])],SIGCONT);
         st[atoi(argv[1])]="R";
}

//signal handle
static void sigchld_hdl (int sig) {
	int status;	
	pid_t s_pid;

	while(1) {
		s_pid = waitpid(-1, &status, WNOHANG);
		if (s_pid < 0){
		//	perror ("waitpid");
			break;
		}
		if (s_pid == 0)
			break;
		//notice_termination (pid, status);
	//	printf("\nSIGCHLD handler Activated.\n");     // Not safe to use printf
		// For safe functions, see "man 7 signal"
		// And see: www.ibm.com/developerworks/linux/library/l-reent/index.html#N101CA
	}
}


int main ( void )
{
  pid_t pidnum[5];
  char *st[5];
  char *dir[5];
  int l=0;
  char *path=getcwd(0,0);
  
  struct sigaction act;

    memset(&act, '\0', sizeof(act));
    act.sa_handler = sigchld_hdl;
    //act.sa_flags = SA_NODEFER;

    if (sigaction(SIGCHLD, &act, 0)) {
		perror ("sigaction");
		return 1;
    }

	for (;;)
	{
  
		char 	*cmd = readline ("shell>");
		printf ("Got: [%s]\n", cmd);
    char *token;
    char * arg[15];
    int i=0;
    token=strtok(cmd," ");
    while(token!=NULL){
    arg[i]=token;
    token=strtok(NULL," ");
    i++;
    }
    arg[i]='\0';


//cmd function
    if(strcmp(arg[0],"ls")==0){
          lsex(arg);

    }else if(strcmp(arg[0],"cd")==0){
          cd(arg,path);

    }else if(strcmp(arg[0],"pwd")==0){
          path=getcwd(0,0);
          printf("Current path is : %s\n",path);

    }else if(strcmp(arg[0],"bg")==0){
          
          if(l<5){ 
           char * cmd1[100];    
           int j; 
           for(j=1;arg[j]!=NULL;j++){
           cmd1[j-1]=arg[j];
           }
           cmd1[j-1]='\0';
           bg(cmd1,pidnum,dir,l,st);
       //    printf("%d",l);
       //    printf("%d",pidnum[l]);
       //    printf("%s",dir[l]);
           l=l+1;
           }else{
           		perror ("cant add more process");
           }
    }else if(strcmp(arg[0],"bglist")==0){

          bglist(pidnum,dir,l,st);
         
    }else if(strcmp(arg[0],"bgkill")==0){

          l=bgkill(pidnum,dir,l,arg,st);

        //  printf("%d",l);
        
    }else if(strcmp(arg[0],"stop")==0){

          stop(pidnum,arg,st);
         
    }else if(strcmp(arg[0],"start")==0){

          start(pidnum,arg,st);
         
    }else if(strcmp(arg[0],"quiz")==0){

          exit(0);
         
    }else{
          execvp(arg[0],arg);
    }

//    sleep(10);
    free (cmd);
 
}
}



