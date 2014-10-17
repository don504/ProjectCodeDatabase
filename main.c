#include <stdio.h>
#include <math.h>

extern parser();
main ()
{
 printf("Welcome to MINIREL database system\n\n");
 int freturn = CreateCats("/home/grunnithan/workspace/MINIREL/Test");
 printf("%d",freturn);
 parser();
 printf("\n\nGoodbye from MINIREL\n");
}
