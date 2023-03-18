//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************

#ifndef MEM_H
#define MEM_H
#define MAXDATA 4096

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct fich {
    int fd;
    char filename[255];
} tFich;

typedef enum cmd {
    MALLOC, SHARED, MMAP
} cmd;

typedef struct tItemM {
    cmd cmdType;
    void *dir;
    size_t size;
    char date[128];
    union {
        int key;
        tFich fich;
    } Union;
} tItemM;

typedef struct tListM{
    tItemM itemM [MAXDATA];
    int lastPos;
}tListM;


void createEmptyListM(tListM *L);

bool isEmptyListM(tListM L);

int firstM(tListM L);

int lastM(tListM L);

tItemM getItemM(int pos, tListM L);

void deleteItemM(int pos, tListM *L);

void deleteListM(tListM *L);

bool insertItemM(tItemM item, tListM *L);

#endif