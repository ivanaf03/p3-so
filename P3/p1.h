//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************


#ifndef P2_1
#define P2_1

#define MAXDATA 4096
#define MAX 1024

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>


void comCreate(char *trozos[], char *comando);
void comStat(char *trozos[], char *comando);
void comList(char *trozos[], char *comando);    
void comDelete(char *trozos[], char *comando);
void comDeltree(char *trozos[], char *comando);

char * ConvierteModo2 (mode_t m);
char LetraTF (mode_t m);

void rutaActual();
void getStats(char *filename);
void getLongStats(char *filename,bool acc,bool link); 
void fecha(time_t t);
void borrarDirectorios(const char *path);
bool esDirectorio(const char *path);
void getDirLongStats(char* trozos, bool listal, bool acc, bool link, bool hid, bool reca, bool recb);
bool isHidden(char *file);
bool esLink(mode_t str);

#endif