//**********Ivan Alvarez Fernandez**********
//**********ivan.alvarez.fernandez**********
//************Marcos Gomez Tejon************
//*****************m.tejon******************
//*************Rita Pardo Lopez*************
//****************rita.pardo****************

#include "p1.h"
#include "p0.h"

char LetraTF (mode_t m) {
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */ 
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}
/*las siguientes funciones devuelven los permisos de un fichero en formato rwx----*/
/*a partir del campo st_mode de la estructura stat */
/*las tres son correctas pero usan distintas estrategias de asignación de memoria*/


char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}

void comCreate(char *trozos[], char *comando){ 
    if(trozos[1]==NULL){ 
        rutaActual(); //Ruta actual
    } else{
        if(strcmp(trozos[1],"-f")==0){ //Crear un fichero
            if(trozos[2]==NULL){
                rutaActual(); //Ruta actual
            } else{
                if(open(trozos[2],O_CREAT | O_EXCL , S_IRWXU | S_IRWXG | S_IROTH |  S_IXOTH )==-1){ //Crear fichero con sus permisos correspondientes
                    fprintf(stderr, "%s\n", strerror(errno));
                }
            }
        } else{ //Crear un directorio
            if(mkdir(trozos[1],S_IRWXU | S_IRWXG | S_IROTH |  S_IXOTH)==-1){ //Crear directorio con sus permisos correspondientes
                fprintf(stderr, "%s\n", strerror(errno));   
            }
        }
    }
}

void comStat(char *trozos[], char *comando){
    bool listal=false, link=false, acc=false;
    int i, nombres = 0;
    char variable[PATH_MAX];
    int b = trocearCadena(comando, trozos);

    for(i=1; i<b; ++i){ //Comprobar parámetros del comando
        if(trozos[1]==NULL){
            rutaActual(); //Ruta actual
        } else if(strcmp(trozos[i], "-long")==0){ 
            listal=true;
        } else if(strcmp(trozos[i], "-link")==0){
            link=true;
        } else if(strcmp(trozos[i], "-acc")==0){
            acc=true;
        } else {
            break;
        }
    }
    while (trozos[i]!=NULL){ 
        if(listal){
            getLongStats(trozos[i], acc, link); //Mostar estadísticas en formato largo
        }
        else{
            getStats(trozos[i]); //Mostrar estadísticas en formato corto
        }
        i++;
        nombres++;
    }
    if (nombres==0){ //Si no se le pasan directorios o ficheros
        rutaActual(); //Ruta actual
    }   
    return;  
} 

void comList(char *trozos[], char *comando){
    bool listal=false, link=false, acc=false, hid=false, reca=false,recb=false;
    int i, nombres = 0;
    char variable[PATH_MAX];

    int b = trocearCadena(comando, trozos); 
    for(i=1; i<b; ++i){ //Comprobar parámetros del comando
        if(trozos[1]==NULL){
            rutaActual(); //Ruta actual
        } else if(strcmp(trozos[i], "-long")==0){ 
            listal=true;
        } else if(strcmp(trozos[i], "-link")==0){
            link=true;
        } else if(strcmp(trozos[i], "-acc")==0){
            acc=true;
        } else if(strcmp(trozos[i], "-hid")==0){
            hid=true;
        } else if(strcmp(trozos[i], "-reca")==0){
            reca=true;
        } else if(strcmp(trozos[i], "-recb")==0){
            recb=true;
        } else{
            break;
        }
    }
    while (trozos[i]!=NULL){ //recorremos o resto de trozos que hai no comando que non sexan -long, -link ou -acc
        if(esDirectorio(trozos[i])){
            getDirLongStats(trozos[i], listal, acc, link, hid, reca, recb);
        } 
        else if (listal){
            getLongStats(trozos[i], acc, link); //Mostar estadísticas en formato largo
        }
        else {
            getStats(trozos[i]); //Mostrar estadísticas en formato corto
        }
        i++;
        nombres++;
    }
    if(nombres==0){ //Si no se le pasan directorios o ficheros
        rutaActual(); //Mostrar ruta
    }
    return;
}
 
