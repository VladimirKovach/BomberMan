#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

// Lunghezza massima del nome del giocatore.
// 16 = 15 caratteri di nome + 1 per il terminatore '\0'.
const int MAX_NAME_LEN = 16;

// Nome del file dei punteggi.
// Path relativo: il file viene creato nella directory da cui si lancia l'eseguibile.
// '/' è un separatore (compatibile sin in Windows che in Linux).
const char LEADERBOARD_FILE[] = "leaderboard.txt";

// Singola voce della classifica.
// è anche un nodo di lista concatenata, per gestire la classifica come struttura dinamica
struct ScoreEntry {
    char name[MAX_NAME_LEN];
    int score;
    ScoreEntry* next;
};

// Gestisce salvatura e il caricamento dei punteggi.
// è un raggruppamento logico di funzioni che lavorano sul file di punteggi, sono tutti public.
class Leaderboard {
public:
    // Appende nome;punteggio in fondo al file.
    // Il file resta NON ordinato; l'ordinamento avviene a lettura (la funzione 'load').
    // Usiamo ';' come il separatore
    static void save(const char* name, int score);

    // Legge tutti i punteggi dal file e ritorna una lista concatenata ORDINATA in modo decrescente per punteggio.
    // Se il file non esiste o è vuoto, ritorna nullptr.
    // ATTENZIONE: il chiamante deve liberare la memoria chiamando free_list() quando ha finito di usare la lista.
    static ScoreEntry* load();

    // Libera la memoria allocata da load(), serve per prevenire la memory leak.
    static void free_list(ScoreEntry* head);
};

#endif