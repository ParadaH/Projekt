#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../cJSON/cJSON.h"

#define swiat_size_x 25
#define swiat_size_y 25

typedef struct _Memory {
    char *response;
    size_t size;
}Memory;

typedef struct _Mapa {
    char *status;
    char *name;
    int current_x;
    int current_y;
    char *current_session;
    char *direction;
    int step;
    char *field_type;
    char *field_bonus;
    
    int x1;
    int y1;
    char *type1;
    int x2;
    int y2;
    char *type2;
    int x3;
    int y3;
    char *type3;
}Mapa;

char swiat[swiat_size_x][swiat_size_y];

static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
    /* to jest rzeczywista liczba bajtów przekazanych przez curl */
    size_t realsize = size * nmemb;

    /* jawnie konwertujemy (void*) na naszą strukturę, bo wiemy, że będziemy ją tutaj otrzymywać */
    Memory *mem = (Memory*) userp;

    char *ptr = NULL;

    /* Sprawdzamy czy pierwszy raz wywołujemy funkcję i trzeba zaalokować pamięć po raz pierwszy,
    czy trzeba zrobić reallokację (która kopiuje automatycznie starą zawartość w nowe miejsce) */
    if (mem->response != NULL) {
        ptr = realloc(mem->response, mem->size + realsize + 1);
    }
    else {
        ptr = malloc(mem->size + realsize + 1);        
    }

    if (ptr == NULL) {
        return 0;
    }
    /* brak pamięci! */

    /* teraz zapamiętujemy nowy wskaźnik i doklejamy na końcu dane przekazane przez curl w 
    obszarze pamięci wskazywanym przez data */
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0; // to na końcu dodajemy bo to w końcu string, i zawsze powinien się skończyć!

    return realsize;
}

char *make_request(char *url)
{
    CURL *curl;
    CURLcode res;
    Memory chunk;
    chunk.size = 0;
    chunk.response = NULL;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // curl_easy_setopt(curl, CURLOPT_HEADER, 1L);

        /* to jest funkcja 'callback', która będzie wywołana przez curl gdy odczyta on kawałek danych,
       ta funkcja musi mieć wywołanie zgodne z wymaganiami, które możesz sprawdzić tutaj:
       https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

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
            printf("%s", chunk.response); /*informacja zwrotna z serwera*/
            fprintf(fin, "%s", chunk.response);
            fclose(fin);
        }

        /* zawsze po sobie sprzątaj */
        free(chunk.response);
        curl_easy_cleanup(curl);
    }
    return 0;
}

void info(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/";
    strcat(url, token);
    make_request(url);
}

void move(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move/";
    strcat(url, token);
    make_request(url);
}

void explore(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore/";
    strcat(url, token);
    make_request(url);
}

void left(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/";
    strcat(url, token);
    const char *left = "/left";
    strcat(url, left);
    make_request(url);
}

void right(const char *token) {
    char url[100] = "http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/";
    strcat(url, token);
    const char *right = "/right";
    strcat(url, right);
    make_request(url);
}

