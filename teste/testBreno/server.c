#include <stdio.h>
#include "bijstruct.h"

/* implementação da função add */
operands * pass_1_svc (operands * argp, struct svc_req *rqstp)
{
   static operands ops;
   ops.y = argp->y;

   ops.parametro = (char *) malloc (50 * sizeof(char));
   int i = 0;
   for(i; i<50; i++)
       ops.parametro[i] = argp->parametro[i];
   
   printf ("Recebi chamado: cast %c \n", argp->y);
   printf ("Recebi a String: %s \n", argp->parametro);
   ops.y = argp->y;
   //ops.vetor = argp->vetor;
   return (&ops);
}
