/*
 * 
    Created by Alex Bautista
    * 
    * 
   *  
   *  
   * 
 * 
 */
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#define BUFFER 1024
 
//setting bool variables for symbol options 
bool background = false;
bool outRe = false;
bool errRe = false;
bool inpRe = false;
bool pipeRe = false;

//used John Baldwin split line code from hw3
static bool
split_line(char *line, char **argv, int max_args)
{
	char *word;
	int words;
    
	words = 0;
	word = strtok(line, " \t\n");
	while (word != NULL) {
		if (words >= max_args) {
			printf("Too many arguments\n");
			return (false);
		}
		argv[words] = word;
		words++;
		word = strtok(NULL, " \t\n");
	}
	argv[words] = NULL;
	return (true);
}


	

int
main(void)
{
	int wfd;//read amd write file descriptors
	int rfd;
	char *argv[6];
	char *words[6];
	char userIn[BUFFER];
	//2 child processes
	pid_t pid,pid2;
	//1 pipe
	int pfds[2];
	pipe(pfds);

	for (;;) {
		printf("alexShell ~ $ ");
		if (fgets(userIn, sizeof(userIn), stdin) == NULL) {
			if (feof(stdin))
				return (0);
			fprintf(stderr, "Failed to read command: %s\n",
			    strerror(errno));
			return (1);
		}
		if (!split_line(userIn, argv, 6)){
			continue;
		}
		for(int j = 0; j < 6; j++){
			words[j] = argv[j];
		}
		
		
		int argc = 0;
    for (int i = 0; words[i] != NULL; i++) {
		
        if (strcmp(words[i], ">") == 0) {//output redirection
           
           outRe = true;
           //close(1);
           wfd = open(words[2],O_WRONLY);
	       if(wfd == -1){
		  fprintf(stderr, "fail to open %s", words[2]);
		  return 1;
	  }
              
        }else if (strcmp(words[i], ">>") == 0) {//output append
              
         
           outRe = true;
           wfd = open(words[2],O_WRONLY, O_APPEND);
	       if(wfd == -1){
		  fprintf(stderr, "fail to open %s", words[2]);
		  return 1;
	  }
         
        }else if (strcmp(words[i], "&") == 0) {//background
          background = true;
              
        }else if (strcmp(words[i], "2>") == 0){//error
		  errRe = true;
		  wfd = open(words[2],O_WRONLY);
	      if(wfd == -1){
		  fprintf(stderr, "fail to open %s", words[2]);
		  return 1;
	  }
		}else if (strcmp(words[i], "2>>") == 0){//error append
		  errRe = true;	 
		  wfd = open(words[2],O_WRONLY, O_APPEND);
	      if(wfd == -1){
		  fprintf(stderr, "fail to open %s", words[2]);
		  return 1;
	  }
			 
		}else if (strcmp(words[i], "<") == 0){//output
		  inpRe = true;
		  rfd = open(words[3],O_RDONLY);
	      if(rfd == -1){
		  fprintf(stderr, "fail to open %s", words[3]);
		  return 1; 
		  
	  }
			 
		}else if (strcmp(words[i], "|") == 0){//pipes
		  pipeRe = true;
		  
	      
	  
			 
		}else {
              //store words in argv if did not match tokens
              argv[argc] = words[i];
              argc++;
         }
    }
     //set last element to null for termination
    argv[argc] = NULL;
		

    
		if (argv[0] == NULL) {
			printf("========TYPE IN COMMANDS========\n");
			continue;
		}
		if (strcmp(argv[0], "exit") == 0){
			return (0);
		}
		
			
			
			
		pid = fork();
		if (pid == 0) {
			//set of if statements to handle file redirection
			if(outRe == true){
				//close(1);
			    dup2(wfd, 1);
			    close(wfd);
		}
		
		    if(errRe == true){
				dup2(wfd,2);
				close(wfd);
		}
		    if(inpRe == true){
				
				dup2(rfd,0);
				close(rfd);
		}
		    if(pipeRe == true){
				
				//setting child stdout to point to what pfds1 is pointing to
				dup2(pfds[1], STDOUT_FILENO);
				//closing pipe end
				close(pfds[1]);
				 
				//forking another child and connecting them
				//through pipes 
				pid2 = fork();
				 if(pid2 == 0){
				//setting child stdin to point to what pdfs0 is pointing to	 
				dup2(pfds[0], STDIN_FILENO);
				//closing pipe end 
				close(pfds[0]);
				execvp(argv[2],argv);
                 fprintf(stderr, "error: could not execute child\n");
                  return 1; 	 
				}
				if (pid == -1) {
			fprintf(stderr, "Failed to fork: %s\n",
			    strerror(errno));
			return (1);
		}	 
				
		}				
			execvp(argv[0],argv);
       fprintf(stderr, "error: could not execute child\n");
       return 1;
		}  
		//closing unused pipes in parent process
		close(pfds[0]);
		close(pfds[1]);
		close(wfd);
		
		
		if (pid == -1) {
			fprintf(stderr, "Failed to fork: %s\n",
			    strerror(errno));
			return (1);
		}
		
		
         //skip wait for background processes
		if (background == false){
         wait(NULL);
         printf("===============\n### Child exited\n===============\n");
	 }
	 else{
		 printf("===============\n### WAIT() omitted\n===============\n### Background process running\n===============\n");
		 background = false;
		 
	 }

	}

}
