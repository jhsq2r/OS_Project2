#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

//Creator: Jarod Stagner
//Turn-in date: 

#define SHMKEY 55555

void help(){
        printf("Program usage\n-h = help\n-n [int] = Num Children to Launch\n-s [int] = Num of children allowed at once\n-t [int] = Max num of seconds for each child to be alive");
        printf("Default values are -n 5 -s 3 -t 3\nThis Program is designed to take in 3 integers for Num Processes, Num of processes allowed at once,\nand Max num of seconds for each process");
}

int main(int argc, char** argv) {

        int proc = 5;
        int simul = 3;
        int time = 3;//default parameters

        int shmid = shmget(SHMKEY, sizeof(int)*2, 0777 | IPC_CREAT);
        if(shmid == -1){
                printf("Error in shmget\n");
                return EXIT_FAILURE;
        }
        int * sharedTime = (int *) (shmat (shmid, 0, 0));
        sharedTime[0] = 5;
        sharedTime[1] = 999000000;

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
                                time = atoi(optarg);
                                break;
                        case '?':
                                help();
                                return EXIT_FAILURE;
                                break;
                }
        }

        for(int i = 0; i < proc; i++){
                if (i >= simul){
                        pid_t done_id = wait(NULL);
                }

                pid_t child_pid = fork();
                if (child_pid == 0){

                        char *args[] = {"./worker", "3", NULL};
                        execvp("./worker", args);//launch worker

                        printf("Something horrible happened...\n");
                        exit(1);
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
