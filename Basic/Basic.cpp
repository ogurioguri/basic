/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

	std::string result;
	result=scanner.nextToken();
	if(!std::isdigit(result[0])){
	  if(result=="RUN"){
		program.line_number=program.getFirstLineNumber();
		while(program.line_number!=-1){
		  program.getParsedStatement(program.line_number)->execute(state,program);
		}
		return;
	  }
	  if(result=="LIST"){
		program.line_number=program.getFirstLineNumber();
		while(program.line_number!=-1){
		  std::cout<<program.getSourceLine(program.line_number)<<'\n';
		  program.line_number=program.getNextLineNumber(program.line_number);
		}
		return;
	  }
	  if(result=="CLEAR"){
		program.clear();
		state.Clear();
		return;
	  }
	  if(result=="QUIT"){
		program.clear();
		exit(0);
	  }
	  if(result=="LET") {
		Expression * left= readT(scanner);
		scanner.nextToken();
		Expression * right = parseExp(scanner);
		try{
		  if(((IdentifierExp *)left)->getName()=="LET"){
			error("SYNTAX ERROR");
		  }
		  state.setValue(((IdentifierExp *)left)->getName(), right->eval(state));
		  delete left;
		  delete right;
		}catch (ErrorException &ex){
		  std::cout << ex.getMessage() << std::endl;
		  delete left;
		  delete right;
		}
	  }
	  if(result=="PRINT"){
		Expression * name = parseExp(scanner);
		try {
		  std::cout << name->eval(state) << '\n';
		  delete name;
		}catch(ErrorException &ex){
		  delete name;
		  throw;
		}
	  }
	  if(result=="INPUT") {
		Expression *name = parseExp(scanner);
		bool flag = true;
		while (flag) {
		  std::string date_input;
		  std::cout << ' ' << "?" << ' ';
		  /*bool flag = false;
		  while(!flag){
			flag=true;
			getline(std::cin,date_input);
			for(int i=0;i<date_input.size();++i){
			  if(!isdigit(date_input[i])||date_input[i]=='-'){
				flag=false;
			  }
			}
		  }*/
		  getline(std::cin, date_input);
		  if (date_input.empty()) {
			break;
		  }
		  TokenScanner scanner_1;
		  scanner_1.ignoreWhitespace();
		  scanner_1.scanNumbers();
		  scanner_1.setInput(date_input);
		  try {
			if (((IdentifierExp *)name)->getName() == "LET") {
			  error("SYNTAX ERROR");
			}
			if(!date_input.empty()&&date_input[0]!='-'&&!isdigit(date_input[0])){
			  error("INVALID NUMBER");
			}
			for(int i=1;i<date_input.size();++i){
			  if(!isdigit(date_input[i])){
				error("INVALID NUMBER");
			  }
			}
			Expression *value = readE(scanner_1);
			state.setValue(((IdentifierExp *)name)->getName(), ((ConstantExp*)value)->eval(state));
			delete value;
			flag = false;
		  } catch (ErrorException &ex) {
			std::cout << ex.getMessage() << std::endl;
		  }
		}
		delete name;
	  }
	}
	else{
	  int line_number=std::stoi(result);
	  std::string conduct;
	  conduct = scanner.nextToken();
	  if(conduct.empty()&&program.exist_1(line_number)){
		program.removeSourceLine(line_number);
	  }
	  if(conduct=="REM"){
		Statement * thing = new REM(line);
		program.addSourceLine(line_number,line);
		program.setParsedStatement(line_number,thing);

	  }
	  if(conduct=="LET"){
		Expression * left= readT(scanner);
		scanner.nextToken();
		Expression * right = parseExp(scanner);
		Statement * thing = new LET(left,right);
		program.addSourceLine(line_number,line);
		program.setParsedStatement(line_number,thing);
	  }
	  if(conduct=="PRINT"){
		Expression * content = parseExp(scanner);
		Statement * thing = new PRINT(content);
		program.addSourceLine(line_number,line);
		program.setParsedStatement(line_number,thing);
	  }
	  if(conduct=="INPUT"){
		Expression * name = parseExp(scanner);
		Statement * thing = new INPUT(name );
		program.addSourceLine(line_number,line);
		program.setParsedStatement(line_number,thing);
	  }
	  if(conduct=="END"){
		Statement * thing = new END();
		program.addSourceLine(line_number,line);
		program.setParsedStatement(line_number,thing);
	  }
	  if(conduct=="GOTO"){
		Expression * number = parseExp(scanner);
		Statement * thing = new GOTO(number->eval(state));
		program.addSourceLine(line_number,line);
		program.setParsedStatement(line_number,thing);
		delete number;
	  }
	  if(conduct=="IF"){
		Expression * left = readE(scanner,1);
		char opt = scanner.nextToken()[0];
		Expression * right = readE(scanner,1);
		scanner.nextToken();
		int n = std::stoi(scanner.nextToken());
		Statement * thing = new IF(left , opt , right , n);
		program.addSourceLine(line_number,line);
		program.setParsedStatement(line_number,thing);
	  }
	}
}

