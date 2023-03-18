#include "p3.h"
#include "proc.h"
#include "lista.h"
#include "mem.h"
#include "p0.h"

//Código dos profes

int BuscarVariable (char * var, char *e[]){  /*busca una variable en el entorno que se le pasa como parÃ¡metro*/
    int pos=0;
    char aux[MAXVAR];
  
    strcpy (aux,var);
    strcat (aux,"=");
  
    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else 
            pos++;
    errno=ENOENT;   /*no hay tal variable*/
    return(-1);
}


int CambiarVariable(char * var, char * valor, char *e[]){ /*cambia una variable en el entorno que se le pasa como parÃ¡metro lo hace directamente, no usa putenv*/
    int pos;
    char *aux;
   
    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);
 
    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
	    return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

/*las siguientes funciones nos permiten obtener el nombre de una senal a partir
del nÃºmero y viceversa */


static struct SEN sigstrnum[]={   
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL}, 
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2}, 
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP}, 
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, 
	{"IO", SIGIO},
	{"SYS", SIGSYS},
/*senales que no hay en todas partes*/

#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
 	{NULL,-1},
	};  
       /*fin array sigstrnum */

/*devuelve el numero de senial a partir del nombre*/ 

int ValorSenal(char * sen)  { 
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	    if (!strcmp(sen, sigstrnum[i].nombre))
		    return sigstrnum[i].senal;
    return -1;
}

 /*devuelve el nombre senal a partir de la senal*/
/* para sitios donde no hay sig2str*/

char *NombreSenal(int sen){
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
  	    if (sen==sigstrnum[i].senal)
		    return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}

//COMANDO PRIORITY

int cambiar_prioridad(char *val, pid_t pid){
	int valor;
	valor = strtol(val, NULL, 10);
	if(setpriority(PRIO_PROCESS, pid, valor)==-1){
		return -1;
	}	
	return valor;
}

void comPriority (char *trozos[], tListP *list_proc){
    int prioridad, valor, valor_prio;
	pid_t pid;
	if(trozos[1]==NULL){
		if((prioridad = getpriority(PRIO_PROCESS, 0))==-1){
			printf("Error: %s\n", strerror(errno));
		}
		else {
			pid = getpid();
			printf("Prioridad del proceso actual %d es %d\n", pid, prioridad);
		}
	}else if(trozos[2]==NULL){
		if((pid = (pid_t)strtol(trozos[1], NULL, 10))==0){
			printf("Número de argumentos inválidos: %s\n", strerror(errno));
		}
		if((prioridad = getpriority(PRIO_PROCESS, 0))==-1){
			printf("Error: %s\n", strerror(errno));
		} 
		else {
			printf("Prioridad del proceso %d es %d\n", pid, prioridad);
		}
	}else if(trozos[3]==NULL){
		if((pid = (pid_t)strtol(trozos[1], NULL, 10))==0){
			printf("Número de argumentos inválidos: %s\n", strerror(errno));
		}
		if((valor = cambiar_prioridad(trozos[2], pid))==-1){
			printf("Error: %s\n", strerror(errno));
			return;
		}
		else 
			printf("Prioridad del proceso %d cambio a %d\n", pid, valor);
		
	} else {
		printf("Comando no encontrado: %s\n", strerror(errno));
	}
}

//COMANDO SHOWVAR

void imprimir_var (char *env[], char *nombre){
	int i; 
	for (i = 0; env[i]!=NULL; i++){
		printf("%p->%s[%d]=(%p) %s\n", &env[i], nombre, i, env[i], env[i]);
	}
} 

void comShowvar (char *trozos[], char *arg3[], char *environ[]){
    int i, j;
	char *valor;
	if (trozos[1]==NULL){ 
		imprimir_var(arg3, "main arg3");
	} else if (trozos[2]==NULL){
		valor = getenv(trozos[1]);
		if(valor == NULL){
			printf("Variable %s no existe\n", trozos[1]);
			return;
		} 
		if((i = BuscarVariable(trozos[1], arg3)) == -1){
			printf("ERROR\n");
			return;
		} 
		if((j = BuscarVariable(trozos[1], environ))==-1){
			printf("ERROR\n");
			return;
		} else {
			printf("Con arg3 main:\t%s (%p) %p\n", arg3[i], arg3[i], &arg3[i]);
			printf("Con environ:\t%s (%p) %p\n", environ[j], environ[j], &environ[j]);
			printf("Con getenv:\t%s (%p)\n", valor, &valor);
		}
	} else {
		printf("ERROR\n");
	}
}

//COMANDO CHANGEVAR

