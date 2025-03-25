#include "types.h"
#include "stat.h"
#include "user.h"


// this file calls the system call that allows us to choose
// whether we want the scheduler to run with the parent-first
// or child-first policy


int
main(int argc, char* argv[]){
   
    if (argc != 2){
        printf(1, "Usage: %s 0|1"
		  "\t0: Parent is scheduled to run most often\n"	
		  "\t1: Child is scheduled to run most often\n", argv[0]);
	exit();
    }
   
    int i = atoi(argv[1]);
    
    if(i != 1 || i != 0){
        printf(1, "Please provide the correct arguments.\n");
	exit();
    }    

    fork_winner(i); // passing arg to user space wrapper
		    // wrapper is void fork_winner(int winner)


   exit(); // return 0;
}
