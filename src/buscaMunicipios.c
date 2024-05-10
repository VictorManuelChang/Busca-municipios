#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/libfacom.h"
#include "../include/cJSON.h"

typedef struct
{
    char codigo_ibge[8];
    char nome[50];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[50];
} tmunicipios;

char *int_to_str(int number)
{
    char *str = malloc(sizeof(char) * 9); // espaço para 8 dígitos e o terminador nulo
    sprintf(str, "%d", number);
    return str;
}

char *get_key(void *reg)
{
    return ((tmunicipios *)reg)->codigo_ibge;
}

void *aloca_municipio(char *codigo_ibge, char *nome, float latitude, float longitude, int capital, int codigo_uf, int siafi_id, int ddd, char *fuso_horario)
{
    tmunicipios *municipio = malloc(sizeof(tmunicipios));

    strcpy(municipio->codigo_ibge, codigo_ibge);
    strcpy(municipio->nome, nome);
    municipio->latitude = latitude;
    municipio->longitude = longitude;
    municipio->capital = capital;
    municipio->codigo_uf = codigo_uf;
    municipio->siafi_id = siafi_id;
    municipio->ddd = ddd;
    strcpy(municipio->fuso_horario, fuso_horario);

    return municipio;
}

int main()
{
    FILE *file = fopen("municipios.json", "r");
    if (!file)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *jsonData = (char *)malloc(fileSize + 1);
    fread(jsonData, 1, fileSize, file);
    jsonData[fileSize] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(jsonData);
    if (!json)
    {
        printf("Não foi possível analisar o json");
        free(jsonData);
        return 1;
    }

    thash h;
    int nbuckets = 10000;
    hash_constroi(&h, nbuckets, get_key);



    for (int i = 0; i < cJSON_GetArraySize(json); i++)
    {
        cJSON *objeto = cJSON_GetArrayItem(json, i);

        int codigo_ibgeAux = cJSON_GetObjectItem(objeto, "codigo_ibge")->valueint;
        char *nome = cJSON_GetObjectItem(objeto, "nome")->valuestring;
        double latitude = cJSON_GetObjectItem(objeto, "latitude")->valuedouble;
        double longitude = cJSON_GetObjectItem(objeto, "longitude")->valuedouble;
        int capital = cJSON_GetObjectItem(objeto, "capital")->valueint;
        int codigo_uf = cJSON_GetObjectItem(objeto, "codigo_uf")->valueint;
        int siafi_id = cJSON_GetObjectItem(objeto, "siafi_id")->valueint;
        int ddd = cJSON_GetObjectItem(objeto, "ddd")->valueint;
        char *fuso_horario = cJSON_GetObjectItem(objeto, "fuso_horario")->valuestring;

        char *codigo_ibge = int_to_str(codigo_ibgeAux);
        hash_insere(&h, aloca_municipio(codigo_ibge, nome, latitude, longitude, capital, codigo_uf, siafi_id, ddd, fuso_horario));
        free(codigo_ibge);
    }


    
    char codigo_buscado[8];

    printf("Digite o código IBGE da cidade que você quer buscar: ");
    scanf("%s", codigo_buscado);
    printf("\n");

    tmunicipios *municipio;
    municipio = hash_busca(h, codigo_buscado);

    printf("Nome: %s\n", municipio->nome);
    printf("Código IBGE: %s\n", municipio->codigo_ibge);
    printf("Latitude: %f\n", municipio->latitude);
    printf("Longitude: %f\n", municipio->longitude);
    printf("Capital: %d\n", municipio->capital);
    printf("Codigo_uf: %d\n", municipio->codigo_uf);
    printf("Siafi_id: %d\n", municipio->siafi_id);
    printf("DDD: %d\n", municipio->ddd);
    printf("fuso_horario: %s\n", municipio->fuso_horario);

    int n = 3;






    hash_apaga(&h);
    cJSON_Delete(json);
    free(jsonData);

    return EXIT_SUCCESS;
}
