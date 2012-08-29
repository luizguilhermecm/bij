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

#define MAX 50

/* Funções Globais */
int hasId (char id, Node _main);
Node Read (char file_name[]);
void Write (char file_name[], Node _node);

/* implementação da função add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        int i = 0;
        static Node _package;

        _package = *argp;     /* _package recebe o conteúdo do ponteiro passado por parâmetro
                                  contendo o arquivo que deverá ser comparado com o arquivo LOCAL
                                  e o mesmo deverá ser atualizado caso algum nó adjacente for desconectado
                                  ou uma nova rota for descoberta ou uma rota de menor custo for encontrada. */
        
        Write (_package.send_file_name, _package);

//        Node _aux = Read(_package.send_file_name); 
        /* Instancia um novo Node chamado _aux e passa por parâmetro
                                                            o nome do arquivo LOCAL ( send_file_name ) que foi enviado
                                                            pelo CLIENT e o _aux para receber o objeto do tipo Node
                                                            contendo informações do arquivo LOCAL                      */
        

//        while (_package._table[i].last_update == 0){                 /* BETA >> */
//                if(hasId(_aux._table[i].destiny_id, _aux) == 0){
//                        _aux._table[i] = _package._table[i]; 
//                }
//                i++;
//        }
        //Node _main;
        // The struct which came of client was in _package, make changes just above
        /* The magic works happens here */

//        Write(_package.send_file_name, _aux);
//        _aux = Read (_package.send_file_name);

        return (&_package);
}

/*
**  Função: Read
**  Descrição: Recebe como parâmetro o nome do arquivo que deverá ser lido ( Arquivo LOCAL )
**             E o Nó que armazenará as informações lidas pela função e depois será retornado.     
*/
Node Read (char file_name[]){

        Node _node;
        int i = 0;

        FILE *file = fopen(_node.node_file, "r");
        
        fread( _node.node_file,      sizeof(char), 17, file);
        fread( _node.node_id,        sizeof(char),  2, file);
        fread( _node.node_ip,        sizeof(char), 15, file);
        fread(&_node.node_region,    sizeof(int),   1, file);
        fread( _node.send_file_name, sizeof(char), 17, file);

        //TODO: colocar numero da linha
        
        printf("+-----------------------------------------------------------+");
        printf("\n");
        printf("|ID:%3s|IP:%17s|Regiao:%3d|File:%17s|", _node.node_id, _node.node_ip, _node.node_region, _node.node_file);
        printf("\n");
        printf("+-----------------------------------------------------------+");
        printf("\n");
        printf("+----------------+---+----------------+---+---+---+----+----+");
        printf("\n");
        printf("|IP Destino      |IDD| IP da Rota     |IDR|Pes|Reg|Last|Out |");
        printf("\n");
        printf("+----------------+---+----------------+---+---+---+----+----+");
        printf("\n");

        while (i < MAX){

                fread( _node._table[i].destiny,     sizeof(char), 15, file);
                fread(_node._table[i].destiny_id,  sizeof(char),  2, file);
                fread( _node._table[i].route_ip,    sizeof(char), 15, file);
                fread(_node._table[i].route_id,    sizeof(char),  2, file);
                fread(&_node._table[i].weight,      sizeof(int),   1, file);
                fread(&_node._table[i].region,      sizeof(int),   1, file);
                fread(&_node._table[i].last_update, sizeof(int),   1, file);
                fread(&_node._table[i].time_out,    sizeof(int),   1, file);

                printf("+----------------+---+----------------+---+---+---+----+----+");
                printf("\n");
                printf("|%17s", _node._table[i].destiny);
                printf("|%3s", _node._table[i].destiny_id); 
                printf("|%17s", _node._table[i].route_ip);
                printf("|%3s", _node._table[i].route_id);
                
                printf("|%3d",    _node._table[i].weight);
                printf("|%3d",      _node._table[i].region);
                printf("|%4d", _node._table[i].last_update);
                printf("|%4d|",    _node._table[i].time_out);
                printf("\n");
                i++;
        }
        printf("+----------------+---+----------------+---+---+---+----+----+");
        printf("\n");

        fclose(file);
}

