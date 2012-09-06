#define PROGRAM_NUMBER 100002494038693
#define VERSION_NUMBER 1

#define MAX 20

struct Adjacent 
{
        char destiny[16];
        char destiny_id[5]; /* if destiny_id was a region it needs be fixed.*/
        char route_ip[16];
        char route_id[3];
        int  weight;
        int  region;
        int  last_update;
        int  time_out;
};

struct Node
{
        char   node_file[18];
        char   node_id[3];
        char   node_ip[16];
        int    node_region;
        char   send_file_name[18];
        char   black_list[5];
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
