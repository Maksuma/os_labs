
#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/wait.h>

//  Максимальная длина одной строки ввода 
#define MAX_INPUT_LEN  1024

//  Максимальное число аргументов в одной команде 
#define MAX_ARGS   64


extern char* CURRENT_PATH;

char** parse_input(char* input, int* arg_count);
void free_args(char** args, int arg_count);
int execute_command(char** args, char** env);

#endif /* MYSHELL_H */
