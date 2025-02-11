/*******************************************
 * File: Token.hpp                         *
 * Author: S. Blythe                       *
 * Date: 12/2024                           *
 * PURPOSE: Basic header fiel for CSC45500 *
 *   project #1. Defines a Token and some  *
 *   useful types and global arrays        *
 *******************************************/

/**********
 **********  DO NOT MODIFY THIS FILE!!!!!
 **********/


#ifndef _TOKEN_HPP_
#define _TOKEN_HPP_

#include <iostream>
#include <string>

// these represent "numeric" constants for each possible token type. 
enum TokenType { ERROR, EOF_TOK, NUM_INT, NUM_REAL, ADDOP, MULOP, ID, RELOP, ASSIGNOP, LPAREN, RPAREN, SEMICOLON, LBRACK, RBRACK, COMMA, AND, OR, INTEGER, FLOAT, WHILE, IF, THEN, ELSE, VOID, BEGIN, END};


// this is an array of string representations of the token types above.
//   * the strings are in the same order as the above TokenType enumeration
//        for example:  TokStr[NUM_REAL] is "NUM_REAL"
//   * actual definition is in Token.cpp to avoid multiple definition
extern std::string TokStr[];
/* { "ERROR", "EOF_TOK", "STRING", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID" ... } */



class Token
{
private:
  std::string _value;      // actual lexeme value for the Token
  TokenType _type;         // token type, from the above enumeration
  unsigned int _line_num;  // line number token is found on
public:
  // constructor (no lexeme value yet, not valid, and on invalid line number)
  Token():_value(), _type(ERROR), _line_num(0) {}

  // accessors/modifiers for instance variables
  TokenType type() const {return _type;}
  TokenType& type() {return _type;}

  const std::string& value() const {return _value;}
  std::string& value() {return _value;}

  unsigned int lineNumber() const {return _line_num;}
  unsigned int& lineNumber() {return _line_num;}

  // **** You will have to code this !!!!! *****
  //   read next Token from input stream and update instance variables
  void get(std::istream &is);

  // print out current Token to specified stream . 
  std::ostream& print(std::ostream &os) const; 

  // Overload comparison operators for a Token (note: Token must be on LHS)
  //   These are helpful when trying to check what a token is. 
  short int operator==(const TokenType tt) {return _type==tt;}
  short int operator!=(const TokenType tt) {return !(*this==tt);}

  short int operator==(const std::string &s) {return _value==s;}
  short int operator!=(const std::string &s) {return !(*this==s);}
};

// overload << operator for a Token. 
inline std::ostream& operator<<(std::ostream &os, const Token &t)
  { return t.print(os); }

#endif
