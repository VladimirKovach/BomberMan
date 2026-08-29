#include "leaderboard.hpp"
#include <cstdlib>
#include <fstream>

// Formato di ogni riga: "<nome>;<score>\n"
// Usiamo ';' come separatore, cosi' i nomi possono
// contenere spazi normalmente, es. "Mario Rossi". L'unico carattere
// vietato nel nome e' proprio ';' (lo sostituiamo con '.' in scrittura
// per non rompere il parsing in lettura).
//
// Apertura in modalita' ios::app: append, cioè scriviamo in coda senza
// cancellare il contenuto preesistente. Se il file non esiste viene creato.
void Leaderboard::save(const char* name, int score) {
    std::ofstream out;
    out.open(LEADERBOARD_FILE, std::ios::app);

    // Se l'apertura fallisce ignoriamo silenziosamente: il punteggio
    // non viene salvato ma il gioco prosegue.
    if (!out.is_open()) return;

    // Scrive il nome carattere per carattere.
    // Filtra solo i caratteri che romperebbero il file: ';' (separatore)
    // e i terminatori di riga ('\n', '\r'). Tutto il resto (spazi compresi)
    // viene mantenuto cosi' com'e'.
    for (int i = 0; name[i] != '\0' && i < MAX_NAME_LEN - 1; i++) {
        char c = name[i];
        if (c == ';' || c == '\n' || c == '\r') c = '.';
        out << c;
    }

    out << ';' << score << '\n';
    out.close();
}


// Funzione di supporto privata (sta solo in questo .cpp, non nell'header):
// inserisce un nuovo nodo in una lista ORDINATA per punteggio decrescente.
// Ritorna la nuova testa della lista (che puo' cambiare se il nuovo nodo
// va in cima).
//
// Algoritmo: scorri finchè trovi un nodo con score minore del nuovo, e
// inserisci prima di lui. Caso speciale: lista vuota o inserimento in testa.
//
// E' "static" in C++ a file scope per limitarne la visibilita' a questo
// file - cosi' non inquina lo spazio dei nomi globali. (Equivalente a
// metterla in un namespace anonimo.)
static ScoreEntry* insert_sorted(ScoreEntry* head, ScoreEntry* new_node) {
    // Caso 1: lista vuota, oppure il nuovo punteggio supera la testa
    // -> il nuovo nodo diventa la nuova testa.
    if (head == NULL || new_node->score > head->score) {
        new_node->next = head;
        return new_node;
    }

    // Caso 2: cerca il punto di inserimento.
    // 'curr' avanza finche' il prossimo nodo ha ancora score >= new->score.
    // Quando il prossimo ha score minore (o non c'e' piu'), inseriamo qui.
    ScoreEntry* curr = head;
    while (curr->next != NULL && curr->next->score >= new_node->score) {
        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;
    return head;
}


// Carica tutti i punteggi dal file in una lista ordinata decrescente.
//
// Strategia: leggi una riga alla volta (formato "nome;score"), splitta
// sul ';', crea un nodo, inseriscilo in ordine nella lista. A fine
// lettura la lista e' gia' ordinata - non serve un sort separato.

ScoreEntry* Leaderboard::load() {
    std::ifstream in;
    in.open(LEADERBOARD_FILE);

    // Se il file non esiste (mai stata salvata una partita) ritorna lista vuota.
    if (!in.is_open()) return NULL;

    ScoreEntry* head = NULL;

    // Buffer per leggere una riga intera dal file.
    // Una riga ben formata e' al massimo: nome (MAX_NAME_LEN-1) + ';' + numero
    // (10 cifre per int) + '\0' + un po' di margine. 64 e' ampiamente sicuro.
    const int LINE_BUF = 64;
    char line[LINE_BUF];

    // getline(stream, buffer, max_len) legge fino a max_len-1 caratteri
    // o fino al primo '\n', e termina la stringa con '\0'.
    // Ritorna lo stream stesso, che e' "truthy" finche' la lettura va bene.
    while (in.getline(line, LINE_BUF)) {
        // Trova il ';' nella riga: ne facciamo lo split manualmente.
        // sep_pos sara' l'indice del ';', oppure -1 se non c'e'.
        int sep_pos = -1;
        int line_len = 0;
        while (line[line_len] != '\0') {
            if (line[line_len] == ';' && sep_pos == -1) sep_pos = line_len;
            line_len++;
        }

        // Riga malformata (nessun ';' oppure ';' all'inizio = nome vuoto):
        // ignora e passa alla prossima.
        if (sep_pos <= 0) continue;

        // Alloca il nuovo nodo solo dopo aver verificato che la riga
        // e' parsabile - cosi' non rischiamo new/delete inutili.
        ScoreEntry* node = new ScoreEntry;
        node->next = NULL;

        // Copia il nome (dall'inizio della riga fino al ';' escluso) nel
        // buffer del nodo, troncando a MAX_NAME_LEN-1 caratteri per lasciare
        // spazio al terminatore.
        int name_len = sep_pos;
        if (name_len > MAX_NAME_LEN - 1) name_len = MAX_NAME_LEN - 1;
        for (int i = 0; i < name_len; i++) {
            node->name[i] = line[i];
        }
        node->name[name_len] = '\0';

        // Parsa il numero dopo il ';'. Conversione manuale ASCII->int,
        // si ferma al primo carattere non-cifra (o a fine stringa).
        int score = 0;
        for (int i = sep_pos + 1; line[i] != '\0'; i++) {
            if (line[i] >= '0' && line[i] <= '9') {
                score = score * 10 + (line[i] - '0');
            } else {
                break;
            }
        }
        node->score = score;

        head = insert_sorted(head, node);
    }

    in.close();
    return head;
}

// Libera una lista di ScoreEntry nodo per nodo.
void Leaderboard::free_list(ScoreEntry* head) {
    while (head != NULL) {
        ScoreEntry* next = head->next;
        delete head;
        head = next;
    }
}