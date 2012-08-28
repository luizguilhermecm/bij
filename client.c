/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <string.h>
#include <stdio.h>
#include "bij.h"

#define MAX 50

Node Read      (char file_name[], Node _node);
void Write     (char file_name[], Node _node);
void SendTable (Node _node);


/*
**  Função: Router
**  Descrição: Recebe como parâmetro o ponteiro do CLIENT
**             E o Node com as informações que serão 
**             enviadas para o servidor REMOTO
*/
void Router(CLIENT *clnt, Node _arg)
{
        Node _package;     /* Objeto do tipo Node que será enviado como parâmetro no router_l ( Método remoto ) */
        Node * _result;    /* ??????????????????????????????? */
        
        _package = _arg;   /* Copia o argumento para a variável _package que será passada para a função remota */
        
        _result = router_1 (&_package,clnt);  /* Efetua a chamada da função remota */
        
        if (_result == NULL)
        {
                printf ("Problemas ao chamar a função remota\n");
        }
}

int main( int argc, char *argv[])
{
        CLIENT *clnt;
        CLIENT *_send_to; /* Será responsável pela comunicação com os outros servidores     */
        Node _arg;        /* Armazenará o conteúdo do arquivo do servidor local             */
        
        Node _result; /* @@@ @@@ @@@ ?????????????????????????????????????????? @@@ @@@ @@@ */

        char file_name [16];          /* Nome do arquivo LOCAL                              */
        char send_file_name[16];      /* Nome do arquivo que será aberto no SERVIDOR REMOTO */

        strcpy (file_name, argv[2]);  /* Parâmetro ID Exemplo: a                            */
        strcat (file_name, argv[1]);  /* Parâmetro IP Exemplo: 192.168.189.210              */
        
        _arg = Read(file_name, _arg); /* Lê o arquivo LOCAL                                 */
        


        /* cria uma KEEPING CLIENT que referencia uma interface RPC */
        clnt = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");
        int i = 0;
        while (_arg._table[i].last_update == 0){
                CLIENT * send;

                send = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");

                _arg.send_file_name[0] = _arg._table[i].destiny_id; /* Muda somente o ID do nome, não efetua mudanças no IP */
                
                i++;

                Router (clnt, _arg); /* Envia a tabela para o adjacente conforme a variável i */
        }


//        _arg = Read(file_name, _arg); // Read my file;
        /* verifica se a referência foi criada */
        if (clnt == (CLIENT *) NULL)
        {
                clnt_pcreateerror (argv[1]);
                return 0;
        }

        /* make your job */

        /* executa os procedimentos remotos */

//        Write(file_name, _result);


 //       _result = Read (file_name, _result);
        /* save _result in somewhere */
        return 0;
} 


/*
**  Função: SendTable
**  Descrição: BULHUFAS
*/
void SendTable (Node _node){
}


/*
**  Função: Read
**  Descrição: Recebe como parâmetro o nome do arquivo que deverá ser lido ( Arquivo LOCAL )
**             E o Nó que armazenará as informações lidas pela função e depois será retornado.     
*/
Node Read (char file_name[], Node _node){

        int i = 0;

        printf("####### ARQUIVO #######");
        FILE *file = fopen(file_name, "r");
        
        fread( _node.node_file,      sizeof(char), 16, file);
        fread(&_node.node_id,        sizeof(char),  1, file);
        fread( _node.node_ip,        sizeof(char), 15, file);
        fread(&_node.node_region,    sizeof(int),   1, file);
        fread( _node.send_file_name, sizeof(char), 16, file);

        printf("file: %s\nid: %c\nip: %s\nregion: %d\nsend_file_name: %s\n\n", _node.node_file, _node.node_id, _node.node_ip, _node.node_region, _node.send_file_name);

        while (i < MAX){

                fread( _node._table[i].destiny,      sizeof(char), 15, file);
                fread(&_node._table[i].destiny_id,   sizeof(char),  1, file);
                fread( _node._table[i].route_ip,     sizeof(char), 15, file);
                fread(&_node._table[i].route_id,     sizeof(char),  1, file);
                fread(&_node._table[i].weight,       sizeof(int),   1, file);
                fread(&_node._table[i].region,       sizeof(int),   1, file);
                fread(&_node._table[i].last_update,  sizeof(int),   1, file);
                fread(&_node._table[i].time_out,     sizeof(int),   1, file);

                if(_node._table[i-1].destiny_id != '0'){
                        //printf("\n\ndestiny: %s\ndestiny_id: %c\nroute_ip: %s\nroute_id: %c", _node._table[i].destiny, _node._table[i].destiny_id, _node._table[i].route_ip, _node._table[i].route_id);
                        //printf("\nweight: %d\nregion: %d\nlast_update: %d\ntime_out: %d\n", _node._table[i].weight, _node._table[i].region, _node._table[i].last_update, _node._table[i].time_out);
                        printf("\n");
                        printf("\ndestiny: %s",    _node._table[i].destiny);
                        printf("\ndestiny_id: %c", _node._table[i].destiny_id); 
                        printf("\nroute_ip: %s",   _node._table[i].route_ip);
                        printf("\nroute_id: %c",   _node._table[i].route_id);
                        
                        printf("\nweight: %d\n",    _node._table[i].weight);
                        printf("\nregion: %d",      _node._table[i].region);
                        printf("\nlast_update: %d", _node._table[i].last_update);
                        printf("\ntime_out: %d",    _node._table[i].time_out);
                }
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
        int i = 0;                    /* Variável de controle p/ o n. de entradas não exceder o tamanho Máximo da _table[MAX] */
        
        FILE *file = fopen(file_name, "w");       /* Abre o arquivo com permissão w
                                         Abrir um arquivo texto para gravação. 
                                         Se o arquivo não existir, ele será criado. 
                                         Se já existir, o conteúdo anterior será destruído. */

        /* Escreve as informações do nó. Nome do arquivo, id, IP e a região a qual ele pertence */
        fwrite( _node.node_file,      sizeof(char), 16, file);
        fwrite(&_node.node_id,        sizeof(char),  1, file);
        fwrite( _node.node_ip,        sizeof(char), 15, file);
        fwrite(&_node.node_region,    sizeof(int),   1, file);
        fwrite( _node.send_file_name, sizeof(char), 16, file);

        while (i < MAX){
                fwrite( _node._table[i].destiny,     sizeof(char), 15, file);
                fwrite(&_node._table[i].destiny_id,  sizeof(char),  1, file);
                fwrite( _node._table[i].route_ip,    sizeof(char), 15, file);
                fwrite(&_node._table[i].route_id,    sizeof(char),  1, file);
                fwrite(&_node._table[i].weight,      sizeof(int),   1, file);
                fwrite(&_node._table[i].region,      sizeof(int),   1, file);
                fwrite(&_node._table[i].last_update, sizeof(int),   1, file);
                fwrite(&_node._table[i].time_out,    sizeof(int),   1, file);

                i++;
        }
        fclose(file);
}
