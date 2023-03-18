//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************

#include "p2.h"
#include "p0.h"
#include "mem.h"

//VARIABLES GLOBALES CREADAS
int int_glocal = 0;
char char_glocal = 'r';
double float_glocal = 1.2;

//CADTOP
void *cadtop(char *string) {
    void *p;
    sscanf(string, "%p", &p);
    return p;
} 

//MOSTRAR LISTA COMPLETA

void mostrarmemoryList(tListM L) { //Función que recorre toda la lista de elementos insertados en ella
    int pos;
    tItemM item;

    printf("******* Lista de bloques asignados para el proceso %d\n", getpid());

    if (isEmptyListM(L)) {
        printf("\b");
    } else {
        for (pos = firstM(L); (pos <= lastM(L)); pos++) {
            item = getItemM(pos, L);
            if (item.cmdType == MALLOC) {
                printf("\t%p\t\t%zu %s malloc\n", item.dir, item.size, item.date);
            } 
            if (item.cmdType == MMAP) {
                printf("\t%p\t\t%zu mmap %s (fd: %d) %s\n", item.dir, item.size, item.Union.fich.filename, item.Union.fich.fd, item.date);
            }  
            if (item.cmdType == SHARED) {
                printf("\t%p\t\t%zu %s shared (key %d)\n", item.dir, item.size,item.date, item.Union.key);
            } 
        }
    }
}

//COMANDO ALLOCATE

void mostrar_malloc(tListM L) { //Función que recorre la lista que contiene los elementos de tipo MALLOC
    int pos;
    tItemM item;

    printf("******* Lista de bloques asignados para el proceso %d\n", getpid());

    if (isEmptyListM(L)) {
        printf("\b");
    } else {
        for (pos = firstM(L); (pos <= lastM(L)); pos++) {
            item = getItemM(pos, L);
            if (item.cmdType == MALLOC) {
                printf("\t%p\t\t%zu %s malloc\n", item.dir, item.size, item.date);
            }
        }
    }
}

void aux_ma(char *trozos[], tListM *list_mem){ //Función auxiliar que inserta los elementos de tipo MALLOC
    size_t tam;
    tItemM item;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    tam = strtol(trozos[2], NULL, 10);
    if(tam==0){ //Si trozos[2] == 0, no insertamos nada.
        printf("No se asignan bloques de 0 bytes\n");
    } else { //En caso contrario, insertamos el elemento con sus campos correspondientes
        item.cmdType = MALLOC;
        item.size = tam;
        item.dir = malloc(item.size);
        strftime(item.date, 128, "%b %d %H:%M", tlocal);

        if (item.dir == NULL) { //En caso de que no se pueda crear la dirección
            printf("ERROR\n");
            return;
        } else { //En caso contrario, insertamos en la lista
            if (insertItemM(item, list_mem)) {
                printf("Asignados %zu bytes en %p\n", item.size, item.dir);
            } else {
                perror("Imposible hacer malloc");
            }
        }
    } 
}
 

void mostrar_shared(tListM L) { //Función que recorre la lista que contiene los elementos de tipo SHARED
    int pos;
    tItemM item;

    printf("******* Lista de bloques asignados de tipo shared para el proceso %d\n", getpid());

    if (isEmptyListM(L)) {
        printf("\b");
    }
    else {
        for ((pos = firstM(L)); (pos <= lastM(L)); (pos++)) {
            item = getItemM(pos, L);
            if (item.cmdType == SHARED) {
                printf("\t%p\t\t%zu %s shared (key %d)\n", item.dir, item.size,item.date, item.Union.key);
            } 
        }
    }
} 

