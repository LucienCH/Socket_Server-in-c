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







int startTCPSocketCli(){

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

  printf("\nLancement de la créatoin du client...\n");

  /*
  * Setting up the socket for all systems
  */

  SOCKADDR_IN sockIn;
  SOCKET sock;
  long hostAddr;
  struct hostent *serveur_info;

  if(!windWSAError){
    /*
    * Creating socket :
    * param 1 : Use TCP/IP
    * param 2 : Use with TCP/IP
    * param 3 : Protocole parameter (useless) -> 0
    */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    /*
    * Check if the socket is correct
    */
    if(sock != INVALID_SOCKET){

      printf("\nLa socket numéro %d en mode TCP/IP est valide  !\n", (int)&sock);
      /*
      * Initialising struct
      * Can change s_addr with given ip inet_addr("192.168.0.0")
      */
      sockIn.sin_addr.s_addr= inet_addr("127.0.0.1");
      sockIn.sin_family = AF_INET;
      sockIn.sin_port = htons(PORT);

      /*
      *
      *If client achieve connection
      */
      if(connect(sock, (SOCKADDR*)&sockIn, sizeof(sockIn)) != SOCKET_ERROR){
        printf("Connexion reussi à : %s sur le port : %d \n", inet_ntoa(sockIn.sin_addr), htons(sockIn.sin_port));
      }
      else{
        printf("Impossble de se connecter au serveur... :( \n");
      }
      getchar();
      closesocket(sock);
    }
    else{
      printf("\nLa socket est invalide :( \n");
    }
  }
  else{
    printf("\nImpossible de créer une socket :( \n");
  }
  return 0;
}
