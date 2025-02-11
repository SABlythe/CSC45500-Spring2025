#include <fstream>
#include <iostream>
#include <string>

#include "Token.hpp"

using namespace std;

Token nextT;

string idlist(istream &is)
{
  if (nextT!=ID)
    {
      return "";
    }

  string id = nextT.value();
  
  nextT.get(is); // consume an ID, read next Token.

  if (nextT!=COMMA)
    {
      return id;
    }

  nextT.get(is);  // consume a COMMA, read next Token.

  string idl = idlist(is);

  string answer = id + ", " + idl;
  return answer;
}

int main(int argc, char *argv[])
{
  ifstream ifile (argv[1]); 

  nextT.get(ifile);

  string pretty = idlist(ifile); 

  cout << pretty << endl;
  
  return 0;
}