void * ObtenerMemoriaShmget (key_t clave, size_t tam, tListM *L){ //Función dada por los profesores, pero modificada por nosotros para insertar los diferentes campos de un elemento de tipo SHARED
    tItemM item;
    void * p;
    int aux,id,flags=0777;
    time_t tiempo = time(0);
    struct shmid_ds s;
    struct tm *tlocal = localtime(&tiempo);
    if (tam)     //tam distito de 0 indica crear 
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  //no nos vale
        {errno=EINVAL; 
        printf("%d no sirve como clave\n", clave);
        return NULL;}
    if ((id=shmget(clave, tam, flags))==-1){
        printf("Imposible asignar memoria compartida clave %d: %s\n", clave, strerror(errno));
        return (NULL);
    }
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        printf("No se puede crear el segmento de memoria compartida: %s\n", strerror(errno));
        return (NULL);
    } 
    if (shmctl (id,IPC_STAT,&s)!=-1){
        item.size=s.shm_segsz;
        item.dir=p;
        item.cmdType=SHARED;
        item.Union.key = clave;
        strftime(item.date, 128, "%b %d %H:%M", tlocal);
        if (p!=NULL) {
            if(insertItemM(item, L))
                printf("Asignados %lu bytes en %p\n", (unsigned long) s.shm_segsz, p);
            else 
                printf("No se puede insertar el elemento\n");
        }
        else{
            printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) clave,strerror(errno));
        }
    }
    return (p);
}

void do_AllocateCreateshared (char *tr[], tListM *L){ //Función creada por los profesores para la creación de un bloque de memoria compartida
    key_t cl;
    size_t tam;

    if (tr[0]==NULL || tr[1]==NULL) {
        mostrar_shared(*L);
        return;
    }

    cl=(key_t)  strtoul(tr[2],NULL,10);
    tam=(size_t) strtoul(tr[3],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    } else{
        ObtenerMemoriaShmget(cl,tam, L);
    }
}


void mostrar_mmap(tListM L) { //Función que recorre la lista que contiene los elementos de tipo MMAP
    int pos;
    tItemM item;

    printf("******* Lista de bloques asignados para el proceso %d\n", getpid());

    if (isEmptyListM(L)) {
        printf("\b");
    }
    
    else {
        for (pos = firstM(L); (pos <= lastM(L)); pos++) {
            item = getItemM(pos, L);
            if (item.cmdType == MMAP) {
                printf("\t%p\t\t%zu mmap %s (fd: %d) %s\n", item.dir, item.size, item.Union.fich.filename, item.Union.fich.fd, item.date);
            }
        }
    }
}

void * MapearFichero (char * fichero, int protection){ //Función dada por los profesores para poder mapear un fichero
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;
    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;
    return p;
}

