/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "bij.h"
void funcao (){
        printf("haushuas");
}
/* implementa��o da fun��o add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        static Node _package;

        _package = *argp;
        _package.node_region = 10;
        _package.node_id = 'X';
        funcao();
        /* The magic works happens here */

        return (&_package);
}
