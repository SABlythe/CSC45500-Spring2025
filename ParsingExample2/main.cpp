#include <fstream>
#include <iostream>
#include <string>

#include "Token.hpp"

using namespace std;

string idlist(istream &is)
{
  Token nextT;

  unsigned int position = is.tellg();
  nextT.get(is);
  
  if (nextT!=ID)
    {
      is.seekg(position); 
      return "";
    }

  string answer = nextT.value();

  position = is.tellg();
  nextT.get(is); // consume a COMMA, read next Token.

  if (nextT!=COMMA) // oops we shouldn't have read the token!
    {
      is.seekg(position); 
      return answer;
    }

  answer += ", ";
  
  string idl = idlist(is);
  answer +=  idl;
  
  return answer;
}

int main(int argc, char *argv[])
{
  ifstream ifile (argv[1]); 

  string pretty = idlist(ifile); 

  cout << pretty << endl;
  
  return 0;
}
