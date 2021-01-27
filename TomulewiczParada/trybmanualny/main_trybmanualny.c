#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../cJSON/cJSON.h"
#include "../mapa.h"
#include "../odpowiedz.h"
#include "../bot.h"
#include "../logika.h"
#define P 5

int main(int argc, char **argv)
{
    char *token = argv[1];
    Mapa *informacje;
    Mapa *mapa;
    Mapa *mapa_bota;
    char *kierunek;
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
    mapa->dynamicznamapa[mapa->dx][mapa->dy] = tlumacz_podloze(informacje->pole->field_type);
    for(int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "eksploruj") == 0)
        {
            eksploruj(token);
    	    FILE *f = fopen("reply.cjson", "r");
    	    fread(tab,1,2048,f);
    	    fclose(f);
            informacje = odkrywanie(tab); 
            wypisz_eksplorowanie(informacje);

            mapa->dynamicznamapa[informacje->pole1->x+offset_x][informacje->pole1->y+offset_y] = tlumacz_podloze(informacje->pole1->field_type);
            mapa->dynamicznamapa[informacje->pole2->x+offset_x][informacje->pole2->y+offset_y] = tlumacz_podloze(informacje->pole2->field_type);
            mapa->dynamicznamapa[informacje->pole3->x+offset_x][informacje->pole3->y+offset_y] = tlumacz_podloze(informacje->pole3->field_type);
            wypisz_eksplorowanie(informacje);
            wypisz_mape(mapa);
            free(informacje);
            free(informacje->pole1);
            free(informacje->pole2);
            free(informacje->pole3);
        }
        else
        {
            ruch(argv[i], token);
    	    FILE *f = fopen("reply.cjson", "r");
    	    fread(tab,1,2048,f);
    	    fclose(f);
            informacje = parametry_ruchu(tab);
            mapa->dynamicznamapa[informacje->pole->current_x+offset_x][informacje->pole->current_y+offset_y] = tlumacz_podloze(informacje->pole->field_type);
            printf("Rozmiar aktualnej mapy:\n%dx%d\n", mapa->wiersze, mapa->kolumny);
            wypisz_ruch(informacje);
            wypisz_mape(mapa);
            mapa = granica(mapa, informacje, offset_x, offset_y);
            free(informacje);
            free(informacje->pole);
        }
    }
    zapisz_mape(mapa);
    
    printf("Ilosc krokow: %d\nZapisalem!\n", informacje->pole->step);
    if(strcmp(informacje->pole->direction, "\"N\"") == 0)
    {
        printf("Pozycja czołgu została skalibrowana!\n");
    }
    return 0;
}