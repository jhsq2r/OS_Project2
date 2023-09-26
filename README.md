This program is designed to take in 3 integers -n -s and -t
-n is the number of process to be launched
-s is the number of processes that can run at one time
-t is the max number of seconds a child can run
-h help menu
Default values: -n 5 -s 3 -t 3
Example execution: ./oss -n 15 -s 3 -t 5
The program will use its own internal clock to know when to launch child
processes and will end when all processes have completed or when 60 real life
seconds have passed.
Every half second the program will output a list of PCBs that will show the
pid, occupied flag, and start time of each process
