//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************

#ifndef PROC_H
#define PROC_H
#define MAXDATA 4096

#include <stdbool.h>
#include <sys/types.h>

typedef enum status {
    FINISHED, STOPPED, SIGNALED, ACTIVE, DESCONOCIDO
} status;

typedef struct tItemP {
    pid_t pid;
    status estado;
    char *user;
    char *command;
    char *date;
    int state;
    int end;
    int priority;
} tItemP;

typedef struct tListP{
    tItemP itemP [MAXDATA];
    int lastPos;
}tListP;


void createEmptyListP(tListP *L);

bool isEmptyListP(tListP L);

int firstP(tListP L);

int lastP(tListP L);

tItemP getItemP(int pos, tListP L);

void deleteItemP(int pos, tListP *L);

void deleteListP(tListP *L);

void updateItemP(tItemP item, int pos, tListP *L);

bool insertItemP(tItemP item, tListP *L);

#endif