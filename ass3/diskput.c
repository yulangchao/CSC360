#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

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
int length;
int start_b;
int n_0_sb;
char ct[60000];
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
        printf("file:%d",file[b].startfile*block_size);
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
   // printf("%c\n",point1[0]);
    
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

int readinput(char *filename){
       FILE *fp=fopen(filename, "rb");
       if(fp==NULL){
 	    	    printf("File not found\n");
    	    	exit(0);
       }
       

       
       fseek(fp,0,SEEK_END);
       length=ftell(fp);
    //   printf("%d\n",length);
       fclose(fp);
   //    printf("%s",ct)   ;    


}


int putfile(char *point,int k, FILE *disk,int n,char *filename){
      int Status=0x03;
      int Starting_Block=0;
      long int unused=0xFFFFFFFFFFFF;
      if(n==0){
      Starting_Block=n_0_sb;
      }else{
      if(file[n-1].size%block_size!=0){
      Starting_Block=file[n-1].startfile+(int)(file[n-1].size/block_size)+1;
      }else{
      Starting_Block=file[n-1].startfile+(int)(file[n-1].size/block_size);
      }
      }
      start_b=Starting_Block;
    //  printf("%d",Starting_Block);
      int N_B=0;
      if(length%block_size!=0){
      N_B=(int)(length/block_size)+1;
      }else{
      N_B=length/block_size;
      }
      time_t rawtime;
      struct tm * timeinfo;

      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
     
      int year=timeinfo->tm_year + 1900;
      int month=timeinfo->tm_mon + 1;
      int day=timeinfo->tm_mday;
      int hour=timeinfo->tm_hour;
      int min=timeinfo->tm_min;
      int sec=timeinfo->tm_sec;
     // printf("%d,%d,%d,%d\n",Status,Starting_Block,N_B,length);
      
      year=ntohs(year);
      
    //  printf( "[%d %d %d %d:%d:%d]",year, month, day, hour, min, sec);
      
      unsigned int rest=0x00;
      
      int p=0;
     // int rest=0x00;
      
      fseek(disk, k, SEEK_SET);	
       
      length=htonl(length);
      N_B=htonl(N_B);
      Starting_Block=htonl(Starting_Block);
      fwrite(&Status, 1, 1, disk);
    	fwrite(&Starting_Block, 1, 4, disk);
    	fwrite(&N_B, 1, 4, disk);    
      fwrite(&length, 1, 4, disk);
      
      fwrite(&year, 1, 2, disk);      
      fwrite(&month, 1, 1, disk);      
      fwrite(&day, 1, 1, disk); 
      fwrite(&hour, 1, 1, disk); 
      fwrite(&min, 1, 1, disk);   
      fwrite(&sec, 1, 1, disk);
       
      fwrite(&year, 1, 2, disk);      
      fwrite(&month, 1, 1, disk);      
      fwrite(&day, 1, 1, disk); 
      fwrite(&hour, 1, 1, disk); 
      fwrite(&min, 1, 1, disk);   
      fwrite(&sec, 1, 1, disk); 
      
      for(p=0;p<31;p++){
      fwrite(&rest, 1, 1, disk);
      }
      fseek(disk,-31,SEEK_CUR); 
       
      int filename_lenth = strlen(filename); 
      
  //    printf("%s,%d\n",filename,filename_lenth);
   
      fwrite(filename, 1, 31, disk);  
                           
      fwrite(&unused, 1, 6, disk); 
      

}

int putcontent(FILE *disk,int startplace,char *filename){
      fseek(disk,startplace*block_size, SEEK_SET);
     // printf("%d\n",startplace*block_size);
      FILE *fp1=fopen(filename, "rb");
   //   char c='a';
 // 	  char buffer[60000];
      int c=0;
      int b=0;
    while (c != EOF)
    {
        /* read a char from the file */
        c = fgetc(fp1);
        /* display the character */
        
        fwrite(&c, 1, 1, disk);
    } 
    //  fclose(fp1);

      



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
       	int imgfile = open(argv[1], O_RDWR);
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
        n_0_sb=rds(point)+rdb(point);
    //    printf("%d",n_0_sb);
        block_size=blocksize(point);
        int rd_s=rds(point);
        FILE *disk=fopen(argv[1], "r+");
    //    fseek(disk, 1, SEEK_SET);
    	  
    //    fwrite(buffer,1,1,disk);


        int rd_b=rdb(point);
        int n=0;
        ss=block_size*rd_s;
        point=point+block_size*rd_s;
        readinput(filename);
        int update=0;
        while(point[0]!=0x00){
        
        readfile(point);
      //  printf("%s\n",point+27);
        if(strcmp(file[n].name,filename)==0){
            //  update(point);
              update=1;
              exit(0);
        }
        n++;

        point=point+64;
        }
     //   printf("%d\n",point[0]);
        if(update==0){
        //now point is the new one;
      //   fseek(disk, block_size*rd_s+64*n, SEEK_SET);
    	   
      //   fwrite(buffer,sizeof(int),sizeof(buffer),disk);
        // printf("%s\n",ct);
         putfile(point,block_size*rd_s+64*n,disk,n,filename);     //startplace=(int)last startplace+size/512
      //   printf("%d\n",start_b);
         putcontent(disk,start_b,filename); //(last startplace+size/512 (int))*size;
      //  getfile(point1,filename,rd_b+1);

         }
        

//info end

        munmap(start, buf.st_size);
        close(imgfile);

return 0;

}
