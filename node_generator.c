/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX 50

/*
**  Estrutura: Adjacent
**  Descrição: Conterá as informações de um adjacente ao Nó ( Computador )
*/
typedef struct Adjacent 
{
        char destiny[15];  /* IP do Destinatário Exemplo: 192.90.30.211                                         */
        char destiny_id;   /* ID do Destinatário Exemplo: y #TODO: if destiny_id was a region it needs be fixed.*/
        char route_ip[15]; /* IP do Adjacente    Exemplo: 192.90.30.221                                         */
        char route_id;     /* ID do Adjacente    Exemplo: x                                                     */
        int  weight;       /* Custo do caminho   Exemplo: Nó <-> x                                              */
        int  region;       /* Região do nó a qual ele pertence Exemplo.: 1                                      */
        int  last_update;  /* Conterá quando foi a última vez que esse Adjacente foi atualizado                 */
        int  time_out;     /* caso  ( horário atual - last_update <= 25 segundos ), o time_out será 1           */
                           /* TIME_OUT range: 0 = FALSE | 1 = TRUE                                              */
}Adjacent;

/*
**  Estrutura: Node
**  Descrição: Conterá as informações do Nó ( Computador )
*/
typedef struct Node
{
        char   node_file[16];         /* Conterá o nome do arquivo Ex.: a192.90.30.211                     */
        char   node_id;               /* Conterá o ID Ex.: b                                               */
        char   node_ip[15];           /* Conterá o IP do Nó Ex.: 192.90.30.211                             */
        int    node_region;           /* Terá a região do no a qual ele pertence                           */
        char   send_file_name[16];    /* Conterá o IP do Nó Ex.: 192.90.30.211                             */
        struct Adjacent _table[MAX];  /* Vetor de estruturas do tipo Adjacent contendo os adjacentes do Nó */
}Node;

