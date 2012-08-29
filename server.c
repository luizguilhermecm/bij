/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "bij.h"

#define MAX 20

/* Fun��es Globais */
Node Read (char file_name[]);
void Write (char file_name[], Node _node);

/* implementa��o da fun��o add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        int i,j,count;
        static Node _package;
        Node _node;
        _package = *argp;     /* _package recebe o conte�do do ponteiro passado por par�metro
                                  contendo o arquivo que dever� ser comparado com o arquivo LOCAL
                                  e o mesmo dever� ser atualizado caso algum n� adjacente for desconectado
                                  ou uma nova rota for descoberta ou uma rota de menor custo for encontrada. */
        
        _node = Read(_package.send_file_name);

        i = 0;
        j = 0;
        count = 0;
        while (i < MAX){
                j = 0;
                count = 0;
                if(strcmp(_package._table[i].destiny_id, "0") != 0){
                        while (j < MAX){
                                if(strcmp(_node._table[j].destiny_id, _package._table[i].destiny_id) == 0){
                                        if(_node._table[j].weight > (_package._table[i].weight + 1) ){
                                                strcpy(_node._table[j].route_ip, _package.node_ip);
                                                strcpy(_node._table[j].route_id, _package.node_id);
                                                _node._table[j].weight = _package._table[i].weight + 1;
                                                
                                                j = MAX;
                                                count = MAX;
                                                Write (_package.send_file_name, _node);
                                        }
                                        else {
                                                j = MAX;
                                                count = MAX;
                                        }
                                }
                                j++;
                        }
                        while (count < MAX){
                                if ( (_node.node_region == _package._table[i].region
                                                || _package._table[i].region == 99) ){

                                        if (strcmp(_node._table[count].destiny_id, "0") == 0){
                                                strcpy(_node._table[count].destiny_id, _package._table[i].destiny_id);
                                                strcpy(_node._table[count].destiny, _package._table[i].destiny);
                                                strcpy(_node._table[count].route_ip, _package.node_ip);
                                                strcpy(_node._table[count].route_id, _package.node_id);
                                                _node._table[count].weight = _package._table[i].weight + 1;
                                                _node._table[count].region = _package._table[i].region;
                                                
                                                count = MAX;
                                                Write (_package.send_file_name, _node);
                                        } 
                                }
                                count++;
                        }
                        
                }
                i++;
        }

        return (&_package);
}

/*
**  Fun��o: Read
**  Descri��o: Recebe como par�metro o nome do arquivo que dever� ser lido ( Arquivo LOCAL )
**             E o N� que armazenar� as informa��es lidas pela fun��o e depois ser� retornado.     
*/
Node Read (char file_name[]){

        Node _node;
        int i = 0;

        FILE *file = fopen(file_name, "r");

        if (file == NULL) {
                return _node;
        }

        fread( _node.node_file,      sizeof(char), 18, file);
        fread( _node.node_id,        sizeof(char),  3, file);
        fread( _node.node_ip,        sizeof(char), 16, file);
        fread(&_node.node_region,    sizeof(int),   1, file);
        fread( _node.send_file_name, sizeof(char), 18, file);

        //TODO: colocar numero da linha
        
        printf("+-------------------------------------------------------------------+");
        printf("\n");
        printf("|File:%18s                                            |", _node.node_file);
        printf("\n");
        printf("|ID:%3s                                                             |", _node.node_id);
        printf("\n");
        printf("|IP:%18s                                              |", _node.node_ip);
        printf("\n");
        printf("|Regiao:%3d                                                         |", _node.node_region);
        printf("\n");
        printf("+-------------------------------------------------------------------+");
        printf("\n");
        printf("+---+------------------+---+------------------+---+---+---+----+----+");
        printf("\n");
        printf("|n  |IP Destino        |IDD| IP da Rota       |IDR|Pes|Reg|Last|Out |");
        printf("\n");
        printf("+---+------------------+---+------------------+---+---+---+----+----+");
        printf("\n");

        while (i < MAX){

                fread( _node._table[i].destiny,     sizeof(char), 16, file);
                fread(_node._table[i].destiny_id,  sizeof(char),  3, file);
                fread( _node._table[i].route_ip,    sizeof(char), 16, file);
                fread(_node._table[i].route_id,    sizeof(char),  3, file);
                fread(&_node._table[i].weight,      sizeof(int),   1, file);
                fread(&_node._table[i].region,      sizeof(int),   1, file);
                fread(&_node._table[i].last_update, sizeof(int),   1, file);
                fread(&_node._table[i].time_out,    sizeof(int),   1, file);

                printf("+---+------------------+---+------------------+---+---+---+----+----+");
                printf("\n");
                printf("|%3d", i);
                printf("|%18s", _node._table[i].destiny);
                printf("|%3s", _node._table[i].destiny_id); 
                printf("|%18s", _node._table[i].route_ip);
                printf("|%3s", _node._table[i].route_id);
                
                printf("|%3d",    _node._table[i].weight);
                printf("|%3d",      _node._table[i].region);
                printf("|%4d", _node._table[i].last_update);
                printf("|%4d|",    _node._table[i].time_out);
                printf("\n");
                i++;
        }
        printf("+---+------------------+---+------------------+---+---+---+----+----+");
        printf("\n");

        fclose(file);
        return _node;
}

/*
**  Fun��o: Write
**  Descri��o: Recebe como par�metro o nome do arquivo que dever� ser criado ou escrito
**             E o N� que armazena as informa��es que ser�o gravadas pela fun��o.
*/
void Write (char file_name[], Node _node)
{
        int check = 1;                /* Condi��o de parada do while(...) */
        int i = 0;                    /* Vari�vel de controle p/ o n. de entradas n�o exceder o tamanho M�ximo da _table[MAX] */
        
        FILE *file = fopen(file_name, "w");       /* Abre o arquivo com permiss�o w
                                                           Abrir um arquivo texto para grava��o. 
                                                           Se o arquivo n�o existir, ele ser� criado. 
                                                           Se j� existir, o conte�do anterior ser� destru�do. */

        /* Escreve as informa��es do n�. Nome do arquivo, id, IP e a regi�o a qual ele pertence */
        fwrite(_node.node_file,      sizeof(char), 18, file);
        fwrite(_node.node_id,       sizeof(char),  3, file);
        fwrite(_node.node_ip,        sizeof(char), 16, file);
        fwrite(&_node.node_region,   sizeof(int),   1, file);
        fwrite(_node.send_file_name, sizeof(char), 18, file);
 
        while (i < MAX){
                fwrite(_node._table[i].destiny,     sizeof(char), 16, file);
                fwrite( _node._table[i].destiny_id,  sizeof(char),  3, file);
                fwrite( _node._table[i].route_ip,    sizeof(char), 16, file);
                fwrite( _node._table[i].route_id,    sizeof(char),  3, file);
                fwrite(&_node._table[i].weight,      sizeof(int),   1, file);
                fwrite(&_node._table[i].region,      sizeof(int),   1, file);
                fwrite(&_node._table[i].last_update, sizeof(int),   1, file);
                fwrite(&_node._table[i].time_out,    sizeof(int),   1, file);

                i++;
        }
        fclose(file);
}
