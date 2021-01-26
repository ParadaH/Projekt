#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bot.h"
#include "logika.h"
#include "mapa.h"
#include "odpowiedz.h"

Mapa *szukaj_sciany(Mapa *mapa, Mapa *informacje, char *token, int offset_x, int offset_y)
{
    FILE *f;
    Mapa *odkryj;
    char tab[2048];
    int x_stop, y_stop;
    x_stop = informacje->pole->current_x;
    y_stop = informacje->pole->current_y;

    printf("\nx: %d  y: %d kierunek: %s\n\n", informacje->pole->current_x, informacje->pole->current_y, informacje->pole->direction);

    eksploruj(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    odkryj = odkrywanie(tab);
    mapa->dynamicznamapa[odkryj->pole1->x+offset_x][odkryj->pole1->y+offset_y] = tlumacz_podloze(odkryj->pole1->field_type);
    mapa->dynamicznamapa[odkryj->pole2->x+offset_x][odkryj->pole2->y+offset_y] = tlumacz_podloze(odkryj->pole2->field_type);
    mapa->dynamicznamapa[odkryj->pole3->x+offset_x][odkryj->pole3->y+offset_y] = tlumacz_podloze(odkryj->pole3->field_type);
    // free(eksploruj);

    ruch_do_przodu(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    informacje = parametry_ruchu(tab);
    mapa->dynamicznamapa[informacje->pole->current_x+offset_x][informacje->pole->current_y+offset_y] = tlumacz_podloze(informacje->pole->field_type);
    wypisz_mape(mapa);
    // free(informacje);

    mapa = granica(mapa, informacje, offset_x, offset_y);
    // zwolnij_mape(mapa);

    if(x_stop != informacje->pole->current_x || y_stop != informacje->pole->current_y)
    {
        mapa = szukaj_sciany(mapa, informacje, token, offset_x, offset_y);
        // zwolnij_mape(mapa);
    }
    // zwolnij_mape(mapa);
    return mapa;
}

Mapa *lewo_ruch(Mapa *mapa, Mapa *informacje, char *token, int offset_x, int offset_y, int pocz_x, int pocz_y)
{
    FILE *f;
    Mapa *eksplorowanie;
    char tab[2048];
    int x_stop, y_stop;
    x_stop = informacje->pole->current_x+offset_x;
    y_stop = informacje->pole->current_y+offset_y; 

    printf("\nx: %d  y: %d kierunek: %s\n\n", informacje->pole->current_x, informacje->pole->current_y, informacje->pole->direction);

    lewo(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    informacje = parametry_ruchu(tab);
    mapa->dynamicznamapa[informacje->pole->current_x+offset_x][informacje->pole->current_y+offset_y] = tlumacz_podloze(informacje->pole->field_type);
    // free(informacje);

    ruch_do_przodu(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    informacje = parametry_ruchu(tab);
    mapa->dynamicznamapa[informacje->pole->current_x+offset_x][informacje->pole->current_y+offset_y] = tlumacz_podloze(informacje->pole->field_type);
    wypisz_mape(mapa);

    // free(informacje);

    mapa = granica(mapa, informacje, offset_x, offset_y);
    // zwolnij_mape(mapa);

    eksploruj(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    eksplorowanie = odkrywanie(tab);
    mapa->dynamicznamapa[eksplorowanie->pole1->x+offset_x][eksplorowanie->pole1->y+offset_y] = tlumacz_podloze(eksplorowanie->pole1->field_type);
    mapa->dynamicznamapa[eksplorowanie->pole2->x+offset_x][eksplorowanie->pole2->y+offset_y] = tlumacz_podloze(eksplorowanie->pole2->field_type);
    mapa->dynamicznamapa[eksplorowanie->pole3->x+offset_x][eksplorowanie->pole3->y+offset_y] = tlumacz_podloze(eksplorowanie->pole3->field_type);
    // free(eksplorowanie);

    if(x_stop == informacje->pole->current_x+offset_x && y_stop == informacje->pole->current_y+offset_y)
    {
        mapa = prawo_ruch(mapa, informacje, token, offset_x, offset_y, pocz_x, pocz_y);
        // zwolnij_mape(mapa);
        mapa = granica(mapa, informacje, offset_x, offset_y);
        return mapa;
    }
    else
    {
        mapa = lewo_ruch(mapa, informacje, token, offset_x, offset_y, pocz_x, pocz_y);
        // zwolnij_mape(mapa);
        mapa = granica(mapa, informacje, offset_x, offset_y);
        return mapa;
    }
    // zwolnij_mape(mapa);
    return mapa;
}

Mapa *prawo_ruch(Mapa *mapa, Mapa *informacje, char *token, int offset_x, int offset_y, int pocz_x, int pocz_y)
{
    FILE *f;
    Mapa *eksplorowanie;
    char tab[2048];
    int x_stop, y_stop;
    x_stop = informacje->pole->current_x+offset_x;
    y_stop = informacje->pole->current_y+offset_y; 

    printf("\nx: %d  y: %d kierunek: %s\n\n", informacje->pole->current_x, informacje->pole->current_y, informacje->pole->direction);

    prawo(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    informacje = parametry_ruchu(tab);
    mapa->dynamicznamapa[informacje->pole->current_x+offset_x][informacje->pole->current_y+offset_y] = tlumacz_podloze(informacje->pole->field_type);
    // free(informacje);

    ruch_do_przodu(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    informacje = parametry_ruchu(tab);
    mapa->dynamicznamapa[informacje->pole->current_x+offset_x][informacje->pole->current_y+offset_y] = tlumacz_podloze(informacje->pole->field_type);
    wypisz_mape(mapa);
    // free(informacje);

    mapa = granica(mapa, informacje, offset_x, offset_y);
    // zwolnij_mape(mapa);

    eksploruj(token);
    f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    eksplorowanie = odkrywanie(tab);
    mapa->dynamicznamapa[eksplorowanie->pole1->x+offset_x][eksplorowanie->pole1->y+offset_y] = tlumacz_podloze(eksplorowanie->pole1->field_type);
    mapa->dynamicznamapa[eksplorowanie->pole2->x+offset_x][eksplorowanie->pole2->y+offset_y] = tlumacz_podloze(eksplorowanie->pole2->field_type);
    mapa->dynamicznamapa[eksplorowanie->pole3->x+offset_x][eksplorowanie->pole3->y+offset_y] = tlumacz_podloze(eksplorowanie->pole3->field_type);
    // free(eksplorowanie);

    if(pocz_x == informacje->pole->current_x+offset_x && pocz_y == informacje->pole->current_y+offset_y)   
    {
        // zwolnij_mape(mapa);
        return mapa;
    }
    if(x_stop == informacje->pole->current_x+offset_x && y_stop == informacje->pole->current_y+offset_y)
    {
        mapa = prawo_ruch(mapa, informacje, token, offset_x, offset_y, pocz_x, pocz_y);
        mapa = granica(mapa, informacje, offset_x, offset_y);
        // zwolnij_mape(mapa);
        return mapa;
    }
    else
    {
        mapa = lewo_ruch(mapa, informacje, token, offset_x, offset_y, pocz_x, pocz_y);
        mapa = granica(mapa, informacje, offset_x, offset_y);
        // zwolnij_mape(mapa);
        return mapa;
    }
    return mapa;
}

Mapa *bot(Mapa *mapa, Mapa *informacje, char *token, int offset_x, int offset_y)
{
    FILE *f;
    char tab[2048];
    int pocz_x, pocz_y;
    pocz_x = informacje->pole->current_x+offset_x;
    pocz_y = informacje->pole->current_y+offset_y;

    mapa = szukaj_sciany(mapa, informacje, token, offset_x, offset_y);
    // zwolnij_mape(mapa);
    mapa = prawo_ruch(mapa, informacje, token, offset_x, offset_y, pocz_x, pocz_y);
    // zwolnij_mape(mapa);
    return mapa;
    
}