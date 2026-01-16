/*
    - Svantaggi: 
        - non fa il controllo se degli utenti hanno lo stesso IP (in caso in cui il programma sia avvitato in WAN è improbabile l'errore)
        - nel caso in cui venga chiuso un programma, tutti gli utenti rimangono bloccati nel caso in cui la bomba la aveva o arriva all'utente che non è più loggato
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
#include "mySocket.h"
#include "bomba.h"

#define true 1
#define false 0
#define MAXPLAYER 4
#define DEFAULTPORT 6000
#define MAXBUFF 128
#define CISONO 67
#define CONFERMA 75
#define RICEVUTABOMBA 82
#define IOHOLABOMBA 65
#define BOMBA 66
#define MAXNICKNAME 20
#define MAXLENIP 16


DWORD WINAPI threadRicezione(LPVOID lpPrameter);
DWORD dwthread;


int main(){

    printf("Benvenuto al gioco BOMBA, passa la BOMBA prima che esploda!!!\n");

    startWSA();

    HANDLE threadHandle = CreateThread(NULL, 0, threadRicezione, (LPVOID) 0, 0, &dwthread);

    if (threadHandle == NULL) {
        printf("Errore durante la creazione del thread\n");
        return 1;
    }

    Sleep(2000); //aspetto che si avvii del tutto il thread

    printf("Inserisci il tuo IP pubblico: ");
    fgets(myIPPublic, MAXLENIP, stdin);
    fflush(stdin);
    printf("Inserisci il tuo nickname di gioco: ");
    fgets(myNickname, MAXNICKNAME, stdin);

    fflush(stdin);

    inserisciIP();

    iAmHere();

    WaitForSingleObject(threadHandle, INFINITE);

    CloseHandle(threadHandle);

    return 0;
}

//apre un thread per la ricezione dei pacchetti
DWORD WINAPI threadRicezione(LPVOID lpPrameter){
    SOCKET sockRx;
    SOCKADDR_IN addrLocale;
    SOCKADDR_IN addrRemoto;
    char recBuff[MAXBUFF];
    int lsockaddr;
    int nbytes;

    lsockaddr = sizeof(SOCKADDR);
    sockRx = socket(AF_INET, SOCK_DGRAM, 0);
    addrLocale.sin_family = AF_INET;
    addrLocale.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrLocale.sin_port = htons(DEFAULTPORT);
    bind(sockRx, (SOCKADDR *) &addrLocale, sizeof(SOCKADDR));

    //printf("In ascolto sulla porta %d...\n", DEFAULTPORT);

    while(1){
        nbytes = recvfrom(sockRx, recBuff, sizeof(recBuff), 0, (SOCKADDR *) &addrRemoto, &lsockaddr);
        recBuff[nbytes] = 0;

        //printf("Ricevuto messaggio: %s da questo ip: %s\n", recBuff, inet_ntoa(addrRemoto.sin_addr));
        //printf("ricevuto hex: %x\n", recBuff[0]);

        switch (recBuff[0]){
            case CISONO : {
                controlloPresenza(inet_ntoa(addrRemoto.sin_addr), recBuff);
                break;
            }
            case CONFERMA : {
                controlloConferma(inet_ntoa(addrRemoto.sin_addr));
                break;
            }
            case RICEVUTABOMBA : {
                ricevutoLaBomba(recBuff[1]);
                break;
            }
            case IOHOLABOMBA : {
                printf("%s HA LA BOMBA\n", utenti[findNickname(inet_ntoa(addrRemoto.sin_addr))].nickName);
                break;
            }
            case BOMBA : {
                ricevutaEsplosione(inet_ntoa(addrRemoto.sin_addr));
                break;
            }
            default : { 
                printf("oh no, questo pacchetto non riesco a riconoscerlo... NON CI STARANNO MICA PWNANDO?!?!?!\n");
                break;
            }
        }
    }

    closesocket(sockRx);
    WSACleanup();

    return 0;
}
