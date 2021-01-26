#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logika.h"
#include "mapa.h"
#include <curl/curl.h>
#include "cJSON/cJSON.h"
#include "logika.h"
#include "odpowiedz.h"
#define Q 2

Mapa *parametry_ruchu(const char * const korzen)//musi btc status (korzen)
{
    Mapa *ruch = (Mapa*) malloc(sizeof(Mapa));
    ruch->pole = (Parametry*) malloc(sizeof(Parametry));

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
    ruch->pole->status = cJSON_Print(status);

    payload = cJSON_GetObjectItemCaseSensitive(korzen_cjson, "payload");
    ruch->pole->name = cJSON_Print(payload->child);
    ruch->pole->current_x = atoi(cJSON_Print(payload->child->next));
    ruch->pole->current_y = atoi(cJSON_Print(payload->child->next->next));
    ruch->pole->current_session = cJSON_Print(payload->child->next->next->next);
    ruch->pole->direction = cJSON_Print(payload->child->next->next->next->next);
    ruch->pole->step = atoi(cJSON_Print(payload->child->next->next->next->next->next));
    ruch->pole->field_type = cJSON_Print(payload->child->next->next->next->next->next->next);
    ruch->pole->field_bonus = cJSON_Print(payload->child->next->next->next->next->next->next->next);
    
    // free(ruch);
    cJSON_Delete(korzen_cjson);
    return ruch;
}
Mapa *odkrywanie(const char * const korzen)
{
    Mapa* odkryj = (Mapa*) malloc(sizeof(Mapa));
    odkryj->pole = (Parametry*) malloc(sizeof(Parametry));
    odkryj->pole1 = (Luneta*) malloc(sizeof(Luneta));
    odkryj->pole2 = (Luneta*) malloc(sizeof(Luneta));
    odkryj->pole3 = (Luneta*) malloc(sizeof(Luneta));

    const cJSON *status = NULL;
    const cJSON *payload = NULL;

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
    odkryj->pole->status = cJSON_Print(status);

    payload = cJSON_GetObjectItemCaseSensitive(korzen_cjson, "payload");
    odkryj->pole1->x = atoi(cJSON_Print(payload->child->child->child));
    odkryj->pole1->y = atoi(cJSON_Print(payload->child->child->child->next));
    odkryj->pole1->field_type = cJSON_Print(payload->child->child->child->next->next);

    odkryj->pole2->x = atoi(cJSON_Print(payload->child->child->next->child));
    odkryj->pole2->y = atoi(cJSON_Print(payload->child->child->next->child->next));
    odkryj->pole2->field_type = cJSON_Print(payload->child->child->next->child->next->next);

    odkryj->pole3->x = atoi(cJSON_Print(payload->child->child->next->next->child));
    odkryj->pole3->y = atoi(cJSON_Print(payload->child->child->next->next->child->next));
    odkryj->pole3->field_type = cJSON_Print(payload->child->child->next->next->child->next->next);
    
    // free(odkryj);
    cJSON_Delete(korzen_cjson);
    return odkryj;
}

void ruch(char *ruchczolgu, char *token)
{   
    if(strcmp(ruchczolgu, "info") == 0)
    {
        info(token);
    }
    if(strcmp(ruchczolgu, "prawo") == 0)
    {
        prawo(token);
    }
    if(strcmp(ruchczolgu, "ruch") == 0)
    {
        ruch_do_przodu(token);
    }
    if(strcmp(ruchczolgu, "lewo") == 0)
    {
        lewo(token);
    }
    if(strcmp(ruchczolgu, "reset") == 0)
    {
        reset(token);
    }
}

int offsetx(Mapa *mapa)
{
    int offset_x;
    offset_x = Q - mapa->pole->current_x;
    return offset_x;
}
int offsety(Mapa *mapa)
{
    int offset_y;
    offset_y = Q - mapa->pole->current_y;
    return offset_y;
}
