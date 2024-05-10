#include <stdio.h>
#include <stdlib.h>
#include "../include/cJSON.h"

int main() {
    FILE *file = fopen("municipios.json", "r");
    if(!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *jsonData = (char *)malloc(fileSize + 1);
    fread(jsonData, 1, fileSize, file);
    jsonData[fileSize + 1] = '\0';
    fclose(file);

    cJSON *json  = cJSON_Parse(jsonData);
    if(!json) {
        printf("Não foi possível analisar o json");
        free(jsonData);
        return 1;
    }

    cJSON *primeiroObjeto = cJSON_GetArrayItem(json, 0);

    printf("Nome: %s\n", cJSON_GetObjectItem(primeiroObjeto, "nome")->valuestring);
    printf("Código IBGE: %d\n", cJSON_GetObjectItem(primeiroObjeto, "codigo_ibge")->valueint);
    printf("Latitude: %f\n", cJSON_GetObjectItem(primeiroObjeto, "latitude")->valuedouble);
    printf("Longitude: %f\n", cJSON_GetObjectItem(primeiroObjeto, "longitude")->valuedouble);
    printf("Capital: %d\n", cJSON_GetObjectItem(primeiroObjeto, "capital")->valueint);
    printf("Codigo_uf: %d\n", cJSON_GetObjectItem(primeiroObjeto, "codigo_uf")->valueint);
    printf("Siafi_id: %d\n", cJSON_GetObjectItem(primeiroObjeto, "siafi_id")->valueint);
    printf("DDD: %d\n", cJSON_GetObjectItem(primeiroObjeto, "ddd")->valueint);
    printf("fuso_horario: %s\n", cJSON_GetObjectItem(primeiroObjeto, "fuso_horario")->valuestring);



    cJSON_Delete(json);
    free(jsonData);
}