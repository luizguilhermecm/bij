#include <stdio.h>
#include "bijcast.h"

/* implementa��o da fun��o add */
char * cast_1_svc (operands *argp, struct svc_req *rqstp)
{
   static char result;
   result = argp->y;
   printf ("Recebi chamado: cast %c \n", argp->y);
   return (&result);
}