void comChangevar (char *trozos[], char *arg3[], char *environ[]){
    int i;
    int cnt;
	char *variable;
	for ( cnt=0; trozos[cnt]!=NULL; cnt++);
	
	if(cnt==4){
	
		variable = malloc(strlen(trozos[2]) + strlen(trozos[3]) + 4);
		strcpy(variable, trozos[2]);
		strcat(variable, "=");
		strcat(variable, trozos[3]);
		if(strcmp(trozos[1], "-a")==0){
			if((i = BuscarVariable(trozos[2], arg3))==-1){
				printf("Variable de entorno %s no existe\n", trozos[2]);
				free(variable);
				return;
			} 
			arg3[i] = variable;
		} else if (strcmp(trozos[1], "-e") == 0){
			if((i = BuscarVariable(trozos[2], environ))==-1){
				printf("Variable de entorno %s no existe\n", trozos[2]);
				free(variable);
				return;
			}
			environ[i] = variable;
		} else if (strcmp(trozos[1], "-p") == 0){
			if(putenv(variable)!=0){
				printf("Variable de entorno %s no existe\n", trozos[2]);
				free(variable);
				return;
			}
		} else{
			free(variable);
			printf("Uso: changevar [-a|-e|-p] var valor\n");
			return;
		}
		
		printf("Variable de entorno %s ha cambiado su valor a %s\n", trozos[2], trozos[3]);
	} else {
		printf("Uso: changevar [-a|-e|-p] var valor\n");
	}
}

//COMANDO SHOWENV

void imprimir_var_entorno(char *env[], char *environ[]){
	printf("main arg3: %p (almacenado en %p)\n", env, &env);
	printf("environ: %p (almacenado en %p)\n", environ, &environ);
}

void comShowenv(char *trozos[], char *arg3[], char *environ[]){
    if(trozos[1]==NULL){
		imprimir_var(arg3, "main arg3");
	} else if(trozos[2]==NULL && strcmp(trozos[1], "-environ")==0){
		imprimir_var(environ, "environ");
	} else if (trozos[2]==NULL && strcmp(trozos[1], "-addr")==0){
		imprimir_var_entorno(arg3, environ);
	} else {
		printf("Uso: showenv[-environ|-addr]\n");
	}
}

// COMANDO FORK

void comFork (char *trozos[], tListP *list_proc){  
    pid_t pid;
	if(trozos[1]==NULL){
		if((pid = fork())==-1){
			printf("Error: %s\n", strerror(errno));
		} else if (pid == 0){
			deleteListP(list_proc);
			printf ("ejecutando proceso %d\n", getpid());
		} else {
			waitpid (pid,NULL,0);
		}

	} else {
		printf("Numero de argumentos invalidos: %s\n", strerror(errno));
	}
}

//COMANDO EXECUTE

void comExecute(char *trozos[], char *comando, tList *list, tListM *list_mem, tListP *list_proc, int *a, char *environ[]){
    if(trozos[1]==NULL){
		printf("Uso: execute var1 var2 .. prog args ... [@pri]\n");
	} else {
		int i,y, variables = 0;
		char *resto[MAX];
		int n = trocearCadena(comando, trozos);
		pid_t pid;
		for(i=1; i<n; i++){ //Bucle para recorrer variables entorno
			if(BuscarVariable(trozos[i], environ)!=-1){
				resto[i-1] = trozos[i];
				variables = variables + 1;
			}
		}
		if (strstr(trozos[n - 1], "@") != NULL) {
            y = atoi(trozos[n - 1]+1);
            trozos[n - 1] = NULL;
        } else{
			y = 0;
		}
		if(variables!=0){
			int posicion = variables + 1;
			if(setpriority(PRIO_PROCESS, pid, y)==-1){
				return;
			}
			if(execvpe(trozos[posicion], trozos+posicion, (i==0)? environ : resto)==-1){
				printf("No se puede ejecutar %s: %s\n", trozos[posicion], strerror(errno));
				delete(list);
        		deleteListM(list_mem);
        		deleteListP(list_proc);
        		*a=0;
			}
		} else {
			if(setpriority(PRIO_PROCESS, pid, y)==-1){
				return;
			}
			if(execvp(trozos[1], &trozos[1])==-1){
				printf("ERROR\n");
			}
		}
	}
}

//COMANDO LISTJOBS

tItemP actualizar_estado(tItemP item, int opciones){
	int estado;
	if(opciones != 0){
		opciones = WNOHANG | WUNTRACED | WCONTINUED; 
	}

	if(waitpid(item.pid, &estado, opciones)==item.pid){
		if(WIFEXITED(estado)){
			item.state = FINISHED;
			item.estado = FINISHED;
			item.end = WEXITSTATUS(estado);
		} else if (WIFCONTINUED(estado)){
			item.state = ACTIVE;
			item.estado = ACTIVE;
		} else if (WIFSTOPPED(estado)){
			item.state = STOPPED;
			item.estado = STOPPED;
			item.end = WTERMSIG(estado);
		} else if (WIFSIGNALED(estado)){
			item.state = SIGNALED;
			item.estado = SIGNALED;
			item.end = WTERMSIG(estado);
		} else {
			item.state = DESCONOCIDO;
			item.estado = DESCONOCIDO;
		}
	}
	return item;
}

