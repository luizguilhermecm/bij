/*
**   Autores:
**      Breno Naodi Kusunoki
**      Luiz Guilherme Castilho Martins
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "bij.h"

/* implementa��o da fun��o add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        static Node _package;

        /* The magic works happens here */

        return (&_package);
}