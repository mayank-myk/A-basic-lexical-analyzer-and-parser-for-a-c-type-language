#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"
#include "lex.c"

char* symbol_table[1024];
int position = 0;
int reljmp=0;
FILE *fp2, *fp3;

int find(char* a){
	int pos=0;
	while(pos < position){
		if(strcmp(symbol_table[pos], a) == 0)
			return pos;
		pos++;
	}
	return -1;
}

int symbol_memory(){
	char temparr[yyleng+1];
	int memory_no;
    memcpy( temparr, yytext, yyleng );
    temparr[yyleng] = '\0';
	memory_no = find(temparr);
	if(memory_no == -1){
		symbol_table[position] = (char *)malloc(yyleng+1);
		memcpy( symbol_table[position],temparr, yyleng+1 );
    	fprintf(fp3, "MOV B, %d\n", position*2+610);
    	fprintf(fp3, "MOV [B], 0\n");
    	memory_no = position;
		position++;
	}
	return memory_no*2+610;
}

int get_register_no(){
	return ((*(yytext+2)-48)*2)+600;
}

int get_no(){
    char tempary[yyleng+1];
    memcpy( tempary, yytext, yyleng );
    tempary[yyleng] = '\0';
	return atoi(tempary);
}


void assemble(){
	while( !match (EOI) ){
		if ( match(LABELID) ){
			fprintf(fp3, "%s", yytext);
			advance();
		}
		else if( match(JumpTo) ){
			advance();
			fprintf(fp3, "JMP %s",yytext);
			advance();
		}
		else if( match(IFZ) ){
			advance();
			int op1 = get_register_no();
			fprintf(fp3, "MOV A, 0\n");
			fprintf(fp3, "MOV B, %d\n", op1);
			fprintf(fp3, "CMP A, [B]\n");
			advance();
			if( match(JumpTo) ){
				advance();
				if( match(LABELID) ){
					fprintf(fp3, "JZ %s\n",yytext);
					advance();
				}
				
			}
		}
		else if( match(ID) ){
			int op1 = symbol_memory();
			advance();
			
			if(match(PLUS)){
				advance();
				advance();
				if(match(TEMPID)){
					int op2 = get_register_no();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n");
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "ADD [B], A\n");
					advance();
				}
				else if(match(ID)){
					int op2 = symbol_memory();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n");
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "ADD [B], A\n");
					advance();
				}
			}
			else if(match(MINUS)){
				advance();
				advance();
				if(match(TEMPID)){
					int op2 = get_register_no();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n", op2);
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "SUB [B], A\n");
					advance();
				}
				else if(match(ID)){
					int op2 = symbol_memory();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n");
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "ADD [B], A\n");
					advance();
				}
			}
			else if(match(EQ)){
				advance();
				if(match(ID)){
					int op2 = symbol_memory();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n");
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "MOV [B], A\n");
					advance();
				}
				else if (match(TEMPID))
				{
					int op2 = get_register_no();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n");
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "MOV [B], A\n");
					advance();
				}
			}
			
		}
		else if( match(TEMPID)){
			int op1 = get_register_no();
			advance();
			if(match(PLUS)){
				advance();
				advance();
				if(match(NUM_OR_ID)){
					if(match(ID)){
						int op2 = symbol_memory();
						fprintf(fp3, "MOV B, %d\n", op2);
						fprintf(fp3, "MOV A, [B]\n");
						fprintf(fp3, "MOV B, %d\n", op1);
						fprintf(fp3, "ADD [B], A\n");
						advance();
					}
					else{
						int op2 = get_no();
						fprintf(fp3, "MOV B, %d\n", op1);
						fprintf(fp3, "ADD [B], %d\n", op2);
						advance();
					}
				}
				else if(match(TEMPID)){
					int op2 = get_register_no();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n");
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "ADD [B], A\n");
					advance();
				}
			}
			else if(match(MINUS)){
				advance();
				advance();
				if(match(NUM_OR_ID)){
					if(match(ID)){
						int op2 = symbol_memory();
						fprintf(fp3, "MOV B, %d\n", op2);
						fprintf(fp3, "MOV A, [B]\n");
						fprintf(fp3, "MOV B, %d\n", op1);
						fprintf(fp3, "SUB [B], A\n");
						advance();
					}
					else{
						int op2 = get_no();
						fprintf(fp3, "MOV B, %d\n", op1);
						fprintf(fp3, "SUB [B], %d\n", op2);
						advance();
					}
				}
				else if(match(TEMPID)){
					int op2 = get_register_no();
					fprintf(fp3, "MOV B, %d\n", op2);
					fprintf(fp3, "MOV A, [B]\n");
					fprintf(fp3, "MOV B, %d\n", op1);
					fprintf(fp3, "SUB [B], A\n");
					advance();
				}
			}
			else if(match(TIMES)){
				advance();
				advance();
				if(match(NUM_OR_ID)){
					if(match(ID)){
						int op2 = symbol_memory();
						fprintf(fp3, "MOV B, %d\n" ,op2);
						fprintf(fp3, "MOV AL, [B]\n");
						fprintf(fp3, "MOV B, %d\n" , op1);
						fprintf(fp3, "MUL [B]\n" );
						fprintf(fp3, "MOV [B],AL \n");
						advance();
					}
					else{
						int op2 = get_no();
						fprintf(fp3, "MOV AL, %d\n" ,op2);
						fprintf(fp3, "MOV B, %d\n" , op1);
						fprintf(fp3, "MUL [B]\n" );
						fprintf(fp3, "MOV [B],AL \n");
						advance();
					}
				}
				else if(match(TEMPID)){
					int op2 = get_register_no();
					fprintf(fp3, "MOV B, %d\n" ,op2);
					fprintf(fp3, "MOV AL, [B]\n");
					fprintf(fp3, "MOV B, %d\n" , op1);
					fprintf(fp3, "MUL [B]\n" );
					fprintf(fp3, "MOV [B],AL \n");
					advance();
				}
			}
			else if(match(DIV)){
				advance();
				advance();
				if(match(NUM_OR_ID)){
					if(match(ID)){
						int op2 = symbol_memory();
						fprintf(fp3, "MOV B, %d\n" ,op1);
						fprintf(fp3, "MOV AL, [B]\n");
						fprintf(fp3, "MOV B, %d\n" , op2);
						fprintf(fp3, "DIV [B]\n" );
						fprintf(fp3, "MOV B, %d\n",op1);
						fprintf(fp3, "MOV [B],AL \n");
						advance();
					}
					else{
						int op2 = get_no();
						fprintf(fp3, "MOV B, %d\n" ,op1);
						fprintf(fp3, "MOV AL, [B]\n");
						fprintf(fp3, "MOV B, %d\n" , op2);
						fprintf(fp3, "DIV B\n" );
						fprintf(fp3, "MOV B, %d\n",op1);
						fprintf(fp3, "MOV [B],AL \n");
						advance();
					}
				}
				else if(match(TEMPID)){
					int op2 = get_register_no();
					fprintf(fp3, "MOV B, %d\n" ,op1);
					fprintf(fp3, "MOV AL, [B]\n");
					fprintf(fp3, "MOV B, %d\n" , op2);
					fprintf(fp3, "DIV [B]\n" );
					fprintf(fp3, "MOV B, %d\n",op1);
					fprintf(fp3, "MOV [B],AL \n");
					advance();
				}
			}
			else if(match(EQ)){
				advance();
				if(match(NUM_OR_ID)){
					if(match(ID)){
						int op2 = symbol_memory();
						fprintf(fp3, "MOV B, %d\n" ,op2);
						fprintf(fp3, "MOV A, [B]\n");
						fprintf(fp3, "MOV B, %d\n" ,op1);
						fprintf(fp3, "MOV [B], A\n");
						advance();
					}
					else{
						int op2 = get_no();
						fprintf(fp3, "MOV B, %d\n" ,op1);
						fprintf(fp3, "MOV [B], %d\n", op2);
						advance();
					}
				}
				else if (match(TEMPID)){
					int op2 = get_register_no();
					advance();
					if ( match(GE) || match(LE) || match(EQ) ){
					    int rel[3];
					    rel[2] = '\0';
					    rel[0] = *yytext;
					    advance();
					    fprintf(fp3, "MOV B, %d\n" ,op2);
					    fprintf(fp3, "MOV A, [B]\n");
					    if ( match (EQ) ){
					        rel[1] = *(yytext);
					        advance();
					        int op3 = get_register_no();
					        fprintf(fp3, "MOV B, %d\n" ,op3);
					    	fprintf(fp3, "CMP A, [B]\n");
					        if (strcmp(rel, "==")){
								fprintf(fp3, "JNZ R%d\n",reljmp);
					            
					        }
					        else if (strcmp(rel, ">=")){
								fprintf(fp3, "JL R%d\n", reljmp);
					        }
					        else{
								fprintf(fp3, "JG R%d\n", reljmp);
					        }
					    }
					    else{
					        int op3 = get_register_no();
					        fprintf(fp3, "MOV B, %d\n" ,op3);
					    	fprintf(fp3, "CMP A, [B]\n");
					        if(rel[0] == '='){
					            
					        }
					        else if (rel[0] == '>'){
								fprintf(fp3, "JLE R%d\n", reljmp);
					        }
					        else{
								fprintf(fp3, "JGE R%d\n", reljmp);
					        }
					        
					    }
					    reljmp++;
					    fprintf(fp3, "MOV B, %d\n" ,op1);
						fprintf(fp3, "MOV [B], 1\n");
						fprintf(fp3, "JMP R%d\n", reljmp);
						fprintf(fp3, "R%d:\n" ,reljmp-1);
						fprintf(fp3, "MOV B, %d\n" ,op1);
						fprintf(fp3, "MOV [B], 0\n");
						fprintf(fp3, "R%d:\n", reljmp++);
					}
					else{
					    fprintf(fp3, "MOV B, %d\n" ,op2);
					    fprintf(fp3, "MOV A, [B]\n");
					    fprintf(fp3, "MOV B, %d\n" ,op1);
					    fprintf(fp3, "MOV [B], A\n");
					}
				}
			}
		}
		
	}
}

int main(){
	fp2 = fopen("myoutput.txt", "r");
	fp3 = fopen("assemblyCode.txt", "w");
	assemble();
	fprintf(fp3, "HALT");
	fclose(fp2);
	fclose(fp3);
}