void do_AllocateMmap(char *arg[], tListM *L){  //Función dada por los profesores para poder hacer un mapeado de un fichero
    char *perm;
    void *p;
    int protection=0;
    tItemM item;
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    struct stat file;
    stat(arg[2], &file);
     
    if ((perm=arg[3])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    p=MapearFichero(arg[2],protection);
    if (p==NULL)
        perror ("Imposible mapear fichero");
    else{
        item.cmdType = MMAP;
        item.size = file.st_size;
        item.dir = p; 
        strftime(item.date, 128, "%b %d %H:%M", tlocal);
        item.Union.fich.fd = (int)open(arg[2], O_CREAT, O_RDONLY);
        strcpy(item.Union.fich.filename,arg[2]);
        if(insertItemM(item,L)){
            printf ("fichero %s mapeado en %p\n", arg[2], p);
        } else {
            printf("No ha sido posible mapear el archivo\n");
        }
    }
}

void comAllocate(char *trozos[], tListM *list_mem){ //Comando Allocate
    if(trozos[1]==NULL){
        mostrarmemoryList(*list_mem);
    }     
    else if(strcmp(trozos[1],"-malloc")==0){
        if(trozos[2]==NULL){
            mostrar_malloc(*list_mem);
        }
        else{
            aux_ma(trozos, list_mem); //Llamada de función para facer malloc
        }
    } else if(strcmp(trozos[1],"-createshared")==0){
        if(trozos[2]==NULL || trozos[3]==NULL){
            mostrar_shared(*list_mem);
        } else {
            do_AllocateCreateshared(trozos, list_mem); //Llamada de función para facer createshared
        }
    } else if(strcmp(trozos[1],"-shared")==0){
        if(trozos[2]==NULL){
            mostrar_shared(*list_mem);
        } else {
            ObtenerMemoriaShmget((key_t)  strtoul(trozos[2],NULL,10), 0, list_mem); //Llamada de función para facer malloc
        }
    } else if(strcmp(trozos[1],"-mmap")==0){
        if(trozos[2]==NULL){
            mostrar_mmap(*list_mem);
        } else {
            do_AllocateMmap(trozos, list_mem); //Llamada de función para facer mmap
        }
    } else {
        printf("uso: allocate [-malloc|-shared|-createshared|-mmap] ....\n");
    } 
}

//COMANDO DEALLOCATE

void daux_ma (char *trozos[], tListM *list_mem){ //Función auxiliar para borrar un elemento de tipo malloc
    size_t tam;
    tItemM item;
    int pos;

    tam = strtol(trozos[2], NULL, 10);
    if(tam==0){
        printf("No se borran bloques de 0 bytes\n");
    } else {
        for ((pos = firstM(*list_mem));(pos <= lastM(*list_mem)); (pos ++)) {
            item = getItemM(pos, *list_mem);
            if(item.size==tam){
                if (item.dir == NULL) {
                    printf("ERROR\n");
                    return;
                }else {
                    printf("Borrado malloc de tamaño %ld que estaba en %p\n", item.size, item.dir);
                    deleteItemM(pos,list_mem);
                    free(item.dir);
                    break;
                }
            }
        }
    }
}

int do_DeallocateDelkey (char *args[]){ //Función dada por los profesores para borrar una clave de un bloque de memoria compartida
    key_t clave;
    int id;
    char *key=args[0];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return -1;
    }
    if ((id=shmget(clave,0,0666))==-1){
        perror ("shmget: imposible obtener memoria compartida");
        return -1;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1){
        perror ("shmctl: imposible eliminar memoria compartida\n");
        return -1;
    }
    return 0;
}

void daux_delkey(char *trozos[], tListM *list_mem){ //Función auxiliar que elimina una clave de memoria compartida
    key_t key;
    int pos;
    tItemM item;
    if(do_DeallocateDelkey(&trozos[2])!=-1){
        key = (key_t) atoi(trozos[2]);
        printf("Key %d eliminada del sistema\n", key);
        for(pos = firstM(*list_mem); pos <= lastM(*list_mem); pos++){
            item = getItemM(pos, *list_mem);
            if(item.cmdType == SHARED && item.Union.key == key){
                shmdt(item.dir);
            }
        }
    }
}

void daux_shared (char *trozos[], tListM *list_mem){ //Función auxiliar que elimina un bloque de memoria compartida
    key_t key = atoi(trozos[2]);
    int pos;
    tItemM item;

    for(pos = firstM(*list_mem); pos <= lastM(*list_mem); pos++){
        item = getItemM(pos, *list_mem);
        if(item.cmdType==SHARED){
            if(item.Union.key == key){
                if((shmget(item.Union.key, 0, 0))!= -1){
                    if(shmdt(item.dir)==-1){
                        printf("ERROR\n");
                        return;
                    } 
                }
                printf("Bloque de memoria compartida en %p (key %d) ha sido eliminado\n", item.dir, item.Union.key);
                deleteItemM(pos, list_mem);
            }
        }
    }
}

void daux_hex(char *trozos[], tListM *list_mem){ //Función auxiliar que una dirección de memoria anteriormente creada
    int pos;
    tItemM item;
    void *p;
    p = (void *) strtol(trozos[1], NULL, 16);
    for((pos = firstM(*list_mem)); (pos <= lastM(*list_mem)); (pos++)){
        item = getItemM(pos, *list_mem);
        if(item.dir==p){
            printf("Eliminada la dirección de memoria %s\n", trozos[1]);
            free(item.dir);
            deleteItemM(pos, list_mem);
        }
    }
    
}

