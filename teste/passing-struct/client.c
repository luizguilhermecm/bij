/* Cliente RPC simples */
#include <string.h>
#include <stdio.h>
#include "bijstruct.h"

/* função que chama a RPC add_1 */
operands pass (CLIENT *clnt, operands arg)
{
   operands ops;
   operands * result;

   /* junta os parâmetros em um struct */
   ops.y = arg.y;

   /* chama a função remota */
   result = pass_1 (&ops,clnt);
   if (result == NULL)
   {
     printf ("Problemas ao chamar a função remota\n");
     exit (1);
   }

   return (*result);
}

int main( int argc, char *argv[])
{
   CLIENT *clnt;
   operands arg;

  /* verifica se o cliente foi chamado corretamente */
   if (argc!=3)
   {
      fprintf (stderr,"Usage: %s hostname string-to-cast\n",argv[0]);
      exit (1);
   }

   /* cria uma struct CLIENT que referencia uma interface RPC */
   clnt = clnt_create (argv[1], BIJSTRUCT_PROG, BIJSTRUCT_VERSION, "udp");

   /* verifica se a referência foi criada */
   if (clnt == (CLIENT *) NULL)
   {
      clnt_pcreateerror (argv[1]);
      exit(1);
   }

   /* obtém os dois inteiros que serão passados via RPC */
   arg.y = *argv[2];

   /* executa os procedimentos remotos */
   operands aux = pass (clnt, arg); 
   
   printf ("string: %c e retorno: %c\n", arg.y, aux.y );

   return (0);
} 
