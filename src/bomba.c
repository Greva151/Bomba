#include "mySocket.h"
#include "bomba.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

utente utenti[MAXPLAYER];
char numeroPlayer = 0;
char myIPPublic[MAXLENIP];
char myNickname[MAXNICKNAME]; 


//dice l'indice a cui è salvato quel giocatore
int findNickname(char IPstr[]){
    for(int i = 0; i < numeroPlayer; i++){
        if(!strncmp(IPstr, utenti[i].IPstr, MAXLENIP)){
            return i; 
        }
    }
}


//quando si riceve un pacchetto dove dice che la bomba � esplosa a un compagno
void ricevutaEsplosione(char IPstr[]){
    printf("Hai vinto, la bomba e' esplosa in mano di a: %s\n", utenti[findNickname(IPstr)].nickName);
    exit(0);
}


//nel caso in cui la bomba esploda senza che noi l'abbiamo ancora passata
void esplosaLaBomba(){
    char pacchetto[2];
    memset(pacchetto, 0, 2);
    pacchetto[0] = BOMBA;

    for(int i = 0; i < numeroPlayer; i++){
        trasmissione(pacchetto, utenti[i].IPstr);
    }

    printf("\n\n\n\n\nHAI PERSO, TI E' ESPLOSA LA BOMBA IN MANO!!!!! game over..................\n"); 

    exit(0); 
}


//dice a tutti i giocatori che si ha la bomba
void ioHoLaBomba(){
    char pacchetto[2];
    memset(pacchetto, 0, 2);
    pacchetto[0] = IOHOLABOMBA;

    for(int i = 0; i < numeroPlayer; i++){
        trasmissione(pacchetto, utenti[i].IPstr);
    }
}


//crea il pacchetto per passare la bomba a qualcuno sorteggiato casualmente
void passaBomba(char time){
    char pacchetto[3];
    memset(pacchetto, 0, 3);
    unsigned char vincitore;

    vincitore = (rand() % numeroPlayer);

    pacchetto[0] = RICEVUTABOMBA;
    pacchetto[1] = time;

    trasmissione(pacchetto, utenti[vincitore].IPstr);
}


//chiede all'utente di premere invio per inviare la bomba a qualcun'altro
void ricevutoLaBomba(char tempo){
    ioHoLaBomba();

    time_t tempoCorrente;
    time(&tempoCorrente);
    time_t tempoMax = tempoCorrente + tempo;

    printf("HAI RICEVUTO LA BOMBA PREMI UN TASTO PER PASSARLA A QUALCUNO!!!!!!! (va bene qualsiasi tasto tranne quello di spegnimento XD)\n");
    getchar();

    time(&tempoCorrente);

    if(tempoCorrente > tempoMax){
        esplosaLaBomba();
    }
    else{
        char tempoCheManca = tempoMax - tempoCorrente;
        passaBomba(tempoCheManca);
    }
}


//prende un ip in formato stringa e lo trasforma in intero se l'ip � errato ritorna un numero negativo
long long ipStrToInt(char IPstr[]) {
    unsigned int intIP = 0;
    int ottetto = 0;
    int ottettoCount = 0;

    for (int i = 0; IPstr[i] && IPstr[i] != '\n'; i++) {
        char currentChar = IPstr[i];

        if (isdigit(currentChar)) {
            ottetto = ottetto * 10 + (currentChar - '0');

            if (ottetto > 255) {
                return -1;
            }
        } else if (currentChar == '.') {
            if (++ottettoCount > 3) {
                return -1;
            }

            intIP = (intIP << 8) | ottetto;
            ottetto = 0;
        } else {
            return -1;
        }
    }

    if (ottettoCount != 3) {
        return -1;
    }

    intIP = (intIP << 8) | ottetto;

    return intIP;
}


//genera il tempo che durerà la bomba
unsigned char randomSecondiBomba(){
    srand(time(NULL));
    return (char) ((rand() % 50) + 10);
}


//controlla quale IP è il maggiore e se siamo noi vuol dire che � il master di gioco
void iamTheMaster(){
    int flag = true;

    for(int i = 0; i < numeroPlayer; i++){
        if(ipStrToInt(utenti[i].IPstr) > ipStrToInt(myIPPublic)){
            flag = false;
            break;
        }
    }

    if(flag){
        printf("\nSei il master di gara\n"); 
        unsigned char secondiBomba = randomSecondiBomba();
        printf("Secondi della Bomba %d\n", secondiBomba); 
        passaBomba(secondiBomba);
    }
}


