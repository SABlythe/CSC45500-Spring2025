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
    default:
      os << "ERROR: ";
    }

  os << '[' << val << ']'; 
}



int getToken(istream &is, string &val)
{
  static int DFA[7][256];
  static bool firstCall=true;

  if (firstCall)
    {
      // fill in table
      for (int row=0; row<7; row++)
	for(int col=0; col<256; col++)
	  DFA[row][col] = ERROR; 

      // state 0 transitions
      for (char ch = 'a'; ch<='z'; ch++)
	DFA[0][(int) ch] = 1;
      for (char ch = 'A'; ch<='Z'; ch++)
	DFA[0][(int) ch] = 1;
      
      for (char ch = '0'; ch<='9'; ch++)
	DFA[0][(int) ch] = 2;

      DFA[0][(int)'-'] = 3;

      DFA[0][(int)'+'] = 4;

      DFA[0][(int)'*'] = 5;

      DFA[0][(int)'/'] = 6;

      // state 1 transitions
      for (char ch = 'a'; ch<='z'; ch++)
	DFA[1][(int) ch] = 1;
      for (char ch = 'A'; ch<='Z'; ch++)
	DFA[1][(int) ch] = 1;
      for (char ch = '0'; ch<='9'; ch++)
	DFA[1][(int) ch] = 1;

      //state 2 transitions
      for (char ch = '0'; ch<='9'; ch++)
	DFA[2][(int) ch] = 2;

      //state 3 transitions
      for (char ch = '0'; ch<='9'; ch++)
	DFA[3][(int) ch] = 2;
      
      firstCall=false;
    }

  int currState = 0;
  int prevState = ERROR;

  while(currState!=ERROR)
    {
      prevState = currState;
      
      // DO NOT USE is >> ch ... it skips whitespace!
      char ch = is.get();

      currState =  DFA[currState][(int) ch];
      
    }

  is.unget();
  
  return prevState;
}
