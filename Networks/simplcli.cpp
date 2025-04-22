// FILE:   simple.cpp
// AUTHOR: Dr. S. A. Blythe
// DATE:   8/5/07 - 8/7/07
//
// A simple client program that utilizes Unix sockets to connect to a
// simple server. Such a server simply responds to a connection when
// one is made by sending a string back

// includes to give data type and function prototypes for sockets
#include <netdb.h>
#include <unistd.h>

#include <iostream>
using namespace std;

// various error codes that correspond to socket problems
#define SUCCESS 0
#define USAGE_ERROR 1
#define CREATE_ERROR 2
#define HOSTNAME_ERROR 3
#define SOCK_ERROR 4
#define READ_ERROR 5

// forward definition of the actual client code
int do_client(char*);

int main(int argc, char *argv[])
{
  // check to make sure program has been invoked properly
  if (argc!=2)
    {
      cout << "Usage: " << argv[0] << " <TOD-server>" << endl;
      exit (USAGE_ERROR);
    }
  
  // invoke client code
  return do_client(argv[1]);

}


// FUNCTION  : do_client()
// PARAMETERS: 
//    char *server:  the name of the time of day server to connect to
int do_client(char *server)
{
  // holds return value (error code) from various socket functions
  int rVal=0;

  // data structure to hold internal address info for server
  struct hostent *serverEntry;
  // socket information; server side will be explcitly set
  struct sockaddr_in serverInfo;

  // convert char string of server to socket data structure representation
  serverEntry = gethostbyname(server);

  // if we couldn't find the server information ...
  if(!serverEntry)
    {
      cout << "Failed gethostbyname(), server=" << server << endl;
      exit(HOSTNAME_ERROR);
    }

  //create the socket we intend on using ...
  //   AF_INET ==> IPv4 
  //  SOCK_STREAM ==> TCP (a.k.a. "stream socket") 
  //  0 ==> use default flags NOTE that
  //
  // a socket is uniquely represented by an integer, which is
  // internally treated as an open file descriptor ...
  int s = socket(AF_INET, SOCK_STREAM, 0);

  // if the socket creation failed  
  if(s < 0)
    {
      cout << "Failed socket()" << endl;
      exit(CREATE_ERROR);
    }

  //Fill in the sockaddr_in struct, which internal represents an
  //internet address
  serverInfo.sin_family = AF_INET;  // IPv4
  serverInfo.sin_addr =  * (struct in_addr *)serverEntry->h_addr_list[0];
  serverInfo.sin_port = htons(13); // Host TO Network Short (integer)

  // now, actually communicate with the server, trying to open a connection 
  // to it. 
  rVal=connect(s, (struct sockaddr *) &serverInfo, sizeof(serverInfo));
 
  // if we couldn't make a connection ...
  if(rVal<0)
    {
      cout << "Failed connect()" << endl;
      exit(SOCK_ERROR);
    }

  // at this point, we have an active connection. By protocol, the
  // server will simply respond ... so we need to read everything
  // being sent to us

  int n;             // number of characters read
  char recvln[81];   // buffer to hold characters

  // as long as read returns some characters, keep going
  while ( (n=read(s,recvln,80))  > 0)
    {
      // null terminate the string that has been read
      recvln[n]=0;
      // dump it out to the screen
      cout << recvln;
    }

  // if the last read performed gave an error (0 indicates EOF)
  if (n<0)
    {
      cout << "read() error" << endl;
      exit(READ_ERROR);
    }

  // close the socket
  close(s);

  // if we get here, things worked just fine. 
  return SUCCESS;
}


