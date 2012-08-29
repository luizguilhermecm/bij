/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <string.h>
#include <stdio.h>
#include "bij.h"

#define MAX 50

Node Read      (char file_name[]);
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
        Node * _result;    /* Receive the return of server */
        
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
        CLIENT * _adjacent; // will be used to send table for adjacents
        Node _arg;        /* Armazenará o conteúdo do arquivo do servidor local             */
        
        char send_file_name[17];      /* Nome do arquivo que será aberto no SERVIDOR REMOTO */
        char file_name [17];          /* Nome do arquivo LOCAL                              */

        strcpy (file_name, argv[1]);  /* Parâmetro ID Exemplo: a                            */
        strcat (file_name, argv[2]);  /* Parâmetro IP Exemplo: 192.168.189.210              */
        
        _arg = Read(file_name); /* Lê o arquivo LOCAL                                 */
        int i = 0;
        while (!strcmp(_arg._table[i].destiny_id, "0") ){

                strcpy (send_file_name, _arg._table[i].destiny_id);
                strcat (send_file_name, _arg._table[i].destiny);
                
                strcpy (_arg.send_file_name, send_file_name);
                
                _adjacent = clnt_create (_arg._table[i].destiny, BIJ_PROG, BIJ_VERSION, "udp"); // the first parameter is the IP of destiny

                Router (_adjacent, _arg); /* Envia a tabela para o adjacente conforme a variável i */
                i++;
        }
//        _arg = Read(file_name, _arg); // Read my file;

        /* cria uma KEEPING CLIENT que referencia uma interface RPC */
//        clnt = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");
        /* verifica se a referência foi criada */
//        if (clnt == (CLIENT *) NULL)
//        {
//                clnt_pcreateerror (argv[1]);
//                return 0;
//        }

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
