#include <stdio.h>
#include "bijstruct.h"

/* implementação da função add */
operands * pass_1_svc (operands * argp, struct svc_req *rqstp)
{
   static operands ops;
   ops.y = argp->y;

   int i = 0;
   for(i; i<50; i++)
       ops.vetor[i] = argp->vetor[i];
   
   printf ("Recebi chamado: cast %c \n", argp->y);
   printf ("Recebi a String: %s \n", argp->vetor);
   ops.y = argp->y;
   //ops.vetor = argp->vetor;
   return (&ops);
}
