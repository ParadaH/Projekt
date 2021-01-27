#ifndef logika_h_
#define logika_h_
#include "mapa.h"

Mapa *parametry_ruchu(const char * const korzen);
Mapa *odkrywanie(const char * const korzen);

void ruch(char *ruchczolgu, char *token);

int offsetx(Mapa *mapa);
int offsety(Mapa *mapa);

#endif