/*
**  Função: Write
**  Descrição: Recebe como parâmetro o nome do arquivo que deverá ser criado ou escrito
**             E o Nó que armazena as informações que serão gravadas pela função.
*/
void Write (char file_name[], Node _node)
{
        int check = 1;                /* Condição de parada do while(...) */
        int i = 0;                    /* Variável de controle p/ o n. de entradas não exceder o tamanho Máximo da _table[MAX] */
        
        FILE *file = fopen(_node.node_file, "w");       /* Abre o arquivo com permissão w
                                                           Abrir um arquivo texto para gravação. 
                                                           Se o arquivo não existir, ele será criado. 
                                                           Se já existir, o conteúdo anterior será destruído. */

        /* Escreve as informações do nó. Nome do arquivo, id, IP e a região a qual ele pertence */
        fwrite(_node.node_file,      sizeof(char), 17, file);
        fwrite(_node.node_id,       sizeof(char),  2, file);
        fwrite(_node.node_ip,        sizeof(char), 15, file);
        fwrite(&_node.node_region,   sizeof(int),   1, file);
        fwrite(_node.send_file_name, sizeof(char), 17, file);
        
        while (check != 0) {                                   /* While para o usuário entrar com os dados dos seus Adjacentes */
                printf("\ndestiny: ");
                scanf (" %s", _node._table[i].destiny);
                getchar();

                printf("\ndestiny_id: ");
                scanf ("%s", _node._table[i].destiny_id);
                getchar();

                printf("\nroute_ip: ");
                scanf(" %s", _node._table[i].route_ip);
                getchar();

                printf("\nroute_ID: ");
                scanf ("%s", _node._table[i].route_id);
                getchar();

                printf("\nweight: ");
                scanf(" %d", &_node._table[i].weight);
                getchar();

                printf("\nregion: ");
                scanf(" %d", &_node._table[i].region);
                getchar();
                
                _node._table[i].last_update = 0;
                _node._table[i].time_out = 0;

                fwrite( _node._table[i].destiny,     sizeof(char), 15, file);
                fwrite( _node._table[i].destiny_id,  sizeof(char),  2, file);
                fwrite( _node._table[i].route_ip,    sizeof(char), 15, file);
                fwrite( _node._table[i].route_id,    sizeof(char),  2, file);
                fwrite(&_node._table[i].weight,      sizeof(int),   1, file);
                fwrite(&_node._table[i].region,      sizeof(int),   1, file);
                fwrite(&_node._table[i].last_update, sizeof(int),   1, file);
                fwrite(&_node._table[i].time_out,    sizeof(int),   1, file);

                i++;

                if (i > MAX) {        /* Somente para verificar se o número */
                        check = 0;    /* de entradas não irá exceder o MAX de entradas da tabela */
                } else {
                        printf("press 0 to quit");
                        scanf ("%d", &check);
                }
        }

        while (i < MAX){
                strcpy(_node._table[i].destiny, "0");
                strcpy(_node._table[i].destiny_id, "0");
                strcpy(_node._table[i].route_ip, "0");
                strcpy(_node._table[i].route_id, "0");
                _node._table[i].weight = 0;
                _node._table[i].region = 0;
                _node._table[i].last_update = 0;
                _node._table[i].time_out = 0;

                fwrite(_node._table[i].destiny,     sizeof(char), 15, file);
                fwrite( _node._table[i].destiny_id,  sizeof(char),  2, file);
                fwrite( _node._table[i].route_ip,    sizeof(char), 15, file);
                fwrite( _node._table[i].route_id,    sizeof(char),  2, file);
                fwrite(&_node._table[i].weight,      sizeof(int),   1, file);
                fwrite(&_node._table[i].region,      sizeof(int),   1, file);
                fwrite(&_node._table[i].last_update, sizeof(int),   1, file);
                fwrite(&_node._table[i].time_out,    sizeof(int),   1, file);

                i++;
        }
        fclose(file);
}
