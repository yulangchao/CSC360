#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>

struct fileinfo{
   char* name;
   int size;
   int startfile;
}file[1000];
int freeb;
int ffff;
int reserve;
int ss;
int block_size;
int k;

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
int getfile(char *point1,char *filename,int m){
    int b=0;
    int fsize=0;
    int checkfile=0;
    for(b=0;b<m;b++){
    //    printf("file:%s",filename);
        if(strcmp(file[b].name,filename)==0){
     //   printf("%d",file[b].startfile);
        point1=point1+file[b].startfile*block_size;
        fsize=file[b].size;
        checkfile++;
        }    
    }
    if(checkfile==0){
        	fprintf(stderr, "File not found.\n");
	        exit(EXIT_FAILURE) ;
    }
   //   char buff[100000];
   //   strcat(buff,point1);
 //   printf("%s\n",point1);
    
    FILE *fp = fopen(filename, "wb");
    fwrite(point1,sizeof(char),fsize,fp);
//printf("%d\n",fsize);
    fclose(fp);
    return 0;
}

int readfile(char *point){
    int file_start=0;
    int file_size=0;
    if(point[4]>=0){
    file_start=(point[1]<<24)+(point[2]<<16)+(point[3]<<8)+(point[4]);
    }else{
    file_start=(point[1]<<24)+(point[2]<<16)+(point[3]<<8)+(point[4]+256);
    }
//    printf("%d\n",file_start);
    file[k].startfile=file_start;
    if(point[12]>=0){
    file_size=(point[9]<<24)+(point[10]<<16)+(point[11]<<8)+(point[12]);
    }else{
    file_size=(point[9]<<24)+(point[10]<<16)+(point[11]<<8)+(point[12]+256);
    }

    if(file_size>=0){
    file[k].size=file_size;
    }else{
    file[k].size=file_size+65536;    
    }
  //  printf("%d\n",file[k].size);
    
    file[k].name=point+27;
  //  printf("%s\n",file[k].name);
    k++;
    return 0;
}




int main(int argc, char *argv[]){
      	struct stat buf;
        void *start;
        char *point;
        char *point1;
      	if (argc != 3) {
	        fprintf(stderr, "Error input \n");
	        exit(EXIT_FAILURE);
      	}
       	int imgfile = open(argv[1], O_RDONLY);
        char *filename=argv[2];
       	if (imgfile < 0){
            fprintf(stderr, "Error no such file \n");
	          exit(EXIT_FAILURE);
      	}
        fstat(imgfile, &buf);
        start = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, imgfile, 0);
     
        if(start == MAP_FAILED) 
        return;
        
        point=start;
        point1=start;
        block_size=blocksize(point);
        int rd_s=rds(point);
        int rd_b=rdb(point);
        int n=0;
        ss=block_size*rd_s;
        point=point+block_size*rd_s;
        while(point[0]!=0x00){
        readfile(point);

        n++;
        point=point+64;
        }
      //  printf("%d",n);
        getfile(point1,filename,n);


        

//info end

        munmap(start, buf.st_size);
        close(imgfile);

return 0;

}
