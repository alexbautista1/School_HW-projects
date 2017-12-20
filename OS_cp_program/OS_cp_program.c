/*
 * Created By Alex Bautista
 * 
 * 
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER 1024


int main(int argc, char **argv)
{
	//return values from system calls and character buffer array
	int inputFile;
	int outputFile;
	ssize_t bytesIn;
	ssize_t bytesOut;
	char charBUF[BUFFER];
	
	//tries to open file in args1
	 inputFile = open(argv[1], O_RDONLY);
	  if(inputFile==-1){
		  fprintf(stderr, "fail to open");
		  return 1;
	  }
	  printf("success open file1\n");
	  
	  //tries to create file in args 2, write only and create
	  //O_EXCL makes sure not to overwrite existing file
	  outputFile = open(argv[2], O_WRONLY | O_CREAT | O_EXCL);
	   if(outputFile == -1){
		   fprintf(stderr, "fail to open");
		   return 1;
	   }
	   printf("success open file2\n");
	   
	   //loops through first file and read and writes through each loop
	   //also writes to second file through each loop the size of buffer each time
	    while((bytesIn = read(inputFile, &charBUF, BUFFER)) != 0){
			
			bytesOut = write(outputFile, &charBUF,(ssize_t)bytesIn);
			
			
			//error for short write
			 if(bytesOut < bytesIn){
				 
				 fprintf(stderr, "fail to write!!!");
				 return 1;
			 }
		}
		
		//struct that uses fstat to find the size of the output file
		struct stat st;
		fstat(outputFile, &st);
		int size = st.st_size;
		
	  //outputs size of output file and closes both files
	  printf("Copied %d bytes\n", size);
	  close(inputFile);
	  close(outputFile); 
	
	return 0;
}

