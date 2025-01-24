/*******************************************
 * File: main.cpp                          *
 * Author: S. Blythe                       *
 * Date: 12/2024                           *
 * PURPOSE: driver file for project 1      *
 *******************************************/

/**********
 **********  DO NOT MODIFY THIS FILE!!!!!
 **********/

#include <iostream>
#include <fstream>

#include "Token.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  // check to make sure an input file was specified. 
  if (argc!=2)
    {
      cerr << "USAGE: " << argv[0] << " <file>" << endl;
      return -1;
    }

  // open input file
  ifstream ifile(argv[1]);

  // if open was not successful, let user know. 
  if (!ifile)
    {
      cerr << "ERROR: Could not open file:\"" << argv[1] << "\"" <<endl;
      return -1;
    }


  // *VERY* simple main function. Just repeatedly call Token's get method!
  Token tok;

  // (try to) get the first token
  tok.get(ifile);

  // did the last get call say there were more tokens? 
  while( tok.type()!=EOF_TOK )
    {
      if (tok.type()!=ERROR)
	{
	  // print out the successfully read Token
	  cout << "Resulting token = " << tok << endl;
	}
      else // tok.type()==ERROR
	{
	  cout << "Syntax error detected on line " << tok.lineNumber() << endl;
	}
      
      // (try to) get the next token 
      tok.get(ifile);
    }

  return 0;
}
