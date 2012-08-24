#define PROGRAM_NUMBER 100002494038693
#define VERSION_NUMBER 1

#define MAX 50

struct Adjacent 
{
        char destiny[15];
        char destiny_id; /* if destiny_id was a region it needs be fixed.*/
        char route_ip[15];
        char route_id;
        int weight;
        int region;
        int last_update;
        bool time_out;
};

struct Node
{
        char node_id;
        char node_ip[15];
        int node_region;
        struct Adjacent _table[MAX]; 
};

program BIJ_PROG
{ 
   version BIJ_VERSION
   {
     struct Node router (Node) = 1;
   }
   = VERSION_NUMBER;
}
= PROGRAM_NUMBER;
