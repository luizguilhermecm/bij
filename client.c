/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <string.h>
#include <stdio.h>
#include "bij.h"

#define MAX 50

Node Read (char file_name[], Node _node);
void Write (char file_name[], Node _node);

Node Router(CLIENT *clnt, Node _arg)
{
        Node _package;
        Node * _result;
        
        _package = _arg;
        /* Put the _arg into _package */

        /* chama a função remota */
        _result = router_1 (&_package,clnt);
        if (_result == NULL)
        {
                printf ("Problemas ao chamar a função remota\n");
                return _arg;
        }
        return (*_result);
}

int main( int argc, char *argv[])
{
        CLIENT *clnt;
        Node _arg;
        Node _result;

        char file_name [16];

        strcpy (file_name, argv[2]);
        strcat (file_name, argv[1]);
        

        _arg = Read(file_name, _arg);

        /* cria uma KEEPING CLIENT que referencia uma interface RPC */
        clnt = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");


        /* verifica se a referência foi criada */
        if (clnt == (CLIENT *) NULL)
        {
                clnt_pcreateerror (argv[1]);
                return 0;
        }

        /* make your job */

        /* executa os procedimentos remotos */
        _result =  Router (clnt, _arg); 

        Write(file_name, _result);


        _result = Read (file_name, _result);
        /* save _result in somewhere */
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


