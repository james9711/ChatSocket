/***************************************/
/*             TCP client              */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024                      // buffer size

int main(int argc, char *argv[] ) {// arg count, arg vector   

   /* Declarations */
   int fd;                         // socket endpt 
   int flags;                      // rcv flags
   struct sockaddr_in server;      // socket name (addr) of server 
   struct sockaddr_in client;      // socket name of client 
   int server_size;                // length of the socket addr. server 
   int client_size;                // length of the socket addr. client 
   int bytes;                      // length of buffer 
   int rcvsize;                    // received bytes
   int trnmsize;                   // transmitted bytes
   int err;                        // error code
   int ip;                         // ip address
   char on;                        // 
   char buffer[1025];              // datagram dat buffer area
   char server_addr[16];           // server address  

   /* Initialization */
   on    = 1;
   flags = 0;
   server_size = sizeof server;
   client_size = sizeof client;
   sprintf(server_addr, "%s", "127.0.0.1");
   ip = inet_addr(server_addr);
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = ip;
   int PORT_NO = atoi(argv[1]);
   server.sin_port        = htons(PORT_NO);
   bytes=BUFSIZE;
   

   /* Creating TCP socket */
   fd = socket(AF_INET, SOCK_STREAM, 0);
   if (fd < 0) {
      printf("%s: Socket creation error.\n",argv[0]);
      exit(1);
   }

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Connecting to the server */
   err = connect(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      printf("%s: Cannot connect to the server.\n", argv[0]);
      exit(2);
   }

    char P1szoba[bytes];
    char P2szoba[bytes];

	   //1:kliensek sikeresen csatlakoztak 
	   char csatlakozas[bytes];
	   recv( fd, csatlakozas, bytes, flags );
	   printf("> System msg: %s \n",csatlakozas);
	    
	   //2:szobaszamok tisztazasa1
	   recv( fd, csatlakozas, bytes, flags );
	   printf(" > %s \n",csatlakozas);

	   fgets(P1szoba,bytes,stdin);
	   send(fd, P1szoba, bytes, flags);


//----------------------------------------------------------------------------------------

    printf("> Welcome in the room... Let's Chaaaaaaaaat!\n");
    printf("> Waiting for your partner...\n");

    char msg[bytes];

   while(1){

      //P1 or P2
      recv( fd, msg, bytes, flags );
      if(strncmp(msg,"kilepes",7)==0){         
         printf("\n\nA chatnek vege...\n");
         break;}
      if(strlen(msg)!=0){
        printf("\n partner: %s", msg);
      }
      printf(" you: ");

      fgets(msg,bytes,stdin);
      send(fd, msg, bytes, flags);
      if (trnmsize < 0) {
         printf("%s: Cannot send data to server.\n", argv[0]);
         exit(3);
      }
      if(strncmp(msg,"kilepes",7)==0){
         printf("\n\nA chatnek vege...\n");
         break;}
            
   } 

    printf("\nClient exited succesfuly...\n");
    close(fd);
    exit(0);   
  
} 

