/*******************************************
 * File: Token.cpp                         *
 * Author: S. Blythe                       *
 * Date: 12/2024                           *
 * PURPOSE: implementation for Token       *
 *******************************************/

#include "Token.hpp"

#include <fstream>
#include <iomanip>

using namespace std;

// the promised global for string equivalents of TokenType enumeration
string TokStr[]=
{ "ERROR", "EOF_TOK", "NUM_INT", "NUM_REAL", "ADDOP", "MULOP", "ID", "RELOP", "ASSIGNOP", "LPAREN", "RPAREN",  "SEMICOLON",  "LBRACK", "RBRACK", "COMMA", "AND", "OR", "INTEGER", "FLOAT", "WHILE", "IF", "THEN", "ELSE", "VOID", "BEGIN", "END"};

// This is a "list" of the keywords. Note that they are in the same order
//   as found in the TokenType enumaration. 
static string reserved[]={"int" , "float", "while", "if", "then", "else", "void", "begin", "end" };

static int __lineNum=1; // global that keeps track of current line number

// skips next "block" of whitespace in the input stream
//  returns true if any whitespace found, false otherwise. 
bool skipSpace(istream &is) 
{
  //start by assuming no whitespace is found ...
  bool spacesFound=false;

  // read next character in input stream
  char ch = is.get();

  // as long as next char read is whitespace
  while(isspace(ch))
  {
    // we've found some whitespace !
    spacesFound=true;

    // if it was a newline char, there's one more line ...
    if (ch=='\n')
      __lineNum++;

    // now try the next char ...
    ch = is.get();
  }

  // when we get out of the above loop, the next char was not whitespace.
  //  ... but that char is part of teh next token, so put it back into
  //      the input stream.
  is.putback(ch);

  // finally return whether or not we found whitespace
  return spacesFound;
}

// skips next comment in the input stream
//  returns true if comment found, false otherwise. 
bool skipComment(istream & is)
{
  // haven't seen a comment yet ....
  bool commFound=false;

  // get next char in the input stream ...
  char ch = is.get();

  // if it is the start of a comment ...
  if (ch=='#')
  {
    // ... remember we have found a comment. 
    commFound=true;

    // comments run to the end of the current line ...
    // ... so read  (and ignore) the rest of the line. 
    string dummy;
    getline(is, dummy);

    // we did, however, just read through the end of the line, so ...
    // ... we have comleted another line. 
    __lineNum++;
  }
  else // if we didn't see a '#', not a comment, so put the char back. 
    is.putback(ch);

  // return whether or not we saw a comment. 
  return commFound;
}

/******************************************************
 *  just prints out the info describing this Token    *
 *    to specified stream                             *
 *                                                    *
 *   os  - the stream to add the Token to             *
 *                                                    *
 *   returns: the updated stream                      *
 ******************************************************/
ostream&
Token::print(ostream& os) const
{
  os
    << "{ Type:"   << left << setw(10) << TokStr[_type] 
    << " Value:"   << left << setw(10) << _value
    << " Line Number:" << _line_num
	     << " }";
  return os;
}

/******************************************************
 *  Fills in information about this Token by reading  *
 *    it from specified input stream                  *
 *                                                    *
 *   is  - the stream to read the Token from          *
 *                                                    *
 *   returns: nothing                                 *
 *                                                    *
 *     **** YOU MUST CODE THIS !!!!!! ****            *
 ******************************************************/