/*
**  Método:    Generator(FILE*, Node)
**  Descrição: Irá gerar o arquivo com o nome do node_file que foi passado pelo parâmetro
*/
void Generator (Node _node)
{
        int check = 1;                /* Condição de parada do while(...) */
        int i = 0;                    /* Variável de controle p/ o n. de entradas não exceder o tamanho Máximo da _table[MAX] */
        
        FILE *file = fopen(_node.node_file, "w");       /* Abre o arquivo com permissão w
                                                           Abrir um arquivo texto para gravação. 
                                                           Se o arquivo não existir, ele será criado. 
                                                           Se já existir, o conteúdo anterior será destruído. */

        /* Escreve as informações do nó. Nome do arquivo, id, IP e a região a qual ele pertence */
        fwrite(_node.node_file,      sizeof(char), 16, file);
        fwrite(&_node.node_id,       sizeof(char),  1, file);
        fwrite(_node.node_ip,        sizeof(char), 15, file);
        fwrite(&_node.node_region,   sizeof(int),   1, file);
        fwrite(_node.send_file_name, sizeof(char), 16, file);
        
        while (check != 0) {                                   /* While para o usuário entrar com os dados dos seus Adjacentes */
                printf("\ndestiny: ");
                scanf (" %s", _node._table[i].destiny);
                getchar();

                printf("\ndestiny_id: ");
                //__fpurge(stdin);
                //rewind(stdin);
                _node._table[i].destiny_id = getchar();
                getchar();

                printf("\nroute_ip: ");
                scanf(" %s", _node._table[i].route_ip);
                getchar();

                printf("\nroute_ID: ");
                //__fpurge(stdin);
                //rewind(stdin);
                _node._table[i].route_id= getchar();
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
                fwrite(&_node._table[i].destiny_id,  sizeof(char),  1, file);
                fwrite( _node._table[i].route_ip,    sizeof(char), 15, file);
                fwrite(&_node._table[i].route_id,    sizeof(char),  1, file);
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
                _node._table[i].destiny_id = '0';
                strcpy(_node._table[i].route_ip, "0");
                _node._table[i].route_id = '0';
                _node._table[i].weight = 0;
                _node._table[i].region = 0;
                _node._table[i].last_update = 0;
                _node._table[i].time_out = 0;

                fwrite(_node._table[i].destiny,     sizeof(char), 15, file);
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

/*
**  Método:    View(FILE*, Node)
**  Descrição: Irá mostrar na tela o conteúdo do arquivo passado pelo parâmetro
*/
void View (Node _node)
{
        int i = 0;

        FILE *file = fopen(_node.node_file, "r");
        
        fread( _node.node_file,      sizeof(char), 16, file);
        fread(&_node.node_id,        sizeof(char),  1, file);
        fread( _node.node_ip,        sizeof(char), 15, file);
        fread(&_node.node_region,    sizeof(int),   1, file);
        fread( _node.send_file_name, sizeof(char), 16, file);

        //TODO: colocar numero da linha
        
        printf("+-----------------------------------------------------------+");
        printf("\n");
        printf("|ID:%3c|IP:%16s|Regiao:%3d|File:%16s|", _node.node_id, _node.node_ip, _node.node_region, _node.node_file);
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
                fread(&_node._table[i].destiny_id,  sizeof(char),  1, file);
                fread( _node._table[i].route_ip,    sizeof(char), 15, file);
                fread(&_node._table[i].route_id,    sizeof(char),  1, file);
                fread(&_node._table[i].weight,      sizeof(int),   1, file);
                fread(&_node._table[i].region,      sizeof(int),   1, file);
                fread(&_node._table[i].last_update, sizeof(int),   1, file);
                fread(&_node._table[i].time_out,    sizeof(int),   1, file);

                printf("+----------------+---+----------------+---+---+---+----+----+");
                printf("\n");
                printf("|%16s", _node._table[i].destiny);
                printf("|%3c", _node._table[i].destiny_id); 
                printf("|%16s", _node._table[i].route_ip);
                printf("|%3c", _node._table[i].route_id);
                
                printf("|%3d",    _node._table[i].weight);
                printf("|%3d",      _node._table[i].region);
                printf("|%4d", _node._table[i].last_update);
                printf("|%4d|",    _node._table[i].time_out);
                printf("\n");
                i++;
        }
                printf("+----------------+---+----------------+---+---+---+----+----+");
                printf("\n");
/*
                printf("\ndestiny: %s",    _node._table[i].destiny);
                printf("\ndestiny_id: %c", _node._table[i].destiny_id); 
                printf("\nroute_ip: %s",   _node._table[i].route_ip);
                printf("\nroute_id: %c",   _node._table[i].route_id);
                
                printf("\nweight: %d\n",    _node._table[i].weight);
                printf("\nregion: %d",      _node._table[i].region);
                printf("\nlast_update: %d", _node._table[i].last_update);
                printf("\ntime_out: %d",    _node._table[i].time_out);
*/ 
        fclose(file);

}

int main(int argc, char *argv[])
{       
        Node _node;    /* Estrutura contendo as informações do Nó ( Computador ) */

        if (argc == 3){
        _node.node_id = *argv[1];            /* Copia o argv[1]  para o node_id   Ex.: a                */
        strcpy(_node.node_ip, argv[2]);      /* Copia o argv[1]  para o node_ip   Ex.: 192.168.110.220  */

        strcpy(_node.node_file, argv[1]);    /* Copia o node_id  para o node_file Ex.: a                */
        strcat(_node.node_file, argv[2]);    /* Copia o node_ip  para o node_file Ex.: a192.168.110.220 */

        strcpy(_node.send_file_name, _node.node_file);

        /* O node_file será o nome do arquivo que terá as informações dos adjacentes e também o ID do nó      */
        /* possibilitando assim, a simulação de vários nodes ( Computadores ) na mesma máquina tendo a        */
        /* diferenciação através do node_id, visto que o node_ip será o mesmo neste caso.                     */

        printf("regiao: ");
        scanf("%d", &_node.node_region);  /* Insere qual região que o nó pertence */

        Generator(_node);    /* Cria o Arquivo de configuração com o node gravado nele */
        View(_node);         /* Visualiza o Arquivo para verificar se os dados foram inseridos corretamente */

        return 1;
        }
        else if (argc == 2){
                strcpy(_node.node_file, argv[1]);
                View (_node);

                return 1;
        }
        return 0;

}
