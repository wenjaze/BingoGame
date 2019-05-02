#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
main(int argc,char ** argv)
{
	int petak,petak1;
	struct sockaddr_in szerver;
	struct sockaddr_in kliens;
	char on = 1;
	char szerver_cim[16];
	int flags = 0;
	int szerver_meret = sizeof szerver;
	int kliens_meret = sizeof kliens;
	int szelveny[9];
	int nyeroszamok[MAX_NYSZ_COUNT+1];
	int tet=0;
	int gameOver = 0;
	int rnr = 1;
	int Bingo = 0;
	char uzi[50];
	int r=atoi(argv[1]);


	// BEALLITASOK

	szerver.sin_family = AF_INET;
	szerver.sin_addr.s_addr = INADDR_ANY;
	szerver.sin_port = htons(9000);

	int fd = socket(AF_INET,SOCK_DGRAM,0);
	error(fd,"Failed to create socket.");

	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   	
   	while (gameOver != 1)
   	{
	   	Bingo == 1 ? gameOver = 1 : gameOver == gameOver;
	   	
	   		Happy();
			int sz = Szelveny(gameOver,szelveny);
			if (sz==3)
			{
				return 0;
			}
			
			int send_sign = sendto(fd,&r,sizeof(r),flags,(struct sockaddr*) &szerver,szerver_meret);
	   		error(send_sign,"Failed to send the message. sign");
	   		int send_rnr = sendto(fd,&rnr,sizeof(rnr),flags,(struct sockaddr*) &szerver,szerver_meret);
	   		error(send_rnr,"Failed to send the message. sign");
	   	// PENZ KULDESE

	   		int recv_petak = recvfrom(fd,&petak,sizeof(petak),flags,(struct sockaddr*)&szerver,&szerver_meret);
		   	error(recv_petak,"Failed to send the message. petak");
		   	
		   	printf("/ - COINS - / : %d\n", petak );
			printf("/ - BET - / : ");
			scanf("%d",&tet);
			printf("/ - BALANCE - / : %d\n",petak -tet);
		// TET KULDESE	
		   	int send_tet = sendto(fd,&tet,sizeof(tet),flags,(struct sockaddr*) &szerver,szerver_meret);
	   		error(send_tet,"Failed to send the message.");
   		// SZELVENY KULDESE
	   		int send_szelveny = sendto(fd,szelveny,sizeof(szelveny),flags,(struct sockaddr*) &szerver,szerver_meret);
	   		error(send_szelveny,"Failed to send the message.");
	   	// NYEROSZAM FOGADASA
	   		// printf("elotte:\n");
	   		// for (int i =0;i<30;i++)
	   		// {
	   		// 	printf("%d. %d\n", i, nyeroszamok[i]);
	   		// }

	   		// printf("%p\n", &nyeroszamok);
	  		// int32_t nyeroszamok[MAX_NYSZ_COUNT] ;
			// int32_t fogadoTomb[MAX_NYSZ_COUNT] ;
			// memset(fogadoTomb,0,MAX_NYSZ_COUNT);
	   		memset(nyeroszamok,0,MAX_NYSZ_COUNT);
		   	int rec_nyeroszam = recvfrom(fd,nyeroszamok,MAX_NYSZ_COUNT*sizeof(int),flags,(struct sockaddr*) &szerver,&szerver_meret);
		   	error(rec_nyeroszam,"Failed to receive the message.11");
		   	Sorsolas(&Bingo,nyeroszamok);
		   	
		 	//   for (int i = 0 ; i < MAX_NYSZ_COUNT ; ++i) 
		 	//   {
			//     nyeroszamok[i] = ntohl(fogadoTomb[i]) ;
			//     printf("%d. elem ---\nfogadoTomb: %d\nnyeroszamok: %d\n\n",i+1,fogadoTomb[i],nyeroszamok[i] );
			// 	}
		   	// printf("utana:\n");
		   	// for (int i =0;i<30;i++)
	   		// {
	   		// 	printf("%d. %d\n", i, nyeroszamok[i]);
	   		// }
		// PENZ FOGADASA
			int rec_petak2 = recvfrom(fd,&petak,sizeof(petak),flags,(struct sockaddr*) &szerver,&szerver_meret);
		   	error(rec_petak2,"Failed to receive the message.22");
		// UZENET FOGADASA	
		   	if (petak < 1)
		   	{
		   		printf("You ran out of money!\n");
		   		int rec_uzi = recvfrom(fd,uzi,sizeof(uzi),flags,(struct sockaddr*) &szerver,&szerver_meret);
		   		error(rec_uzi,"Failed to receive the message.33");
		   		break;
		   	}
		   	

		   	
		
			OnlyCheck(nyeroszamok,szelveny);
			printf("/ - NEW BALANCE - / :%d\n",petak);
	   		printf("\n\n\t\tPress [Enter] to continue.\n");
        	fflush(stdin); // option ONE to clean stdin
        	getchar(); // wait for ENTER
			getchar();
			system("clear");
			printf("\t\tSTARTING NEW ROUND\n");
			
			Varakozas(3);
			system("clear");
			rnr++;
	   		

   	}




	close(fd);


	return 0;
}
