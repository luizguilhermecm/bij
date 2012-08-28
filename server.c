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

int hasId (char id, Node _main);
Node Read (char file_name[], Node _node);
void Write (char file_name[], Node _node);

/* implementa��o da fun��o add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        static Node _package;
        _package = *argp;


        Node _aux = Read(_package.send_file_name, _aux);
        int i = 0;
        while (_package._table[i].last_update == 0){
                if(hasId(_aux._table[i].destiny_id, _aux) == 0){
                        _aux._table[i] = _package._table[i]; 
                }
                i++;
        }
        //Node _main;
        // The struct which came of client was in _package, make changes just above
        /* The magic works happens here */

        Write(_package.send_file_name, _aux);
        _aux = Read (_package.send_file_name, _aux);

        return (&_package);
}

int hasId (char id, Node _main){
        int i = 0;
        while (_main._table[i].last_update == 0){
                if (_main._table[i].destiny_id == id) return 1;
                i++;
        }
        return 0;
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
        int i = 0;                    /* Vari�vel de controle p/ o n. de entradas n�o exceder o tamanho M�ximo da _table[MAX] */
        
        FILE *file = fopen(file_name, "w");       /* Abre o arquivo com permiss�o w
                                         Abrir um arquivo texto para grava��o. 
                                         Se o arquivo n�o existir, ele ser� criado. 
                                         Se j� existir, o conte�do anterior ser� destru�do. */

        /* Escreve as informa��es do n�. Nome do arquivo, id, IP e a regi�o a qual ele pertence */
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