void Token::get(istream &is)
{
  // you must write this code !!!!
  static int **DFA=nullptr;

  if (DFA==nullptr) // if the table does not exist, build memory for it ...
    {
      DFA = new int*[20];
      for (int state=0; state<20; state++)
	{
	  DFA[state]=new int[256];
	  for(int ch=0; ch<256; ch++)
	    DFA[state][ch] = -1;
	}
      
      // ... and assign the transition values to the table ...


      // ***** from start state (0) ...

      // ID characters
      for(char ch = 'a'; ch<='z'; ch++)
	DFA[0][(int) ch] = 1;
      for(char ch = 'A'; ch<='Z'; ch++)
	DFA[0][(int) ch] = 1;
      
      // numeric characters
      for(char ch='0'; ch<='9'; ch++)
	DFA[0][(int) ch] = 2;
      
      // addop chars
      DFA[0][(int)'+'] = 5;
      DFA[0][(int)'-'] = 5;
      
      // mulop chars
      DFA[0][(int)'*'] = 6;
      DFA[0][(int)'/'] = 6;

      // relop chars
      DFA[0][(int)'<'] = 7;
      DFA[0][(int)'>'] = 7;
      
      // assignop (but might become relop)
      DFA[0][(int)'='] = 9;
      
      // PAREN chars
      DFA[0][(int)'('] = 10;
      DFA[0][(int)')'] = 11;
      
      // AND char
      DFA[0][(int)'&'] = 12;
      
      // OR char
      DFA[0][(int)'|'] = 14;
      
      // SEMICOLON char
      DFA[0][(int)';'] = 16;
      
      // BRACK chars
      DFA[0][(int)'['] = 17;
      DFA[0][(int)']'] = 18;
      
      // COMMA char
      DFA[0][(int)','] = 19;
      


      // ***** from state 1

      // continuing ID chars 
      for(char ch = 'a'; ch<='z'; ch++)
	DFA[1][(int) ch] = 1;
      for(char ch = 'A'; ch<='Z'; ch++)
	DFA[1][(int) ch] = 1;
      for(char ch='0'; ch<='9'; ch++)
	DFA[1][(int) ch] = 1;
      

      // ***** from state 2

      // continuing int chars
      for(char ch='0'; ch<='9'; ch++)
	DFA[2][(int) ch] = 2;
      
      // transitioning to real chars
      DFA[2][(int)'.'] = 3;
      

      // ***** from state 3
      
      // real char after a decimal point
      for(char ch='0'; ch<='9'; ch++)
	      DFA[3][(int) ch] = 4;
      

      // ***** from state 4

      // real chars after first digit after decimal point
      for(char ch='0'; ch<='9'; ch++)
	DFA[4][(int) ch] = 4;
      
      // ***** no transition out of state 5 or 6 


      // ***** from state 7

      // no, it wasn't < or >, it's <= or >=
      DFA[7][(int) '=']=8;
      

      // ***** no transition out of state 8 


      // ***** from state 9

      // not just = but ==
      DFA[9][(int) '=']=8;
      

      // ***** from state 12

      // whew ... we have && not just &
      DFA[12][(int) '&']=13;
      

      // ***** no transition out of state 13


      // ***** from state 14
      // whew ... we have || not just |
      DFA[14][(int) '|']=15;

     
      // ***** no transitions out of any state number 15 or larger. 
    }

  // OK, now that we're certain we have a table to use, use it to get
  // next token. 

  // start by assuming token is an error (adjusted later in code)
  // and we don't yet have a string for a token we have not yet read
  _type=ERROR;
  _value="";
  
  // keep skipping whitespace or comments as long as we find either one
  while (skipSpace(is) || skipComment(is))
    ; // little bit tricky here ... loop body does nothing. 

  // after skipping comments and whitespace, if we have nothing left ...
  if (!is)
    {
      // ... we must be at end of inut stream
      _type=EOF_TOK;
      return;
    }
  
  // our token's line number is the line number we're keeping track of
  _line_num=__lineNum;

  // ready to go through DFA ... start in the start state (0)
  //   ... prev would be in error, since haven't gotten anywhere yet. 
  int curr=0;
  int prev=-1;
  char ch;
  
  // as long as we aren't in an error state (-1) ...
  while(curr!=-1)
    {
      // get next character
      ch = is.get();

      // hop to next state in diagram (represented by the table)
      prev = curr; // prev keeps old state
      curr = DFA[prev][(int) ch]; // updated curr to next state in diagram

      // if curr is still valid, the char is part of the next token
      if (curr!=-1)
	_value+=ch;
    }

  // when we get out of the above loop, an extra char was read. put it back
  //  ... unless we are at end of input stream, then there is no sense in
  //      trying to put a char that does not exist back. 
  if (is)
    is.putback(ch);

  // now, prev holds the DFA state we ended in ... use it to assign
  //  the token's type ...
  switch(prev)
    {
    case 1:
      _type=ID; // state 1 is likley an ID, but ...
      
      // ... it could be a keyword ...
      // check the reserved array of keywords 
      //   - conveniently all keywords are "together" in the TokenType enum
      for(int offset=0; offset<=END-INTEGER; offset++)
	if (_value==reserved[offset])
	  {
	    _type=(TokenType) (INTEGER+offset);
	    break;
	  }
      break;
    case 2:
      _type=NUM_INT;
      break;
    case 4:
      _type=NUM_REAL;
      break;
    case 5:
      _type=ADDOP;
      break;
    case 6:
      _type=MULOP;
      break;
    case 7: case 8: // both are RELOPs
      _type=RELOP;
      break;
    case 9:
      _type=ASSIGNOP;
      break;
    case 10:
      _type=LPAREN;
      break;
    case 11:
      _type=RPAREN;
      break;
    case 13:
      _type=AND;
      break;
    case 15:
      _type=OR;
      break;
    case 16:
      _type=SEMICOLON;
      break;
    case 17:
      _type=LBRACK;
      break;
    case 18:
      _type=RBRACK;
      break;
    case 19:
      _type=COMMA;
      break;
    default:
      _type=ERROR;
    }
  
}
