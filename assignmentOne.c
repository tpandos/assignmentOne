/*
Tamara Pando 
Assignment 1 

*/
#include <sys/stat.h>       // for waitpid()
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>       // for waitpid()
#include <stdlib.h>         // for exit(1)

#define CHILDREN 2          // constant definition of number of children 

  int main() 
{ 
        
        pid_t spawns[CHILDREN]; // array of children
        int ret; 
        int pipefds[2]; // creating pipe 
    
        if(pipe(pipefds) < 0){  // error handling for pipe 

                perror("pipe error"); 
                exit(1); 
        }

        for(int i = 0; i < CHILDREN; i++) // loop will run n times (n=5) 
        { 
                spawns[i] = fork(); // create child

                if(spawns[i] == -1){
                        perror("failure to fork"); 
                        exit(1); 
                }

                if( spawns[i]  == 0) // if child was successfully created
                { 
                        if(i == 0){     
                                //sleep(10);  

                                // *** actions of child 1
                                
                                close(STDOUT_FILENO);       //closing stdout
                                dup(pipefds[1]);            //dup replaced stdout with write 
                                close(pipefds[0]);          //closing pipe read
                                close(pipefds[1]);

                                char* prog1[] = { "ls", "-F", 0};
                                ret = execvp(prog1[0], prog1);    // executing ls -F 
                                if(ret == -1){
                                        perror("exec error prog 1"); 
                                        exit(0); 
                                }
                                exit(1);

                         }else if(i == 1){
                                 //sleep(10); 
                                 
                                // *** actions of child 2

                                close(STDIN_FILENO);     //closing stdin
                                dup(pipefds[0]);         //replacing stdin with pipe read
                                close(pipefds[1]);       //clise pipe write
                                close(pipefds[0]);

                                char* prog2[] = { "nl", 0};
                                ret = execvp(prog2[0], prog2);
                                if(ret == -1){
                                        perror("exec error prog 2"); 
                                        exit(1); 
                                }
                                exit(0); 
                        }
                } 
        } 
        // sleep(10);

        // *** parent process

        int status, ret2; 

            ret2 = waitpid(spawns[0], &status, 0); // wait for children processes to terminate 
            //printf("\n %d terminated with return %d\n", 1, ret2); // confirm they are terminating by returning their pids before parent terminating  
        
         return 0; 
}       