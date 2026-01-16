#include <windows.h>
#include <stdio.h>
#include "mySocket.h"

#define MAXLENIP 16
#define MAXBUFF 128
#define DEFAULTPORT 6000

WSADATA wsaData;


//start WSA
void startWSA(){
    WORD wVersionRequested = MAKEWORD(2, 2);

    if(WSAStartup(wVersionRequested, &wsaData)) exit(1);
    if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2){
        WSACleanup();
        exit(2);
    }
}


//invia la stringa che passiamo come parametro all'indirizzo che passiamo come parametro
void trasmissione(char str[], char ipDest[]) {
    SOCKET sockTx;
    SOCKADDR_IN addrRemoto;

    char sendBuff[MAXBUFF];
    char szIPDest[MAXLENIP];

    strncpy(szIPDest, ipDest, MAXLENIP);

    sockTx = socket(AF_INET, SOCK_DGRAM, 0);
    addrRemoto.sin_family = AF_INET;
    addrRemoto.sin_port = htons(DEFAULTPORT);
    addrRemoto.sin_addr.S_un.S_addr = inet_addr(szIPDest);

    strncpy(sendBuff, str, MAXBUFF);

    sendto(sockTx, sendBuff, strlen(sendBuff) + 1, 0, (SOCKADDR *) &addrRemoto, sizeof(SOCKADDR));

    fflush(stdout);

    //printf("Il messaggio: '%s', e' stato inviato a: '%s'\n", str, ipDest);

    closesocket(sockTx);
}
