/* Cliente RPC simples */
#include <string.h>
#include <stdio.h>
#include "bijcast.h"

/* função que chama a RPC add_1 */
char cast (CLIENT *clnt, char y)
{
   operands ops;
   char * result;

   /* junta os parâmetros em um struct */
   ops.y = y;

   /* chama a função remota */
   result = cast_1 (&ops,clnt);
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
   char y;

  /* verifica se o cliente foi chamado corretamente */
   if (argc!=3)
   {
      fprintf (stderr,"Usage: %s hostname string-to-cast\n",argv[0]);
      exit (1);
   }

   /* cria uma struct CLIENT que referencia uma interface RPC */
   clnt = clnt_create (argv[1], BIJCAST_PROG, BIJCAST_VERSION, "udp");

   /* verifica se a referência foi criada */
   if (clnt == (CLIENT *) NULL)
   {
      clnt_pcreateerror (argv[1]);
      exit(1);
   }

   /* obtém os dois inteiros que serão passados via RPC */
   y = *argv[2];

   /* executa os procedimentos remotos */
   printf ("string: %c e retorno: %c\n", y, cast (clnt,y));

   return (0);
} 
