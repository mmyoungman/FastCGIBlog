// Created by Mark Youngman 18 Feb 2018
// Opens a port at localhost:9000

#include "lib-mmy.h"
#include <string.h> // for memset()
#include <sys/types.h>
#include <sys/wait.h> // for waitpid()
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> // for close()

#define MYPORT "9000"
#define BACKLOG 10

// http://www.mit.edu/~yandros/doc/specs/fcgi-spec.html
typedef struct {
   unsigned char version;
   unsigned char type;
   unsigned char requestIdB1;
   unsigned char requestIdB0;
   //u16 requestId;
   unsigned char contentLengthB1;
   unsigned char contentLengthB0;
   //u16 contentLength;
   unsigned char paddingLength;
   //u8 paddingLength;
   unsigned char reserved;
   //unsigned char contentData[contentLength];
   unsigned char contentData[8];
   //unsigned char paddingData[paddingLength];
} FCGI_Record;

typedef struct {
   unsigned char roleB1;
   unsigned char roleB0;
   //u16 role;
   unsigned char flags;
   unsigned char reserved[5];
} FCGI_BeginRequestBody;

int main()
{
   int status;
   int sockfd;
   struct addrinfo hints;
   struct addrinfo *servinfo;

   int newfd;
   struct sockaddr_storage clientaddr;
   socklen_t clientaddrsize;

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   status = getaddrinfo(NULL, MYPORT, &hints, &servinfo);
   if (status != 0)
   {
      dbg("%s", gai_strerror(status));
      return 1;
   }

   sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
   if (sockfd < 0)
   {
      perror("socket");
      return 1;
   }

   // use setsockopt() here to handle "Address already in use" case when bind() is called
   int yes = 1;
   status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
   if (status < 0)
   {
      perror("setsockopt");
      return 1;
   }

   status = bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
   if (status < 0)
   {
      perror("bind");
      return 1;
   }

   freeaddrinfo(servinfo);

   status = listen(sockfd, BACKLOG);
   if (status < 0)
   {
      perror("listen");
      return 1;
   }
   dbg("Waiting for incoming connections...");

   while (1)
   {
      newfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientaddrsize);
      if (newfd < 0)
      {
         perror("accept");
         return 1;
         //continue;
      }
      pid_t pid = fork();
      if (pid < 0)
      {
         perror("fork");
         return 1;
      }
      if (pid != 0)
      {
         //dbg("This the parent process!");

         // need to save child pid?
         // clean up zombie processes?

         waitpid(-1, NULL, WNOHANG);
         continue;
      }

      // child doesn't need sockfd
      dbg("Connection accepted!");

      int bufferLen = 4096;
      char buffer[bufferLen];
      while ((status = recv(newfd, &buffer, bufferLen, 0)) != 0)
      {
         if (status < 0)
         {
            perror("recv");
            return 1;
            //continue;
         }
         FCGI_Record *record;
         record = (FCGI_Record*) buffer;
         
         assert(record->version == 1);

         dbg("recv status: %d", status);

         dbg("version: %d", record->version);
         dbg("type: %d", record->type);

         //dbg("requestIdB1: %d", record->requestIdB1);
         //dbg("requestIdB0: %d", record->requestIdB0);
         u16 requestId = (record->requestIdB1 << 8) + record->requestIdB0;
         dbg("requestId: %d", requestId);

         //dbg("contentLengthB1: %d", record->contentLengthB1);
         //dbg("contentLengthB0: %d", record->contentLengthB0);
         u16 contentLength = (record->contentLengthB1 << 8) + record->contentLengthB0;
         dbg("contentLength: %d", contentLength);

         dbg("paddingLength: %d", record->paddingLength);
         dbg("reserved: %d", record->reserved);
         //dbg("contentData: %d", record->contentData);

         //dbg("role: %d", *((u16*) &record->contentData));
         FCGI_BeginRequestBody *begin = (FCGI_BeginRequestBody*) &(record->contentData);
         u16 role = (begin->roleB1 << 8) + begin->roleB0;
         dbg("role: %d", role);
         dbg("flags: %d", begin->flags);
         //dbg("flags: %d", begin->flags);

         //for(int i = 0; i < 32; i++) {
         //   dbg("i: %d, %d", i, *ptr);
         //   ptr++;
         //}

         unsigned char *ptr = (unsigned char*)record;
         ptr += 16;

         FCGI_Record *nextrecord = (FCGI_Record*) ptr;
         //FCGI_Record *nextrecord = record + 16;


         dbg("Should now go on to the FCGI_PARAMS record...");
         dbg("version: %d", nextrecord->version);
         dbg("type: %d", nextrecord->type);

         //dbg("requestIdB1: %d", nextrecord->requestIdB1);
         //dbg("requestIdB0: %d", nextrecord->requestIdB0);
         requestId = (nextrecord->requestIdB1 << 8) + nextrecord->requestIdB0;
         dbg("requestId: %d", requestId);

         //dbg("contentLengthB1: %d", nextrecord->contentLengthB1);
         //dbg("contentLengthB0: %d", nextrecord->contentLengthB0);
         contentLength = (nextrecord->contentLengthB1 << 8) + nextrecord->contentLengthB0;
         dbg("contentLength: %d", contentLength);

         dbg("paddingLength: %d", nextrecord->paddingLength);
         dbg("reserved: %d", nextrecord->reserved);

         ptr += 8;

         //dbg("contentData: %s", ptr);
         for(int i = 0; i < 688; i++) {
            printf("%c", *ptr);
            ptr++;
         }

         //status = send(newfd, &buffer, status, 0);
         memset(buffer, 0, sizeof(char) * 4096);
      }

      dbg("Connection closed!");

      //shutdown(sockfd, 2);
      close(sockfd);
   }
}
