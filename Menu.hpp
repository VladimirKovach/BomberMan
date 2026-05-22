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

    // LEADERBOARD
    // todo Implementare la Classifica
    // Viene chiamata quando l'utente sceglie LEADERBOARD
    // adesso mostra un messaggio di "non disponibile", aspetta un tasto qualsiasi e ritorna.
    void show_leaderboard_stub();

public:
    Menu();

    // Mostra il menu e gestisce l'input finche' l'utente non sceglie una voce.
    // Ritorna NEW_GAME o QUIT, o todo LEADERBOARD (per adesso no)
    MenuChoice show();
};

#endif