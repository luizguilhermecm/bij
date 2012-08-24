#include <stdio.h>
#include "bijstruct.h"

/* implementação da função add */
operands * pass_1_svc (operands * argp, struct svc_req *rqstp)
{
   static operands ops;
   ops.y = argp->y;
   printf ("Recebi chamado: cast %c \n", argp->y);
   ops.y = argp->y;
   return (&ops);
}
