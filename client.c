/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <string.h>
#include <stdio.h>
#include "bij.h"

#define MAX 50

Node Router(CLIENT *clnt, Node _arg)
{
        Node _package;
        Node * _result;

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
        
        Node _node;
        int i = 0;

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

                printf("\n\ndestiny: %s\ndestiny_id: %c\nroute_ip: %s\nroute_id: %c", _node._table[i].destiny, _node._table[i].destiny_id, _node._table[i].route_ip, _node._table[i].route_id);
                printf("\nweight: %d\nregion: %d\nlast_update: %d\ntime_out: %d\n", _node._table[i].weight, _node._table[i].region, _node._table[i].last_update, _node._table[i].time_out);

                i++;
        }

        fclose(file);


        /* cria uma KEEPING CLIENT que referencia uma interface RPC */
        clnt = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");


        /* verifica se a referência foi criada */
        if (clnt == (CLIENT *) NULL)
        {
                clnt_pcreateerror (argv[1]);
                return 0;
        }

        /* make your job */
        _arg = _node;
        /* executa os procedimentos remotos */
        _result =  Router (clnt, _arg); 

        /* save _result in somewhere */
        return 0;
} 