void comDelete(char *trozos[], char *comando){
    int i;
    int troz = trocearCadena(comando, trozos);
    
    if(trozos[1]==NULL){
        rutaActual(); //Mostar ruta
    } 
    else {
        for (i=1 ; i < troz ; i++){ 
            if(esDirectorio(trozos[i])){ //Si es un directorio
                if(rmdir(trozos[i])!=0){ //Borrar un directorio si está vacío
                   fprintf(stderr, "No se puede eliminar %s: %s\n", trozos[i], strerror(errno));
                }
            } else { //Si es un fichero
                if(unlink(trozos[i])!=0){ //Borrar un fichero
                    fprintf(stderr, "No se puede eliminar %s: %s\n", trozos[i], strerror(errno));
                }
            } 
        }          
    }  
}

void comDeltree(char *trozos[], char *comando){
    int troz = trocearCadena(comando, trozos);

    if(trozos[1]==NULL){
        rutaActual(); //Ruta actual
    }
    for (int i=1 ; i < troz ; i++){ 
        if(esDirectorio(trozos[i])){ //Si es un directorio, se llama a la función de borrado recursivo
            borrarDirectorios(trozos[i]);            
        } 
        else if(unlink(trozos[i])==0){ //Borrar un fichero
        //Dejamos este espacio porque como no queremos mostrar nada de información, se borra y listo ya que en caso de que no sea posible, entra en la siguiente condición.
        } else {
            fprintf(stderr, "No se puede eliminar %s: %s\n", trozos[i], strerror(errno));
        }
    } 
}

void borrarDirectorios(const char *path){ 
    DIR *dir;
    struct dirent *directorio;
    char subdirectorio[MAX];

    dir = opendir(path); //Abrir el directorio que se le pasa

    if(dir){ 
        while ((directorio = readdir(dir))!=NULL){ //Leer el contenido de un directorio, excepto el actual y el padre
            if(strcmp(directorio->d_name, ".")!=0 && strcmp(directorio->d_name, "..")!=0){
                sprintf(subdirectorio, "%s/%s", path, directorio->d_name); //Guardar el valor de los archivos del directorio
                if(esDirectorio(subdirectorio)){ //Si es un directorio, se llama de forma recursiva a la funcion
                    borrarDirectorios(subdirectorio);
                } else {
                    if(unlink(subdirectorio)!=0){ //Si es un fichero, se borra
                        printf("No se puede eliminar fichero %s: %s\n", directorio->d_name, strerror(errno)); 
                    }
                }
            }
        }
        closedir(dir); //Cerrar el directorio
    }
    if(rmdir(path)!=0){
        printf("No se puede eliminar directorio %s: %s\n", path, strerror(errno));
    } 
}

void mostrarsubs(char* trozos, bool listal, bool acc, bool link, bool hid, bool reca, bool recb){
    DIR *subdirectorio;
    struct dirent *estructurasub;
    char ruta[MAX];

    subdirectorio = opendir(trozos); //Abrir el directorio que se le pasa
    if(subdirectorio){ //Si se puede abrir
        while((estructurasub = readdir(subdirectorio))!=NULL){ //Leer el contenido de un directorio, excepto el actual y el padre
            sprintf(ruta, "%s/%s", trozos, estructurasub->d_name); //Guardar el valor de los archivos del directorio
            if((strcmp(estructurasub->d_name, ".")!=0) && (strcmp(estructurasub->d_name, "..")!=0) && (esDirectorio(ruta)) && (hid || estructurasub->d_name[0]!='.')){ //Si no es un archivo oculto
                getDirLongStats(ruta, listal, link, acc, hid, reca, recb); 
            } 
        }
    closedir(subdirectorio); //Cerrar el directorio
    } 
}

