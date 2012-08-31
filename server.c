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

/* Funções Globais */
Node Read (char file_name[]);
Node Print (char file_name[]);
void Write (char file_name[], Node _node);
int WeightFor (Node _node, char id[]);

/* Variáveis Globais */
int test_Clock = 0;

/* implementação da função Router do Servidor */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        int i,j,count;
        char id_region[2];
        static Node _package;
        Node _node;
        _package = *argp;     /* _package recebe o conteúdo do ponteiro passado por parâmetro
                               *  contendo o arquivo que deverá ser comparado com o arquivo LOCAL
                               *  e o mesmo deverá ser atualizado caso algum nó adjacente for desconectado
                               *  ou uma nova rota for descoberta ou uma rota de menor custo for encontrada. */
        
        _node = Read(_package.send_file_name);
        if (strcmp(_node.send_file_name, "NULL") == 0){
             strcpy(_package.send_file_name, _node.send_file_name);
             return (&_package);
        }
/*
        i = 0;
        while (i < MAX){
                if(strcmp(_node._table[i].destiny_id, "0") != 0){
                        int tempo_atual = time(NULL);
                        if(tempo_atual - _node._table[i].last_update > 55){
                                strcpy(_node._table[i].destiny, "0");
                                strcpy(_node._table[i].destiny_id, "0");
                                strcpy(_node._table[i].route_ip, "0");
                                strcpy(_node._table[i].route_id, "0");
                                _node._table[i].weight = 0;
                                _node._table[i].region = 0;
                                _node._table[i].last_update = 0;
                                _node._table[i].time_out = 0;
                        }
                }
                i++;
        }
        Write (_package.send_file_name, _node);
        _node = Read(_package.send_file_name);
*/
        int int_region;
        //_node [j] -> minha tabela
        //_package [i] -> tabela recebida
        for (i = 0; i < MAX; i++){ // percorre a tabela recebida
                for (j = 0; j < MAX; j++){ //percorre minha tabela

                        if (_package._table[i].region == _node.node_region         // entra apenas se mesma regiao ou rota de regiao 
                                || _package._table[i].region == 99){      //ou seja, elimina inuteis

                                id_region[0] = 'r';
                                id_region[1] = (char)(((int)'0')+_node.node_region);

                                int_region = atoi(&_package._table[j].destiny_id[1]);

                                if (strcmp(_node._table[j].destiny_id, _package._table[i].destiny_id) == 0){ //verifica se mesmo destino
                                        int all_weight = _package._table[i].weight + WeightFor(_node, _package.node_id); //recebe o peso que tinha + distancia entre nos
                                        if (_node._table[j].weight > all_weight){ //veririca se meu peso eh maior
                                                strcpy(_node._table[j].route_id, _package.node_id); // coloco nova rota sendo pelo proprio _package
                                                strcpy(_node._table[j].route_ip, _package.node_ip);
                                                _node._table[j].weight = all_weight;

                                                Write (_package.send_file_name, _node);
                                                _node = Read(_package.send_file_name);
                                        }
                                        break;
                                }
                                else if (int_region == _node.node_region){
                                        break;
                                }
                                else if (strcmp(_node.node_id, _package._table[i].destiny_id) == 0){
                                        // do nothing :)
                                }
                                else if (strcmp(_node._table[j].destiny_id, "0") == 0){ //procura por uma linha nula na tabela
                                        strcpy(_node._table[j].destiny_id, _package._table[i].destiny_id);
                                        strcpy(_node._table[j].destiny, _package._table[i].destiny);
                                        strcpy(_node._table[j].route_id, _package.node_id);
                                        strcpy(_node._table[j].route_ip, _package.node_ip);
                                        _node._table[j].weight = _package._table[i].weight + WeightFor(_node, _package.node_id);
                                        _node._table[j].region = _package._table[i].region;
                                        _package._table[i].time_out = 0;

                                        Write (_package.send_file_name, _node);
                                        _node = Read(_package.send_file_name);

                                        break;
                                }
                                else { // nao pode entrar nesse else, se entrou esta errado
                                        //strcpy(_node._table[j].destiny, "ERRO_ELSE");
                                        // atualizar tempo de linhas onde nao se faz nada
                                        Write (_package.send_file_name, _node);
                                        _node = Read(_package.send_file_name);


                                }
                        } // end if de pertinencia
                }
        }
        return (&_package);
}

int WeightFor (Node _node, char id[]){
        int j;
        for (j = 0; j < MAX; j++){
                if (strcmp(_node._table[j].destiny_id, id) == 0){
                        return _node._table[j].weight;
                }
        }
        return 99;
}

