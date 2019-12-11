#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include "votazione.h"

static int inizializzato = 0;

static Giudice giudice;
static Output output;
static Tabella tabella;

// Definisco la tabella.persona dei dati sono internamente perché
// non verrà utilizzata dal client

void inizializza(){
	int i, j;

	if(inizializzato == 1) // CONTROLLO CHE LA TABELLA SIA GIA' INIZIALIZZATA
		return;			   // EVENTUALMENTE SALTO LA FUNZIONE

	// ALLOCHIAMO LA MEMORIA PER TUTTE LE STRINGHE NECESSARIE
	for(i = 0; i < NUMGIUDICI; i++){
		output.giudici[i].nome = (char*) malloc(MAXSTRINGLENGHT);
	}
	for(i = 0; i < NUMPART; i++){
		tabella.persona[i].candidato = (char*) malloc(MAXSTRINGLENGHT);
		tabella.persona[i].giudice = (char*) malloc(MAXSTRINGLENGHT);
		tabella.persona[i].nomeFile = (char*) malloc(MAXSTRINGLENGHT);
	}
	// ALLOCHIAMO LA MEMORIA PER TUTTE LE STRINGHE NECESSARIE
	
	// GENERO L'ARRAY DEI GIUDICI
	strcpy(output.giudici[0].nome, "Endri"); 	output.giudici[0].punteggioTot = 0;
	strcpy(output.giudici[1].nome, "Karina");	output.giudici[1].punteggioTot = 0;
	strcpy(output.giudici[2].nome, "Andrei");		output.giudici[2].punteggioTot = 0;
	strcpy(output.giudici[3].nome, "Daniel");	output.giudici[3].punteggioTot = 0;
	strcpy(output.giudici[4].nome, "Hiari");	output.giudici[4].punteggioTot = 0;
	// GENERO L'ARRAY DEI GIUDICI
	
	// GENERO UNA TABELLA DI PERSONE CHE PARTECIPANO AL CONCORSO
	char * name[NUMPART];
	for (i = 0; i < NUMPART; i++) 
		name[i] = (char*) malloc(MAXSTRINGLENGHT);
	strcpy(name[0], "Isabel");
	strcpy(name[1], "Aurora");
	strcpy(name[2], "Luca");
	strcpy(name[3], "Marco");
	strcpy(name[4], "Piero");
	strcpy(name[5], "Andrea");
	strcpy(name[6], "John");
	strcpy(name[7], "Lucia");
	strcpy(name[8], "Elisa");
	strcpy(name[9], "Franca");

	// Popolo la tabella con i nomi dei partecipanti, col nome del file e con il voto
	for(i = 0; i < NUMPART; i++){
		strcpy(tabella.persona[i].candidato, name[i]);
		strcpy(tabella.persona[i].nomeFile, name[i]);
		tabella.persona[i].voto = random() % 20;
	}
	// Assegno a ogni partecipante un giudice
	for(i = 0; i < 5; i++){
		j = i * 2;
		tabella.persona[j].giudice = output.giudici[i].nome;
		tabella.persona[j + 1].giudice = output.giudici[i].nome;
	}
	//Assegno una categoria e una fase a ogni partecipante
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
	// GENERO UNA TABELLA DI PERSONE CHE PARTECIPANO AL CONCORSO

	inizializzato = 1;	// DOPO LA ALLOCAZIONE E CREAZIONE DELLE TABELLE METTO UN CONTROLLO PER EVITARE UNA REINIZIALIZZAZIONE DELLA TABELLA
	printf("Terminata inizializzazione del Server!\n");

	// PRINT DELLA TABELLA DI TUTTI PARTECIPANTI A CONSOLE
	printf("[Nome]\t\t[Giudice]\t[Cat]\t\t[Fase]\t\t[PT]\n");
	for(i = 0; i < 10; i++){
		printf("%s\t\t%s\t\t%c\t\t%c\t\t%d\n", tabella.persona[i].candidato, tabella.persona[i].giudice, tabella.persona[i].categoria, tabella.persona[i].fase, tabella.persona[i].voto);
	}
	printf("\n");
	// PRINT DELLA TABELLA DI TUTTI PARTECIPANTI A CONSOLE

}

