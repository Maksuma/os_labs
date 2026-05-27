#include "myshell.h"


char** parse_input(char* input, int* arg_count) {
    char** args = malloc(MAX_ARGS * sizeof(char*));
    char* token;
    int count = 0;

    token = strtok(input, " \t\n");
    while (token != NULL && count < MAX_ARGS) {
        args[count++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[count] = NULL;
    *arg_count = count;
    return args;
}


// cd <directory> - change directory
// clr - clear the screen
// dir <directory> - list contents of directory
// environ - list environment variables
// echo <comment> - display comment on the screen
// pause - pause operation until "Enter" is pressed
// help - display user manual
int execute_command(char** args, char** env) {
    (void)env;
    
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            char* cwd = getcwd(NULL, 0);
            if (cwd) {
                printf("%s\n", cwd);
                free(cwd);
            } else {
                perror("getcwd");
            }
        } else if (chdir(args[1]) == 0) {
            CURRENT_PATH = getcwd(NULL, 0);
            setenv("PWD", CURRENT_PATH, 1);
        } else {
            perror("cd");
        }
    } 
    
    else if (strcmp(args[0], "clr") == 0) {
        printf("\033c");
    } 
    
    else if (strcmp(args[0], "dir") == 0) {
        char* dir_path = args[1] ? args[1] : ".";
        
        DIR* dir = opendir(dir_path);
        if (dir == NULL) {
            perror("dir");
        } else {
            struct dirent* entry;
            while ((entry = readdir(dir)) != NULL) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
                printf("%s\n", entry->d_name);
            }
            closedir(dir);  
        }
    } 
    
    else if (strcmp(args[0], "environ") == 0) {
        for (int i = 0; env[i] != NULL; i++) {
            printf("%s\n", env[i]);
        }
    } 
    
    else if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; args[i] != NULL; i++) {
            printf("%s", args[i]);
            if (args[i + 1] != NULL) {
                printf(" ");
            }
        }
        printf("\n");
    } 
    
    else if (strcmp(args[0], "pause") == 0) {
        printf("Press 'Enter' to continue...");
        fflush(stdout);
        getchar();
    } 
    
    else if (strcmp(args[0], "help") == 0) {
        printf("\nAvailable commands:\n");
        printf("cd <directory> - change the current directory to <directory>\n");
        printf("clr - clear the screen\n");
        printf("dir <directory> - list contents of <directory>\n");
        printf("environ - list all environment variables\n");
        printf("echo <comment> - display <comment> on the screen\n");
        printf("help - display this user manual\n");
        printf("pause - pause operation until 'Enter' is pressed\n");
        printf("quit or exit - exit the shell\n\n");

    } 
    
    else if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
        exit(EXIT_SUCCESS);
    } 
    
    else {
        pid_t pid = fork();

        if (pid == 0) {
            setenv("PARENT", getenv("SHELL"), 1);


            execvp(args[0], args);

            fprintf(stderr, "myshell: %s: command not found\n", args[0]);
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
        } else {
            waitpid(pid, NULL, 0);
        }
    }

    return 0;
}


