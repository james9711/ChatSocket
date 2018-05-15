/***************************************/
/*              TCP server             */
/***************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#define BUFSIZE 1024                      // buffer size

void szobavalt(char* tesztelt_uzenet, char* new_szoba){
	if (strncmp(tesztelt_uzenet,"szoba:1",7)==0) { 		
	   printf("A P1 az 1-es szobaba lepett at...\n");
	   strcpy(new_szoba,"1");}
	else if (strncmp(tesztelt_uzenet,"szoba:2",7)==0) { 		
	   printf("A P1 az 2-es szobaba lepett at...\n");
	   strcpy(new_szoba,"2");}
}

int main(int argc, char *argv[] ){ 	// arg count, arg vector 

   /* Declarations */
   int fd;	        	           		// socket endpt
   int fdc1;
   int fdc2;                        	// socket endpt
   int flags;                      	// rcv flags
   struct sockaddr_in server;      	// socket name (addr) of server
   struct sockaddr_in client;	     	// socket name of client
   int server_size;                	// length of the socket addr. server
   int client_size;                	// length of the socket addr. client
   int bytes;		           					// length of buffer 
   int rcvsize1;
   int rcvsize2;                    	// received bytes
   int trnmsize;                   	// transmitted bytes
   int err;                        	// error code
   char on;                        	// 

   char buffer[BUFSIZE+1];            

   /* Initialization */
   on                     = 1;
   flags                  = 0;
   server_size            = sizeof server;
   client_size            = sizeof client;
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   int PORT_NO = atoi(argv[1]);
   server.sin_port        = htons(PORT_NO);
   bytes                  = BUFSIZE;

   /* Creating socket */
   fd = socket(AF_INET, SOCK_STREAM, 0 );
   if (fd < 0) {
      printf("%s: Socket creation error\n",argv[0]);
      exit(1);
      }

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Binding socket */
   err = bind(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      printf("%s: Cannot bind to the socket\n",argv[0]);
      exit(2);
      }

   /* Listening */
   err = listen(fd, 2);
   if (err < 0) {
      printf("%s: Cannot listen to the socket\n",argv[0]);
      exit(3);
      }

   /* Accepting connection request */
   
   fdc1 = accept(fd, (struct sockaddr *) &client, &client_size);
   fdc2 = accept(fd, (struct sockaddr *) &client, &client_size);
   if (fdc1 < 0 || fdc2<0) {
      printf("%s: Cannot accept on socket\n",argv[0]);
      exit(4);
      }

   //1:kliensek sikeresen csatlakoztak 
    send(fdc1, "A kliensek sikeresen csatlakoztak", bytes, flags);
    send(fdc2, "A kliensek sikeresen csatlakoztak", bytes, flags);

    char P1szoba[bytes];
    char P2szoba[bytes];

		//2:szobaszamok tisztazasa1
		//1
		send(fdc1, "Válassz szobat, 1 vagy 2: ", 40, flags);
			printf("> Waiting for P1 to choose.....\n");
		recv( fdc1, P1szoba, bytes, flags );

		//2:szobaszamok tisztazasa2
		//1
		send(fdc2, "Válassz szobat, 1 vagy 2: ", 40, flags);
			printf("> Waiting for P2 to choose.....\n");
		recv( fdc2, P2szoba, bytes, flags );

    //if(*P1szoba == *P2szoba){

  	printf("> Szobatarsak tarsítvaaaa... Let's Chaaaaaaaaat!\n");

  	char msgP1[bytes];
  	char msgP2[bytes];
  	strcpy(msgP2, "");

	while(1){

		if(*P1szoba!=*P2szoba){
		   	 send(fdc1, "", bytes, flags);
		   	 send(fdc2, "", bytes, flags);

		     recv( fdc1, msgP1, bytes, flags );
		     printf("p1: %s", msgP1);
		     recv( fdc2, msgP2, bytes, flags );
		     printf("p2: %s", msgP2); 

		    if ((strncmp(msgP1,"vege",4)==0) || (strncmp(msgP2,"vege",4)==0)) { 		
		     	send(fdc1, "kilepes", bytes, flags);
	    		send(fdc2, "kilepes", bytes, flags);  }
	    	if (strncmp(msgP1,"kilepes",7)==0) { 		
			   printf("A P1 kilepett a chatbol...\n");
			   close(fdc1);
				strcpy(msgP1, "");}
		 	if (strncmp(msgP2,"kilepes",7)==0) { 		
			   printf("A P2 kilepett a chatbol...\n");
			   close(fdc2);
				strcpy(msgP2, "");}

			   szobavalt(msgP1, P1szoba);
			   szobavalt(msgP2, P2szoba);

			if(*msgP2==0 && *msgP1==0){
				close(fd);
   				exit(0);
			}
		}
		else{
			//p1
		   	 send(fdc1, msgP2, bytes, flags);

		     recv( fdc1, msgP1, bytes, flags );
		     printf("p1: %s", msgP1); 

		    if (strncmp(msgP1,"vege",4)==0) { 		
		     	send(fdc1, "kilepes", bytes, flags);
	    		send(fdc2, "kilepes", bytes, flags);  }
	    	if (strncmp(msgP1,"kilepes",7)==0) { 		
			   printf("A P1 kilepett a chatbol...\n");
			   strcpy(msgP1, "");
			   close(fdc1);}

			   szobavalt(msgP1, P1szoba);
			
			//P2
		     send(fdc2, msgP1, bytes, flags);

		     recv( fdc2, msgP2, bytes, flags );
		     printf("p2: %s\n", msgP2); 

		    if (strncmp(msgP2,"vege",4)==0) { 		
			   printf("A chat veget ert, valaki kilepett a szobabol...\n");
		     	send(fdc1, "kilepes", bytes, flags);
	    		send(fdc2, "kilepes", bytes, flags); }	     
			if (strncmp(msgP2,"kilepes",7)==0) { 		
			   printf("A P2 kilepett a chatbol...\n");
			   strcpy(msgP2, "");
			   close(fdc2);}
			if(*P2szoba!=*P1szoba){
				strcpy(msgP2, "");
			}

			szobavalt(msgP2, P2szoba);

			if(*msgP2==0 && *msgP1==0){
				close(fd);
   				exit(0);
			}
		}

	}
   
   printf("A chat veget ert, valaki kilepett a szobabol...\n");
   close(fdc1);
   close(fdc2);
   close(fd);
   exit(0);
   
   } 

