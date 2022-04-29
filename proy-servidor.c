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







int funcion_ssh(char cadena[MAXDATASIZE]){ //Cadena recibida del cliente

  char comando[7];  //en esta variable se guardan los primeros 6 caracteres
  char salir[7]="salida";//variable para comparar
  FILE *archivo;          //variable que apuntará al archivo que se creará
  char car;               //variable para extraer informacion del documento
  int i;                //enteros usados para contadores
  for(i=0; i<6; i++)                     //Con este for guardamos los caracteres de
    comando[i]=cadena[i];              // la cadena en la variable comando
  comando[6]='\0';                       //Agregamos el caracter de fin de cadena
  if(strcmp(comando,salir)==0){     //Comparamos para salir
    printf("Saliendo\n");            //Imprimimos la indicación del lado del servidor
    return 1;                         //Regresamos uno para saber que debemos salir de la aplicación
  }
  else{
    char aux_cadena[] = " > comando.txt";
    strcat(cadena,aux_cadena);
    system(cadena);
    archivo=fopen("comando.txt","r");  //Abrimos el archivo en modo lectura
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
    return 0;
  }

  


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
      int salida=funcion_ssh(buffer);

      if(salida==1)                    //Al ser 1 en la salida significa que el cliente se desconectará
          send(new_fd, "salida", 8, 0);  //El servidor envia la cadena "salida"
      else if(salida==0){              //Al ser cero el servidor envia el resultado del comando enviado
          send(new_fd, cadena_aux, cont_aux, 0);    //Enviamos el resultado del comando
          memset(cadena_aux, 0,cont_aux);
          }



/*
      FILE * fd = fopen(comando , "r" );
fseek( fd , 0 , SEEK_END ); // Se posiciona al final del archivo
int tamanio = ftell(fd); // Devuelve el tamaño del archivo (en bytes)
close(fd);
printf ("tamaño %i\n", tamanio);
longList = htonl(tamanio); //asi envia la longitud del archivo
if((send(s1, (char*)&longList, sizeof(longList), 0)) < 0){
printf("Error en envio\n");
exit(1);
}
long tam_buffer=512;
longList= tamanio; //tamaño del archivo

flista= fopen ("comando.txt", "rb");
if (flista!=NULL){
falta = longList;
// Se envia el archivo en partes
i = 0;
while(i < longList){
if (falta<tam_buffer) {
tam_buffer=falta;
char buffer[falta];
char bufferAux[falta+1];
}
printf("SERVIDOR ---- tam_buffer: %d - falta: %d\n", tam_buffer, falta);

leidos = fread(buffer, sizeof(char),tam_buffer,flista);

enviados= send(s1, bufferAux, leidos, 0);
printf("SERVIDOR ---- Envie %d bytes \n", enviados);

falta = falta-leidos;
i = i + leidos;
}
}
else printf("SERVIDOR ---- No abrio el archivo \n");
close(flista);

*/




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



