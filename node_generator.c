#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX 50

typedef struct Adjacent 
{
        char destiny[25];
        char destiny_id; /* if destiny_id was a region it needs be fixed.*/
        char route_ip[15];
        char route_id;
        int weight;
        int region;
        int last_update;
        int time_out;
}Adjacent;

typedef struct Node
{
        char node_file[26];
        char node_id[2];
        char node_ip[15];
        int node_region;
        struct Adjacent _table[MAX]; 
}Node;

Node GetAdjacents (FILE *file, Node _node)
{
        int check = 1;
        int i = 0;
        while (check != 0){
                printf("\ndestiny: ");
                scanf ("%s", _node._table[i].destiny);
                printf("\ndestiny_id: ");
                _node._table[i].destiny_id = getchar();
                //scanf ("%c", &_node._table[i].destiny_id);
                printf("\nroute_ip: ");
                scanf("%s", _node._table[i].route_ip);
                printf("\nroute_ID: ");
                _node._table[i].route_id = getchar();
                //scanf("%c", &_node._table[i].route_id);
                printf("\nweight: ");
                scanf("%d", &_node._table[i].weight);
                printf("\nregion: ");
                scanf("%d", &_node._table[i].region);
                _node._table[i].last_update = 0;
                _node._table[i].time_out = 0;
                
                i++;
                printf("press 0 to quit");
                scanf ("%d", &check);
        }
       return _node;
}

void Generator (FILE *file, Node _node)
{
        char *aux = _node.node_file;
        file = fopen(aux, "w");

        fwrite(&_node.node_file, sizeof(char), 26, file);
        fwrite(&_node.node_id, sizeof(char), 2, file);
        fwrite(&_node.node_ip, sizeof(char), 15, file);
        fwrite(&_node.node_region, sizeof(int), 1, file);
        int check = 1;
        int i = 0;
        while (check != 0){

                printf("\ndestiny: ");
                scanf ("%s", _node._table[i].destiny);
                printf("\ndestiny_id: ");
                rewind(stdin);
                _node._table[i].destiny_id = getchar();
                //scanf ("%c", &_node._table[i].destiny_id);
                printf("\nroute_ip: ");
                scanf("%s", _node._table[i].route_ip);
                printf("\nroute_ID: ");
                rewind(stdin);
                _node._table[i].destiny_id = getchar();
                //scanf ("%c", &_node._table[i].destiny_id);
                _node._table[i].route_id = getchar();
                //scanf("%c", &_node._table[i].route_id);
                printf("\nweight: ");
                scanf("%d", &_node._table[i].weight);
                printf("\nregion: ");
                scanf("%d", &_node._table[i].region);
                _node._table[i].last_update = 0;
                _node._table[i].time_out = 0;

                fwrite(&_node._table[i].destiny, sizeof(char), 25, file);
                fwrite(&_node._table[i].destiny_id, sizeof(char), 1, file);
                fwrite(&_node._table[i].route_ip, sizeof(char), 15, file);
                fwrite(&_node._table[i].route_id, sizeof(char), 1, file);
                fwrite(&_node._table[i].weight, sizeof(int), 1, file);
                fwrite(&_node._table[i].region, sizeof(int), 1, file);
                fwrite(&_node._table[i].last_update, sizeof(int), 1, file);
                fwrite(&_node._table[i].time_out, sizeof(int), 1, file);

               
                i++;
                printf("press 0 to quit");
                scanf ("%d", &check);
        }
 
        fclose(file);
}

void View (FILE *file, Node _node)
{
        int i = 0;
        file = fopen(_node.node_file, "r");
        
        fread(&_node.node_file, sizeof(char), 26, file);
        fread(&_node.node_id, sizeof(char), 2, file);
        fread(&_node.node_ip, sizeof(char), 15, file);
        fread(&_node.node_region, sizeof(int), 1, file);

        printf("file: %s\nid: %s\nip: %s\nregion: %d\n \ndestiny: %s\n\n", _node.node_file, _node.node_id, _node.node_ip, _node.node_region, _node._table[1].destiny);

        while (!feof(file)){
                fread(&_node._table[i].destiny, sizeof(char), 25, file);
                fread(&_node._table[i].destiny_id, sizeof(char), 1, file);
                fread(&_node._table[i].route_ip, sizeof(char), 15, file);
                fread(&_node._table[i].route_id, sizeof(char), 1, file);
                fread(&_node._table[i].weight, sizeof(int), 1, file);
                fread(&_node._table[i].region, sizeof(int), 1, file);
                fread(&_node._table[i].last_update, sizeof(int), 1, file);
                fread(&_node._table[i].time_out, sizeof(int), 1, file);
                i++;

                printf("\n\ndestiny: %s\ndestiny_id: %c\nroute_ip: %s\nroute_id: %c", _node._table[i].destiny, _node._table[i].destiny_id, _node._table[i].route_ip, _node._table[i].route_id);
                printf("\nweight: %d\nregion: %d\nlast_update: %d\ntime_out: %d\n", _node._table[i].weight, _node._table[i].region, _node._table[i].last_update, _node._table[i].time_out);
        }

        fclose(file);

}

int main(int argc, char *argv[])
{
        FILE *file;
        Node _node;

        strcpy(_node.node_id, argv[1]);
        strcpy(_node.node_ip, argv[2]);
        strcat(_node.node_file, _node.node_id);
        strcat(_node.node_file, _node.node_ip);

        printf("regiao: ");
        scanf("%d", &_node.node_region);

        Generator(file, _node);
        View(file, _node);

}