void imprimir_job(tItemP item){
	char *estado;
	if(item.estado == STOPPED){
		estado = "STOPPED";
		printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.command);
	}
	if(item.estado == ACTIVE){
		estado = "ACTIVE";
		printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.command);
	}
	if(item.estado == SIGNALED){
		estado = "SIGNALED";
		printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.command);
	}
	if(item.estado == FINISHED){
		estado = "FINISHED";
		printf("%d\t%s p=%d %s %s (%d) %s\n", item.pid, item.user, getpriority(PRIO_PROCESS, item.pid), item.date, estado, item.end, item.command);
	}
	if(item.estado == DESCONOCIDO){
		estado = "DESCONOCIDO";
	}
}

void listar(tListP *list_proc){
	int pos;
	tItemP item;
	if(isEmptyListP(*list_proc)){
		return;
	} else {
		for(pos = firstP(*list_proc); pos <= lastP(*list_proc); pos++ ){
			item = getItemP(pos, *list_proc);
			item = actualizar_estado(item, 1);
			updateItemP(item, pos, list_proc);
			imprimir_job(item);
		}
	}
}

void comListjobs (char *trozos[], tListP *list_proc){
	if(trozos[1]==NULL){
		listar(list_proc);
	}
	else {
		printf("Número de argumentos inválido\n");
	}
}

//COMANDO DELJOBS

void auxDeleteSig (tListP *list_proc){
	int pos;
	tItemP item;
	if(isEmptyListP(*list_proc)){
		return;
	} else {
		pos = firstP(*list_proc);
		for(pos; pos<=lastP(*list_proc); pos++){
			item = getItemP(pos, *list_proc);
			if(item.estado==SIGNALED){
				deleteItemP(pos, list_proc);
			}
		}
	}
}

void auxDeleteTerm (tListP *list_proc){
	int pos;
	tItemP item;
	if(isEmptyListP(*list_proc)){
		return;
	} else {
		pos = firstP(*list_proc);
		for(pos; pos<=lastP(*list_proc); pos++){
			item = getItemP(pos, *list_proc);
			if(item.estado==FINISHED){
				deleteItemP(pos, list_proc);
			}
		}
	}
}

void comDeljobs (char *trozos[], tListP *list_proc){
    if(trozos[1]==NULL){
		listar(list_proc);
		return;
	} else if(trozos[2]==NULL && strcmp(trozos[1], "-term")==0){
		auxDeleteTerm(list_proc);
		return;
	} else if (trozos[2]==NULL && strcmp(trozos[1], "-sig")==0){
		auxDeleteSig(list_proc);
		return;
	} else {
		listar(list_proc);
		return;
	}
}

//COMANDO JOB

void imprimir_fin(tItemP item){
	switch (item.estado){
	case FINISHED:
		printf("Proceso %d ha sido finalizado con señal %d (%s)\n", item.pid, item.end, NombreSenal(item.end));
		break;
	case STOPPED:
		printf("Proceso %d ha sido parado con señal %d (%s)\n", item.pid, item.end, NombreSenal(item.end));
		break;
	case SIGNALED:
		printf("Proceso %d ha existido con %d\n", item.pid, item.end);
		break;
	case ACTIVE:
		printf("Proceso %d esta activo ahora\n", item.pid);
		break;
	default:
		printf("Proceso %d estado\n", item.pid);
		break;
	}
}

void comJob (char *trozos[], tListP *list_proc){
    pid_t pid;
	int pos;
	tItemP item, aux;
	bool llego = false;
	int i;
	if(trozos[1]==NULL){
		comListjobs(trozos, list_proc);
	} else if (trozos[2]==NULL){
		if(isEmptyListP(*list_proc)){
			return;
		} else {
			pid = strtol(trozos[1], NULL, 10);
			for(i=0; i<=lastP(*list_proc); i++){
				aux = getItemP(i, *list_proc);
				if(aux.pid == pid){
					aux = actualizar_estado(aux, 1);
					updateItemP(aux, pos, list_proc);
					imprimir_job(aux); 
					llego = true;
				}
			} 
			if(llego==false){
				comListjobs(trozos, list_proc);
			}
		}	
	} else if (trozos[3]==NULL){
		pid = strtol(trozos[2], NULL, 10);
		if(strcmp(trozos[1], "-fg")!=0){
			printf("Argumentos invalidos\n");
			return;
		} 
		if(isEmptyListP(*list_proc)){
			return;
		} else { 
			for(i=0; i<=lastP(*list_proc); i++){
				aux = getItemP(i, *list_proc);
				if(aux.pid == pid){
					aux = actualizar_estado(aux, 0); 
					free(item.command);
					free(item.date);
					free(item.user);
					deleteItemP(pos, list_proc);
					llego = true;
				} 
			}	 
			if(llego==false){
				printf("Proceso %d no esta en la lista de procesos en segundo plano\n", pid);
				return;
			}
		}	
	} else {
		printf("Uso: job [-fg] pid\n");
	}
}

