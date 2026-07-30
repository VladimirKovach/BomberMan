#ifndef MENU_HPP
#define MENU_HPP

// Le scelte che il menu puo' ritornare al main.
// Ogni variante del menu corrisponde a un valore di questo enum.
enum MenuChoice {
    NEW_GAME,
    LEADERBOARD,
    QUIT
};

const int MENU_ITEM_COUNT = 3;

class Menu {
protected:
    // Indice della voce attualmente selezionata
    int selected;

    // Etichette mostrate a schermo, una per ogni voce
    const char* items[MENU_ITEM_COUNT];

    // Disegna il menu completo: cornice, titolo, voci
    // Viene richiamato ogni volta che l'utente preme un tasto.
    void draw();

    // Helper privato: legge una stringa da tastiera in modo "bloccante".
    // Scrive il risultato in `buffer` (terminato da '\0'). max_len include
    // lo spazio per il terminatore (es. max_len=16 -> 15 caratteri utili).
    void read_string(int y, int x, char* buffer, int max_len);
public:
    Menu();

    // Mostra il menu e gestisce l'input finche' l'utente non sceglie una voce.
    // Ritorna NEW_GAME o QUIT, o LEADERBOARD
    MenuChoice show();

    // Schermata di fine partita: chiede il nome e salva il punteggio.
    // Viene chiamata dal MAIN dopo game.run().
    void prompt_save_score(int score);

    // LEADERBOARD
    // chiede all'utente: il numero N di top punteggi
    // legge il file tramite Leaderboard::load(),
    // visualizza i primi N e attende un tasto per tornare al menu.
    void show_leaderboard();
};

#endif