/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "bij.h"

#define MAX 20

Node Read      (char file_name[]);
void Write     (char file_name[], Node _node);
void Erro ();


/*
**  Fun��o: Router
**  Descri��o: Recebe como par�metro o ponteiro do CLIENT
**             E o Node com as informa��es que ser�o 
**             enviadas para o servidor REMOTO
*/
Node Router(CLIENT *clnt, Node _arg)
{
        Node _package;     /* Objeto do tipo Node que ser� enviado como par�metro no router_l ( M�todo remoto ) */
        Node * _result;    /* Receive the return of server */
        _package = _arg;   /* Copia o argumento para a vari�vel _package que ser� passada para a fun��o remota */
        
        _result = router_1 (&_package,clnt);  /* Efetua a chamada da fun��o remota */
        
        if (_result == NULL)
        {
                printf ("Problemas ao chamar a fun��o remota\n");
                exit(EXIT_FAILURE);
        }
        return *_result;
}

int main( int argc, char *argv[])
{
        CLIENT *clnt;
        CLIENT * _adjacent; /* � usado para enviar a tabela de Adjac�ncia                     */
        Node _arg;          /* Armazenar� o conte�do do arquivo do servidor local             */
        Node _erro;
        int i,j,count;
        char id_region[2];
        
        char send_file_name[17];      /* Nome do arquivo que ser� aberto no SERVIDOR REMOTO */
        char file_name [17];          /* Nome do arquivo LOCAL                              */

        strcpy (file_name, argv[1]);  /* Par�metro ID Exemplo: a                            */
        strcat (file_name, argv[2]);  /* Par�metro IP Exemplo: 192.168.189.210              */
        
        _arg = Read(file_name); /* L� o arquivo LOCAL                                 */

        i = 0;

        /* Procura Adjacentes que n�o s�o da mesma Regi�o, para inser�-los na tabela como Regi�es */
        while (strcmp(_arg._table[i].destiny, "0") != 0){    /* Enquanto o IP do adjacente i n�o for 0 */
                _arg._table[i].last_update = time(NULL);
                
                Write(file_name, _arg);
                _arg = Read(file_name);
                /* Se o Adjacente tiver a regi�o diferente do Arquivo LOCAL e o mesmo n�o � uma Regi�o entra no IF  */
                if(_arg.node_region != _arg._table[i].region && _arg._table[i].region != 99){
                        id_region[0] = 'r';
                        id_region[1] = (char)(((int)'0')+_arg._table[i].region);
                        count = 0;
                        j = 0;


                        while (count < MAX){
                                /* Se j� constar um registro na tabela de Adjac�ncias entra no IF
                                 * e compara se o Peso do caminho na Tabela LOCAL � MAIOR, se for MAIOR
                                 * ele � substitu�do pelo da Tabela recebida */
                                if (strcmp(_arg._table[count].destiny_id, id_region) == 0){
                                        if (_arg._table[count].weight > _arg._table[i].weight){
                                                strcpy(_arg._table[count].route_ip, _arg._table[i].route_ip);
                                                strcpy(_arg._table[count].route_id, _arg._table[i].route_id);
                                                _arg._table[count].weight = _arg._table[i].weight;

                                                _arg._table[count].last_update = time(NULL);
                                                count = MAX;
                                                j = MAX; 

                                                Write(file_name, _arg);
                                                _arg = Read(file_name);
                                        }
                                        else{
                                                count = MAX;
                                                j = MAX;
                                        }
                                }
                                count++;
                        }

                        /* Se n�o existir, procura-se uma linha vazia e adiciona a Regi�o Nova */
                        while (j < MAX){
                                /* Acha uma linha vazia e verifica se a linha n�o � uma Regi�o */
                                if (strcmp(_arg._table[j].destiny_id, "0") == 0
                                                && _arg._table[i].region != 99){
                                        id_region[0] = 'r';
                                        id_region[1] = (char)(((int)'0')+_arg._table[i].region);
                                        strcpy(_arg._table[j].destiny_id, id_region);
                                        strcpy(_arg._table[j].route_ip  , _arg._table[i].route_ip);
                                        strcpy(_arg._table[j].route_id  , _arg._table[i].route_id);
                                        _arg._table[j].weight = _arg._table[i].weight;
                                        _arg._table[j].region = 99;
                                        _arg._table[j].last_update = time(NULL);

                                        j = MAX;
                                        Write(file_name, _arg);
                                        _arg = Read(file_name);
                                }
                                j++;
                        }
                }
                i++;
        }

        i = 0;
        clock_t temp;
        while (1){
                temp = clock() + CLOCKS_PER_SEC * 10;
                while (clock() < temp){}

                while (strcmp(_arg._table[i].destiny_id, "0") != 0
                                && _arg._table[i].region != 99){

                        strcpy (send_file_name, _arg._table[i].destiny_id);
                        strcat (send_file_name, _arg._table[i].destiny);

                        strcpy (_arg.send_file_name, send_file_name);

                        // Primeiro par�metro � o IP do Destino
                        _adjacent = clnt_create (_arg._table[i].destiny, BIJ_PROG, BIJ_VERSION, "udp"); 


                        _erro = Router (_adjacent, _arg); /* Envia a tabela para o adjacente conforme a vari�vel i */
                        if (strcmp(_erro.send_file_name, "NULL") == 0){
                                printf("Arquivo n�o existe no server");
                                exit(EXIT_FAILURE);
                                //TODO: what we gonna do here?
                        }
                        i++;
                }
                i = 0;
        }
//        _arg = Read(file_name, _arg)) // Read my file;

        /* cria uma KEEPING CLIENT que referencia uma interface RPC */
//        clnt = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");
        /* verifica se a refer�ncia foi criada */
//        if (clnt == (CLIENT *) NULL)
//        {
//                clnt_pcreateerror (argv[1]);
//                return 0;
//        }

        /* Magic is here */

        /* executa os procedimentos remotos */

//        Write(file_name, _result);


 //       _result = Read (file_name, _result);
        /* Salva _result em algum lugar... */
        return 0;
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
                printf("\nArquivo n�o encontrado\n");
                exit(EXIT_FAILURE);
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
                fread( _node._table[i].destiny_id,  sizeof(char),  5, file);
                fread( _node._table[i].route_ip,    sizeof(char), 16, file);
                fread( _node._table[i].route_id,    sizeof(char),  3, file);
                fread(&_node._table[i].weight,      sizeof(int),   1, file);
                fread(&_node._table[i].region,      sizeof(int),   1, file);
                fread(&_node._table[i].last_update, sizeof(int),   1, file);
                fread(&_node._table[i].time_out,    sizeof(int),   1, file);

                printf("+---+------------------+---+------------------+---+---+---+----+----+");
                printf("\n");
                printf("|%3d",  i);
                printf("|%18s", _node._table[i].destiny);
                printf("|%5s",  _node._table[i].destiny_id); 
                printf("|%18s", _node._table[i].route_ip);
                printf("|%3s",  _node._table[i].route_id);
                
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
        if (file == NULL) {
                return;
        }                                                           
        /* Escreve as informa��es do n�. Nome do arquivo, id, IP e a regi�o a qual ele pertence */
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
