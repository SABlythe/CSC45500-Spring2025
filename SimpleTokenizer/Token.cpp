#include "Token.hpp"

using namespace std;

#define ERROR -1

void printToken(int tokType, const std::string &val, std::ostream &os)
{
  if (tokType==EOI)
  {
    os << "EOI" << endl;
    return;
  }
  switch (tokType)
    {
    case ID:
      os << "ID:    ";
      break;
    case INT:
      os << "INT:   ";
      break;
    case MINUS:
      os << "MINUS: ";
      break;
    case PLUS:
      os << "PLUS:  ";
      break;
    case MULT:
      os << "MULT:  ";
      break;
    case DIV:
      os << "DIV:   ";
      break;
    case PLUSPLUS:
      os << "PLUSPLUS:";
      break;
    default:
      os << "ERROR: ";
    }

  os << '[' << val << ']'; 
}



int getToken(istream &is, string &val)
{
  static int DFA[8][256];
  static bool firstCall=true;

  if (firstCall)
    {
      // fill in table
      for (unsigned int row=0; row<8; row++)
	for(unsigned int col=0; col<256; col++)
	  DFA[row][col] = ERROR; 

      // state 0 transitions
      for (char ch = 'a'; ch<='z'; ch++)
	DFA[0][(unsigned int) ch] = 1;
      for (char ch = 'A'; ch<='Z'; ch++)
	DFA[0][(unsigned int) ch] = 1;
      
      for (char ch = '0'; ch<='9'; ch++)
	DFA[0][(unsigned int) ch] = 2;

      DFA[0][(unsigned int)'-'] = 3;

      DFA[0][(unsigned int)'+'] = 4;

      DFA[0][(unsigned int)'*'] = 5;

      DFA[0][(unsigned int)'/'] = 6;

      // state 1 transitions
      for (char ch = 'a'; ch<='z'; ch++)
	DFA[1][(unsigned int) ch] = 1;
      for (char ch = 'A'; ch<='Z'; ch++)
	DFA[1][(unsigned int) ch] = 1;
      for (char ch = '0'; ch<='9'; ch++)
	DFA[1][(unsigned int) ch] = 1;

      //state 2 transitions
      for (char ch = '0'; ch<='9'; ch++)
	DFA[2][(unsigned int) ch] = 2;

      //state 3 transitions
      for (char ch = '0'; ch<='9'; ch++)
	DFA[3][(unsigned int) ch] = 2;

      // state 4 transition
      DFA[4][(unsigned int) '+'] = 7;
      
      firstCall=false;
    }

  // skip white space
  char dummy;
  dummy = is.get();
  while(isspace(dummy) )
    {
      /*
	if (dummy=='\n')
	  lineNumber++;
       */
      dummy = is.get();
    }
  is.unget();

  if (!is)
    return EOI;
  
  
  //cout << "Getting next token ... " << endl;

  int currState = 0;
  int prevState = ERROR;

  val = "";
  
  while(currState!=ERROR)
    {
      prevState = currState;

      
      // DO NOT USE is >> ch ... it skips whitespace!
      char ch = is.get();

      currState =  DFA[currState][(unsigned int) ch];

      /*
      cout << "old state: " << prevState << " on char '" << ch
	   << "'(" << (unsigned int) ch << ") --> " << currState << endl; 
      */
      
      if (currState!=ERROR)
	val += ch;
    }

  is.unget();

  //cout << "... done getting next token " << endl;
  return prevState;
}
