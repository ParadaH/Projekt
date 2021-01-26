#ifndef odpowiedz_h_
#define odpowiedz_h_

typedef struct _Memory {
    char *odpowiedz;
    size_t rozmiar;
}Memory;

static size_t zapisz_odpowiedz(void *data, size_t size, size_t nmemb, void *userp);
char *wyslij_zapytanie(char *url);

void info(const char *token);
void ruch_do_przodu(const char *token);
void eksploruj(const char *token);
void lewo(const char *token);
void prawo(const char *token);
void reset(const char *token);

#endif 