// COMANDO FUNCIONES DE LINUX

void comFuncionesLinux(char *trozos[], char *comando, tListP *list_proc, char *environ[]) {
    int n = trocearCadena(comando, trozos);
    int i, i_variables = 0;
	uid_t uid;
	pid_t pid;
	bool hay_arroba = false;
	bool hay_var = false;

    for (i = 0; BuscarVariable(trozos[i], environ) != -1; ++i) {
		i_variables = i_variables + 1;
		hay_var = true;
    }

	char *variables_l[i_variables];

	for (i = 0; BuscarVariable(trozos[i], environ) != -1; ++i) {
		variables_l[i] = trozos[i];
    }
	 

	
	uid=geteuid();
   
    int x = 0;
    int y = 0;
    if (strcmp(trozos[n - 1], "&") == 0) {
        x = 1;
        trozos[n - 1] = NULL;
        tItemP item;
        time_t t;
        char fecha[128], *usuario_i;
   		struct passwd *usuario;
        if (strstr(trozos[n - 2], "@") != NULL) {
            y = atoi(trozos[n - 2]+1);
            hay_arroba = true;
        } else {
			y = 0;
		}

		item.command = malloc(strlen(trozos[i_variables]) + 2);
        strcpy(item.command, trozos[i_variables]);
	
        for(i = i_variables + 1; i < n-1; i++){
        	item.command = realloc(item.command, strlen(item.command) + strlen(trozos[i]) + 2);
			strcat(item.command, " ");
			strcat(item.command, trozos[i]);	
        } 

		if(hay_arroba){
			trozos[n-2] = NULL;
		}
		if((pid = fork())==-1){
        	fprintf(stderr, "%s '%s' \n", strerror(errno), comando);
        	return;
    	}
    	if (pid == 0) {
			if(hay_var){
				if (execvpe(trozos[i_variables], trozos + i_variables, variables_l) == -1){
            		fprintf(stderr, "%s '%s'\n", strerror(3), trozos[i_variables]);
        			exit(1);
				}
			}
        	else {
				if (execvpe(trozos[i_variables], trozos + i_variables, environ) == -1){
            		fprintf(stderr, "%s '%s'\n", strerror(3), trozos[i_variables]);
        			exit(1);
				}
			}
            // En caso de ser o pai
    	} else {
        	//O proceso pai e mais o proceso fillo executanse ao mesmo tempo
    	}
		t = time(0);
        strftime(fecha, 128, "%d/%m/%y %H:%M:%S", localtime(&t));
        item.pid = pid;
        item.date = malloc(strlen(fecha)+1);
        strcpy(item.date, fecha);
        item.end = 0;
        item.estado = ACTIVE;
		item.state = ACTIVE;
        usuario = getpwuid(uid);
		usuario_i = usuario->pw_name;
        item.user = malloc(strlen(usuario_i)+1);
        strcpy(item.user, usuario_i);
		item.priority = setpriority(PRIO_PROCESS, item.pid, y);
        
        insertItemP(item, list_proc);
	} 
	else {
        if (strstr(trozos[n - 1], "@") != NULL) {
            y = atoi(trozos[n - 1]+1);
            trozos[n - 1] = NULL;
        } else{
			y = 0;
		}
		if((pid = fork())==-1){
        	fprintf(stderr, "%s '%s' \n", strerror(errno), comando);
        	return;
    	}
    	else if (pid == 0) {
			if(setpriority(PRIO_PROCESS, pid, y)==-1){
				return;
			}
        	if(hay_var){
				if (execvpe(trozos[i_variables], trozos + i_variables, variables_l) == -1){
            		fprintf(stderr, "%s '%s'\n", strerror(3), trozos[i_variables]);
        			exit(1);
				}
			}
        	else {
				if (execvpe(trozos[i_variables], trozos + i_variables, environ) == -1){
            		fprintf(stderr, "%s '%s'\n", strerror(3), trozos[i_variables]);
        			exit(1);
				}
			}
            // En caso de ser o pai
    	} else {
        	waitpid(pid, NULL, 0);
    	} 
	}
	hay_arroba = false;
	hay_var = false;
}
