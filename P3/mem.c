//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************

#include "mem.h"

void createEmptyListM(tListM *L) {
    L->lastPos=-1;
}

bool isEmptyListM(tListM L) {
    return L.lastPos==-1;
}

int firstM(tListM L) {
    return 0;
}

int lastM(tListM L) {
    return L.lastPos;
}


tItemM getItemM(int pos, tListM L) {
    return L.itemM[pos]; 
} 

void deleteItemM(int pos, tListM *L) {
    for(int i=0;i<lastM(*L);i++){
        if(i>=pos) {
            L->itemM[i] = L->itemM[i + 1];
        }
    }
    L->lastPos--;
}

void deleteListM(tListM *L) {
    for(int i=0;i<=L->lastPos;i++){
        deleteItemM(i, L);
        L->lastPos--;
    }
}


bool insertItemM(tItemM i, tListM *L) {
    bool fin=false;
    if(L->lastPos < MAXDATA){
        L->lastPos++;
        L->itemM[L->lastPos]=i;
        fin = true;
    } else {
        fin=false;
    }
    return fin;
}