//mette la presenza alla conferma della ricezione della mia presenza
void controlloConferma(char IPstr[]){
    unsigned int IPint = ipStrToInt(IPstr);  
    for(int i = 0; i < numeroPlayer; i++){
        if(utenti[i].IPint == IPint){ 
            utenti[i].haveRisposto = 1;

            if(areTuttiPresenti()){
                iamTheMaster();
            }

            break;
        }
    }
}


//se sono tutti presenti i partecipanti allora ritorna 1 altrimenti 0
char areTuttiPresenti(){
    char flag = true;

    for(int i = 0; i < numeroPlayer; i++){ 
        if(!utenti[i].isPresente || !utenti[i].haveRisposto){
            flag = false;
        }
    }

    return flag;
}


//invia il pacchetto di conferma che è arrivata la presenza
void confermaPresenza(char IPstr[]){
    char head2[2];
    memset(head2, 0, 2);
    head2[0] = CONFERMA;
    trasmissione(head2, IPstr);
}


//controlla se chi è online e nel caso gli fa sapere che pure lui è presente
void controlloPresenza(char IPstr[], char recBuff[]){
    unsigned int IPint = ipStrToInt(IPstr);  
    for(int i = 0; i < numeroPlayer; i++){ 
        //printf("IP salvato (valore): %llu IP ricevuto to int: %llu\n", utenti[i].IPint, IPint);
        if(utenti[i].IPint == IPint){ 
            utenti[i].isPresente = 1;
            confermaPresenza(IPstr);

            for(int q = 1; recBuff[q]; q++){
                utenti[i].nickName[q - 1] = recBuff[q]; 
            }

            //printf("nickname ricevuto: %s", utenti[i].nickName); 

            if(areTuttiPresenti()){
                iamTheMaster();
            }

            if(utenti[i].isPresente && !utenti[i].haveRisposto){
                iAmHere();
            }

            break;
        }
    }
}


//manda a tutti i giorcatori un pacchetto dove dice che è arrivato in partita
void iAmHere(){
    for(int i = 0; i < numeroPlayer; i++){
        char head1[22];
        memset(head1, 0, 22);
        head1[0] = CISONO;

        for(int i = 0; myNickname[i + 1]; i++){
            head1[i + 1] = myNickname[i];
        }
        
        trasmissione(head1, utenti[i].IPstr); 
    }
}


//toglie gli \n dalla stringa degli IP
void pulisciIP(){
    for(int q = 0; q < numeroPlayer; q++){
        for(int i = 0; i < MAXLENIP; i++){
            if(utenti[q].IPstr[i] == '\n') utenti[q].IPstr[i] = '\0';
        }
    }
}


//stampa in versione dotted un ip passato come intero
void stampaIP(unsigned int IP){
    printf("IP inserito: %d.%d.%d.%d\n", IP >> 24, (IP >> MAXLENIP) & 0xff, (IP >> 8) & 0xff, IP & 0xff);
}


// chiedere all'utente quali sono gli ip dei giocatori e li salva dentro poolIP
void inserisciIP(){
    char IPstr[MAXLENIP];
    long long flag = 0;
    char letter[2];
    unsigned char counter = 0;

    do {
        do {
            printf("inserisci l'IP: ");
            fgets(IPstr, MAXLENIP, stdin);
            fflush(stdin);

            flag = ipStrToInt(IPstr);

            if(flag < 0) printf("l'IP inserito e' sbagliato, inseriscilo di nuovo...\n");
        } while(flag < 0);

        strncpy(utenti[counter].IPstr, IPstr, MAXLENIP);
        utenti[counter].IPint = flag;

        stampaIP(utenti[counter].IPint);

        counter++;

        if(counter < MAXPLAYER){
            do {
                printf("vuoi inserire un'altro IP? (s/n): ");
                fgets(letter, 2, stdin);
                fflush(stdin);
                if(letter[0] != 's' && letter[0] != 'n') printf("lettera non accettata, reinseriscila...\n");
            } while(letter[0] != 's' && letter[0] != 'n');
        }

    } while(letter[0] == 's' && counter < MAXPLAYER);

    numeroPlayer = counter;

    pulisciIP();
}
