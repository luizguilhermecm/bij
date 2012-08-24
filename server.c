#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "bij.h"

/* implementação da função add */
Node * router_1_svc (Node * argp, struct svc_req *rqstp)
{
        static Node _package;

        /* the magic works happens here */

        return (&_package);
}
