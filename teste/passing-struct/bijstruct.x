/* addsub.x : defini��o da interface */

/*#define PROGRAM_NUMBER 12345678
#define VERSION_NUMBER 1*/

#define PROGRAM_NUMBER 1111111
#define VERSION_NUMBER 1

/* tipo de dado que ser� passado aos procedimentos remotos */

struct operands
{
        char y;
};

/* Defini��o da interface que ser� oferecida aos clientes */

program BIJSTRUCT_PROG { 

   version BIJSTRUCT_VERSION
   {
     operands PASS (operands) = 1;
   }
   = VERSION_NUMBER;
}
= PROGRAM_NUMBER;

 