void getDirLongStats(char* trozos, bool listal, bool acc, bool link, bool hid, bool reca, bool recb){
    DIR *dir;
    struct dirent *dir_s;
    char ruta[MAX];

    dir=opendir(trozos); //Abrir el directorio que se le pasa
    if(dir){
        if(reca && recb){
            mostrarsubs(trozos, listal, acc, link, hid, reca, false); //Si se le pasan reca y recb, hace reca
        }
        if(!reca && recb){
            mostrarsubs(trozos, listal, acc, link, hid, false, recb); //recb
        }
        printf("**********%s\n", trozos);
        while((dir_s=readdir(dir))!=NULL){ //Leer el contenido de un directorio
            if(hid || dir_s->d_name[0]!='.'){
                sprintf(ruta, "%s/%s", trozos, dir_s->d_name);
                if(listal){ //Si está en formato largo
                    getLongStats(ruta, acc, link);
                } else {
                    getStats(ruta);
                }
            }
        }
        if(reca && !recb){
            mostrarsubs(trozos, listal, acc, link, hid, reca, false); //reca
        }
        closedir(dir); //Cerrar el directorio
    } else{
        fprintf(stderr, "%s\n", strerror(errno));
    }
} 

void getStats(char* filename){
    struct stat estadisticas;
    if(lstat(filename,&estadisticas)==0){
        printf("%9ld %s\n", estadisticas.st_size, filename); //Muestra el archivo y el tamaño
    } else{
        perror("stat");
    }
}

bool esDirectorio(const char *path){ //función auxiliar para saber se é un directorio
    struct stat str;
    if(lstat(path, &str)!=-1) { //comprobamos se existe
        if(S_ISDIR(str.st_mode)){
            return true;
        } else { //en caso do resto
            return false;
        }         
    } 
    return false;
}

bool esLink(mode_t str){
    if(S_ISLNK(str)){ //Si es un acceso directo devuelve true
        return true;
    } else { 
        return false;
    }         
} 

void rutaActual(){
    char dir[MAX];
        if(getcwd(dir, MAX)==NULL){ //Imprime la ruta actual
           perror("getcwd");
        }  
        else{
           printf("%s\n", dir);
        }
}

void fecha(time_t t){
    struct tm tm;
    tm = *localtime(&t);
    printf("%04d/%02d/%02d-%02d:%02d ", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,  tm.tm_hour, tm.tm_min); //Cambia el formato de la fecha de stats 
} 

void getLongStats(char *filename, bool acc, bool link){
    struct stat estadisticas;
    struct passwd *usuario;
    struct group *grupo;
    ssize_t bufsiz, bytes; 
    char symlink[MAX] = "";
    char linke[MAX], usuarioe[MAX], grupoe[MAX];

    if(lstat(filename,&estadisticas)==0){ //Si el formato es largo
        if(acc==true){
            fecha(estadisticas.st_atime); //Fecha de acceso
        } else {
            fecha(estadisticas.st_mtime); //Fecha de modificación
        }
        if((usuario= getpwuid(estadisticas.st_uid))!=NULL){ 
            sprintf(usuarioe, "%s", usuario->pw_name); //Nombre de usuario
        }else{  
            sprintf(usuarioe, "%d", estadisticas.st_uid); //Id de usuario
        }
        if((grupo= getgrgid(estadisticas.st_gid))!=NULL){ 
            sprintf(grupoe, "%s", grupo->gr_name); //Nombre de grupo
        }else{  
            sprintf(grupoe, "%d", estadisticas.st_gid); //Nombre de grupo
        }
        printf("\t%2lu (%8lu) %8s %8s %s\t%ld\t%s", estadisticas.st_nlink, estadisticas.st_ino, usuarioe, grupoe, ConvierteModo2(estadisticas.st_mode), estadisticas.st_size, filename); //Mostrar estadísticas
        
        if(link==true && esLink(estadisticas.st_mode)==true){ //Archivo con acceso directo
            bufsiz = estadisticas.st_size+1;
            bytes = readlink(filename, symlink, bufsiz);
            if(bytes < 0){ 
                perror("No se puede acceder al link");
            }else {
                printf(" -> %s", symlink);
            } 
        } 
        printf("\n");
    } else{
        perror("stat");
    }
}

   

   