void daux_mmap (char *trozos[], tListM *list_mem){ //Función auxiliar que elimina un mapeado de un fichero 
    int pos;
    tItemM item;
    for((pos = firstM(*list_mem)); (pos <= lastM(*list_mem)); (pos++)){
        item = getItemM(pos, *list_mem);
        if(strcmp(item.Union.fich.filename, trozos[2])==0){
            if(munmap(item.dir, item.size)==-1){
                perror("No se puede eliminar");
            } else {
                close(item.Union.fich.fd);
                printf("Desmapeado fichero %s\n", item.Union.fich.filename);
                deleteItemM(pos,list_mem);
                return;
            }
        } else {
            printf("El fichero %s no está mapeado\n", trozos[2]);
        }
    }
}

void comDeallocate(char *trozos[], tListM *list_mem){ //Comando deallocate
    if(trozos[1]==NULL){
        mostrarmemoryList(*list_mem);
    }     
    else if(strcmp(trozos[1],"-malloc")==0){
        if(trozos[2]==NULL){
            mostrar_malloc(*list_mem);
        }
        else{
            daux_ma(trozos, list_mem); //Chamada de función para facer malloc
        }
    } else if(strcmp(trozos[1],"-delkey")==0){
        if(trozos[2]==NULL){
            mostrar_shared(*list_mem);
        } else {
            daux_delkey(trozos,  list_mem); //chamada de función para facer delkey
        }
    } else if(strcmp(trozos[1],"-shared")==0){
        if(trozos[2]==NULL){
            mostrar_shared(*list_mem);
        } else {
            daux_shared(trozos, list_mem); //chamada de función para facer shared
        }
    } else if(strcmp(trozos[1],"-mmap")==0){
        if(trozos[2]==NULL){
            mostrar_mmap(*list_mem);
        } else {
            daux_mmap(trozos, list_mem); //chamada de función para facer mmap
        }
    } else {
        if(strtol(trozos[1], NULL,16)==0){
            mostrarmemoryList(*list_mem);
        } else {
            daux_hex(trozos, list_mem);
        }
    } 
}

//COMANDO I-O

ssize_t LeerFichero (char *f, void *p, size_t cont){ //Función dada por los profesores para leer un fichero
    struct stat s;
    ssize_t  n;  
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	    return -1;     
    if (cont==-1)   // si pasamos -1 como bytes a leer lo leemos entero
	    cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
	    aux=errno;
	    close(df);
	    errno=aux;
	    return -1;
    }
    close (df);
    return n;
}

void do_I_O_read (char *ar[]){ //Función dada por los profesores para leer un fichero
    void *p;
    size_t cont=-1;
    ssize_t n;
    if (ar[0]==NULL || ar[1]==NULL || ar[2] ==NULL || ar[3] == NULL || ar[4] == NULL){
	    printf ("faltan parametros\n");
	    return;
    }
    p=cadtop(ar[3]);  /*convertimos de cadena a puntero*/
    
    if (ar[4]!=NULL)
	    cont=(size_t) atoll(ar[4]);

    if ((n=LeerFichero(ar[2],p,cont))==-1)
	    perror ("Imposible leer fichero");
    else
	    printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[2],p);
} 

/*ssize_t EscribirFichero (char *f, void *p, size_t cont,int overwrite){
    ssize_t  n;
    int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
	    flags=O_CREAT | O_WRONLY | O_TRUNC;

    if ((df=open(f,flags,0777))==-1)
	    return -1;

    if ((n=write(df,p,cont))==-1){
	    aux=errno;
	    close(df);
	    errno=aux;
	    return -1;
    }
    close (df);
    return n;
}*/

