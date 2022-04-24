#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

/* the port users will be connecting to */
#define MYPORT 3490

/* how many pending connections queue will hold */
#define BACKLOG 10
#define MAXDATASIZE 300

char cadena_aux[MAXDATASIZE]; //variable global para guardar cadenas que salen de la función base_datos
int cont_aux=0;               //variable global para guardar el numero de bytes de la cadena global

void sigchld_handler(int s){
  while(wait(NULL) > 0);
}







int base_datos(char cadena[MAXDATASIZE]){ //Cadena recibida del cliente
char comando[7];  //en esta variable se guardan los primeros 6 caracteres
char numero_cuenta[15]; //en esta variable se guardan los siguientes 9 caracteres
char nombre[282];       //en esta variable se guardan el resto de caracteres
char insert[7]="insert";//variable para comparar
char select[7]="select";//variable para comparar
FILE *archivo;          //variable que apuntará al archivo que se creará
char car;               //variable para extraer informacion del documento
int i,j;                //enteros usados para contadores
if (cadena[6]!=' ')                    //condicional para verificar el espacio en la cadena
    return 2;                          //Regresa un 2 si no existe el espacio
for(i=0; i<6; i++)                     //Con este for guardamos los caracteres de
    comando[i]=cadena[i];              // la cadena en la variable comando
comando[6]='\0';                       //Agregamos el caracter de fin de cadena
for(i=7; i<16; i++){                   //Con este for guardamos los caracteres de
    numero_cuenta[i-7]=cadena[i];      // la cadena en la variable comando
    if(!isdigit(numero_cuenta[i-7]))   //verificamos que todos los caracteres del numero de cuenta sean digitos
        return 3;                      //sale y regresa un 3 si algún caracter no es un digito
                   }
numero_cuenta[9]='\0';                 //Agregamos el caracter de fin de cadena
i=17;              
j=0;
while(cadena[i]!='\0'){                //Utilizamos este while para guardar el nombre 
     nombre[j]=cadena[i];              
     i++;
     j++;
                      }
nombre[j]='\0'; 
if(strcmp(comando,insert)==0){         //comparamos el comando para saber si es un insert
    if (cadena[16]!=' ')               //Verificamos si existe el espacio entre el numero de cuenta y el nombre
        return 2;
    strcat(numero_cuenta,".txt");      //A la variable del numero de cuenta le concatenamos ".txt"
    archivo=fopen(numero_cuenta,"w");       //Abrimos o creamos un archivo de texto que se llame como el numero de cuenta 
    fprintf(archivo,nombre);           //Agregamos el nombre al archivo de texto
    fclose(archivo);                   //Cerramos el archivo
    return 0;        
}
else if(strcmp(comando,select)==0){    //comparamos el comando para saber si es un select
    strcat(numero_cuenta,".txt");      //A la variable del numero de cuenta le concatenamos ".txt"
    archivo=fopen(numero_cuenta,"r");  //Abrimos el archivo en modo lectura
    if(archivo!=NULL){                 //Verificamos que no este vacio el archivo
        i=0;                           
        while(1){
            car=fgetc(archivo);        //leemos un caracter del archivo de texto y lo guardamos en car
            cadena_aux[i]=car;         //guardamos ese caracter en nuestra variable global
            i++;                       //aumentamos el contador
            if(feof(archivo))          //verificamos si llegamos al final del archivo
                break;                 //salimos si se cumple la condición
                }
        cadena_aux[i]='\0';
        cont_aux=i+1;
                     }
    fclose(archivo);                   //Cerramos el archivo
    return 1;
                                    }
else{
return 2;}

}










int main(int argc, char *argv[ ]){
  /* listen on sock_fd, new connection on new_fd */
  int sockfd, new_fd;
  char buffer[MAXDATASIZE];

  /* my address information */
  struct sockaddr_in my_addr;

  /* connectors address information */
  struct sockaddr_in their_addr;
  int sin_size;
  struct sigaction sa;
  int yes = 1;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("Server-socket() error lol!");
    exit(1);
  }
  else
    printf("Server-socket() sockfd is OK...\n");

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
    perror("Server-setsockopt() error lol!");
    exit(1);
  }
  else
    printf("Server-setsockopt is OK...\n");

  /* host byte order */
  my_addr.sin_family = AF_INET;

  /* short, network byte order */
  my_addr.sin_port = htons(MYPORT);

  /* automatically fill with my IP */
  my_addr.sin_addr.s_addr = INADDR_ANY;
  printf("Server-Using %s and port %d...\n", inet_ntoa(my_addr.sin_addr), MYPORT);

  /* zero the rest of the struct */memset(&(my_addr.sin_zero), '\0', 8);
  if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
    perror("Server-bind() error");
    exit(1);
  }
  else
    printf("Server-bind() is OK...\n");

  if(listen(sockfd, BACKLOG) == -1){
    perror("Server-listen() error");
    exit(1);
  }

  printf("Server-listen() is OK...Listening...\n");
  /* clean all the dead processes */

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if(sigaction(SIGCHLD, &sa, NULL) == -1){
    perror("Server-sigaction() error");
    exit(1);
  }
  else
    printf("Server-sigaction() is OK...\n");

  /* accept() loop */
  while(1){
    sin_size = sizeof(struct sockaddr_in);
    if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1){
      perror("Server-accept() error");
      continue;
    }
    else
      printf("Server-accept() is OK...\n");
    printf("Server-new socket, new_fd is OK...\n");
    printf("Server: Got connection from %s\n", inet_ntoa(their_addr.sin_addr));

    /* this is the child process */
    if(!fork()){
      /* child doesnt need the listener */
      close(sockfd);
      /*if(send(new_fd, "This is a test string from server!\n", 37, 0) == -1)
        perror("Server-send() error lol!");*/
      





      //((((((((((  A partir de aqui     ))))))))))))))))))))))))
      //Función para recibir el comando del cliente del cliente y se almacena en buffer
      int num_bytes_leidos = recv( new_fd, buffer, MAXDATASIZE - 1, 0 );
      //imprimimos el comando
      printf("Mensaje: %s\n",buffer);
      //Llamamos a la funcion base_datos y la función regresará un valor que determinará
      // si el comando fue exitoso o no y el mensaje que se le enviará al cliente
      int salida=base_datos(buffer);
      // Condicionales anidados para enviar un mensaje al cliente
      if(salida==0)
          send(new_fd, "Insert exitoso!\n", 16, 0);
      else if(salida==1){
          send(new_fd, cadena_aux, cont_aux, 0);
          memset(cadena_aux, 0,cont_aux);
          }
      else if(salida==2)
          send(new_fd, "Comando erroneo!\n", 17, 0);
      else if(salida==3)
          send(new_fd, "El campo de numero de cuenta debe ser un entero de 9 digitos", 62, 0);
      //((((((((((  Hasta aqui     ))))))))))))))))))))))))

      memset(buffer, 0,num_bytes_leidos);



      close(new_fd);
      exit(0);
    }
    else
      printf("Server-send is OK...!\n");

    /* parent doesnt need this */
    close(new_fd);
    printf("Server-new socket, new_fd closed successfully...\n");
  }
  return 0;
}



