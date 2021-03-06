%option c++ yyclass="pwn_scanner" outfile="pwn_scanner.cpp"
%option debug stack noyywrap yylineno 8bit
%{ 
/* $Id: pwn_scanner.l,v 1.8 2015/05/20 08:04:53 ist176031 Exp $ */
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "pwn_scanner.h"
#include "pwn_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT
%%

		      

">="                    return tGE; 
"<="                    return tLE; 
"=="                    return tEQ; 
"<>"                    return tNE;
"++"			return tINC;

"if"                    return tIF; 
"else"                  return tELSE; 
"!!"                    return tPRINTLN; 
"@"                     return tREAD; 
"begin"                 return tBEGIN; 
"end"			return tEND; 

"repeat"		return tREPEAT;
"next"			return tNEXT;
"stop"			return tSTOP;
"local"			return tLOCAL;
"import"  	 	return tIMPORT;
"noob"			return tNOOB;
"return"		return tRETURN;


\/\/.*$					;						

"/*"					yy_push_state(X_COMMENT);
<X_COMMENT>"/*"				yy_push_state(X_COMMENT);
<X_COMMENT>"*/"				yy_pop_state();
<X_COMMENT>.|\n				;


[A-Za-z_][A-Za-z0-9_]*  		yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                    			yy_push_state(X_STRING); yylval.s = new std::string("");

<X_STRING>\"           			yy_pop_state(); return tSTRING; 

<X_STRING>\\\"         			*yylval.s += yytext + 1;
<X_STRING>"\\"        			*yylval.s += yytext + 1;
<X_STRING>\\n		   	*yylval.s += "\n";
<X_STRING>\\r		   	*yylval.s += "\r";
<X_STRING>\\t		   	*yylval.s += "\t";
<X_STRING>\\[0-9a-fA-F]{1,2}		*yylval.s += strtol(yytext +1, nullptr, 16);

<X_STRING>\0.*\"	   		yy_pop_state(); return tSTRING;
<X_STRING>.            			*yylval.s += yytext;
<X_STRING>\n           			yyerror("newline in string");



0|([1-9][0-9]*)				yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;
0x[0-9a-fA-F]+				yylval.i = strtol(yytext, nullptr, 16); return tINTEGER;

[0-9]+(\.)?([0-9]+)?e-?[0-9]+		yylval.d = strtod(yytext, nullptr); return tREAL;
([0-9]+)?\.[0-9]+(e-?[0-9]+)?		yylval.d = strtod(yytext, nullptr); return tREAL;


[-()<>=+*/%;{}.\[\]#$!&|]       	return *yytext;

[ \n\r\t]+               		; /* ignore whitespace */

.                      			yyerror("Unknown character");

%%
// Very, very dirty hack: flex is a mess generating C++ scanners.
int pwn_scanner::yywrap() { return 1; }