Node Read (char file_name[]){

        Node _node;
        int i = 0;

        FILE *file = fopen(file_name, "r");
        
        if (file == NULL) {
                printf("\nArquivo não encontrado\n");
                exit(EXIT_FAILURE);
        }

        fread( _node.node_file,      sizeof(char), 18, file);
        fread( _node.node_id,        sizeof(char),  3, file);
        fread( _node.node_ip,        sizeof(char), 16, file);
        fread(&_node.node_region,    sizeof(int),   1, file);
        fread( _node.send_file_name, sizeof(char), 18, file);

        while (i < MAX){

                fread( _node._table[i].destiny,     sizeof(char), 16, file);
                fread( _node._table[i].destiny_id,  sizeof(char),  5, file);
                fread( _node._table[i].route_ip,    sizeof(char), 16, file);
                fread( _node._table[i].route_id,    sizeof(char),  3, file);
                fread(&_node._table[i].weight,      sizeof(int),   1, file);
                fread(&_node._table[i].region,      sizeof(int),   1, file);
                fread(&_node._table[i].last_update, sizeof(int),   1, file);
                fread(&_node._table[i].time_out,    sizeof(int),   1, file);

               i++;
        }

        fclose(file);
        return _node;
}
/*
**  Funçãoó quer o negocio funcionando nao precisa deletar...
Read
**  Descrição: Recebe como parâmetro o nome do arquivo que deverá ser lido ( Arquivo LOCAL )
**             E o Nó que armazenará as informações lidas pela função e depois será retornado.     
*/
Node Print (char file_name[]){

        Node _node;
        int i = 0;

        FILE *file = fopen(file_name, "r");

        if (file == NULL) {
                strcpy(_node.send_file_name, "NULL");
                return _node;
        }

        fread( _node.node_file,      sizeof(char), 18, file);
        fread( _node.node_id,        sizeof(char),  3, file);
        fread( _node.node_ip,        sizeof(char), 16, file);
        fread(&_node.node_region,    sizeof(int),   1, file);
        fread( _node.send_file_name, sizeof(char), 18, file);

        
        printf("\n\n ( Read() Server ) Function Call Number: %d\n\n", test_Clock);
        
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
                fread(_node._table[i].destiny_id,   sizeof(char),  5, file);
                fread( _node._table[i].route_ip,    sizeof(char), 16, file);
                fread(_node._table[i].route_id,     sizeof(char),  3, file);
                fread(&_node._table[i].weight,      sizeof(int),   1, file);
                fread(&_node._table[i].region,      sizeof(int),   1, file);
                fread(&_node._table[i].last_update, sizeof(int),   1, file);
                fread(&_node._table[i].time_out,    sizeof(int),   1, file);

                printf("+---+------------------+---+------------------+---+---+---+----+----+");
                printf("\n");
                printf("|%3d" , i);
                printf("|%18s", _node._table[i].destiny);
                printf("|%5s" , _node._table[i].destiny_id); 
                printf("|%18s", _node._table[i].route_ip);
                printf("|%3s" , _node._table[i].route_id);
                
                printf("|%3d",  _node._table[i].weight);
                printf("|%3d",  _node._table[i].region);
                printf("|%4d",  _node._table[i].last_update);
                printf("|%4d|", _node._table[i].time_out);
                printf("\n");
                i++;
        }
        printf("+---+------------------+---+------------------+---+---+---+----+----+");
        printf("\n");

        fclose(file);

        test_Clock++;
        
        return _node;
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
        
        FILE *file = fopen(file_name, "w");       /* Abre o arquivo com permissão w
                                                           Abrir um arquivo texto para gravação. 
                                                           Se o arquivo não existir, ele será criado. 
                                                           Se já existir, o conteúdo anterior será destruído. */

        /* Escreve as informações do nó. Nome do arquivo, id, IP e a região a qual ele pertence */
        fwrite(_node.node_file,      sizeof(char), 18, file);
        fwrite(_node.node_id,        sizeof(char),  3, file);
        fwrite(_node.node_ip,        sizeof(char), 16, file);
        fwrite(&_node.node_region,   sizeof(int),   1, file);
        fwrite(_node.send_file_name, sizeof(char), 18, file);
 
        while (i < MAX){
                
                fwrite(_node._table[i].destiny,      sizeof(char), 16, file);
                fwrite( _node._table[i].destiny_id,  sizeof(char),  5, file);
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
