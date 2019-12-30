/*
This code was written By: Noha Abuaesh

A recursive-descent parser that will handle syntax errors 
for the C- grammar specified in: 
Kenneth Louden, Compiler Construction: Principles and Practice,
 Course Technology. 
*/

#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include<string>
//#include "scan.h"
/*global vars:*/
enum TokenType {ELSE_TK=0, IF_TK, INT_TK, FLOAT_TK, RETURN_TK, VOID_TK, TYPE_SPECIFIER, WHILE_TK, SEMICOL_TK,
		COMMA_TK, PERIOD_TK, LPARA_TK, RPARA_TK, LBRACKET_TK, RBRACKET_TK, LBRACE_TK, 
		RBRACE_TK, LCOMMENT_TK, RCOMMENT_TK, ADDOP, MULOP, RELOP, ASGNOP, ID, NUM};
char* tokens[]={"ELSE_TK", "IF_TK", "INT_TK", "FLOAT_TK", "RETURN_TK", "VOID_TK", "TYPE_SPECIFIER", "WHILE_TK", "SEMICOL_TK",
		"COMMA_TK", "PERIOD_TK", "LPARA_TK", "RPARA_TK", "LBRACKET_TK", "RBRACKET_TK", "LBRACE_TK", 
		"RBRACE_TK", "LCOMMENT_TK", "RCOMMENT_TK", "ADDOP", "MULOP", "RELOP", "ASGNOP", "ID", "NUM"};
char token[20] = "";
int line = 1;
FILE *srcfile;
FILE *outfile;
//int srcfile_marker = 0; //marks the position that scan has reached so far
/*function prototypes*/
void program();
void declaration_list();
void declaration();
void var_declaration();
//void type_specifier();
void params();
void param_list();
void param();
void compound_stmt();
void statement_list();
void statement();
void selection_stmt();
void iteration_stmt();
void assignment_stmt();
void var();
void expression();
//void relop();
void additive_expression();
//void addop();
void term();
//void mulop();
void factor();

