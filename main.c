#include <stdio.h>
#include "code_gen.c"
#include "lex.h"
// #include "gen_8086.c"

FILE *output, *fp2;
void main ()
{
	fp2 = fopen("myown.txt", "r");
	output = fopen("myoutput.txt", "w");

	statements();
	fclose(output);
	fclose(fp2);
}