#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>


int freeb;
int ffff;
int reserve;
int blocksize(char *point){
    int num=point[8]<<8;
    num+=point[9];
    return num;
  
}

int blockcount(char *point){
    int num=point[10]<<24;
    num+=point[11]<<16;
    num+=point[12]<<8;
    num+=point[13];
    return num;
}

int fats(char *point){
    int num=point[14]<<24;
    num+=point[15]<<16;
    num+=point[16]<<8;
    num+=point[17];
    return num;
}


int fatb(char *point){
    int num=point[18]<<24;
    num+=point[19]<<16;
    num+=point[20]<<8;
    num+=point[21];
    return num;
}

int rds(char *point){
    int num=point[22]<<24;
    num+=point[23]<<16;
    num+=point[24]<<8;
    num+=point[25];
    return num;
}

int rdb(char *point){
    int num=point[26]<<24;
    num+=point[27]<<16;
    num+=point[28]<<8;
    num+=point[29];
    return num;
}
int fatinfo(char *point){
    int total=0;
    int i=fats(point)*blocksize(point);
    while(total<blockcount(point)){
          int num=0;
   	  num=point[i]<<24;
  	  num+=point[i+1]<<16;
 	  num+=point[i+2]<<8;
  	  num+=point[i+3];
      
  	   if(num==0){
	      freeb++;
 	      total++; 
 	   }else if(num==1){
 	      reserve++;
 	      total++;
  	   }else{
              ffff++;
              total++;
           }
          i=i+4;
    }



}
int main(int argc, char *argv[]){
	struct stat buf;
        void *start;
        char *point;
	if (argc != 2) {
	    fprintf(stderr, "Error input \n");
	    exit(EXIT_FAILURE);
	}
   	int imgfile = open(argv[1], O_RDONLY);

   	if (imgfile < 0){
            fprintf(stderr, "Error no such file \n");
	    exit(EXIT_FAILURE);
	}
        fstat(imgfile, &buf);
        start = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, imgfile, 0);
     
        if(start == MAP_FAILED) 
        return;
        
        point=start;
  //      printf("%x,%x,%d",(point[3]),1,point[1019]+point[1018]+point[1017]+point[1016]);
//print info
        fatinfo(point);
        printf("Super block information:\n");
	printf("Block size: %d\nBlock count: %d\n",blocksize(point),blockcount(point));
	printf("FAT starts: %d\nFAT blocks: %d\n",fats(point),fatb(point));
	printf("Root directory start: %d\nRoot directory blocks: %d\n\n",rds(point),rdb(point));
	printf("FAT information:\n");
	printf("Free Blocks: %d\n",freeb);
	printf("Reserved Blocks: %d\n",reserve);
	printf("Allocated Blocks: %d\n",ffff);


//info end

        munmap(start, buf.st_size);
        close(imgfile);

return 0;

}
