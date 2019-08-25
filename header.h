#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

/*
 *  header.h
 *  parse
 *
 *  Created by Pedro Sobral on 09/01/23.
 *  Copyright 2009 UFP. All rights reserved.
 *
 */
 #define _GNU_SOURCE 1

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXARGS 100
#define MAX_SIZE 4096
#define MAXLINE 2000
#define MAX_LEN 45

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KBLU  "\x1B[34m"
#define RESET "\x1B[0m"

#define TRUE 1
#define FALSE 0

typedef struct command {
    char *name;
    char *argv[MAXARGS];
    char *infile;
    char *outfile;
    char *errfile;
    struct command *next;
} CMD;

struct char_parms
{
  char* dirname;
  int i;
};

struct Find {
    char* filename;
    char* directory;
};

CMD *insert_command();
void free_command_list();
void print_command_list();
CMD * parse_line(char *);

struct sigaction sa;

void msh_cd (char *args);
void msh_pwd();
void execute (CMD* root);
char **character_name_completion(const char *text, int start, int end);
char *character_name_generator(const char *text, int state);
char **scan_directory(char** dirnames, size_t *elems);
char **get_path();
void print_files_color (char* dirname, struct stat mystat, int option);
void print_files_color2 (char* dirname, mode_t mystat, int option);
void print_permissions(mode_t mode);
void print_filetype(mode_t mode);
void print_time(time_t mod_time);
void* RecDir(void* param);

void findRec(char *fn);
void* consumidor(void* param);
void* produtor(void* param);


#endif // HEADER_H_INCLUDED
