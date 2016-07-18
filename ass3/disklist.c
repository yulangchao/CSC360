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
int readfile(char *point){
    int num1=(point[9]<<24)+(point[10]<<16)+(point[11]<<8)+(point[12]);
    if(num1==0){
        return 0;
    }
    int num=point[0];
    if (num==3)
    printf("F"); 
    if(num1>0){
    printf("     %6d",num1);
    }else{
    printf("     %6d",65536+num1);    
    }
    printf("%31s",point+27);
    int num2=(point[20]<<8)+(point[21]);    
    if((point[21])<0){
    printf("     %d",num2+256);
    }else{
    printf("     %d",num2);    
    }
    printf("/%02d/%02d %02d:%02d:%02d\n",point[22],point[23],point[24],point[25],point[26]);
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
        int block_size=blocksize(point);
        int rd_s=rds(point);
        int rd_b=rdb(point);
        int n=rd_b;
        point=point+block_size*rd_s;
        while(point[0]!=0x00){
        
        readfile(point);

        
        point=point+64;
   //     printf("%d",point[0]);
        }
//printf("%d",point[0]);
//info end

        munmap(start, buf.st_size);
        close(imgfile);

return 0;

}
