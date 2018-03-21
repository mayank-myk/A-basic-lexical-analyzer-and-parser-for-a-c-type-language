#include "lex.h"
#include <stdio.h>
#include <ctype.h>


char* yytext = ""; /* Lexeme (not '\0'
                      terminated)              */
int yyleng   = 0;  /* Lexeme length.           */
int yylineno = 0;  /* Input line number        */

extern FILE *fp2;

int lex(void){

   static char input_buffer[1024];
   char        *current;

   current = yytext + yyleng; /* Skip current
                                 lexeme        */

   while(1){       /* Get the next one         */
      while(!*current ){
         /* Get new lines, skipping any leading
         * white space on the line,
         * until a nonblank line is found.
         */

         current = input_buffer;
         if(!fgets(input_buffer, 1000, fp2 ) ){
            *current = '\0' ;

            return EOI;
         }
         ++yylineno;
         while(isspace(*current))
            ++current;
      }
      for(; *current; ++current){
         /* Get the next token */
         yytext = current;
         yyleng = 1;
         switch( *current ){
           case ';':
            return SEMI;
           case '+':
            return PLUS;
           case '-':
            return MINUS;
           case '*':
            return TIMES;
           case '/':
            return DIV;
           case '(':
            return LP;
           case ')':
            return RP;
           case '<':
            return LE;
           case '>':
            return GE;
           case '=':
            return EQ;
           case ':':
            return COL;
            case '_':
            ++current;
            if(*current == 't')
            {
                yyleng = 3;
                return TEMPID;
            }
            else
            {
                while(isalnum(*current))
                  ++current;
               yyleng = current - yytext + 1;
                return LABELID;
            }
           case '\n':
           case '\t':
           case ' ' :
            break;
           default:
            if(!isalnum(*current))
               fprintf(stderr, "Not alphanumeric <%c>\n", *current);
            else{
               while(isalnum(*current))
                  ++current;
               yyleng = current - yytext;
               char lexeme[yyleng+1];
               memcpy( lexeme, yytext, yyleng );
               lexeme[yyleng] = '\0';
               char subbuff[yyleng+1];
               memcpy( subbuff, yytext, yyleng );
               subbuff[yyleng] = '\0';
               if(strcmp(lexeme, "while") == 0){
                  return WHILE;
               }
               else if(strcmp(lexeme, "do") == 0){
                  return DO;
               }
               else if(strcmp(lexeme, "if") == 0){
                  return IF;
               }
               else if(strcmp(lexeme, "then") == 0){
                  return THEN;
               }
               else if(strcmp(lexeme, "begin") == 0){
                  return BEGIN;
               }
               else if(strcmp(lexeme, "end") == 0){
                  return END;
               }
               if(strcmp(subbuff, "IFZ") == 0)
               {
                  return IFZ;
               }
               else if(strcmp(subbuff, "JumpTo") == 0)
               {
                  return JumpTo;
               }
               return NUM_OR_ID;
            }
            break;
         }
      }
   }
}


static int Lookahead = -1; /* Lookahead token  */

int match(int token){
   /* Return true if "token" matches the
      current lookahead symbol.                */

   if(Lookahead == -1)
      Lookahead = lex();

   if(token == ID  && Lookahead == NUM_OR_ID ){
      if(isalpha(*yytext)) 
        return 1;
      else
       return 0;
   }
   return token == Lookahead;
}

void advance(void){
/* Advance the lookahead to the next
   input symbol.                               */

    Lookahead = lex();
}

int matchoutput(int token){
   /* Return true if "token" matches the
      current lookahead symbol.                */
      

   if(Lookahead == -1)
      Lookahead = lex();
   if(token == ID && Lookahead == NUM_OR_ID)
   {
       int i;
       char *current = yytext;
       for(i=0;i<yyleng;i++)
       {
           if(!isdigit(*current))
                return 1;
            current++;
       }
       return 0;
       
   }

   return token == Lookahead;
}


