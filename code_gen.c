#include <stdio.h>
#include "lex.h"
#include "name.c"
#include "lex.c"

char    *factor     ( void );
char    *term       ( void );
char    *expression ( void );
char    *condition  ( void );
extern char *newname( void       );
extern void freename( char *name );
extern FILE *output;
extern FILE *fp2;

int flag_no = 0;
statements(){
    while(!match(EOI)){
        statement();
    }
}
statement_list(){

//stmt_list → stmt_list; stmt | stmt
    while(!match(END) && !match(EOF))
        statement();
    
}


statement()
{
    /*  statements -> expression SEMI  |  expression SEMI statements  */
    int flag_local;
    char *tempvar ;


        if(match(IF)){
            advance();

            tempvar = condition();//need to make it
            fprintf(output, "    IFZ %s JumpTo _F%d\n",tempvar,flag_no );
            freename(tempvar);
            flag_local= flag_no;
            flag_no++;
            if(match(THEN)){
                advance();
                statement();
                fprintf(output, "_F%d:\n", flag_local);
            }
            else
                fprintf(output,  stderr, "%d: missing THEN after if\n", yylineno );
        }

        else if(match(WHILE)){
            advance();
            fprintf(output, "_F%d:\n", flag_no);
            flag_local=flag_no;
            flag_no++;
            tempvar = condition();
            int returnto = flag_no;
            fprintf(output, "    IFZ %s JumpTo _F%d\n",tempvar,returnto );
            freename(tempvar);

            if(match(DO)){
                advance();
                flag_no++;
                statement();
                flag_no--;
                fprintf(output, "    JumpTo _F%d\n", flag_local);
                fprintf(output, "_F%d:\n", returnto );
            }
            else
                fprintf(output,  stderr, "%d: missing DO after WHILE\n", yylineno);


        }

        else if(match(BEGIN)){
            advance();
            statement_list();

            if ( match (EOF) )
                fprintf(output, stderr, "%d: No END statement after BEGIN\n", yylineno );
            else
                advance();
        }

        else if(match(ID)){
            char *ident = yytext;
            int ident_len = yyleng;
            advance();
            if( match(COL) )
            {
                advance();
                if ( match(EQ) ) {
                    advance();
                    tempvar = condition();
                    fprintf(output, "    %0.*s = %s\n", ident_len, ident, tempvar);
                }
                else
                    fprintf(output, stderr, "%d: no equal to after : in assignment operator\n", yylineno );
            }
            else {
                yytext = ident;
                yyleng = 0;
                advance();
                tempvar = condition();
            }
            if( match( SEMI ) )
                    advance();
            else
                fprintf(output, stderr, "%d: Inserting missing semicolon\n", yylineno );
            freename( tempvar );
        }

        else{
            tempvar = condition();

            if( match( SEMI ) )
                advance();
            else
                fprintf(output,  stderr, "%d: missing semicolon\n", yylineno );

            freename( tempvar );
        }
}

char *condition(){ //condition comes after if 
    char *tempvar, *tempvar2;
    tempvar = expression();

    if(match(LE) || match(EQ) || match(GE)){ //the comparison operator in condition 
        char operator = *yytext;
        advance();
        tempvar2 = expression();
        //fprintf(output, "check1\n");
        fprintf(output, "    %s = %s %c %s\n", tempvar, tempvar, operator, tempvar2);
        freename(tempvar2);
        
    }
    /*else
        fprintf(output, stderr, "%d: no comparison operator found\n", yylineno);*/
    
    
    return tempvar;

}


char    *expression()
{
    /* expression -> term expression'
     * expression' -> PLUS term expression' |  epsilon
     */

    char  *tempvar, *tempvar2;

    tempvar = term();
    while( match( PLUS ) || match(MINUS))
    {

        if(match(PLUS))
        {   advance();
            tempvar2 = term();
            fprintf(output, "    %s += %s\n", tempvar, tempvar2 );
            freename( tempvar2 );
        }

        if(match(MINUS)){
            advance();
            tempvar2 = term();
            fprintf(output, "    %s -= %s\n", tempvar, tempvar2 );
            freename( tempvar2 );
        }
    }

    return tempvar;
}

char    *term()
{
    char  *tempvar, *tempvar2 ;

    tempvar = factor();
    while( match( TIMES ) || match(DIV))
    {

        if(match(TIMES))
        {   advance();
            tempvar2 = factor();
            fprintf(output, "    %s *= %s\n", tempvar, tempvar2 );
            freename( tempvar2 );
        }

        if(match(DIV))
        {   advance();
            tempvar2 = factor();
            fprintf(output, "    %s /= %s\n", tempvar, tempvar2 );
            freename( tempvar2 );
        }

    }

    return tempvar;
}

char    *factor()
{
    char *tempvar;

    if( match(NUM_OR_ID) )
    {
    /* Print the assignment instruction. The %0.*s conversion is a form of
     * %X.Ys, where X is the field width and Y is the maximum number of
     * characters that will be printed (even if the string is longer). I'm
     * using the %0.*s to print the string because it's not \0 terminated.
     * The field has a default width of 0, but it will grow the size needed
     * to print the string. The ".*" tells fprintf(output, ) to take the maximum-
     * number-of-characters count from the next argument (yyleng).
     */

        fprintf(output, "    %s = %0.*s\n", tempvar = newname(), yyleng, yytext );
        advance();
        //fprintf(output, "    %s = %0.*s\n", "check", yyleng, yytext );
    }
    else if( match(LP) )
    {
        advance();
        tempvar = expression();
        if( match(RP) )
            advance();
        else
            fprintf(output, stderr, "%d: Mismatched parenthesis\n", yylineno );
    }
    else
    fprintf(output,  stderr, "%d: Number or identifier expected\n", yylineno );

    return tempvar;
}