void lex_print(char *);
void syn_err(char *, char *);
void match(TokenType);
void scan();
//char* token2string(TokenType);
//TokenType string2token(char*);
//======PROGRAM================================================================
void program()
{
	lex_print("PROGRAM:Enter\t");
//{type_specifier}{blank_str}{identifier}"("{params}")"{blank_str}"{"{declaration_list}{blank_str}{compound_stmt}"}""."
match(TYPE_SPECIFIER);
//match(BLANK_STR);
match(ID);
match(LPARA_TK);
params();
match(RPARA_TK);
//match(BLANK_STR);
match(LBRACE_TK);
declaration_list();
//match(BLANK_STR);
compound_stmt();
match(RBRACE_TK);
match(PERIOD_TK);
lex_print("PROGRAM:Leave");
}
//======DECLARATION_LIST=========================================================
void declaration_list()
{
	lex_print("declaration_list:Enter\t");
//({declaration_list}{blank_str}{declaration})|{declaration}
declaration();
while(strstr(token, tokens[TYPE_SPECIFIER]) != 0){
	//match(BLANK_STR);
	declaration();
	}
lex_print("declaration_list:Leave");
}
//======DECLARATION==============================================================
void declaration()
{
	lex_print("declaration:Enter\t");
//{var_declaration}
var_declaration();
lex_print("declaration:Leave");
}
//======VAR_DECLARATION==========================================================
void var_declaration()
{
	lex_print("var_declaration:Enter\t");
//{type_specifier}{blank_str}{identifier}";"|{type_specifier}{blank_str}{identifier}"["number"]";
match(TYPE_SPECIFIER);
match(ID);
if(strstr(token, tokens[LBRACKET_TK]) != '\0'){
	match(LBRACKET_TK);
	match(NUM);
	match(RBRACKET_TK);
	}
match(SEMICOL_TK);
lex_print("var_declaration:Leave");
}
/*//======TYPE_SPECIFIER===========================================================
void type_specifier()
{
//[iI][nN][tT]|[fF][lL][oO][aA][tT]|[vV][oO][iI][dD]
case token is
"i","I":	match(INT_TK);
"f","F":	match(FLOAT_TK);
"v","V":	match(VOID_TK);
others:		syn_err("a type specifier: int, float or void", token);
}*/
//======PARAMS===================================================================
void params()
{
	lex_print("params:Enter\t");
//{param_list}|[vV][oO][iI][dD]
/*case token is
TYPE_SPECIFIER:		param_list();
"v","V":		match(VOID);
others:			syn_err("parameters list or void", token);
*/
param_list();
lex_print("params:Leave");
}
//======PARAM_LIST===============================================================
void param_list()
{
	lex_print("param_list:Enter\t");
//{param_list}","{param}|{param}
//This means it accepts for example a param list like: void x, void, int y
param();
while(strstr(token, tokens[COMMA_TK]) != '\0'){
	match(COMMA_TK);
	param();
	}
lex_print("param_list:Leave");
}
//======PARAM====================================================================
void param()
{
	lex_print("param:Enter\t");
//{type_specifier}{blank_str}{identifier}|{type_specifier}{blank_str}{identifier}"[""]"
match(TYPE_SPECIFIER);
//match(BLANK_STR);
match(ID);
if(strstr(token, tokens[LBRACKET_TK]) != '\0'){
	match(LBRACKET_TK);
	match(RBRACKET_TK);
	}
lex_print("param:Leave");
}
//======COMPOUND_STMT============================================================
void compound_stmt()
{
	lex_print("compound_stmt:Enter\t");
//"{"{statement_list}"}"
match(LBRACE_TK);
statement_list();
match(RBRACE_TK);
lex_print("compound_stmt:Leave");
}
//======STATEMENT_LIST===========================================================
void statement_list()
{
	lex_print("statement_list:Enter\t");
//{statement_list} {statement}|""
//if(token == ID || token == LBRACE_TK || token == IF_TK || token == WHILE_TK)
statement(); //put in mind that a statement can be nothing, which grants 0+ statements in a statement list
lex_print("statement_list:Leave");
}
//======STATEMENT================================================================
void statement()
{
	lex_print("statement:Enter\t");
//{assignment_stmt}|{compound_stmt}|{selection_stmt}|{iteration_stmt} or nothing.
/*switch(token){
	case ID://assignment statement
		assignment_stmt();
	case LBRACE_TK://compound statement
		compound_stmt();
	case IF_TK://selection statement
		selection_stmt();
	case WHILE_TK://iteration statement
		iteration_stmt();
	default://null statement to fix the nullable rule for statement_list, ignore
		return; //ignore
}*/
	if(strstr(token, tokens[ID]) != '\0')	assignment_stmt();	else{ 
		if(strstr(token, tokens[LBRACE_TK]) != '\0')	compound_stmt(); else{ 
			if(strstr(token, tokens[IF_TK]) != '\0')	selection_stmt(); else{
				if(strstr(token, tokens[WHILE_TK]) != '\0')	iteration_stmt(); //else return;
			}
		}
	}
				
lex_print("statement:Leave");
}
//======SELECTION_STMT===========================================================
void selection_stmt()
{
	lex_print("selection_stmt:Enter\t");
//[iI][fF]{blank_str}"("{expression}")"{statement}|[iI][fF]{blank_str}"("{expression}")"{statement}{blank_str}[eE][lL][sS][eE]{blank_str}{statement}
match(IF_TK);
match(LPARA_TK);
expression();
match(RPARA_TK);
statement();
if(strstr(token, tokens[ELSE_TK]) != '\0'){
	match(ELSE_TK);
	statement();
	}
lex_print("selection_stmt:Leave");
}
//======ITERATION_STMT===========================================================
void iteration_stmt()
{
	lex_print("iteration_stmt:Enter\t");
//[wW][hH][iI][lL][eE]{blank_str}"("{expression}")"{statement}
match(WHILE_TK);
match(LPARA_TK);
expression();
match(RPARA_TK);
statement();
lex_print("iteration_stmt:Leave\t");
}
//======ASSIGNMENT_STMT==========================================================
void assignment_stmt()
{
	lex_print("assignment_stmt:Enter\t");
//{var}"="{expression}
var();
match(ASGNOP);
expression();
lex_print("assignment_stmt:Leave\t");
}
//======VAR======================================================================
void var()
{
	lex_print("var:Enter\t");
//{identifier}|{identifier}"["{expression}"]"
match(ID);
if(strstr(token, tokens[LBRACKET_TK]) != '\0'){
	match(LBRACKET_TK);
	expression();
	match(RBRACKET_TK);
	}
lex_print("var:Leave\t");
}
//======EXPRESSION===============================================================
void expression()
{
	lex_print("expression:Enter\t");
//{expression}{relop}{additive_expression}|{additive_expression}
additive_expression();
while(strstr(token, tokens[RELOP]) != '\0'){
	match(RELOP);
	additive_expression();
	}
lex_print("expression:Leave\t");
}
//======RELOP====================================================================
/*void relop()
{
//"<="|"<"|">="|">"|"=="|"!="
}*/
//======ADDITIVE_EXPRESSION======================================================
void additive_expression()
{
	lex_print("additive_expression:Enter\t");
//{additive_expression}{addop}{term}|{term}
term();
while(strstr(token, tokens[ADDOP]) != '\0'){
	match(ADDOP);
	term();
	}
lex_print("additive_expression:Leave\t");
}
//======ADDOP====================================================================
/*void addop()
{
//"+"|"-"
}*/
//======TERM=====================================================================
void term()
{
	lex_print("term:Enter\t");
//{term}{mulop}{factor}|{factor}
factor();
while(strstr(token, tokens[MULOP]) != '\0'){
	match(MULOP);
	factor();
	}
lex_print("term:Leave\t");
}
//======MULOP====================================================================
/*void mulop()
{
//"*"|"/"
}*/
//======FACTOR===================================================================
void factor()
{
	lex_print("factor:Enter\t");
//"("{expression}")"|{var}|{number}
/*switch(token){
	case LPARA_TK:	
		{match(LPARA_TK);
		expression();
		match(RPARA_TK);
		}
	case ID:		var();
	case NUM:		match(NUM);
	default:	syn_err("left paranthesis, variable or a number", token);
	}*/
	if(strstr(token, tokens[LPARA_TK]) != '\0'){
		match(LPARA_TK);
		expression();
		match(RPARA_TK);
	} else{
		if(strstr(token, tokens[ID]) != '\0') var(); else{
			if(strstr(token, tokens[NUM]) != '\0') match(NUM); else
				syn_err("left paranthesis, variable or a number", token);
		}
	}
	lex_print("factor:Leave\t");
}
//===============================================================================
//======MATCH====================================================================
//===============================================================================
void match(TokenType expected_token)
{
	//fprintf(stderr, "matching %s...  ", tokens[expected_token]);
if(strstr(token, tokens[expected_token]) != '\0'){
	//lex_print("matched successfully :)\n");
	//strcpy(token,scan());
	scan();
}
else{
	lex_print("PROBLEM: does not match :(\n");
	//syntax error
	syn_err(tokens[expected_token], token);
}
if(strcmp(token, "") == 0)	//end of file reached--useless
	return;
}
//===============================================================================
void scan(){
	//returns next token, keeps track and increments the line number, 
	//remembers the position it has reached reading since last  call(s)
	char t[50] = "";
	char c;
	int i = 0;
	
	//for(int i = 0; i <= srcfile_bookmark; i++) fscanf(srcfile, "%c", &c);
	
	//fscanf(srcfile, "%c", &c);
	//fprintf(outfile, "\nJust scanned: %c\n\n", c);
	//convert c from character into string
	//string str; str.append(1, c);
	//concat the result to t
	//append(t, c);
	//srcfile_bookmark++;
		while(c != EOF){
			fscanf(srcfile, "%c", &c);
			//fprintf(outfile, "\nJust scanned: %c\n\n", c);

			if(c == '\n'){
				fprintf(outfile, "\n\nNEWLINE SEEN!\n\n");
				line++; 
				continue;
			}
			if(c == ' ') {
				//fprintf(outfile, "\n\nSPACE SEEN!\n\n");
				if(i == 0)	
					continue;
				else break;
			}
			//append c to t
			//t = t + c;
			t[i] = c;
			i++;
			//srcfile_bookmark++;
		}
		t[i] = '\0';
		//fprintf(outfile, "\n\nt is now = %s\n", t);
		strcpy(token, t);
		//fprintf(outfile, "FROM SCAN: token is now = %s\nline %d", token, line);
	//return t;
}
//===============================================================================
char* token2string(TokenType t)
{
	/*enum TokenType {ELSE_TK=300, IF_TK, INT_TK, FLOAT_TK, RETURN_TK, VOID_TK, TYPE_SPECIFIER, WHILE_TK, SEMICOL_TK,
		COMMA_TK, PERIOD_TK, LPARA_TK, RPARA_TK, LBRACKET_TK, RBRACKET_TK, LBRACE_TK, 
		RBRACE_TK, LCOMMENT_TK, RCOMMENT_TK, ADDOP, MULOP, RELOP, ASGNOP, ID, NUM};*/
	char* map[] = {"else", "if", "int", "float", "return", "void", "type specifier", 
		"while", ";", ",", ".", "(", ")", "[", "]", "{", "}", "/*", "*/", "+ or -", 
		"* or /", "relational operator", "identifier", "number"};

	return map[t];

}
//===============================================================================
/*TokenType string2token(char* s)
{
	TokenType t;
	switch(s){
	case "ELSE_TK":		t = ELSE_TK;
	case "IF_TK":		t = IF_TK;
	case "TYPE_SPECIFIER":	t = TYPE_SPECIFIER;
	case "RETURN_TK":		t = RETURN_TK;
	case "WHILE_TK":		t = WHILE_TK;
	case "SEMICOL_TK":		t = SEMICOL_TK;
	case "COMMA_TK":		t = COMMA_TK;
	case "PERIOD_TK":		t = PERIOD_TK;
	case "LPARA_TK":		t = LPARA_TK;
	case "RPARA_TK":		t = RPARA_TK;
	case "LBRACKET_TK":		t = LBRACKET_TK;
	case "LBRACE_TK":		t = LBRACE_TK;
	case "RBRACE_TK":		t = RBRACE_TK;
	case "LCOMMENT_TK":		t = LCOMMENT_TK;
	case "RCOMMENT_TK":		t = RCOMMENT_TK;
	case "ADDOP":		t = ADDOP;
	case "MULOP":		t = MULOP;
	case "RELOP":		t = RELOP;
	case "ASGNOP":		t = ASGNOP;
	case "ID":		t = ID;
	case "NUM":		t = NUM;
	//default:		
	}

	return t;
}*/
//===============================================================================
void syn_err(char* s1, char* s2){
	fprintf(stderr, "line %d:\tExpected %s, but found %s", line, s1, s2);
	fprintf(stderr, "\n");
	//total_errors++;
}
//===============================================================================
void lex_print(char* str){
	fprintf(outfile, "%s\tNext = %s", str, token);
	fprintf(outfile, "\n");
}
/*//===============================================================================
void lex_err(char* s1, char* s2){
	fprintf(stderr, "line %d, character %d:\t%s%s\n", line, epos, s1, s2);
	total_errors++;
}
/*int yywrap(){
	return 1;
}*/
//===============================================================================
bool lex_init(char* src, char* out){
	srcfile = fopen(src, "r");
	outfile = fopen(out, "w");
	bool done = true;
	if(srcfile == 0){
		printf("Can't open %s\n", src);
		done = false;
	}else //read the file name
	if(outfile == false){
		printf("Can't write to %s", out);
		done = false;
	}
	return done;
}
//===============================================================================
//===============================================================================
int main(int argc, char **argv) {
        ++argv; --argc;  
       /* if (argc > 0) {
            srcfile = fopen(argv[0], "r");
        } else {
            srcfile = stdin;
        }*/
        //char* srcfile;
        //char* outfile;
        
        char f1[50]="";
        char f2[50]="";
        
        printf("Enter the source file name:\t");
        scanf("%s", f1);
		//gets(srcfile);
        
        printf("Enter the output file name:\t");
        scanf("%s", f2);
        //gets(outfile);
        
        if(lex_init(f1,f2) == true){
			//yylex();
			//strcpy(token,scan());
			scan();
			//fprintf(outfile, "\n\nFROM MAIN: t is now = %s\n", token);
			program();
		}
		else	printf("Cannot initialize files.\n");
		
		//printf("\n%d Token Errors.", total_errors);
	return 0;
    }