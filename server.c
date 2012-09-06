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
int getLast(Node _node);

/* Variáveis Globais */
int test_Clock = 0;

/* implementação da função Router do Servidor */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        int i,j,count;
        char id_region[3];
        char _black_list[5];
        static Node _package;
        static int delete_count;
        Node _node;
        _package = *argp;     /* _package recebe o conteúdo do ponteiro passado por parâmetro
                               *  contendo o arquivo que deverá ser comparado com o arquivo LOCAL
                               *  e o mesmo deverá ser atualizado caso algum nó adjacente for desconectado
                               *  ou uma nova rota for descoberta ou uma rota de menor custo for encontrada. */
        


        FILE * server_file = fopen(_package.send_file_name, "r");
        if (server_file == NULL){
                return NULL;
        }
        fclose(server_file);

        
        _node = Read(_package.send_file_name);
//        int now;
//        for (j = 0; j < MAX; j++){
//                now = time(NULL);
//                if (now - _node._table[j].last_update > 100){
//                        strcpy(_node._table[j].destiny_id, "0");
//                        strcpy(_node._table[j].destiny, "0");
//                        strcpy(_node._table[j].route_id, "0");
//                        strcpy(_node._table[j].route_ip, "0");
//                        _node._table[j].weight = 0;
//                        _node._table[j].region = 0;
//                        _node._table[j].time_out = 0;
//                }
//        }

        if(_node.ibackup == 1){
                strcpy(_node.black_list, "bij");
                _node.ibackup = 0;
                _node.delete_count = 0;

                Write(_package.send_file_name, _node);
                return (&_package);
                
        }
        else if(strcmp(_package.black_list, "bij") != 0
                        && (_package.ibackup == 0 || _package.ibackup == 2)){

                strcpy(_node.black_list, _package.black_list);
                _node.delete_count++;
                if(_node.delete_count > 4){ 
                        _node.ibackup++;
                        _node.delete_count = 0;
                }
                for (j = 0; j < MAX; j++)
                {
                        if(strcmp(_node._table[j].destiny_id, _node.black_list) == 0){
                                _node._table[j] = _node._table[getLast(_node)];                                     
                                _node._table[getLast(_node)] = _node._table[18];                                     
                        }
                        else if(strcmp(_node._table[j].route_id, _node.black_list) == 0){
                                 _node._table[j] = _node._table[getLast(_node)];                                     
                                _node._table[getLast(_node)] = _node._table[18];                                     
                        }
                
                }
                Write(_package.send_file_name, _node);
                return (&_package);
        }

        Write(_package.send_file_name, _node);
        _node = Read(_package.send_file_name);

        int int_region;
        //_node [j] -> minha tabela
        //_package [i] -> tabela recebida
        for (i = 0; i < MAX; i++)
        { // percorre a tabela recebida
                for (j = 0; j < MAX; j++){ //percorre minha tabela

                        if (_package._table[i].time_out == 88 || _package._table[i].time_out == 77){

                        }

                        else if (_node._table[j].time_out == 88 || _node._table[j].time_out == 77){
                        
                        }

                        else if (strcmp(_node._table[j].destiny_id, _package._table[i].destiny_id) == 0
                                        && strcmp(_node._table[j].route_ip, "0") != 0 ) //verifica se mesmo destino
                        {


                                int all_weight = _package._table[i].weight + WeightFor(_node, _package.node_id); //recebe o peso que tinha + distancia entre nos
                                if (_node._table[j].weight > all_weight) //veririca se meu peso eh maior
                                { 
                                        _node._table[j].last_update = time(NULL);
                                        strcpy(_node._table[j].route_id, _package.node_id); // coloco nova rota sendo pelo proprio _package
                                        strcpy(_node._table[j].route_ip, _package.node_ip);
                                        _node._table[j].weight = all_weight;

                                }
//                                Write (_package.send_file_name, _node);
//                                _node = Read(_package.send_file_name);
                                break;
                        }

                        else if (_package._table[i].region == _node.node_region         // entra apenas se mesma regiao ou rota de regiao 
                                || _package._table[i].region == 99)
                        {                                                          //ou seja, elimina inuteis
                                int_region = atoi(&_package._table[i].destiny_id[1]);

                                if (int_region == _node.node_region)
                                {
                                        break;
                                }
                                else if (strcmp(_node.node_id, _package._table[i].destiny_id) == 0)
                                {
                                        // do nothing :)
                                }
                                else if (strcmp(_node._table[j].destiny_id, _package.node_id) == 0)
                                {
                                        _node._table[j].last_update = time(NULL); 
 //                                        Write (_package.send_file_name, _node);
 //                                       _node = Read(_package.send_file_name);
                                }
                                else if (strcmp(_node._table[j].destiny_id, "0") == 0)    //procura por uma linha nula na tabela
                                {
                                        strcpy(_node._table[j].destiny_id, _package._table[i].destiny_id);
                                        strcpy(_node._table[j].destiny, _package._table[i].destiny);
                                        strcpy(_node._table[j].route_id, _package.node_id);
                                        strcpy(_node._table[j].route_ip, _package.node_ip);
                                        _node._table[j].weight = _package._table[i].weight + WeightFor(_node, _package.node_id);
                                        _node._table[j].region = _package._table[i].region;

                                        _node._table[j].time_out = 0; //FIX: acredito que seja _node, trocar depois de um commit final

                                        _node._table[j].last_update = time(NULL);


//                                        Write (_package.send_file_name, _node);
//                                        _node = Read(_package.send_file_name);

                                        break;
                                }
                                else 
                                {       // nao pode entrar nesse else, se entrou esta errado
                                        //strcpy(_node._table[j].destiny, "ERRO_ELSE");
                                        // atualizar tempo de linhas onde nao se faz nada
                                }
                        } // end if de pertinencia
                        else if (strcmp(_package.node_id, _node._table[j].destiny_id) == 0){
//                               _node._table[j].last_update = time(NULL); 
//                               Write (_package.send_file_name, _node);
//                               _node = Read(_package.send_file_name);
                      }
                }
        }
        Write (_package.send_file_name, _node);
        return (&_package);
}

