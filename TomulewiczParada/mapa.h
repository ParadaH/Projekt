#ifndef mapa_h_
#define mapa_h_

typedef struct _Luneta //dla odkrywania (explore)
{
    int x;
    int y;
    char *field_type;
}Luneta;

typedef struct _Parametry //dla ruchu (ruch_do_przodu, lewo, prawo, info, reset)
{
    char *status;
    char *name;
    int current_x;
    int current_y;
    char *current_session;
    char *direction;
    int step;
    char *field_type;
    char *field_bonus;
}Parametry;

typedef struct _Mapa 
{
    int **dynamicznamapa;
    int dx;//przesunięcie X
    int dy;//przesunięcie Y
    int wiersze;//aktualny rozmiar mapy w osi OX
    int kolumny;//aktualny rozmiar mapy w osi OY

    Parametry *pole;
    Luneta *pole1;
    Luneta *pole2;
    Luneta *pole3;

}Mapa;

void zapisz_mape(Mapa* mapa);
void wypisz_mape(Mapa *mapa);

int tlumacz_podloze(char *podloze);

void wypisz_ruch(Mapa *mapa);
void wypisz_eksplorowanie(Mapa *mapa);

Mapa *buduj_mape(int nowe_wiersze, int nowe_kolumny);
Mapa *granica(Mapa *mapa, Mapa *info, int offset_x, int offset_y);
Mapa *puzzle_prawo(Mapa *mapa);
Mapa *puzzle_lewo(Mapa *mapa);
Mapa *puzzle_gora(Mapa *mapa);
Mapa *puzzle_dol(Mapa *mapa);

void zwolnij_mape(Mapa *mapa);

#endif