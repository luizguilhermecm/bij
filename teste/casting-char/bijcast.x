/* addsub.x : definição da interface */

/*#define PROGRAM_NUMBER 12345678
#define VERSION_NUMBER 1*/

#define PROGRAM_NUMBER 1111111
#define VERSION_NUMBER 1

/* tipo de dado que será passado aos procedimentos remotos */

struct operands
{
        char y;
};

/* Definição da interface que será oferecida aos clientes */

program BIJCAST_PROG {

   version BIJCAST_VERSION
   {
     char CAST (operands) = 1;
   }
   = VERSION_NUMBER;
}
= PROGRAM_NUMBER;

 
