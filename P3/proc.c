//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************

#include "proc.h"

void createEmptyListP(tListP *L) {
    L->lastPos=-1;
}

bool isEmptyListP(tListP L) {
    return L.lastPos==-1;
}

int firstP(tListP L) {
    return 0;
}

int lastP(tListP L) {
    return L.lastPos;
}


tItemP getItemP(int pos, tListP L) {
    return L.itemP[pos]; 
} 

void deleteItemP(int pos, tListP *L) {
    for(int i=0;i<lastP(*L);i++){
        if(i>=pos) {
            L->itemP[i] = L->itemP[i + 1];
        }
    }
    L->lastPos--;
}

void deleteListP(tListP *L) {
    for(int i=0;i<=L->lastPos;i++){
        deleteItemP(i, L);
        L->lastPos--;
    }
}

void updateItemP(tItemP item, int pos, tListP *L){
int i;
    for(i=0;i<=lastP(*L);i++){
        if(i==pos){
            L->itemP[i] = item;
        }
    }
}

bool insertItemP(tItemP i, tListP *L) {
    bool fin=false;
    if(L->lastPos < MAXDATA){
        L->lastPos++;
        L->itemP[L->lastPos]=i;
        fin = true;
    } else {
        fin=false;
    }
    return fin;
}
