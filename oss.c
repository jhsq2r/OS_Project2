#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>

//Creator: Jarod Stagner
//Turn-in date:

#define SHMKEY 55555

struct PCB {
        int occupied;
        pid_t pid;
        int startSeconds;
        int startNano;
};

void displayTable(int i, struct PCB *processTable){
        printf("Process Table:\nEntry Occupied PID StartS StartN\n");
        for (int x = 0; x < i; x++){

                printf("%d      %d      %d      %d      %d\n", x,processTable[x].occupied,processTable[x].pid,processTable[x].startSeconds,processTable[x].startNano);

        }
}

void updateTime(int *sharedTime){
        sharedTime[1] = sharedTime[1] + 100000000;
        if (sharedTime[1] >= 1000000000 ){
                sharedTime[0] = sharedTime[0] + 1;
                sharedTime[1] = sharedTime[1] - 1000000000;
        }
}

void help(){
        printf("Program usage\n-h = help\n-n [int] = Num Children to Launch\n-s [int] = Num of children allowed at once\n-t [int] = Max num of seconds for each child to be alive");
        printf("Default values are -n 5 -s 3 -t 3\nThis Program is designed to take in 3 integers for Num Processes, Num of processes allowed at once,\nand Max num of seconds for each process");
}

int main(int argc, char** argv) {

        int proc = 5;
        int simul = 3;
        int maxTime = 3;//default parameters

        int shmid = shmget(SHMKEY, sizeof(int)*2, 0777 | IPC_CREAT);
        if(shmid == -1){
                printf("Error in shmget\n");
                return EXIT_FAILURE;
        }
        int * sharedTime = (int *) (shmat (shmid, 0, 0));
        sharedTime[0] = 0;
        sharedTime[1] = 0;

        int option;
        while((option = getopt(argc, argv, "hn:s:t:")) != -1) {//Read command line arguments
                switch(option){
                        case 'h':
                                help();
                                return EXIT_FAILURE;
                                break;
                        case 'n':
                                proc = atoi(optarg);
                                break;
                        case 's':
                                simul = atoi(optarg);
                                break;
                        case 't':
                                maxTime = atoi(optarg);
                                break;
                        case '?':
                                help();
                                return EXIT_FAILURE;
                                break;
                }
        }

        struct PCB processTable[20];
        int status;
        int i = 0;
        while(i < proc){
                srand(time(NULL));
                int pid = waitpid(-1,&status,WNOHANG);
                //printf("i = %d\n", i);
                updateTime(sharedTime);
                if (sharedTime[1] == 500000000){
                        displayTable(i, processTable);
                }

                if((i >= simul && pid != 0) || i < simul){
                        pid_t child_pid = fork();
                        if(child_pid == 0){
                                char convertSec[20];
                                char convertNan[20];

                                int randomSec = (rand() % ((maxTime - 1) -1 + 1)) + 1;
                                int randomNano = (rand() % (999999999 - 1 + 1)) + 1;

                                sprintf(convertSec, "%d", randomSec);
                                sprintf(convertNan, "%d", randomNano);

                                char *args[] = {"./worker", convertSec, convertNan, NULL};
                                execvp("./worker", args);

                                printf("Something horrible happened...\n");
                                exit(1);
                        }else{
                                processTable[i].occupied = 1;
                                processTable[i].pid = child_pid;
                                processTable[i].startSeconds = sharedTime[0];
                                processTable[i].startNano = sharedTime[1];
                        }
                        i++;
                }

        }

        for(int i = 0; i < simul; i++){
                wait(NULL);//wait for all children to die
        }

        shmdt(sharedTime);
        shmctl(shmid,IPC_RMID,NULL);

        printf("Done\n");

        return 0;
}


