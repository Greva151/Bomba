#ifndef BOMBA_H
#define BOMBA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>

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

typedef struct {
    unsigned int IPint;
    char IPstr[MAXLENIP];
    char nickName[MAXNICKNAME]; 
    char isPresente;
    char haveRisposto;
} utente;

extern utente utenti[MAXPLAYER];
extern char numeroPlayer;
extern char myIPPublic[MAXLENIP];
extern char myNickname[MAXNICKNAME]; 

void inserisciIP();
void stampaIP(unsigned int IP);
long long ipStrToInt(char IPstr[]);
void pulisciIP();
void iAmHere();
void controlloPresenza(char IPstr[], char recBuff[]);
void confermaPresenza(char IPstr[]);
char areTuttiPresenti();
void controlloConferma(char IPstr[]);
void iamTheMaster();
unsigned char randomSecondiBomba();
void passaBomba(char time);
void ricevutoLaBomba(char time);
void ioHoLaBomba();
void esplosaLaBomba();
void ricevutaEsplosione(char IPstr[]);
int findNickname(char IPstr[]);

#endif
