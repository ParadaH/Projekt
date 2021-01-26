#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cJSON/cJSON.h"
#include "mapa.h"
#include "odpowiedz.h"
#include "bot.h"
#include "logika.h"
#define P 5

int main(int argc, char **argv)
{
    char *token = argv[1];
    Mapa *informacje;
    Mapa *mapa;
    Mapa *mapa_bota;
    char tab[2048];
    int offset_x, offset_y;

    reset(token);
    FILE *f = fopen("reply.cjson", "r");
    fread(tab,1,2048,f);
    fclose(f);
    informacje = parametry_ruchu(tab);
    mapa = buduj_mape(P, P);
    offset_x = offsetx(informacje);
    offset_y = offsety(informacje);
    wypisz_ruch(informacje);
    mapa->dynamicznamapa[mapa->dx][mapa->dy] = tlumacz_podloze(informacje->pole->field_type);

    mapa_bota = bot(mapa, informacje, token, offset_x, offset_y);
    wypisz_mape(mapa_bota);
    zapisz_mape(mapa_bota);
    printf("Zapisałem!\n");
    zapisz_mape(mapa_bota);

    return 0;
}