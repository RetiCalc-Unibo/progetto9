#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include "votazione.h"

#define NUMPART 10
#define RAND_MAX 10

static int inizializzato = 0;
static Giudice giudice;
static Output output;
static Tabella tabella;

// Definisco la tabella.persona dei dati sono internamente perché
// non verrà utilizzata dal client


void inizializza(){
	int i, j;

	if(inizializzato == 1)
		return;

	// Init struct giudici
	output.giudici[0].nome = "Endri";
	output.giudici[0].punteggioTot = 0;

	output.giudici[1].nome = "Karina";
	output.giudici[1].punteggioTot = 0;
	
	output.giudici[2].nome = "Ivan";
	output.giudici[2].punteggioTot = 0;
	
	output.giudici[3].nome = "Daniel";
	output.giudici[3].punteggioTot = 0;
	
	output.giudici[4].nome = "Hiari";
	output.giudici[4].punteggioTot = 0;
	// Fine init giudici
	
	// Init struct tabella.persona
	char * name[9];
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
		tabella.persona[i].candidato = name[i];
		tabella.persona[i].nomeFile = name[i];
		strcat(tabella.persona[i].nomeFile, ".txt");
		tabella.persona[i].voto = random();
	}
	for(i = 0; i < 5; i++){
		j = i * 2;
		tabella.persona[j].giudice = output.giudici[i].nome;
		tabella.persona[j + 1].giudice = output.giudici[i].nome;
	}
	tabella.persona[0].categoria = 'D'; tabella.persona[0].fase = 'S';
	tabella.persona[1].categoria = 'D'; tabella.persona[1].fase = 'S';
	tabella.persona[2].categoria = 'U'; tabella.persona[2].fase = 'B';
	tabella.persona[3].categoria = 'O'; tabella.persona[3].fase = 'A';
	tabella.persona[4].categoria = 'U'; tabella.persona[4].fase = 'S';
	tabella.persona[5].categoria = 'B'; tabella.persona[5].fase = 'B';
	tabella.persona[6].categoria = 'B'; tabella.persona[6].fase = 'B';
	tabella.persona[7].categoria = 'D'; tabella.persona[7].fase = 'A';
	tabella.persona[8].categoria = 'D'; tabella.persona[8].fase = 'B';
	tabella.persona[9].categoria = 'D'; tabella.persona[9].fase = 'A';
	// Fine init tabella.persona

	inizializzato = 1;

	printf("Terminata inizializzazione del Server!\n");
	printf("[Nome]\t\t[Giudice]\t\t[Cat]\t\t[Fase]\t\t[Punteggio]\n");
	for(i = 0; i < NUMPART; i++){
		printf("%s\t\t%s\t\t%c\t\t%c\t\t%d\n", tabella.persona[i].candidato, tabella.persona[i].giudice, tabella.persona[i].categoria, tabella.persona[i].fase, tabella.persona[i].voto);
	}
}

Output * classifica_giudici_1_svc(void *in, struct svc_req * rqstp){
	int i, j;
	int index = 0, max = 0, now = 0;
	Giudice maxG;
	Output *localOut;

	inizializza();
	printf("Ricevuta richiesta di stampa della classifica dei giudici.\n");
	
	for(i = 0; i < NUMGIUDICI; i++)
		for(j = 0; j < NUMPART; j++)
			if(strcmp(output.giudici[i].nome, tabella.persona[j].giudice) == 0)
				output.giudici[i].punteggioTot += tabella.persona[j].voto;

	for(i = 0; i < NUMGIUDICI; i++)
		if(output.giudici[i].punteggioTot > max){
			max = output.giudici[i].punteggioTot;
			maxG = output.giudici[i];
		}

	localOut->	giudici[0] = maxG;

	while(index != NUMGIUDICI - 1){
		for(i = 0; i < NUMGIUDICI; i++){
			if(output.giudici[i].punteggioTot < max && output.giudici[i].punteggioTot > now && now != max){
				maxG = output.giudici[i];
			}
		}
		localOut->giudici[i + 1] = maxG;
		index++;
	}

	return localOut;
}               

int * esprimi_voto_1_svc(Input * input, struct svc_req * rqstp){
	static int res = -1;
	int i, found = 0;
	inizializza();
	printf("Ricevuta richiesta di votazione.\n");
	for(i = 0; i < NUMPART && found != 0; i++){
		if(strcmp(input->nomeCandidato, tabella.persona[i].candidato) == 0){
			found = i;
		}
	}
	if(found){
		if(input->tipoOp == 'A'){
			tabella.persona[found].voto += 1;
			printf("Voto aggiunto!\n");
		}
		else{
			tabella.persona[found].voto -= 1;
			printf("Voto sottratto!\n");
		}
		res = 1;
	}
	return(&res);

}