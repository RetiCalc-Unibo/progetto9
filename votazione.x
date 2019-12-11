/* 
 * votazione.x
 *  +definizione Struttura Giudice, utile per la classifica.
 *  +definizione Input e Output (vedi slide 5 proposta).
 *  +definizione metodi e tipi richiesti/restituiti.
 */

const NUMGIUDICI=5;
const NUMPART=10;
const MAXSTRINGLENGHT=128;

struct Input{
    string nomeCandidato<MAXSTRINGLENGHT>;
    char tipoOp;
};

struct Giudice{
    string nome <MAXSTRINGLENGHT>;
    int punteggioTot;
};

struct Persona {
	string candidato <MAXSTRINGLENGHT>;
	string giudice <MAXSTRINGLENGHT>;
	char categoria;
	string nomeFile <MAXSTRINGLENGHT>;
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
