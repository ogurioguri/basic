/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

REM::REM(std::string input) {
  content = input;
}
REM::~REM() =default;

void REM::execute(EvalState &state, Program &program) {
  program.line_number=program.getNextLineNumber(program.line_number);
}

LET::LET(Expression * vary , Expression * iter) {
  variable=vary;
  exp=iter;
}
LET::~LET() {
  delete variable;
  delete exp;
}

void LET::execute(EvalState &state, Program &program) {
  state.setValue(((IdentifierExp *)variable)->getName(), exp->eval(state));
  program.line_number=program.getNextLineNumber(program.line_number);
}

INPUT::INPUT(Expression * vary){
  variable=vary;
}

INPUT::~INPUT()  {
  delete variable;
}

void INPUT::execute(EvalState &state, Program &program) {
  bool flag = true;
  while(flag){
	std::string date_input;
	std::cout<<' '<< "?"<<' ';
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
	getline(std::cin,date_input);
	if(date_input.empty()){
	  break;
	}
	TokenScanner scanner_1;
	scanner_1.ignoreWhitespace();
	scanner_1.scanNumbers();
	scanner_1.setInput(date_input);
	Expression * value = parseExp(scanner_1);
	try{
	  if(((IdentifierExp *)variable)->getName()=="LET"){
		error("SYNTAX ERROR");
	  }
	  state.setValue(((IdentifierExp *)variable)->getName(), value->eval(state));
	  delete value;
	  flag= false;
	}catch (ErrorException &ex){
	  std::cout << ex.getMessage() << std::endl;
	  delete value;
	}
  }

  program.line_number=program.getNextLineNumber(program.line_number);
}

PRINT::PRINT(Expression* want){
  content=want;
}

PRINT::~PRINT(){
  delete content;
}

void PRINT::execute(EvalState &state, Program &program) {
  std::cout<<content->eval(state)<<'\n';
  program.line_number=program.getNextLineNumber(program.line_number);
}

END::END(){
}

END::~END(){
}

void END::execute(EvalState &state, Program &program) {
  program.line_number=-1;
}

IF::IF(Expression * input_exp1,char input_opt, Expression * input_exp2 , int input_n){
  exp1=input_exp1;
  exp2=input_exp2;
  n=input_n;
  opt=input_opt;
}
IF::~IF(){
  delete exp1;
  delete exp2;
}

void IF::execute(EvalState &state, Program &program) {
  if(opt=='='){
	if(exp1->eval(state)==exp2->eval(state)){
	  if(program.exist_1(n)){
		program.line_number=n;
	  }
	  else{
		std::cout<<"Line Number Error";
	  }
	}
	else{
	  program.line_number=program.getNextLineNumber(program.line_number);
	}
	return;
  }
  if(opt=='>'){
	if(exp1->eval(state)>exp2->eval(state)){
	  if(program.exist_1(n)){
		program.line_number=n;
	  }
	  else{
		std::cout<<"Line Number Error";
	  }
	}
	else{
	  program.line_number=program.getNextLineNumber(program.line_number);
	}
	return;
  }
  if(opt=='<'){
	if(exp1->eval(state)<exp2->eval(state)){
	  if(program.exist_1(n)){
		program.line_number=n;
	  }
	  else{
		std::cout<<"Line Number Error";
	  }
	}
	else{
	  program.line_number=program.getNextLineNumber(program.line_number);
	}
	return;
  }
}

GOTO::GOTO(int input_n) {
  n=input_n;
}

GOTO::~GOTO(){
}

void GOTO::execute(EvalState &state, Program &program) {
  if (program.exist_1(n)) {
	program.line_number = n;
  } else {
	std::cout << "LINE NUMBER ERROR"<<std::endl;
	program.line_number = program.getNextLineNumber(program.line_number);
  }
}
