#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "bingo.h"
#include <errno.h>


void
error(int errnum,char* err_msg)
{
	if (errnum == -1)
	{
		printf("An error occured : \n");
		printf("%s\n",err_msg );
		printf("Error : %s (%d)\n",strerror(errno),errno );
	}
}



int
Connection()
{
	int petak = 1000;
	struct sockaddr_in szerver;
	struct sockaddr_in kliens;
	int kliens_meret = sizeof(kliens);
	char on = 1;
	char szerver_cim[16];
	int flags = 0;
	int szerver_meret = sizeof szerver;
	int szelveny[9];
	// int nyeroszamok[MAX_NYSZ_COUNT+1];
	int nyeroszamok[MAX_NYSZ_COUNT];
	int Bingo = 0;
	int tet = 0;
	int gameOver = 0;
	char uzi[512];
	int r=0;
	int r1=0;
	int rnr=0;

	szerver.sin_family = AF_INET;
	szerver.sin_addr.s_addr = htonl(INADDR_ANY);
	szerver.sin_port = htons(9000);

	int fd = socket(AF_INET,SOCK_STREAM,0);
	error(fd,"Failed to create socket.");
	printf("[+] Server socket created\n");
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

	
	//printf("%s\n",buffer );
	


	int binding = bind(fd,(struct sockaddr*)&szerver,sizeof(szerver));
	error(binding,"Failed to bind the socket.");

	if (listen(fd,10) == 0) printf("Listening...\n");
	else
	{
		printf("[-] Error in binding\n");
	}
	while (gameOver !=1)
	{
		

	Random_Generator(45,30,nyeroszamok);

	while(1)
	{
			

			// Bingo == 1 ? gameOver = 1 : gameOver == gameOver;

			int ujSocket = accept(fd,(struct sockaddr*)&kliens,&kliens_meret);
			if (ujSocket < 0){
				exit(1);
			}

			printf("[+] Connection accepted\n");
			if (fork() == 0)
			{

				close(fd);

			while (1)
			{
			int rec_size = recv(ujSocket,&r,sizeof(r),flags);
			error(rec_size,"Failed to recieve msg");
			printf("uzi : %d",r);
			int recv_rnr= recv(ujSocket,&rnr,sizeof(rnr),flags);
			error(recv_rnr,"Failed to recv message. sign");
			printf("\n\n\n\t\t // PLAYER #%d  //\n\t\t// %d. ROUND //\n\n\n\n",r,rnr );
			int send_petak = send(ujSocket,&petak,sizeof(petak),flags);
		   	error(send_petak,"Failed to send the message. petak");
		// TET FOGADASA
			int rec_tet = recv(ujSocket,&tet,sizeof(tet),flags);
		   	error(rec_tet,"Failed to receive the message. tet");
		   	printf("/ - BET - / : %d\n",tet);
		   	printf("/ - BALANCE - / : %d\n",petak-tet);
		// SZELVENY FOGADASA
		   	int rec_szelveny = recv(ujSocket,szelveny,sizeof(szelveny),flags);
		   	error(rec_szelveny,"Failed to receive the message.");
	   		printf("/ - PLAYER %d's coupon - / : \n\n",r);
	   		ShowSzelveny(szelveny);
	   		// for (int i =0;i<30;i++)
	   		// {
	   		// 	printf("%d. %d\n", i, nyeroszamok[i]);
	   		// }
	   		// NYEROSZAMOK KULDESE
	   		// Random_Generator(NYSZ_PLAFON,MAX_NYSZ_COUNT,nyeroszamok);  ------
			// int32_t kuldoTomb[MAX_NYSZ_COUNT] ;
			// for (int i = 0 ; i < MAX_NYSZ_COUNT ; ++i) 
			// {
			//     kuldoTomb[i] = htonl(nyeroszamok[i]) ;
			//     printf("%d. elem ---\nkuldoTomb: %d\nnyeroszamok: %d\n\n",i+1,kuldoTomb[i],nyeroszamok[i] );
			// }

		   	int send_nyeroszam = send(ujSocket,nyeroszamok,sizeof(nyeroszamok),flags);
		   	error(send_nyeroszam,"Failed to send the message. nyszam");
		   	petak = CheckBingo(&Bingo,nyeroszamok,szelveny,petak,tet);
			// PENZ KULDESE
		   	int send_petak2 = send(ujSocket,&petak,sizeof(petak),flags);
		   	error(send_petak2,"Failed to send the message.");
		// UZENET KULDESE 
	   		if (petak < 1)
		   	{
		   		printf("\t\tPLAYER %d# ran out of money.\n",r);
		   		printf("\t\tPLAYER %d# left the game.\n",r );
		   		break;
			}
		}
			
	}


	}
}



	close(fd);
	return 0;

}


int
main(int argc,char**argv)
{
	
	Connection(argv);

}