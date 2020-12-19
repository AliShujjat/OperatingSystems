#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include <signal.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include <errno.h>
#include <fcntl.h>
#include <assert.h>

// Clearing the shell using escape sequences 
#define clear() printf("\033[H\033[J") 
extern  char** environ;
  
// Greeting shell during startup 
void Greet() 
{ 
    clear(); 
    char* username = getenv("USER"); 
    printf("\n\n\n\n******************"
        "************************"); 
    printf("\n\n\n\t****Habib University SHell****"); 
    printf("\n\n\thello user: %s", username); 
    printf("! :) \n\n\n\n*******************"
        "***********************");  
    printf("\n"); 
    sleep(3); 
    clear(); 
} 



// Function to print Current Directory. 
void printDir() 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\nDir: %s", cwd); 
} 


int shell()
{
    //input stored here
    char* inputLine = (char*)malloc(80);
    //stores each command into array
    char* argv[80];
    char* input;
    int i;
    int numofArgs = 0;

    printf("%s\n",argv[0]);

    //command = "i";
    argv[0] = "i";

    while (strcmp(argv[0], "exit") != 0)
    {
        numofArgs = 0;
        printf("\n>>>");

        //repeated slices the input, with space being the delimiter
        fgets(inputLine, 80, stdin);
        input = strtok(inputLine, " \n");

        

        for (i=0; input != NULL; i++) 
        {
            numofArgs++;
            argv[i] = malloc(strlen(input) + 1);
            strncpy(argv[i], input, strlen(input));
            input = strtok(NULL, " \n");
        }
  
        i = 0;

        //creates a child process
        int rc = fork();
        if (rc < 0) 
        {
            // fork failed; exit
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        else if (rc == 0) 
        {
            // child (new process)
            //printf("%s\n", argv[numofArgs -1]);
            printf("%s\n", argv[numofArgs-2]);

            printf("hello, I am child! (pid:%d)\n", (int) getpid());
            char *myargs[5];

            //implementation of > and >> file redirection
            if (strcmp(argv[numofArgs-2], ">") == 0 || strcmp(argv[numofArgs-2], ">>") == 0 )
            {
                printf("success");
                char file[80];
                int fd1;
                strcpy(file, "./");
                strcat(file,argv[numofArgs-1]);
                printf("%s\n",file );
                if (strcmp(argv[numofArgs-2], ">") == 0 )
                {
                    //creates new file if it doesn't exist, otherwise overwrites
                    fd1 = creat(file , 0644) ;
                }
                else
                {
                    //opens existing file and appends into it
                    fd1 = open(file, O_APPEND | O_RDWR, S_IREAD | S_IWRITE);
                }
                dup2(fd1, STDOUT_FILENO);
                close(fd1);
                myargs[0] = strdup(argv[0]);
                myargs[1] = NULL;
                execvp(myargs[0], myargs);  
            }
            else
            {
                
                for (i; i < numofArgs; i++)
                {
                    myargs[i] = strdup(argv[i]); 
                }
                myargs[i] = NULL;

                //implementation of cd
                if (strcmp(argv[0], "cd") == 0)
                {
                    if (numofArgs > 1)
                    {
                        chdir(argv[1]);
                    }
                    else
                    {
                        chdir(getenv("HOME"));
                    }
                }
                else if (strcmp(argv[0], "mylsenv") == 0) 
                {
                    char* env;
                    int count = 0;
                    printf("Environment variables: \n");
                    while (env == environ [count++])
                    {
                        printf("\n%s", env);
                    }
                    getchar();
                } 
                else if (strncmp(argv[0], "VAR=", 4) == 0)
                {
                    const char* from = argv[0];
                    char* to = (char*) malloc(strlen(argv[0])-5);
                    strncpy(to, from+4, strlen(argv[0])-4);
                    setenv("VAR", to, 1);  
                    printf("yesss");
                }
                else if (strcmp(argv[0], "show") == 0)
                {
                    if (strcmp(argv[1], "VAR") == 0)
                        printf("The env is: %s\n",getenv("VAR"));
                    else
                        printf("The variable has not been assigned.");
                }
                else
                {
                    execvp(myargs[0], myargs);  // runs command
                    printf("this shouldn't print out");
                }   
            }
        } 
        else 
        {
            // parent goes down this path (original process)
            int wc = wait(NULL);
            printf( "hello , I am parent of %d (pid:%d) \n",rc,(int)getpid());

        }        
    }
    free(inputLine);
    printf("Goodbye!\n");
    return 0; 
}



int main() 
{ 
   // Greet();
    shell();
    return 0; 
} 