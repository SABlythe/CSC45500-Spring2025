// FILE:  cserver.cpp
// AUTHOR: Dr. S. A. Blythe
// DATE:   6/8/07
//
// A simple concurrent server program that utilizes Unix sockets and
// returns each connecting client's IP address back to them.
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
using namespace std;


// various error codes that correspond to socket problems
#define SUCCESS 0
#define USAGE_ERROR 1
#define SOCK_ERROR 2
#define BIND_ERROR 3
#define LISTEN_ERROR 4

// limits the number of clients that can be waiting to connect to the server
#define MAX_WAITING 25

// structure used to pass data to pthread activated function
struct mywork_struct
{
  int with_sock;                // socket for function to use
  struct sockaddr_in *from_cli; // client address info to use
};

// forward definition of the actual server code
int do_server(unsigned int on_port);

// do the actual serving of the data, given a client
// wierd prototype is to keep pthread happy with the function "type"
void* do_work(void *);


int main(int argc, char *argv[])
{
  // check to make sure program has been invoked properly
  if (argc!=2)
    {
      cout << "Usage: " << argv[0] << " <port number>" << endl;
      exit (USAGE_ERROR);
    }

  // invoke server code
  return do_server(atoi(argv[1]));
}


// FUNCTION  : do_server(unsigned int on_port)
// PARAMETERS: 
//    unsigned int on_port:  the port number to utilize
// PURPOSE: deal with clients as they connect to a listening socket
int do_server(unsigned int on_port)
{
  // the listening socket
  int listen_sock;

  //The sockaddr_in specifies the address of the socket's
  //server address for TCP/IP sockets.
  struct sockaddr_in local_addr;

  //The socket() function creates our listening socket
  listen_sock=socket(AF_INET,SOCK_STREAM,0);
      
  //If the socket() function fails we exit
  if(listen_sock<0)
    {
      cout << "Could not create listening socket!" << endl;
      return SOCK_ERROR;
    }

  // Fill in local (server) half of socket info
  local_addr.sin_family=AF_INET;         //IPv4 Address family
  local_addr.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
  local_addr.sin_port=htons(on_port);    //port to use

  // bind (i.e. "fill in") our socket info to our socket
  if(bind(listen_sock,(sockaddr*)&local_addr,sizeof(local_addr))!=0)
    {
      cout << "Binding failed - this could be caused by:" << endl
	   << "  * an invalid port (no access, or already in use?)" << endl
	   << "  * an invalid local address (did you use the wildcard?)" 
	   << endl;
      return BIND_ERROR;
    }

  //listen for a new client connection 
  if(listen(listen_sock,MAX_WAITING)!=0)
    {
      cout << "Listen error" << endl;
      return LISTEN_ERROR;
    }
  
  
  while(true)//keep handling connections forever
    {
      int connected_sock;       // socket for the actual connected client
      struct sockaddr_in from;  // holds client address data
      unsigned int from_len;    // holds size of client address data

      // really only needed for windows, but doesn't break things here ...
      from_len=sizeof(from);
      
      // wait for the listening socket to get an attempted
      //   client connection
      connected_sock=accept(listen_sock,
			    (struct sockaddr*)&from,
			    &from_len);

      // build up the paramater to pass to our thread-invoked function
      struct mywork_struct *params=new struct mywork_struct;
      params->with_sock=connected_sock;
      params->from_cli=&from;

      // create (and run) the thread. returns immediately, but thread
      // continues to run!
      pthread_create(new pthread_t, NULL, do_work, (void *) params);

  }

  // if we get here, things worked just fine. Of course, we should
  // never get here ...
  return SUCCESS;
}

// FUNCTION : do_work(void *param) 
// PARAMETERS: void *param: actually points to a mywork_struct, which
//                 contains the socket and client address we need to
//                 communicate with. 
// PURPOSE: generate reply string and send it to client. 
void* do_work(void *generic_ptr)
{
  // convert the "generic paramter" to what we actually have
  struct mywork_struct *mwsp=(struct mywork_struct *) generic_ptr;

  // extract the components we need. 
  int connsock=mwsp->with_sock;
  struct sockaddr_in *client_addr=mwsp->from_cli;

  // get rid of the generic pointer (avoids memeory leak).
  delete mwsp;

  // what the server generates ...
  string buffer;
  
  buffer="Your IP is ";
  buffer+=inet_ntoa(client_addr->sin_addr);
  buffer+='\n';
  
  //we simply send this string to the client
  char *cbuff=(char *)buffer.c_str();

  int needed=buffer.length();
  while(needed) // as long as writing is not yet completed, 
    { 
      // keep wriitng more of the buffer
      int n=write(connsock, cbuff, needed);
      needed-=n;
      cbuff+=n;
    }

  // make a local log of who connected ...
  cout << "Connection from " << inet_ntoa(client_addr->sin_addr) << endl;
  
  //close the connected client socket (NOT the listening socket!)
  close(connsock);

  // we have to return something ... the thread function requires it!
  return NULL;
}

