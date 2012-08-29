/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <string.h>
#include <stdio.h>
#include "bij.h"

#define MAX 20

Node Read      (char file_name[]);
void Write     (char file_name[], Node _node);
void SendTable (Node _node);


/*
**  Fun��o: Router
**  Descri��o: Recebe como par�metro o ponteiro do CLIENT
**             E o Node com as informa��es que ser�o 
**             enviadas para o servidor REMOTO
*/
void Router(CLIENT *clnt, Node _arg)
{
        Node _package;     /* Objeto do tipo Node que ser� enviado como par�metro no router_l ( M�todo remoto ) */
        Node * _result;    /* Receive the return of server */
        
        _package = _arg;   /* Copia o argumento para a vari�vel _package que ser� passada para a fun��o remota */
        
        _result = router_1 (&_package,clnt);  /* Efetua a chamada da fun��o remota */
        
        if (_result == NULL)
        {
                printf ("Problemas ao chamar a fun��o remota\n");
        }
}

int main( int argc, char *argv[])
{
        CLIENT *clnt;
        CLIENT * _adjacent; // will be used to send table for adjacents
        Node _arg;        /* Armazenar� o conte�do do arquivo do servidor local             */
        
        char send_file_name[17];      /* Nome do arquivo que ser� aberto no SERVIDOR REMOTO */
        char file_name [17];          /* Nome do arquivo LOCAL                              */

        strcpy (file_name, argv[1]);  /* Par�metro ID Exemplo: a                            */
        strcat (file_name, argv[2]);  /* Par�metro IP Exemplo: 192.168.189.210              */
        
        _arg = Read(file_name); /* L� o arquivo LOCAL                                 */
        int i = 0;
        while (strcmp(_arg._table[i].destiny_id, "0") != 0){

                printf("DEBUG");
                strcpy (send_file_name, _arg._table[i].destiny_id);
                strcat (send_file_name, _arg._table[i].destiny);
                
                strcpy (_arg.send_file_name, send_file_name);
                
                _adjacent = clnt_create (_arg._table[i].destiny, BIJ_PROG, BIJ_VERSION, "udp"); // the first parameter is the IP of destiny

                Router (_adjacent, _arg); /* Envia a tabela para o adjacente conforme a vari�vel i */
                i++;
        }
//        _arg = Read(file_name, _arg); // Read my file;

        /* cria uma KEEPING CLIENT que referencia uma interface RPC */
//        clnt = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");
        /* verifica se a refer�ncia foi criada */
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
**  Fun��o: SendTable
**  Descri��o: BULHUFAS
*/
void SendTable (Node _node){
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