void eswrite (char *trozos[]){ //Función auxiliar creada para escribir o sobreescribir un fichero
    int cont, flags = 0, df;
    char *dir, *nombre;
    
    if((strcmp(trozos[2],"-o")==0) && (trozos[3]!=NULL) && (trozos[4]!=NULL) && (trozos[5]!=NULL)){    
        nombre = trozos[3];
        cont = atoi(trozos[5]);
        dir = (char*)strtoul(trozos[4],NULL,16);
        flags = O_WRONLY | O_CREAT | O_TRUNC;

    } else if (trozos[2]==NULL || trozos[3]==NULL || trozos[4]==NULL){
        printf("Faltan parametros\n"); 
    
    } else { 
        nombre = trozos[2];
        cont = atoi(trozos[4]);
        dir = (char*)strtoul(trozos[3],NULL,16);
        flags = O_WRONLY | O_CREAT | O_EXCL;
    }
    
    if((df=open(nombre, flags, S_IRWXU | S_IRWXG | S_IRWXO))==-1){
        perror("Error al abrir el archivo");
        return;
    }
    
    if(write(df,dir,cont)!=-1)
        printf("Escritos %d bytes en %s desde %p\n", cont, nombre, dir);
    close(df);
}

void comIo(char *trozos[], char *comando){ //Comando i-o
    int tro = trocearCadena(comando, trozos);

    if(trozos[1]==NULL){
        printf("uso: i-o [read|write] ...\n");
    }
    
    else if(strcmp(trozos[1], "read")==0){
        if (tro == 5 ){
            do_I_O_read(trozos);
        } else {
            printf("Faltan parámetros\n");
        }
    }
    
    else if (strcmp(trozos[1], "write")==0){
        if (tro == 5 || tro == 6){
            eswrite(trozos);
        }
        else {
            printf("Faltan parámetros\n");
        }
    
    } else {
        printf("%s no es una opción\n", trozos[1]);
    }
}

//COMANDO MEMDUMP

void imprimir_char (char c){// Función auxiliar para mostrar caracteres
    bool imprimir = false;
    if (c > 32 && c < 126)
        imprimir = true;
    else 
        imprimir = false;
    
    if(imprimir)
        printf("%4c", c);
    else 
        printf("%4c", ' ');
    
    imprimir = false;
}

void imprimir_hex (char c){ //Función auxiliar para mostrar los caracteres en código hexadecimal
    bool imprimir = false;
    if (c >= 32 && c <= 126)
        imprimir = true;
    else 
        imprimir = false;

    if(imprimir)
        printf("%4x", c);
    else 
        printf("%3c%c", '0','0');
    imprimir = false;
}

void imprimir_mem (char *p, int nbytes){ //Función auxiliar para poder hacer el comando Mendump
    int i, j;
    int pos_char;
    int veces = 0, lineas = 0;

    for (i = 0; i < nbytes; i++){
        imprimir_char(p[i]);
        veces++;

        if((veces == nbytes - 25 * lineas) || (veces == 25)){
            printf("\n");
            pos_char = i + 1 - veces;

            for (j = pos_char; j < pos_char + veces; ++j){
                imprimir_hex(p[j]);
            }
            printf("\n");
            lineas++;
            veces = 0;
        }
    }
}

void comMemdump(char *trozos[], char *comando){ //Comando memdump
    char *p;
    int nbytes = 25;

    int num = trocearCadena(comando, trozos);

    if(num == 1 || num > 3){
        perror("Número de argumentos inválido");    
    }

    p = (char *) strtol(trozos[1], NULL, 16);

    if (num == 3){
        nbytes = (int) strtol(trozos[2], NULL, 10);
    }
    
    imprimir_mem(p, nbytes);
}

//COMANDO MEMFILL

void LlenarMemoria (void *p, size_t cont, unsigned char byte){ //Función creada por los profesores para el comando Memfill
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    printf("Llenando %ld bytes de memoria con el byte %c(%x) a partir de la direccion %p\n",cont, byte, byte, p);

    for (i=0; i<cont ;i++){
		arr[i]=byte;
    }
}

