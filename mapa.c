#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapa.h"

#define P 5
#define Q 2

void zapisz_mape(Mapa *mapa)
{
    FILE *f = fopen("zapismapy.txt", "w+");
    for(int i = 0; i < mapa->wiersze; i++)
    {
        for(int j = 0; j < mapa->kolumny; j++)
        {
            fprintf(f, "| %d ", mapa->dynamicznamapa[i][j]);
        }
        fprintf(f, "|\n");
    }
    printf("\n");
    fclose(f);
}
void wypisz_mape(Mapa *mapa)
{
    for(int i = 0; i < mapa->wiersze; i++)
    {
        for(int j = 0; j < mapa->kolumny; j++)
        {
            printf(" %d", mapa->dynamicznamapa[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int tlumacz_podloze(char *podloze)
{
    if(strcmp(podloze, "\"grass\"") == 0)
    {
        return 1;
    }
    if(strcmp(podloze, "\"sand\"") == 0)
    {
        return 2;
    }
    if(strcmp(podloze, "\"wall\"") == 0)
    {
        return 3;
    }
}

void wypisz_ruch(Mapa *mapa)
{
    /*Niektóre parametry zakomentowane z uwagi na ich małe znaczenie*/
    printf("Status: %s\n", mapa->pole->status);
    // printf("Nazwa swiata: %s\n", mapa->pole->name);
    printf("X: %d\n", mapa->pole->current_x);
    printf("Y: %d\n", mapa->pole->current_y);
    // printf("Sesja: %s\n", mapa->pole->current_session);
    printf("Kierunek: %s\n", mapa->pole->direction);
    printf("Krok: %d\n", mapa->pole->step);
    printf("Typ pola: %s\n", mapa->pole->field_type);
    // printf("Bonus: %s\n", mapa->pole->field_bonus);
    printf("\n");
}
void wypisz_eksplorowanie(Mapa *mapa)
{
    printf("Status: %s\n", mapa->pole->status);
    printf("X1: %d\n", mapa->pole1->x);
    printf("Y1: %d\n", mapa->pole1->y);
    printf("Typ pola 1: %s\n", mapa->pole1->field_type);
    printf("X2: %d\n", mapa->pole2->x);
    printf("Y2: %d\n", mapa->pole2->y);
    printf("Typ pola 2: %s\n", mapa->pole2->field_type);
    printf("X3: %d\n", mapa->pole3->x);
    printf("Y3: %d\n", mapa->pole3->y);
    printf("Typ pola 3: %s\n", mapa->pole3->field_type);
    printf("\n");
}

Mapa *buduj_mape(int nowe_wiersze, int nowe_kolumny)
{
    Mapa *nowamapa;
    nowamapa = calloc(1, sizeof(Mapa));
    nowamapa->wiersze = nowe_wiersze;
    nowamapa->kolumny = nowe_kolumny;
    
    nowamapa->dynamicznamapa = (int**) calloc(nowamapa->wiersze, sizeof(int*));
    for(int i = 0; i < nowamapa->wiersze; i++)
    {
        nowamapa->dynamicznamapa[i] = (int*) calloc(nowamapa->kolumny, sizeof(int));
    }
    nowamapa->dx = Q;
    nowamapa->dy = Q;

    return nowamapa;
}
Mapa *granica(Mapa *mapa, Mapa *informacje, int offset_x, int offset_y)
{

    Mapa *nowamapa = NULL;
    int x, y;
    x = informacje->pole->current_x+offset_x;
    y = informacje->pole->current_y+offset_y;

    if(y == mapa->kolumny-1)
    {
        nowamapa = puzzle_gora(mapa);
        return nowamapa;
    }
    if(x == mapa->wiersze-1)
    {
        nowamapa = puzzle_prawo(mapa);
        return nowamapa;
    }
    if(y == mapa->kolumny-1)
    {
        nowamapa = puzzle_dol(mapa);
        return nowamapa;
    }
    if(x == mapa->wiersze-1)
    {
        nowamapa = puzzle_lewo(mapa);
        return nowamapa;
    }
    return mapa;
}
Mapa *puzzle_prawo(Mapa *mapa)
{
    int i, j;
    Mapa *nowamapa = NULL;
    nowamapa = buduj_mape(mapa->wiersze*2, mapa->kolumny);

    for(i = 0; i < mapa->wiersze; i++)
    {
        for(j = 0; j < mapa->kolumny; j++)
        {
            nowamapa->dynamicznamapa[i][j] = mapa->dynamicznamapa[i][j];
        }
    }
    zwolnij_mape(mapa);
    return nowamapa;
}
Mapa *puzzle_lewo(Mapa *mapa)
{
    int i, j;
    Mapa *nowamapa = NULL;
    nowamapa = buduj_mape(mapa->wiersze*2, mapa->kolumny);

    for(i = 0; i < mapa->wiersze; i++)
    {
        for(j = 0; j < mapa->kolumny; j++)
        {
            nowamapa->dynamicznamapa[i+mapa->wiersze][j] = mapa->dynamicznamapa[i][j];
        }
    }
    zwolnij_mape(mapa);
    return nowamapa;
}
Mapa *puzzle_gora(Mapa *mapa)
{
    int i, j;
    Mapa *nowamapa = NULL;
    nowamapa = buduj_mape(mapa->wiersze, mapa->kolumny*2);

    for(i = 0; i < mapa->wiersze; i++)
    {
        for(j = 0; j < mapa->kolumny; j++)
        {
            nowamapa->dynamicznamapa[i][j] = mapa->dynamicznamapa[i][j];
        }
    }
    zwolnij_mape(mapa);
    return nowamapa;
}
Mapa *puzzle_dol(Mapa *mapa)
{
    int i, j;
    Mapa *nowamapa = NULL;
    nowamapa = buduj_mape(mapa->wiersze, mapa->kolumny*2);
    for(i = 0; i < mapa->wiersze; i++)
    {
        for(j = 0; j < mapa->kolumny; j++)
        {
            nowamapa->dynamicznamapa[i][j+mapa->kolumny] = mapa->dynamicznamapa[i][j];
        }
    }
    zwolnij_mape(mapa);
    return nowamapa;
}

void zwolnij_mape(Mapa *mapa)
{
    for(int i = 0; i < mapa->wiersze; i++)
    {
        free(mapa->dynamicznamapa[i]);
    }
    free(mapa->dynamicznamapa);
    free(mapa);
}
