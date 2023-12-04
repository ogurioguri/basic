/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() = default;

Program::~Program() = default;

void Program::clear() {
    // Replace this stub with your own code
    queue.clear();
	source.clear();
	for( auto element:All_statement){
	  delete element.second;
	}
	All_statement.clear();
	line_number=0;

}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    queue.insert(lineNumber);
	source[lineNumber]=line;
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    queue.erase(lineNumber);
	source.erase(source.find(lineNumber));
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    return source[lineNumber];
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
	if(exist_1(lineNumber)){
	  delete All_statement[lineNumber];
	}
    All_statement[lineNumber]=stmt;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
   return All_statement[lineNumber];
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    if(queue.empty()){
	  return -1;
	}
	else{
	  return *queue.begin();
	}
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    if(next(queue.find(lineNumber))==queue.end()){
	  return -1;
	}
	else{
	  return *(next(queue.find(lineNumber)));
	}
}
bool Program::exist_1(int line_number) {
  return queue.find(line_number)!=queue.end();
}

//more func to add
//todo