void comMemfill(char *trozos[], char *comando){ //Comando Memfill
    void *p;
    int caracter;
    unsigned char defecto = 'A';
    size_t nbytes = 128;
    int ntrozos = trocearCadena(comando, trozos);

    if (ntrozos == 1 || ntrozos > 4){
        printf("Numero de argumentos inválido\n");
    }

    p = (void *) strtol(trozos[1], NULL, 16);

    if(ntrozos==2){
        LlenarMemoria(p, nbytes, defecto);
        return;
    }
    
    if(ntrozos == 3){
        if((nbytes = atoi(trozos[2]))!=0){
            LlenarMemoria(p, nbytes, defecto);
            return;
        }
        caracter = 2;
        nbytes = 128;
    }

    if (ntrozos == 4){
        nbytes = atoi(trozos[2]);
        caracter = 3;
    }

    if(trozos[caracter][1]=='x'){
        defecto = strtol(trozos[caracter], NULL, 16);
    } else {
        defecto = trozos[caracter][0];
    }

    LlenarMemoria(p, nbytes, defecto);
}
//COMANDO MEMORY

void Do_pmap (void){ //sin argumentos
    pid_t pid;       //hace el pmap (o equivalente) del proceso actual
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};
   
    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");
         
        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;   
        if (execvp(argv[0],argv)==-1)//No hay pmap, probamos procstat FreeBSD 
            perror("cannot execute procstat (FreeBSD)");
         
        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;    
        if (execvp(argv[0],argv)==-1)  //probamos procmap OpenBSD
            perror("cannot execute procmap (OpenBSD)");
         
        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) //probamos vmmap Mac-OS
            perror("cannot execute vmmap (Mac-OS)");      
        exit(1);
    }
    waitpid (pid,NULL,0);
} 

void comMemory(char *trozos[], tListM list_mem){ //Comando Memory
    bool blocks = false, funcs = false, vars = false, pmap = false;
    
    //Variables locales creadas
    int int_local = 0;
    char char_local = 'r';
    double float_local = 1.2;

    //Variables estáticas creadas
    static int int_slocal = 0;
    static char char_slocal = 'r';
    static double float_slocal = 1.2;

    if (trozos[1]==NULL || (strcmp(trozos[1], "-all")==0)){
        blocks = true;
        funcs = true;
        vars = true; 
    }
    else if (strcmp(trozos[1], "-blocks")==0){
        blocks = true;
    }
    else if (strcmp(trozos[1], "-funcs")==0){
        funcs = true;
    }
    else if (strcmp(trozos[1], "-vars")==0){
        vars = true;
    }
    else if (strcmp(trozos[1], "-pmap")==0){
        pmap = true;
    }
    else {
        printf("%s no es una opción\n", trozos[1]);
    }

    if(vars){
        printf("Variables locales\t%p,\t%p,\t%p\n", &int_local, &char_local, &float_local);
        printf("Variables globales\t%p,\t%p,\t%p\n", &int_glocal, &char_glocal, &float_glocal);
        printf("Variables estaticas\t%p,\t%p,\t%p\n", &int_slocal, &char_slocal, &float_slocal);
    } 
    if(funcs){
        printf("Funciones programa\t%p,\t%p,\t%p\n", &comCarpeta, &comFecha, &comAutores);
        printf("Funciones libreria\t%p,\t%p,\t%p\n", &printf, &malloc, &perror);
    }
    if(blocks){
        mostrarmemoryList(list_mem);
    }
    if (pmap){
        Do_pmap();
    }
    vars = false;
    funcs = false;
    blocks = false;
    pmap = false;
}

//COMANDO RECURSE
void Recursiva (int n){ //Función creada por los profesores para hacer el comando recurse
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

void comRecurse(char *trozos[]){ //Comando recurse
    if (trozos[1]==NULL){
        printf("recurse [n]\t Invoca a la funcion recursiva n veces\n"); 
    }
    else {
        Recursiva(atoi(trozos[1]));
    } 
}