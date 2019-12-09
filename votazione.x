/* 
 * votazione.x
 *  +definizione Struttura Giudice, utile per la classifica.
 *  +definizione Input e Output (vedi slide 5 proposta).
 *  +definizione metodi e tipi richiesti/restituiti.
 */

const NUMGIUDICI=5;

struct Input{
    string nomeCandidato<128>;
    char tipoOp;
};

struct Giudice{
    string nome <128>;
    int punteggioTot;
};

struct Persona {
	string candidato <128>;
	char giudice;
	char categoria;
	string nomeFile <128>;
	char fase;
	int voto;
};

struct Tabella{
	Persona persona[10];
};

struct Output {
    Giudice giudici [NUMGIUDICI];
};


program VOTAZIONE {
    version VOTAZIONEVERS{
        Output CLASSIFICA_GIUDICI(void) = 1;                  
        int  ESPRIMI_VOTO(Input) = 2;
    } = 1;
} = 0x20000013;
