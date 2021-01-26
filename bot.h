#ifndef bot_h_
#define bot_h_
#include "mapa.h"

Mapa *szukaj_sciany(Mapa *mapa, Mapa *informacje, char *token, int offset_x, int offset_y);
Mapa *lewo_ruch(Mapa *mapa, Mapa *informacje, char *token, int offset_x, int offset_y, int pocz_x, int pocz_y);
Mapa *prawo_ruch(Mapa *mapa, Mapa *informacje, char *token, int offset_x, int offset_y, int pocz_x, int pocz_y);
Mapa *bot(Mapa *mapa, Mapa *info, char *token, int offset_x, int offset_y);


#endif 