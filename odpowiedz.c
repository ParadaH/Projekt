#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "odpowiedz.h"
#include "logika.h"
#include "mapa.h"

static size_t zapisz_odpowiedz(void *data, size_t size, size_t nmemb, void *userp)
{
    /* to jest rzeczywista liczba bajtów przekazanych przez curl */
    size_t realsize = size * nmemb;

    /* jawnie konwertujemy (void*) na naszą strukturę, bo wiemy, że będziemy ją tutaj otrzymywać */
    Memory *mem = (Memory*) userp;

    char *ptr = NULL;

    /* Sprawdzamy czy pierwszy raz wywołujemy funkcję i trzeba zaalokować pamięć po raz pierwszy,
    czy trzeba zrobić reallokację (która kopiuje automatycznie starą zawartość w nowe miejsce) */
    if (mem->odpowiedz != NULL) {
        ptr = realloc(mem->odpowiedz, mem->rozmiar + realsize + 1);
    }
    else {
        ptr = malloc(mem->rozmiar + realsize + 1);        
    }

    if (ptr == NULL) {
        return 0;
    }
    /* brak pamięci! */

    /* teraz zapamiętujemy nowy wskaźnik i doklejamy na końcu dane przekazane przez curl w 
    obszarze pamięci wskazywanym przez data */
    mem->odpowiedz = ptr;
    memcpy(&(mem->odpowiedz[mem->rozmiar]), data, realsize);
    mem->rozmiar += realsize;
    mem->odpowiedz[mem->rozmiar] = 0; // to na końcu dodajemy bo to w końcu string, i zawsze powinien się skończyć!

    return realsize;
}
char *wyslij_zapytanie(char *url)
{
    CURL *curl;
    CURLcode res;
    Memory chunk;
    chunk.rozmiar = 0;
    chunk.odpowiedz = NULL;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

        /* to jest funkcja 'callback', która będzie wywołana przez curl gdy odczyta on kawałek danych,
       ta funkcja musi mieć wywołanie zgodne z wymaganiami, które możesz sprawdzić tutaj:
       https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, zapisz_odpowiedz);

        /* to jest adress struktury, który będzie przekazywany do naszej funkcji 'callback',
       do tej struktury nasz funkcja 'callback' będzie dopisywać wynik */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

        /* Wykonaj zapytanie 'synchronicznie', to znaczy następna linijka kodu nie wykona się
       dopóki nie nadejdzie odpowiedź z serwera. */
        res = curl_easy_perform(curl);

        /* Sprawdzamy czy wystapił jakis błąd? */
        if (res != CURLE_OK) {
            fprintf(stderr, "Błąd! curl_easy_perform() niepowodzenie: %s\n", curl_easy_strerror(res));
        }
        else {
            FILE *fin = fopen("reply.cjson", "w+");
            // printf("%s", chunk.response); /*informacja zwrotna z serwera*/
            fprintf(fin, "%s", chunk.odpowiedz);
            fclose(fin);
        }

        /* zawsze po sobie sprzątaj */
        free(chunk.odpowiedz);
        curl_easy_cleanup(curl);
    }
    return 0;
}

void info(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/";
    strcat(url, token);
    wyslij_zapytanie(url);
}
void ruch_do_przodu(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move/";
    strcat(url, token);
    wyslij_zapytanie(url);
}
void eksploruj(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore/";
    strcat(url, token);
    wyslij_zapytanie(url);
}
void lewo(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/";
    strcat(url, token);
    const char *left = "/left";
    strcat(url, left);
    wyslij_zapytanie(url);
}
void prawo(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/";
    strcat(url, token);
    const char *right = "/right";
    strcat(url, right);
    wyslij_zapytanie(url);
}
void reset(const char *token){
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/reset/";
    strcat(url, token);
    wyslij_zapytanie(url);
}