Output * classifica_giudici_1_svc(void *in, struct svc_req * rqstp){
	inizializza();	// Chiamo la funzione inizializza per eventualmente generare la tabella
	int i, j;
	int index = 0, max = 0, now = 0, getG, gDiff, strcmpPlaceholder = -1;
	static Output localOut;

	// Alloco in memoria le stringhe di localOut - inoltre resetto i punteggi
	for(i = 0; i < NUMGIUDICI; i++){
		localOut.giudici[i].nome = (char*) malloc(MAXSTRINGLENGHT);
		output.giudici[i].punteggioTot = 0;
	}

	printf("\nRicevuta richiesta di stampa della classifica dei giudici.\n");
	
	// Assegno ad ogni Giudice i punti dei suoi partecipanti
	for(i = 0; i < NUMGIUDICI; i++){
		for(j = 0; j < NUMPART; j++){
			if(strcmp(output.giudici[i].nome, tabella.persona[j].giudice) == 0){
				output.giudici[i].punteggioTot += tabella.persona[j].voto;
			}
		}
	}
	
	// Trovo chi ha il punteggio più alto tra tutti
	for(i = 0; i < NUMGIUDICI; i++)
		if(output.giudici[i].punteggioTot > max){
			max = output.giudici[i].punteggioTot;	// Ne assegno a max il punteggio
			getG = i;								// e ne salvo l'indice
		}

	localOut.giudici[0] = output.giudici[getG];		// Assegno come primo valore di localOut il giudice che ho riconosciuto con il punteggio maggiore

	// Per i restanti giudici, invoco questa funzione di sorteggio con riferimento il giudice maggiore
	while(index != NUMGIUDICI - 1){
		for(i = 0; i < NUMGIUDICI; i++){
			if(strcmpPlaceholder == -1){																// strcmpPlaceholder serve ad occuparsi di giudici con il medesimo punteggio
				if(i == getG)																			// Se l'indice attuale è identico alla getG salto la i
					continue;
				if(output.giudici[i].punteggioTot < max && output.giudici[i].punteggioTot > now){		// Cerco il nuovo massimo
					getG = i;																			// Se trovato riassegno getG
					now = output.giudici[i].punteggioTot;												// Now lo utilizziamo per verificare che nell'iterazione attuale,
				}																						// ci siano eventualmente valori maggiori di quello appena trovato.
				if(getG != i && output.giudici[i].punteggioTot == output.giudici[getG].punteggioTot){	// Se ci sono due giudici con lo stesso punteggio
					if((gDiff = strcmp(output.giudici[getG].nome, output.giudici[i].nome)) != 0){		// eventuale controllo dei nomi tramite string compare
						if(gDiff > 0)																	// Ordino alfabeticamente i giudici con lo stesso punteggio
							getG = i;
						else
							strcmpPlaceholder = i;														// Entra in gioco strcmpPlaceholder, che prende il valore dell'indice
					}
				}
			} else {																					// Clausola else in caso di strcmpPlaceholder != -1
				getG = strcmpPlaceholder;																// Assegno a getG il valore del placeholder
				strcmpPlaceholder = -1;																	// Ne resetto il valore
				break;																					// ed esco dal ciclo
			}
		}																								
		now = 0;																						// Resetto now
		max = output.giudici[getG].punteggioTot;														// Assegno a max il nuovo valore di riferimento
		index++;																						// Aumento l'index del vettore ordinato
		localOut.giudici[index] = output.giudici[getG];													// Assegno il valore successivo in ordine a localOut
	}

	return (&localOut); // Ritorno al cliente il puntatore a localOut
}               

int * esprimi_voto_1_svc(Input * input, struct svc_req * rqstp){
	inizializza();			// Chiamo la funzione inizializza per eventualmente generare la tabella
	static int res = -1; 	// Il risultato, in caso di errore è già fissato a -1
	int i, found = -1;
	printf("Ricevuta richiesta di votazione.\n");
	
	// Itero la tabella dei candidati per verificare se il nome passato fosse corretto; eventualemnte assegno l'indice a found
	for(i = 0; i < NUMPART && found == -1; i++){
		if(strcmp(input->nomeCandidato, tabella.persona[i].candidato) == 0){
			found = i;
		}
	}

	// Controllo che found sia maggiore del valore di default(-1)
	if(found > -1){
		// Controllo a che tipo di operazione si riferisce tipoOp
		// A = Aggiungi
		if(input->tipoOp == 'A'){
			tabella.persona[found].voto++;
			printf("Voto aggiunto a %s, con un totale attuale di %d punti!\n", tabella.persona[found].candidato, tabella.persona[found].voto);
		}
		// Non aggiungo un controllo di verifica che tipoOp sia S(sottrai), dato che il cliente
		// controlla opportunamente che passino solo "A" ed "S" come valori
		else{
			if(tabella.persona[found].voto > 0){
				tabella.persona[found].voto--;
				printf("Voto tolto a %s, con un totale attuale di %d punti.\n", tabella.persona[found].candidato, tabella.persona[found].voto);
			} 
		}
		// Assegno a res il valore di found, che essendo arrivato fin qui non può essere -1
		res = found;
	}
	return(&res);	// Ritorno al cliente il puntatore a res

}