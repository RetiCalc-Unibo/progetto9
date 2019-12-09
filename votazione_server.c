#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include "votazione.h"

#define NUMPART 10

static int inizializzato = 0;
static Input input;
static Giudice giudice;
static Output output;

// Definisco la tabella dei dati sono internamente perché
// non verrà utilizzata dal client
struct Tabella {
	char candidato[128];
	char giudice;
	char categoria;
	char nomeFile[128];
	char fase;
	int voto;
};
typedef Tabella Tabella;

static Tabella tabella;

void inizializza(){
	int i, j;

	if(inizializzato == 1)
		return;

	// Init struct giudici
	giudice[0].nome = "Endri";
	giudice[0].punteggioTot = 0;

	giudice[1].nome = "Hiari";
	giudice[1].punteggioTot = 0;
	
	giudice[2].nome = "Ivan";
	giudice[2].punteggioTot = 0;
	
	giudice[3].nome = "Daniel";
	giudice[3].punteggioTot = 0;
	
	giudice[4].nome = "Karina";
	giudice[4].punteggioTot = 0;
	// Fine init giudici
	
	// Init struct tabella
	const char * name[9];
	name[0] = "Isabel";
	name[1] = "Aurora";
	name[2] = "Luca";
	name[3] = "Marco";
	name[4] = "Piero";
	name[5] = "Andrea";
	name[6] = "Giuseppe";
	name[7] = "Lucia";
	name[8] = "Elisa";
	name[9] = "Francesca";
	for(i = 0; i < 10; i++){
		tabella[i].candidato[i] = name[i];
		tabella[i].nomeFile[i] = name[i];
		strcat(tabella[i].nomeFile[i], ".txt");
		tabella[i].voto[i] = rand(10);
	}
	for(i = 0; i < 5; i++){
		j = i * 2;
		tabella[j].giudice = gidice[i].nome;
		tabella[j + 1].giudice = giudice[i].nome;
	}
	tabella[0].categoria = 'D'; tabella[0].fase = 'S';
	tabella[1].categoria = 'D'; tabella[1].fase = 'S';
	tabella[2].categoria = 'U'; tabella[2].fase = 'B';
	tabella[3].categoria = 'O'; tabella[3].fase = 'A';
	tabella[4].categoria = 'U'; tabella[4].fase = 'S';
	tabella[5].categoria = 'B'; tabella[5].fase = 'B';
	tabella[6].categoria = 'B'; tabella[6].fase = 'B';
	tabella[7].categoria = 'D'; tabella[7].fase = 'A';
	tabella[8].categoria = 'D'; tabella[8].fase = 'B';
	tabella[9].categoria = 'D'; tabella[9].fase = 'A';
	// Fine init tabella

	inizializzato = 1;

	printf("Terminata inizializzazione del Server!\n");
	printf("[Nome]\t\t[Giudice]\t\t[Cat]\t\t[Fase]\t\t[Punteggio]\n");
	for(i = 0; i < NUMPART; i++){
		printf("%s\t\t%s\t\t%c\t\t%c\t\t%d\n", tabella[i].candidato, tabella[i].giudice, tabella[i].categoria, tabella[i].fase, tabella[i].voto);
	}
}

Output * classifica_giudici_1_svc(void *, struct svc_req *){
	int i, j, index = 0, max = 0;
	Giudice startMax;
	printf("Ricevuta richiesta di stampa della classifica dei giudici.\n");
	inizializza();
	for(i = 0; i < NUMGIUDICI; i++){
		for(j = 0; j < NUMPART; j++){
			if(strcmp(giudice[i].nome, tabella[j].giudice) == 0){
				giudice[i].punteggioTot += tabella[j].voto;
			}
		}
	}
	for(i = 0; i < NUMGIUDICI; i++)
		if(giudice[i].punteggioTot > max){
			max = giudice[i].punteggioTot;
			startMax = giudice[i];
		}

	printf("--Classifica--\n[GIUDICE]\t\t[PUNTEGGIO]\n");
	printf("%s\t\t%d", startMax.nome, startMax.punteggioTot);
	while(index != NUMGIUDICI - 1){
		/*
		********** DA FINIRE ALG DI SORTING PER IL PRINT DEI GIUDICI IN ORDINE
		 */
	}
}               

int * esprimi_voto_1_svc(Input *, struct svc_req *){
	inizializza();
	/*
	*********** DA INIZIARE L'ALG DI ADD/SUB DEI VOTI (anche se credo non sia troppo difficile)
	 */
}