#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <sys/times.h>
#include <sys/resource.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h> 

#define SIGTERM 15
#define NUMBER_OF_THE_BEAST 666

#define MUCH_MEMORY_VERY_SHARED 256


int main(int argc, char *argv[]) {
    int fd = shm_open("/napalm", O_RDWR|O_CREAT, 0644);
    int ftruncate(fd, MUCH_MEMORY_VERY_SHARED);
    char* awesome_memory = mmap(NULL, MUCH_MEMORY_VERY_SHARED, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    //Write
    if(argc>1 && argv[1][0]=='w') {
        printf("Writing alphabet into memory.\n");
        char a = 'a';
        for(;a<='z';++a) {
            int i = a-'a';
            awesome_memory[2*i] = a;
            if(a%3==0)
                sleep(1);
            if(a!='z')
                awesome_memory[2*i + 1] = ' ';
        }
    }
    else if(argc>1 && argv[1][0]=='n') {
        printf("Cleanup of memory done.\n");
        memset(awesome_memory, 0, MUCH_MEMORY_VERY_SHARED);
    }
    else {
        printf("Waiting for some data. Will check every one second.\n");
        printf("Will terminate after finding 'z' char in memory.\n");
        int end = 0;
        while(!end) {
            int pos = 0;
            for(;pos<MUCH_MEMORY_VERY_SHARED && awesome_memory[pos]!=0;++pos) {
                if(pos == 0)
                    printf("Incoming data: ");
                printf("%c", awesome_memory[pos]);
                if(awesome_memory[pos]=='z') {
                    end = ~end;
                    pos = 0;
                    printf("\nEnd of input.\n");
                    break;
                }    
            }
            if(pos>0)
                printf("\n");
            sleep(1);
        }
    }
    sleep(1);

    return 0;
}
