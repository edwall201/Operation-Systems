#include<stdio.h>  
#include<sys/time.h>  
#include<stdlib.h>  
#include<sys/types.h>  
#include<unistd.h>  
#include<string.h>  
#include<sys/wait.h>

int main(int argc,char *argv[]) {      
    struct timeval start,end;      
    int fd[2];      
    pipe(fd);      
    int pid = fork();      
    if(pid < 0) {          
        perror("Unable to fork\n");          
        exit(1);      
    }else if(pid == 0) {          
        close(fd[0]);          
        gettimeofday(&start,NULL);          
        char buffer[100];          
        sprintf(buffer,"%ld %ld",start.tv_sec,start.tv_usec);          
        printf("Child: %ld\n",start.tv_usec);          
        write(fd[1],buffer,strlen(buffer)+1);          
        execvp(argv[1],argv + 1);          
        close(fd[1]);      
    }else{          
        double timetaken;          
        long int startsec,startusec;          
        close(fd[1]);          
        char buffer[100];          
        dup2(fd[0],0);          
        //read(fd[0],buffer,100);          
        scanf("%ld",&startsec);          
        scanf("%ld",&startusec);          
        //printf("%ld %ld",startsec,startusec);          
        printf("Parent: %ld\n",startusec);          
        gettimeofday(&end,NULL);          
        timetaken = (end.tv_sec - startsec) * 1e6;          
        timetaken = (timetaken + (end.tv_usec - startusec)) * 1e-6;          
        printf("Parent:%ld\n",end.tv_usec);          
        printf("Elapsed time: %0.6lf sec\n",timetaken);          
        close(fd[0]);          
        wait(NULL);      
    }  
} 
