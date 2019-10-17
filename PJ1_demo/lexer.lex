%{
#include "lexer.h"
%}
%option     nounput
%option     noyywrap

DIGIT       	[0-9]
LETTER			[a-zA-Z]
INTEGER     	{DIGIT}+
REAL        	{DIGIT}+"."{DIGIT}*
WS          	[ \t\n]
COMMENT	    	"(*"([^\*]|(\*)*[^\*\)])*(\*)*"*)"	
NOMATCHINGCOMMENT	("(*"[^\*\)]*)|"*)"
ID	    	{LETTER}({LETTER}|{DIGIT})*
RK	    	AND|ELSIF|LOOP|PROGRAM|VAR|ARRAY|END|MOD|READ|WHILE|BEGIN|EXIT|NOT|RECORD|WRITE|BY|FOR|OF|RETURN|DIV|IF|OR|THEN|DO|IN|OUT|TO|ELSE|IS|PROCEDURE|TYPE
OP		":="|"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>"
DELIM	    	":"|";"|","|"."|"("|")"|"["|"]"|"{"|"}"|"[<"|">]"|\\
STRING		\"[^\"]*\"
NOMATCHINGSTRING	\"[^\"\t\n]*
BOOLEAN			TRUE|FALSE
NIL				NIL
LEFT			.

%%
{WS}        	return WS; // skip blanks and tabs
 <<EOF>>     	return T_EOF;
{INTEGER}   	return INTEGER;
{REAL}	    	return REAL;
{COMMENT}   	return COMMENT;
{NOMATCHINGCOMMENT}	return NOMATCHINGCOMMENT;
{RK}	    	return RK;
{NIL}			return NIL;
{BOOLEAN}		return BOOLEAN;
{ID}	    	return ID;
{OP}			return OP;
{DELIM}			return DELIM;
{STRING}		return STRING;
{NOMATCHINGSTRING}	return NOMATCHINGSTRING;
{LEFT}			return LEFT;

%%