Mapa *parameters(const char * const korzen)//musi btc status (korzen)
{
    Mapa *zodiak;
    const cJSON *status = NULL;
    const cJSON *payload = NULL;
    const cJSON *name = NULL;
    const cJSON *current_x = NULL;
    const cJSON *current_y = NULL;
    const cJSON *current_session = NULL;
    const cJSON *direction = NULL;
    const cJSON *step = NULL;
    const cJSON *field_type = NULL;
    const cJSON *field_bonus = NULL;
    
    cJSON *korzen_cjson = cJSON_Parse(korzen);
    if (korzen_cjson == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    status = cJSON_GetObjectItemCaseSensitive(korzen_cjson, "status");
    zodiak = malloc(sizeof(Mapa));

    zodiak->status = cJSON_Print(status);
    payload = cJSON_GetObjectItemCaseSensitive(korzen_cjson, "payload");
    zodiak->name = cJSON_Print(payload->child);
    zodiak->current_x = atoi(cJSON_Print(payload->child->next));
    zodiak->current_y = atoi(cJSON_Print(payload->child->next->next));
    zodiak->current_session = cJSON_Print(payload->child->next->next->next);
    zodiak->direction = cJSON_Print(payload->child->next->next->next->next);
    zodiak->step = atoi(cJSON_Print(payload->child->next->next->next->next->next));
    zodiak->field_type = cJSON_Print(payload->child->next->next->next->next->next->next);
    zodiak->field_bonus = cJSON_Print(payload->child->next->next->next->next->next->next->next);

    cJSON_Delete(korzen_cjson);
    return zodiak;
}

void wypisz (Mapa *mapa)
{
    printf("Status: %s\n", mapa->status);
    printf("Nazwa swiata: %s\n", mapa->name);
    printf("X: %d\n", mapa->current_x);
    printf("Y: %d\n", mapa->current_y);
    printf("Sesja: %s\n", mapa->current_session);
    printf("Kierunek: %s\n", mapa->direction);
    printf("Krok: %d\n", mapa->step);
    printf("Typ pola: %s\n", mapa->field_type);
    printf("Bonus: %s\n", mapa->field_bonus);
}

Mapa *odkrywanie(const char * const korzen)
{
    Mapa *zodiak;
    const cJSON *status = NULL;
    const cJSON *payload = NULL;
    const cJSON *name = NULL;
    const cJSON *current_x = NULL;
    const cJSON *current_y = NULL;
    const cJSON *current_session = NULL;
    const cJSON *direction = NULL;
    const cJSON *step = NULL;
    const cJSON *field_type = NULL;
    const cJSON *field_bonus = NULL;

    cJSON *korzen_cjson = cJSON_Parse(korzen);
    if (korzen_cjson == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

    status = cJSON_GetObjectItemCaseSensitive(korzen_cjson, "status");
    zodiak->status = cJSON_Print(status);

    payload = cJSON_GetObjectItemCaseSensitive(korzen_cjson, "payload");
    zodiak->x1 = atoi(cJSON_Print(payload->child->child->child));
    zodiak->y1 = atoi(cJSON_Print(payload->child->child->child->next));
    zodiak->type1 = cJSON_Print(payload->child->child->child->next->next);

    zodiak->x2 = atoi(cJSON_Print(payload->child->child->next->child));
    zodiak->y2 = atoi(cJSON_Print(payload->child->child->next->child->next));
    zodiak->type2 = cJSON_Print(payload->child->child->next->child->next->next);

    zodiak->x3 = atoi(cJSON_Print(payload->child->child->next->next->child));
    zodiak->y3 = atoi(cJSON_Print(payload->child->child->next->next->child->next));
    zodiak->type3 = cJSON_Print(payload->child->child->next->next->child->next->next);

    return zodiak;
}

void wypisz1(Mapa *mapa)
{
    printf("Status: %s\n", mapa->status);
    printf("X1: %d\n", mapa->x1);
    printf("Y1: %d\n", mapa->y1);
    printf("Typ pola 1: %s\n", mapa->type1);
    printf("X2: %d\n", mapa->x2);
    printf("Y2: %d\n", mapa->y2);
    printf("Typ pola 2: %s\n", mapa->type2);
    printf("X3: %d\n", mapa->x3);
    printf("Y3: %d\n", mapa->y3);
    printf("Typ pola 3: %s\n", mapa->type3);
}

void buduj_mape()
{
    for(int i = 0; i < swiat_size_x; i++)
    {
        for(int j = 0; j < swiat_size_y; j++)
        {
            printf("%c", swiat[i][j]);
        }
        printf("\n");
    }
}

void wczytaj_mape()
{
    system("touch swiat.txt");
    FILE *f = fopen("swiat.txt", "r");
    for(int i = 0; i < swiat_size_x; i++)
    {
        for(int j = 0; j < swiat_size_y; j++)
        {
            fscanf(f, "%c", &swiat[i][j]);
        }
    }
    fclose(f);
    system("rm swiat.txt");
}

void zapisz_mape()
{
    FILE *f = fopen("swiat.txt", "w");
    for(int i = 0; i < swiat_size_x; i++)
    {
        for(int j = 0; j <swiat_size_y; j++)
        {
            fprintf(f, "%c", swiat[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void wyzeruj_mape()
{
    for(int i = 0; i < swiat_size_x; i++)
    {
        for(int j = 0; j < swiat_size_y; j++)
        {
            swiat[i][j] = '0';
        }
    }
}






int main(int argc, char **argv)
{
    char *token= argv[1];
    Mapa *komorka_mapy;

    wyzeruj_mape();

for(int i = 2; i <argc; i++)
{
    if (strcmp(argv[i], "explore") == 0)
    {
        explore(token);
        char tab[2048];
	    FILE *f = fopen("reply.cjson", "r");
	    fread(tab,1,2048,f);
	    fclose(f);
        komorka_mapy = odkrywanie(tab);
        wypisz1(komorka_mapy);

        if (strcmp(komorka_mapy->type1, "\"grass\"") == 0)
        {
            swiat[komorka_mapy->x1][komorka_mapy->y1] = 'g';
        }
        if (strcmp(komorka_mapy->type1, "\"wall\"") == 0)
        {
            swiat[komorka_mapy->x1][komorka_mapy->y1] = 'w';
        }
        if (strcmp(komorka_mapy->type1, "\"sand\"") == 0)
        {
            swiat[komorka_mapy->x1][komorka_mapy->y1] = 's';
        }

        if (strcmp(komorka_mapy->type2, "\"grass\"") == 0)
        {
            swiat[komorka_mapy->x2][komorka_mapy->y2] = 'g';
        }
        if (strcmp(komorka_mapy->type2, "\"wall\"") == 0)
        {
            swiat[komorka_mapy->x2][komorka_mapy->y2] = 'w';
        }
        if (strcmp(komorka_mapy->type2, "\"sand\"") == 0)
        {
            swiat[komorka_mapy->x2][komorka_mapy->y2] = 's';
        }

        if (strcmp(komorka_mapy->type3, "\"grass\"") == 0)
        {
            swiat[komorka_mapy->x3][komorka_mapy->y3] = 'g';
        }
        if (strcmp(komorka_mapy->type3, "\"wall\"") == 0)
        {
            swiat[komorka_mapy->x3][komorka_mapy->y3] = 'w';
        }
        if (strcmp(komorka_mapy->type3, "\"sand\"") == 0)
        {
            swiat[komorka_mapy->x3][komorka_mapy->y3] = 's';
        }
    }

    else 
    {
        if(strcmp(argv[i], "info") == 0)
        {
            info(token);
        }
        if(strcmp(argv[i], "right") == 0)
        {
            right(token);
        }
        if(strcmp(argv[i], "move") == 0)
        {
            move(token);
        }
        if(strcmp(argv[i], "left") == 0)
        {
            left(token);
        }
        char tab[2048];
	    FILE *f = fopen("reply.cjson", "r");
	    fread(tab,1,2048,f);
	    fclose(f);
        komorka_mapy = parameters(tab);
        wypisz(komorka_mapy);

        if (strcmp(komorka_mapy->field_type,"\"grass\"") == 0)
        {
            swiat[komorka_mapy->current_x][komorka_mapy->current_y] ='g';
        }
        else if (strcmp(komorka_mapy->field_type, "\"sand\"") == 0)
        {
            swiat[komorka_mapy->current_x][komorka_mapy->current_y] ='s';
        }
    }
}
    buduj_mape();
    zapisz_mape();

    return 0;
}