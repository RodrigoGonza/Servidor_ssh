#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

// the port client will be connecting to
//#define PORT 3490
// max number of bytes we can get at once
#define MAXDATASIZE 500

int main(int argc, char *argv[]){
  int sockfd, numbytes;
  char buf[MAXDATASIZE];
  struct hostent *he;
  unsigned int PORT= atoi(argv[2]);
  printf("\n %i  \n",&PORT);

  // connectors address information
  struct sockaddr_in their_addr;
  // if no command line argument supplied
  if(argc != 3){
    fprintf(stderr, "Client-Usage: %s host_servidor\n", argv[0]);

    // just exit
    exit(1);
  }

  // get the host info
  if((he=gethostbyname(argv[1])) == NULL){
    perror("gethostbyname()");
    exit(1);
  }
  else
    printf("Client-The remote host is: %s\n", argv[1]);
  while(1){

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    perror("socket()");
    exit(1);
  }
  else 
    printf("Client-The socket() sockfd is OK...\n");

  // host byte order
  their_addr.sin_family = AF_INET;
  // short, network byte order
  printf("Server-Using %s and port %d...\n", argv[1], PORT);
  their_addr.sin_port = htons(PORT);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);

  // zero the rest of the struct
  memset(&(their_addr.sin_zero), '\0', 8);
  if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1){
    perror("connect()");
    exit(1);
  }
  else
    printf("Client-The connect() is OK...\n");

  /*if((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1){
    perror("recv()");
    exit(1);
  }
  else
    printf("Client-The recv() is OK...\n");*/









 //((((((((((  A partir de aqui     ))))))))))))))))))))))))
printf("Comando:");
char cadena_de_caracteres[MAXDATASIZE];
fgets(cadena_de_caracteres, MAXDATASIZE, stdin);  //Leemos una cadena de caracteres
//if ((strlen(cadena_de_caracteres) > 0) && (cadena_de_caracteres[strlen(cadena_de_caracteres) - 1] == '\n'))
//        cadena_de_caracteres[strlen(cadena_de_caracteres) - 1] = '\0';
int i=0;

while(cadena_de_caracteres[i]!=NULL)
i++;
cadena_de_caracteres[i-1]='\0';


//Se envia la cadena leida antes
 int res_send = send( sockfd, cadena_de_caracteres, i, 0 );
 printf("res_send=%d\n",res_send); 
 
//Guardamos la cadena enviada por el servidor
 numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0);





/*
int j;
long falta;
unsigned char bufferAux[513];
int escritos;
FILE *flista;
long tam_buffer= 512;
long longlista;
longlista= ntohl(cadena_de_caracteres);
if (longlista >=0){
flista= fopen ("listaarchs.txt", "wb");
falta = longlista;
//en este ciclo se recibe el archivo en partes
i = 0;
while(i < longlista){
  if (falta<tam_buffer){
  tam_buffer = falta;
  char buffer[falta];
  char bufferAux[falta+1];
  }

numbytes = recv(sockfd, buf, sizeof(buf), 0 );
printf("CLIENTE ---- Recibi %d bytes \n", numbytes);
//printf("CLIENTE ---- Recibi %s \n", buffer);
for (j=0; j< numbytes; j++){
bufferAux[j]= buffer[j];
}
bufferAux[numbytes+1]='\0';
escritos= fwrite(bufferAux, sizeof(char), numbytes, flista);
printf("CLIENTE ---- Recibi %s \n", bufferAux);

printf("CLIENTE ---- escritos %d\n", escritos);

falta = falta-numbytes;
i = i+numbytes;
printf("CLIENTE ---- Hasta ahora recibi %d bytes \n", i);
if (numbytes==0) i= longlista;
}
}
*/







//((((((((((  Hasta aqui     ))))))))))))))))))))))))
i=0;
char comando[7];  //en esta variable se guardan los primeros 6 caracteres
for(i=0; i<6; i++)                     //Con este for guardamos los caracteres de
    comando[i]=buf[i];              // la cadena en la variable comando
  comando[6]='\0';                       //Agregamos el caracter de fin de cadena


  numbytes--;
  buf[numbytes-1] = '\0';
  if(strcmp(comando,"salida")==0){           //Comparamos las cadenas para saber que son iguales
    printf("Cerrando conexión\n\n");         //Imprimimos mensaje de salida
    close(sockfd);                           //Cerramos el socket
    return 0;                                //Finalizamos el programa

  }
  printf("Servidor:\n");
  printf("%s\n\n",buf);                       //Se imprime el mensaje del servidor
  /*if(numbytes==-1)
      printf("No hay registros con este numero de cuenta\n");
  printf("bytes: %d\n", numbytes);
  printf("Client-Closing sockfd\n");*/
  close(sockfd);
  //close(flista);
}
  return 0;
}
