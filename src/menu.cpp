#include "menu.hpp"
#include "leaderboard.hpp"
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


// Numero di cifre decimali di un intero (0 -> 1).
// Serve solo a calcolare la larghezza del testo da centrare a schermo:
// la stampa vera e propria la fa mvprintw con "%d".
// Sostituisce snprintf, che appartiene a <cstdio> (libreria non ammessa).
static int digit_count(int n) {
    if (n < 0) {
        n = -n;
    }

    int digits = 1;

    while (n >= 10) {
        n /= 10;
        digits++;
    }

    return digits;
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


void Menu::read_string(int y, int x, char* buffer, int max_len) {
    noecho();
    curs_set(1);

    int pos = 0;
    bool done = false;

    while (!done) {
        move(y, x + pos);
        refresh();
        int key = getch();

        switch (key) {
            case '\n':
            case KEY_ENTER:
                done = true;
                break;

            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if (pos > 0) {
                    pos--;
                    mvaddch(y, x + pos, ' ');
                    move(y, x + pos);
                }
                break;

            default:
                // Accetta caratteri stampabili (incluso lo spazio).
                // L'unico vietato e' ';' (separatore nel file dei punteggi).
                if (key >= ' ' && key < 127 && key != ';' && pos < max_len - 1) {
                    buffer[pos] = (char) key;
                    // disegna il carattere a schermo manualmente (echo è disattivato)
                    mvaddch(y, x + pos, key);
                    pos++;
                }
                break;
        }
    }

    buffer[pos] = '\0';

    noecho();
    curs_set(0);
}


void Menu::show_leaderboard() {
    // Bloccante per la durata della schermata classifica.
    // Senza questo, getch() ritorna ERR immediatamente perche' show()
    // in uscita ha rimesso nodelay TRUE (pensando di tornare al game loop),
    // ma siamo invece in un'altra schermata di menu.
    // flushinp() scarta eventuali tasti pendenti dall'input precedente.
    nodelay(stdscr, FALSE);
    flushinp();
    clear();

    const char* title = "CLASSIFICA";
    int title_len = 0;
    while (title[title_len] != '\0') title_len++;
    mvprintw(2, (COLS - title_len) / 2, "%s", title);

    const char* prompt = "Quanti top? ";
    int prompt_len = 0;
    while (prompt[prompt_len] != '\0') prompt_len++;
    int prompt_x = (COLS - prompt_len - 4) / 2;
    mvprintw(4, prompt_x, "%s", prompt);

    char n_str[6];
    read_string(4, prompt_x + prompt_len, n_str, 6);

    int n = 0;
    for (int i = 0; n_str[i] != '\0'; i++) {
        if (n_str[i] >= '0' && n_str[i] <= '9') {
            n = n * 10 + (n_str[i] - '0');
        } else {
            n = 0;
            break;
        }
    }
    if (n <= 0) n = 10;

    ScoreEntry* head = Leaderboard::load();

    clear();
    mvprintw(1, (COLS - title_len) / 2, "%s", title);

    if (head == nullptr) {
        const char* empty = "Nessun punteggio salvato.";
        int empty_len = 0;
        while (empty[empty_len] != '\0') empty_len++;
        mvprintw(LINES / 2, (COLS - empty_len) / 2, "%s", empty);
    }
    else {
        ScoreEntry* curr = head;
        int rank = 1;
        int row = 4;
        int col = (COLS - 30) / 2;

        while (curr != nullptr && rank <= n) {
            mvprintw(row, col, "%2d. %-15s %6d", rank, curr->name, curr->score);
            curr = curr->next;
            rank++;
            row++;
        }
    }

    Leaderboard::free_list(head);

    const char* back = "Premi un tasto per tornare al menu...";
    int back_len = 0;
    while (back[back_len] != '\0') back_len++;
    mvprintw(LINES - 2, (COLS - back_len) / 2, "%s", back);

    refresh();
    getch();
    nodelay(stdscr, TRUE);
}


void Menu::prompt_save_score(int score) {
    nodelay(stdscr, FALSE);
    clear();

    const char* title = "PARTITA TERMINATA";
    int title_len = 0;
    while (title[title_len] != '\0') title_len++;
    mvprintw(LINES / 2 - 4, (COLS - title_len) / 2, "%s", title);

    // Lunghezza della riga "Punteggio: <n>" calcolata a mano, per centrarla.
    // Non serve un buffer: mvprintw formatta direttamente sullo schermo.
    const char* score_label = "Punteggio: ";
    int score_len = 0;
    while (score_label[score_len] != '\0') score_len++;
    score_len += digit_count(score);
    if (score < 0) score_len++;  // spazio per il segno meno

    mvprintw(LINES / 2 - 2, (COLS - score_len) / 2, "%s%d", score_label, score);

    const char* prompt = "Inserisci il tuo nome: ";
    int prompt_len = 0;
    while (prompt[prompt_len] != '\0') prompt_len++;
    int prompt_x = (COLS - prompt_len - MAX_NAME_LEN) / 2;
    mvprintw(LINES / 2, prompt_x, "%s", prompt);

    char name[MAX_NAME_LEN];
    read_string(LINES / 2, prompt_x + prompt_len, name, MAX_NAME_LEN);

    if (name[0] == '\0') {
        name[0] = 'A'; name[1] = 'n'; name[2] = 'o'; name[3] = 'n';
        name[4] = 'i'; name[5] = 'm'; name[6] = 'o'; name[7] = '\0';
    }

    Leaderboard::save(name, score);
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

            case '\n':
            case KEY_ENTER:
            case ' ':
                // Qualunque voce sia selezionata, la ritorniamo al main.
                // Il main si occupera' di gestirla.
                result = (MenuChoice) selected;
                chosen = true;
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