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
  // you are adding code here ...
}