int getLast(Node _node)
{
        int j;
        for(j = 0; j < MAX; j++){
                if(_node._table[j+1].last_update == 0){
                        return j;
                }
        }
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
Node Print (char file_name[]){

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
         
        fread(&_node.delete_count,   sizeof(int),   1, file);
        fread(&_node.ibackup,   sizeof(int),   1, file);
        fread(_node.cbackup, sizeof(char), 5, file);
      
        fread(_node.black_list, sizeof(char), 5, file);

        printf("+----------------------------------------------------------------------------+");
        printf("\n");
        printf("|File:%18s                                                     |", _node.node_file);
        printf("\n");
        printf("|ID:%3s                                                                      |", _node.node_id);
        printf("\n");
        printf("|IP:%18s                                                       |", _node.node_ip);
        printf("\n");
        printf("|Regiao:%3d                                                                  |", _node.node_region);
        printf("\n");
        printf("+----------------------------------------------------------------------------+");
        printf("\n");
        printf("|BL: %s \t\t\t Count: %3d\t\t |", _node.black_list, _node.delete_count);
        printf("\n");
        printf("+----------------------------------------------------------------------------+");
        printf("\n");
        printf("|iBackup: %5d \t\t cBackup: %5s \t\t\t", _node.ibackup, _node.cbackup);
        printf("\n");
        printf("+----------------------------------------------------------------------------+");
        printf("\n");
        printf("+---+------------------+----+------------------+---+---+---+------------+----+");
        printf("\n");
        printf("|n  |IP Destino        |IDD | IP da Rota       |IDR|Pes|Reg|Last        |Out |");
        printf("\n");
        printf("+---+------------------+----+------------------+---+---+---+------------+----+");
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

                printf("+---+------------------+----+------------------+---+---+---+------------+----+");
                printf("\n");
                printf("|%3d", i);
                printf("|%18s", _node._table[i].destiny);
                printf("|%4s",  _node._table[i].destiny_id); 
                printf("|%18s", _node._table[i].route_ip);
                printf("|%3s",  _node._table[i].route_id);
                
                printf("|%3d",  _node._table[i].weight);
                printf("|%3d",  _node._table[i].region);
                printf("|%12d",  _node._table[i].last_update);
                printf("|%4d|", _node._table[i].time_out);
                printf("\n");
                i++;
        }
        printf("+---+------------------+----+------------------+---+---+---+------------+----+");
        printf("\n");

        fclose(file);
        return _node;
}


/*
**  Função: Read
**  Descrição: Recebe como parâmetro o nome do arquivo que deverá ser lido ( Arquivo LOCAL )
**             E o Nó que armazenará as informações lidas pela função e depois será retornado.     
*/
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
          
        fread(&_node.delete_count,   sizeof(int),   1, file);
        fread(&_node.ibackup,   sizeof(int),   1, file);
        fread(_node.cbackup, sizeof(char), 5, file);
        
        fread(_node.black_list, sizeof(char), 5, file);
        
 
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
        if (file == NULL) {
                return;
        }                                                           
        /* Escreve as informações do nó. Nome do arquivo, id, IP e a região a qual ele pertence */
        fwrite(_node.node_file,      sizeof(char), 18, file);
        fwrite(_node.node_id,        sizeof(char),  3, file);
        fwrite(_node.node_ip,        sizeof(char), 16, file);
        fwrite(&_node.node_region,   sizeof(int),   1, file);
        fwrite(_node.send_file_name, sizeof(char), 18, file);
 
        fwrite(&_node.delete_count,   sizeof(int),   1, file);
        fwrite(&_node.ibackup,   sizeof(int),   1, file);
        fwrite(_node.cbackup, sizeof(char), 5, file);
        
        fwrite(_node.black_list, sizeof(char), 5, file);
 
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
