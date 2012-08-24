#include <string.h>
#include <stdio.h>
#include "bij.h"

Node Router(CLIENT *clnt, Node _arg)
{
        Node _package;
        Node * _result;

        /* Put the _arg into _package */

        /* chama a função remota */
        _result = router_1 (&_package,clnt);
        if (_result == NULL)
        {
                printf ("Problemas ao chamar a função remota\n");
                exit (1);
        }

        return (*_result);
}

int main( int argc, char *argv[])
{
        CLIENT *clnt;
        Node _arg;
        Node _result;

        /* cria uma KEEPING CLIENT que referencia uma interface RPC */
        clnt = clnt_create (argv[1], BIJ_PROG, BIJ_VERSION, "udp");

        /* verifica se a referência foi criada */
        if (clnt == (CLIENT *) NULL)
        {
                clnt_pcreateerror (argv[1]);
                exit(1);
        }

        /* make your job */

        /* executa os procedimentos remotos */
        _result =  Router (clnt, _arg); 

        /* save _result in somewhere */
        return (0);
} 
