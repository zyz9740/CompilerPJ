#include <iostream>
#include <string.h>
#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include "lexer.h"
using namespace std;

int yylex();
extern "C" FILE* yyin;
extern "C" char* yytext;

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            cerr << "Cannot open file." << endl;
            return 1;
        } else {
            yyin = file;
        }
    } else {
        yyin = stdin;
    }
    
    // output header
    cout<<setw(5)<<left<<"Row"<<setw(5)<<left<<"Col"<<setw(20)<<left<<"Type"<<"Token/Error"<<endl;
    
    int row = 1, col = 1;
    int tokenCount = 0;
    int errCount = 0;
    while (true) {
        int n = yylex();
        string type = "";
        string token = "";
    	bool omit = false;
    	bool error = false;
    	string errMessage = "";
        int len = strlen(yytext);

        if (n == T_EOF) break;
        switch(n) {
            case INTEGER:
                type = "integer";
                token = yytext;
        		if(len > 11){
        		    error = true;
                    errMessage = "Integer Overflow";	
        		}else if(len == 11){
                    long number = atol(yytext);
                    if(number > 2147483647){
                        error = true;
                        errMessage = "Integer Overflow";
                    }
                }
                tokenCount++;
                break;
            case REAL:
                type = "real";
                token = yytext;
                tokenCount++;
                break;
            case WS:
                type = "whitespace";
		        omit = true;
                break;
    	    case COMMENT:
        		type = "comment";
        		token = yytext;
        		omit = true;
                break;
            case NOMATCHINGCOMMENT:
                type = "comment";
                token = yytext;
                error = true;
                errMessage = "No matching comment";
                break;
    	    case RK:
        		type = "reserved keyword";
        		token = yytext;
                tokenCount++;
        		break;
    	    case ID:
        		type = "identifier";
        		token = yytext;
                if(len > 255){
                    error = true;
                    errMessage = "Identifier is too long";
                }
                tokenCount++;
        		break;
    	    case OP:
        		type = "operator";
        		token = yytext;
                tokenCount++;
        		break;
    	    case DELIM:
        		type = "delimiter";
        		token = yytext;
                tokenCount++;
        		break;
    	    case STRING:
        		type = "string";
        		token = yytext;
                if(len > 255 + 2){
                    error = true;
                    errMessage = "String is too long";
                }else{
                    for(int i=0;i<len;i++){
                        if(yytext[i] == '\t' || yytext[i] == '\n'){
                            error = true;
                            errMessage = "Invalid character \\t or \\n";
                            break;
                        }else if(yytext[i]>=0 && yytext[i]<=31 || yytext[i]>=127){
                            error = true;
                            errMessage = "Unprintable character";
                            break;
                        }
                    }
                }
                tokenCount++;
        		break;
            case NOMATCHINGSTRING:
                type = "string";
                token = yytext;
                error = true;
                errMessage = "No matching string";
                break;
            case BOOLEAN:
                type = "boolean";
                token = yytext;
                tokenCount++;
                break;
            case NIL:
                type = "nil";
                token = yytext;
                tokenCount++;
                break;
            default:
                // LEFT case, only contain 1 character
                type = "error";
                token = yytext;
                if(yytext[0]>=0 && yytext[0]<=31 || yytext[0]>=127){
                    error = true;
                    errMessage = "Unprintable character";
                }
        }
        
        // print (rows and cols?)
    	if(!omit){
            if(error){
                cout<<setw(5)<<left<<row<<setw(5)<<left<<col<<setw(20)<<left<<"error"<<errMessage<<endl;
            }
            else{
                cout<<setw(5)<<left<<row<<setw(5)<<left<<col<<setw(20)<<left<<type<<token<<endl;
            }
        }

        if(n == WS || n == COMMENT || n == NOMATCHINGCOMMENT || n == STRING || n == NOMATCHINGSTRING){
            for(int i=0;i<len;i++){
                if(yytext[i] == '\t') col += 4;
                else if(yytext[i] == '\n') {col = 1; row++;}
                else col++;
            }
        }else{
            col += len;
        }
        if(error == true) errCount++;
    }    
    // count num of tokens and errors?
    cout << "Tokens: " << tokenCount << endl << "Errors: " << errCount << endl; 
    return 0;
}
