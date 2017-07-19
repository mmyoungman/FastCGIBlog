#include <fcgi_stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int count = 0;
    while(FCGI_Accept() >= 0)
    {

      
      printf("Content-Type: text/plain\n\n");
      printf("Count: %d\n", ++count);

      //char *reqURI = getenv("REQUEST_URI");
      if( strcmp(getenv("REQUEST_URI"), "/admin") == 0 )
        printf("You're in the secret admin area!\n");
      else {
        char *reqMeth = getenv("REQUEST_METHOD");
        if(reqMeth != NULL)
          printf("Request Method: %s\n", reqMeth);
        else
          printf("Request Method is NULL!\n");

        // In nginx:
        // $request_length
        // $request_body
        // Why to discover number of connections? So can hold back if flooded?

        char *reqURI = getenv("REQUEST_URI");
        if(reqURI != NULL)
          printf("Request URI: %s\n", reqURI);
        else
          printf("Request URI is NULL!\n");

        char *scriptName = getenv("SCRIPT_NAME");
        if(scriptName != NULL)
          printf("Script Name: %s\n", scriptName);
        else
          printf("Script Name is NULL!\n");

        char *serverProto = getenv("SERVER_PROTOCOL");
        if(serverProto != NULL)
          printf("Server Protocol: %s\n", serverProto);
        else
          printf("Server Protocol is NULL!\n");

        char *reqScheme = getenv("REQUEST_SCHEME");
        if(reqScheme != NULL)
          printf("Request Scheme: %s\n", reqScheme);
        else
          printf("Request Scheme is NULL!\n");

        char *serverSoft = getenv("SERVER_SOFTWARE");
        if(serverSoft != NULL)
          printf("Server Software: %s\n", serverSoft);
        else
          printf("Server Software is NULL!\n");

        // Client Address
        char *remoteAdd = getenv("REMOTE_ADDR");
        if(remoteAdd != NULL)
          printf("Remote Address: %s\n", remoteAdd);
        else
          printf("Remote Address is NULL!\n");

        char *serverAdd = getenv("SERVER_ADDR");
        if(serverAdd != NULL)
          printf("Server Address: %s\n", serverAdd);
        else
          printf("Server Address is NULL!\n");

        char *path = getenv("PATH");
        if(path != NULL)
          printf("PATH: %s\n", path);
        else
          printf("PATH is NULL!\n");

      }

    }
}