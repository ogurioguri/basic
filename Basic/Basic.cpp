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
		  state.setValue(((IdentifierExp *)left)->getName(), right->eval(state));
		  delete left;
		  delete right;
		}catch (ErrorException &ex){
		  delete left;
		  delete right;
		  throw;
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
	  if(result=="INPUT"){
		int date;
		std::string date_input;
		std::cout<< "?"<<'\n';
		Expression * name = parseExp(scanner);
		bool flag = false;
		while(!flag){
		  flag=true;
		  getline(std::cin,date_input);
		  for(int i=0;i<date_input.size();++i){
			if(!(isdigit(date_input[i])||date_input[i]=='-')){
			  flag=false;
			}
		  }
		}
		date=std::stoi(date_input);
		date=std::stoi(date_input);
		state.setValue(((IdentifierExp *)name)->getName(), date);
		delete name;
	  }
	}
	else{
	  int line_number=std::stoi(result);
	  std::string conduct;
	  conduct = scanner.nextToken();
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

