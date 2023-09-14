#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

#define SHMKEY 55555

int main(int argc, char** argv){

        int shmid = shmget(SHMKEY, sizeof(int)*2, 0777);
        if(shmid == -1){
                printf("Error in shmget\n");
                return EXIT_FAILURE;
        }
        int * sharedTime = (int*) (shmat (shmid, 0, 0));

        printf("This is Child: %d, From Parent: %d, Seconds: %d, NanoSeconds: %d\n", getpid(), getppid(), sharedTime[0], sharedTime[1]);

        shmdt(sharedTime);

        return 0;

}
