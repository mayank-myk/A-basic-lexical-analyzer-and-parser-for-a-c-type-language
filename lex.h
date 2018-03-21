#define EOI			0	/* End of input			*/
#define SEMI		1	/* ; 				*/
#define PLUS 		2	/* + 				*/
#define TIMES		3	/* * 				*/
#define LP			4	/* (				*/
#define RP			5	/* )				*/
#define NUM_OR_ID	6	/* Decimal Number or Identifier */
#define LE          7   /* >                */
#define GE          8   /* <                */
#define MINUS       9   /* -                */
#define DIV         10   /* /                */
#define EQ          11   /* =                */
#define IF          14   /* if               */
#define THEN        15   /* then             */
#define WHILE       12   /* WHILE            */
#define DO          13   /* do               */
#define BEGIN       16   /* begin            */
#define END         17   /* end              */
#define ID          18   /* identifier       */
#define COL         19   /* : colon       */
#define TEMPID         20   /* : colon       */
#define LABELID         21   /* : colon       */ 
#define IFZ         22   /* : colon       */ 
#define JumpTo         23   /* : colon       */ 


extern char *yytext;		/* in lex.c			*/
extern int yyleng;
extern yylineno;
