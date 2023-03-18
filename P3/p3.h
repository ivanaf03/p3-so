//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************

#ifndef P3_3
#define P3_3

#define MAXDATA 4096
#define MAXVAR 4096
#define MAXNAME 1024
#define MAX 1024
#define TAMANO 2048
#define _GNU_SOURCE 

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>
#include "proc.h"
#include "lista.h"
#include "mem.h"

struct SEN{
    char *nombre;
    int senal;
};

void comPriority (char *trozos[], tListP *list_proc);

void comShowvar (char *trozos[], char *arg3[], char *environ[]);

void comChangevar (char *trozos[], char *arg3[], char *environ[]);

void comShowenv(char *trozos[], char *arg3[], char *environ[]);

void comFork (char *trozos[], tListP *list_proc);

void comExecute(char *trozos[], char *comando, tList *list, tListM *list_mem, tListP *list_proc, int *a, char *environ[]);

void comListjobs (char *trozos[], tListP *list_proc);

void comDeljobs (char *trozos[], tListP *list_proc);

void comJob (char *trozos[], tListP *list_mem);

void comFuncionesLinux(char *trozos[], char *comando, tListP *list_proc, char *environ[]);

#endif
