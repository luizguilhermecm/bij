/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "bij.h"

#define MAX 50

Node Read (char file_name[], Node _node);
void Write (char file_name[], Node _node);

/* implementação da função add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        static Node _package;

        _package = *argp;
        _package.node_region = 10;
        _package.node_id = 'X';
        /* The magic works happens here */

        return (&_package);
}

Node Read (char file_name[], Node _node){

        int i = 0;

        printf("####### ARQUIVO #######");
        FILE *file = fopen(file_name, "r");
        
        fread(_node.node_file,   sizeof(char), 16, file);
        fread(&_node.node_id,     sizeof(char),  1, file);
        fread(_node.node_ip,     sizeof(char), 15, file);
        fread(&_node.node_region, sizeof(int),   1, file);

        printf("file: %s\nid: %c\nip: %s\nregion: %d\n\n", _node.node_file, _node.node_id, _node.node_ip, _node.node_region);

        while (i < MAX){

                fread(_node._table[i].destiny,     sizeof(char), 15, file);
                fread(&_node._table[i].destiny_id,  sizeof(char),  1, file);
                fread(_node._table[i].route_ip,    sizeof(char), 15, file);
                fread(&_node._table[i].route_id,    sizeof(char),  1, file);
                fread(&_node._table[i].weight,      sizeof(int),   1, file);
                fread(&_node._table[i].region,      sizeof(int),   1, file);
                fread(&_node._table[i].last_update, sizeof(int),   1, file);
                fread(&_node._table[i].time_out,    sizeof(int),   1, file);

                if(_node._table[i-1].destiny_id != '0'){
                        printf("\n\ndestiny: %s\ndestiny_id: %c\nroute_ip: %s\nroute_id: %c", _node._table[i].destiny, _node._table[i].destiny_id, _node._table[i].route_ip, _node._table[i].route_id);
                        printf("\nweight: %d\nregion: %d\nlast_update: %d\ntime_out: %d\n", _node._table[i].weight, _node._table[i].region, _node._table[i].last_update, _node._table[i].time_out);
                }
                i++;
        }

        fclose(file);
        return _node;

}

void Write (char file_name[], Node _node)
{
        int i = 0;                    /* Variável de controle p/ o n. de entradas não exceder o tamanho Máximo da _table[MAX] */
        
        FILE *file = fopen(file_name, "w");       /* Abre o arquivo com permissão w
                                         Abrir um arquivo texto para gravação. 
                                         Se o arquivo não existir, ele será criado. 
                                         Se já existir, o conteúdo anterior será destruído. */

        /* Escreve as informações do nó. Nome do arquivo, id, IP e a região a qual ele pertence */
        fwrite(_node.node_file,   sizeof(char), 16, file);
        fwrite(&_node.node_id,     sizeof(char),  1, file);
        fwrite(_node.node_ip,     sizeof(char), 15, file);
        fwrite(&_node.node_region, sizeof(int),   1, file);

        while (i < MAX){
                fwrite(_node._table[i].destiny,     sizeof(char), 15, file);
                fwrite(&_node._table[i].destiny_id,  sizeof(char),  1, file);
                fwrite(_node._table[i].route_ip,    sizeof(char), 15, file);
                fwrite(&_node._table[i].route_id,    sizeof(char),  1, file);
                fwrite(&_node._table[i].weight,      sizeof(int),   1, file);
                fwrite(&_node._table[i].region,      sizeof(int),   1, file);
                fwrite(&_node._table[i].last_update, sizeof(int),   1, file);
                fwrite(&_node._table[i].time_out,    sizeof(int),   1, file);

                i++;
        }
        fclose(file);
}
