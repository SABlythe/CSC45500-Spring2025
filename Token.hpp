#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <iostream>
#include <string>

#define ID 1
#define INT 2
#define MINUS 3
#define PLUS 4
#define MULT 5
#define DIV 6

#define EOI 0



void printToken(int tokType, const std::string &val, std::ostream &os=std::cout);


int getToken(std::istream &is, std::string &val); 


#endif
