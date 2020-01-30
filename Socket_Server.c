#include <stdio.h>
#include <stdlib.h>
#include "Socket_Server.h"
#define PORT 6595

/*
* If program run on Windows
*/
#if defined _WIN64 || defined (WIN32) || defined _WIN32
  #include <winsock2.h>
  /*
  * Needed  non-existent type with winsock2
  */
  typedef int socklen_t;
/*
* Else if program run on Linux
*/
#elif __unix__ || defined __APPLE__
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  /*
  * Avoid differences to close a socket
  * between socket.h and winsock2.h
  * Adding missing definitions with socket.h
  */
  #define closesocket(param) close(param)
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR -1
  /*
  * Adding missing types with socket.h
  */
  typedef int SOCKET;
  typedef struct sockaddr_in SOCKADDR_IN;
  typedef struct sockaddr SOCKADDR;

#endif


int startTCPSocketServ(){

  #if defined _WIN64 || defined (WIN32) || defined _WIN32
    /*
    * Change the cmd codepage
    */
    system("chcp 65001");
    system("cls");
    WSADATA WSAData;
    /*
    * Creating var to manage errors
    */
    int windWSAError;

    /*
    * WSAStratup Initialising winsock2 library
    * return 0 if there is no problems
    */
    windWSAError = WSAStartup(MAKEWORD(2,2), &WSAData);
  #else
    int windWSAError= 0;
  #endif

  printf("\nLancement de la créatoin du serveur...\n");

  /*
  * Setting up the socket for all systems
  */

  SOCKADDR_IN sockServIn = { 0 };
  int sock;
  int socketConnected;
  int sockError;
  SOCKADDR_IN sockConnectedAddr;
  socklen_t sizeofSocketConnected;
  int userChoice = 0;

  if(!windWSAError){


    /*
    * Check if the socket is correct
    */

    printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", (int)&sock);
    /*
    * Initialising struct
    * Can change s_addr with given ip inet_addr("192.168.0.0") or INADDR_ANY
    */
    sockServIn.sin_addr.s_addr=htonl(INADDR_ANY) ;
    sockServIn.sin_family = AF_INET;
    sockServIn.sin_port = htons (PORT);
    /*
    * Creating socket :
    * param 1 : Use TCP/IP
    * param 2 : Use with TCP
    * param 3 : Protocole parameter (useless) -> 0
    */
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET){
      /*
      *bind info to the socket
      */
      sockError = bind(sock, (SOCKADDR*)&sockServIn, sizeof(sockServIn));

      if(sockError != SOCKET_ERROR){
        printf("\nDémarrage du serveur... \n");
        /*
        * Starting to connect
        * (max number of connection 5)
        */
        sockError = listen(sock,5);

        if(sockError != SOCKET_ERROR){

          printf("\nEn attente de la connexion d'un client...\n");

          sizeofSocketConnected = sizeof(sockConnectedAddr);

          socketConnected = accept(sock, (struct  sockaddr  *)&sockConnectedAddr, &sizeofSocketConnected);

          printf("\nConnexion établie avec le client !\n");
          printf("\nChargement de la partie... \n");


          char buffer[80] = "Bonjour ! Bienvenue sur mon serveur :) \n";

            printf("mesasge à envoyer : %s \n", buffer);
            sockError = send(sock, buffer, 80, 0);

              if(sockError != SOCKET_ERROR){
                printf("Chaine envoyée : %s\n", buffer);
              }
              else{
                printf("Erreur de transmission\n");
              }


              /* Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens) */
              shutdown(sock, 2);

            sendMsgSrv(sock);

          closesocket(sock);
          }
      }
        else{
          printf("\nUn problème est survenu lors de la connexion du client :( \n");
        }
      }
      else{
        printf("\nUne érreure est survenu lors de la création du serveur :( \n");
      }
  }
  else{
    printf("\nImpossible de créer une socket :( \n");
  }
  return 0;
}
