#include "myshell.h"

char* CURRENT_PATH = NULL;

void shell_loop(FILE* file, char** env) {

    char** args;
    int arg_count;

    while(1) {
        char line[MAX_INPUT];

        if (file == stdin) {
            printf("myshell:%s>", CURRENT_PATH);
        }

        if (fgets(line, sizeof(line), file) == NULL) {
            printf("\nExiting myshell.\n");
            break;
        }

        args = parse_input(line, &arg_count);

        if (arg_count > 0) {
            execute_command(args, env);
        }
        free(args);
    }
    
}

int main(int argc, char** argv, char** env) {

    CURRENT_PATH = getcwd(NULL, 0);

    char SHELL_PATH[MAX_INPUT_LEN];
    snprintf(SHELL_PATH, sizeof(SHELL_PATH), "%s/myshell", CURRENT_PATH);
    setenv("SHELL", SHELL_PATH, 1);

    if (argc > 1) {
        FILE* batch_file = fopen(argv[1], "r");
        
        if (batch_file == NULL) {
            perror(argv[1]);
            return EXIT_FAILURE;
        }
        shell_loop(batch_file, env);
        fclose(batch_file);
    } else {
        shell_loop(stdin, env);
    }


    return 0;
}