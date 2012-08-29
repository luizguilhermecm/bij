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

/* Fun��es Globais */
int hasId (char id, Node _main);
Node Read (char file_name[]);
void Write (char file_name[], Node _node);

/* implementa��o da fun��o add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        int i = 0;
        static Node _package;

        _package = *argp;     /* _package recebe o conte�do do ponteiro passado por par�metro
                                  contendo o arquivo que dever� ser comparado com o arquivo LOCAL
                                  e o mesmo dever� ser atualizado caso algum n� adjacente for desconectado
                                  ou uma nova rota for descoberta ou uma rota de menor custo for encontrada. */
        
        Write(_package.send_file_name, _package);

//        Node _aux = Read(_package.send_file_name); 
        /* Instancia um novo Node chamado _aux e passa por par�metro
                                                            o nome do arquivo LOCAL ( send_file_name ) que foi enviado
                                                            pelo CLIENT e o _aux para receber o objeto do tipo Node
                                                            contendo informa��es do arquivo LOCAL                      */
        

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
**  Fun��o: Read
**  Descri��o: Recebe como par�metro o nome do arquivo que dever� ser lido ( Arquivo LOCAL )
**             E o N� que armazenar� as informa��es lidas pela fun��o e depois ser� retornado.     
*/
Node Read (char file_name[]){

        Node _node;
        int i = 0;

        FILE *file = fopen(file_name, "r");
        
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
**  Fun��o: Write
**  Descri��o: Recebe como par�metro o nome do arquivo que dever� ser criado ou escrito
**             E o N� que armazena as informa��es que ser�o gravadas pela fun��o.
*/
void Write (char file_name[], Node _node)
{
        int i = 0;                    /* Vari�vel de controle p/ o n. de entradas n�o exceder o tamanho M�ximo da _table[MAX] */
        
        FILE *file = fopen("00", "w");       /* Abre o arquivo com permiss�o w
                                                           Abrir um arquivo texto para grava��o. 
                                                           Se o arquivo n�o existir, ele ser� criado. 
                                                           Se j� existir, o conte�do anterior ser� destru�do. */

        /* Escreve as informa��es do n�. Nome do arquivo, id, IP e a regi�o a qual ele pertence */
        fwrite(_node.node_file,      sizeof(char), 17, file);
        fwrite(_node.node_id,       sizeof(char),  2, file);
        fwrite(_node.node_ip,        sizeof(char), 15, file);
        fwrite(&_node.node_region,   sizeof(int),   1, file);
        fwrite(_node.send_file_name, sizeof(char), 17, file);
        
        while (i < MAX){
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
