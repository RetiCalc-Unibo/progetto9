/* votazione_client.c
 *
 */

#include <stdio.h>
#include <rpc/rpc.h>
#include "votazione.h"

#define NUMGIUDICI 5

int main (int argc, char *argv[]){
	char *host, ch;
	CLIENT *cl;
	int *ris; //esito dell'operazione (sia per visualizza_classifica_1, che per esprimi_voto_2)
	void * v; //puntatore a void, utile per invocare la procedura visualizza classifica
	Output *classifica; //classifica dei giudici
	Input input; //input (nome partecipante e voti corrispondenti)
	int  i;
	char azione[2]; //scelta dell'azione

	if (argc != 2){
		printf("usage: %s server_host\n", argv[0]);
		exit(1);
	}

	host = argv[1];
	cl = clnt_create(host, VOTAZIONE, VOTAZIONEVERS, "udp");
	
	if (cl == NULL){
		clnt_pcreateerror(host);
		exit(1);
	}

	input.nomeCandidato = (char*) malloc(128);

	printf("Inserire:\nC) per visualizzare la classifica dei giudici\nV) per esprimere un voto\n^D per terminare: ");

	while (gets(azione)){

		//V: esprimiVoto
		if (strcmp(azione, "V") == 0){
			printf("Inserisci il nome del partecipante \n");
<<<<<<< HEAD
			scanf("%s", input.nomeCandidato);
			getchar();
			// Verifico il tipo di azione, e se non valida la richiedo
			//while (ch != 'A' && ch != 'S') {
				printf("Aggiungi voto (A), sottrai voto (S):\n");
			//	ch = getchar();
				input.tipoOp = getchar();
			/*	if(ch == 'A')
=======
			gets(&(input.nomeCandidato));
			// Verifico il tipo di azione, e se non valida la richiedo
			while (ch != 'A' && ch != 'S') {
				printf("Aggiungi voto (A), sottrai voto (S):\n");
				ch = getchar();
				getchar();
				if(ch == 'A')
>>>>>>> b0bcae2c2dd154c7e456551c669be8e150980531
					input.tipoOp = 'A';
				else if(ch == 'S')
					input.tipoOp = 'S';
				else
					printf("Voto non valido, riprova!\n");
			}
*/


			ris = esprimi_voto_1(&input, cl);
			
			if (ris == NULL) {
				clnt_perror(cl, host);
				exit(1);
			}

			if (*ris < 0){
				printf("Errore nell'attribuzione del voto: partecipante non trovato\n");
			} else printf("Votazione effettuata con successo\n");
		} // if V

		//visualizza_classifica
		else if (strcmp(azione, "C") == 0 ){
			
			classifica = classifica_giudici_1(v, cl);
			
			//controllo che la classifica restituita non sia nulla
			if (classifica == NULL) {
				clnt_perror(cl, host);
				exit(1);
			}

			printf("Classifica giudici:\n");
			
			for (i = 0; i < NUMGIUDICI; i++){
				if(classifica -> giudici[i].punteggioTot > 0)
					printf("%s con %d voti in totale\n", classifica -> giudici[i].nome, classifica -> giudici[i].punteggioTot);
			}
		} // if C
		
		else 
			printf("Valore inserito non valido\n");
		
		printf("Inserire:\nC) per visualizzare la classifica dei giudici\nV) per esprimere un voto\n^D per terminare: ");
	} // while

	// Libero le risorse, distruggendo il gestore di trasporto
	clnt_destroy(cl);
	exit(0);
}
