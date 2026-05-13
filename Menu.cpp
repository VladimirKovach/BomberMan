#include "Menu.hpp"
#include <ncurses.h>

// Dimensioni della finestra del menu.
// Tenute piccole perche' il menu ha poche voci e deve stare comoda
// anche in terminali non grandissimi.
const int MENU_WIN_HEIGHT = 11;
const int MENU_WIN_WIDTH = 40;

// Titolo mostrato in cima alla finestra del menu
const char* MENU_TITLE = "SUPER BOMBERMAN";


Menu::Menu() {
    selected = 0;
    items[NEW_GAME]    = "Nuova partita";
    items[LEADERBOARD] = "Classifica";
    items[QUIT]        = "Esci";
}


// Disegna il menu da capo a ogni frame/pressione di tasto.
// influisce poco la performance.
void Menu::draw() {
    // Pulisce tutto lo schermo prima di disegnare.
    clear();

    // Posizione della finestra: centrata nel terminale.
    // LINES e COLS sono variabili globali di ncurses che danno la dimensione dello schermo in righe e colonne.
    int start_y = (LINES - MENU_WIN_HEIGHT) / 2;
    int start_x = (COLS - MENU_WIN_WIDTH) / 2;

    // newwin crea una sotto-finestra ncurses. Ritorna un WINDOW* che useremo
    // per disegnare al suo interno con le funzioni mv*w* (move-window-...).
    WINDOW* win = newwin(MENU_WIN_HEIGHT, MENU_WIN_WIDTH, start_y, start_x);

    // box disegna una cornice attorno alla finestra usando i caratteri di
    // bordo di default (gli zeri dicono "usa i default": ACS_VLINE per i
    // lati verticali, ACS_HLINE per quelli orizzontali).
    box(win, 0, 0);

    // TITOLO
    // mvwprintw scrive a una posizione specifica (y, x) di una finestra.
    int title_len = 0;
    while (MENU_TITLE[title_len] != '\0') title_len++;
    int title_x = (MENU_WIN_WIDTH - title_len) / 2; // centrato orizzontalmente
    mvwprintw(win, 1, title_x, "%s", MENU_TITLE);

    // LINEA ORIZZONTALE di separazione sotto il titolo.
    // mvwhline disegna una linea orizzontale lunga N caratteri.
    // ACS_HLINE - è il carattere di linea orizzontale di ncurses
    mvwhline(win, 2, 1, ACS_HLINE, MENU_WIN_WIDTH - 2);

    // LE VOCI del menu. La voce selezionata viene evidenziata con
    // A_REVERSE, che inverte foreground e background (standard ncurses per
    // "questa e' la voce attiva").
    // start_row_in_win: la prima voce viene disegnata su questa riga.
    // 4 = bordo superiore + titolo + separatore + spazio.
    const int start_row_in_win = 4;

    for (int i = 0; i < MENU_ITEM_COUNT; i++) {
        int row = start_row_in_win + i;

        // Calcolo della x per centrare l'etichetta orizzontalmente.
        // Nota: uso un loop manuale per la lunghezza perche' non possiamo usare strlen
        int len = 0;
        while (items[i][len] != '\0') len++;
        int item_x = (MENU_WIN_WIDTH - len) / 2;

        if (i == selected) {
            wattron(win, A_REVERSE);
            mvwprintw(win, row, item_x, "%s", items[i]);
            wattroff(win, A_REVERSE);
        }
        else {
            mvwprintw(win, row, item_x, "%s", items[i]);
        }
    }

    // refresh() fa apparire le modifiche su stdscr (lo schermo principale).
    // wrefresh(win) fa apparire le modifiche sulla nostra sotto-finestra.
    // prima stdscr (lo "sfondo"), poi la finestra sopra di esso.
    refresh();
    wrefresh(win);

    // delwin libera la memoria allocata da newwin.
    // Lo facciamo qui (non a fine show()) perche' la finestra viene ricreata
    // a ogni chiamata di draw().
    delwin(win);
}


// todo CLASSIFICA
void Menu::show_leaderboard_stub() {
    clear();

    const char* msg1 = "CLASSIFICA";
    const char* msg2 = "Funzione non ancora disponibile";
    const char* msg3 = "Premi un tasto per tornare al menu...";

    // Calcolo lunghezze manualmente per centrare.
    int len1 = 0; while (msg1[len1] != '\0') len1++;
    int len2 = 0; while (msg2[len2] != '\0') len2++;
    int len3 = 0; while (msg3[len3] != '\0') len3++;

    int center_y = LINES / 2;
    mvprintw(center_y - 2, (COLS - len1) / 2, "%s", msg1);
    mvprintw(center_y,     (COLS - len2) / 2, "%s", msg2);
    mvprintw(center_y + 2, (COLS - len3) / 2, "%s", msg3);

    refresh();

    getch();
}


MenuChoice Menu::show() {
    // nodelay: getch() BLOCCHI in attesa di un tasto,
    // Lo ripristiniamo all'uscita - cosi' il main loop del gioco (che usa nodelay true) non viene rotto.
    nodelay(stdscr, FALSE);

    keypad(stdscr, TRUE);
    curs_set(0);

    bool chosen = false;
    MenuChoice result = QUIT;  // valore di default

    while (!chosen) {
        draw();
        int key = getch();

        switch (key) {
            case KEY_UP:
            case 'w':
            case 'W':
                // Dalla prima voce vai all'ultima.
                // Se passiamo da 0 → -1. -1 non è una voce valida del menu, quindi usiamo '%'
                // ma in C++ -1 % 3 non è = 2, è = -1.
                // Aggiungendo N prima del modulo lo fixiamo.
                // è una proprietà matematica: Sommare N al dividendo non cambia il risultato del modulo.
                selected = (selected - 1 + MENU_ITEM_COUNT) % MENU_ITEM_COUNT;
                break;

            case KEY_DOWN:
            case 's':
            case 'S':
                selected = (selected + 1) % MENU_ITEM_COUNT;
                break;

            case '\n':       // Enter normale
            case KEY_ENTER:  // Enter del tastierino numerico
            case ' ':        // spazio
                if (selected == LEADERBOARD) {
                    // todo
                    show_leaderboard_stub();
                }
                else {
                    result = (MenuChoice) selected;
                    chosen = true;
                }
                break;

            case 'q':
            case 'Q':
                // Scorciatoia: Q esce direttamente
                result = QUIT;
                chosen = true;
                break;

            default:
                // Tasto non riconosciuto: ignora, ridisegna e basta.
                break;
        }
    }

    nodelay(stdscr, TRUE);

    return result;
}