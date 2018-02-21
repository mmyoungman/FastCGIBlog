// Created by Mark Youngman 18 Feb 2018

#include "lib-mmy.h"
#include <sys/types.h>
#include <sys/wait.h> // for waitpid()
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h> // for close()

#define MYPORT "9000"
#define BACKLOG 10

// http://www.mit.edu/~yandros/doc/specs/fcgi-spec.html
#define FCGI_VERSION_1     1

// Listening socket file number
#define FCGI_LISTENSOCK_FILENO   0

typedef struct {
   unsigned char version;
   unsigned char type;
   unsigned char requestIdB1;
   unsigned char requestIdB0;
   unsigned char contentLengthB1;
   unsigned char contentLengthB0;
   unsigned char paddingLength;
   unsigned char reserved;
} FCGI_Header;

#define FCGI_HEADER_LEN    8

// Values for fcgi_header type 
#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11
//#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)

typedef struct {
   unsigned char roleB1;
   unsigned char roleB0;
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

   // TODO: Make sockets stuff more robust.
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

   // INFO: Use setsockopt() here to handle "Address already in use" case when bind() is called.
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
      // INFO: accept(...) is blocking
      newfd = accept(sockfd, (struct sockaddr *)&clientaddr, &clientaddrsize);
      if (newfd < 0)
      {
         perror("accept");
         return 1;
      }
      pid_t pid = fork();
      if (pid < 0)
      {
         perror("fork");
         return 1;
      }
      if (pid != 0)
      {
         // INFO: This the parent process!

         // TODO: need to save child pid?
         // TODO: clean up zombie processes?

         waitpid(-1, NULL, WNOHANG);
         continue;
      }

      // TODO: child doesn't need sockfd?

      dbg("Connection accepted!");

      // TODO: Handle data spread over multiple packets?
      int bufferLen = 4096;
      char buffer[bufferLen];
      while ((status = recv(newfd, &buffer, bufferLen, 0)) != 0)
      {
         if (status < 0)
         {
            perror("recv");
            return 1;
         }

         // INFO: Loop over fcgi records until all packets bytes have been consumed
         // TODO: Handle multiple requestIds
         void *data_ptr = buffer;
         while(status > 0) {
            FCGI_Header *header = (FCGI_Header*) data_ptr;

            assert(header->version == FCGI_VERSION_1);

            u16 requestId = (header->requestIdB1 << 8) + header->requestIdB0;
            u16 contentLength = (header->contentLengthB1 << 8) + header->contentLengthB0;
            u8 paddingLength = header->paddingLength;

            data_ptr += FCGI_HEADER_LEN;

            u16 role;
            u8 flags;
            FCGI_BeginRequestBody *begin;

            unsigned char *param_ptr;
            u32 param_pos;
            u32 nameLength;
            u32 valueLength;
            char *param_name;


            switch(header->type) {
               case FCGI_BEGIN_REQUEST:
                     dbg("FCGI_BEGIN_REQUEST");
                     begin = (FCGI_BeginRequestBody*) data_ptr;
                     role = (begin->roleB1 << 8) + begin->roleB0;
                     flags = begin->flags;
                     dbg("role: %d", role);
                     dbg("flags: %d", flags);
                     break;
               case FCGI_ABORT_REQUEST:
                     dbg("FCGI_ABORT_REQUEST");
                     break;
               case FCGI_PARAMS:
                     dbg("FCGI_PARAMS");
                     // TODO: Process name-value pairs
                     // format: nameLengthBX, valueLengthBX, nameData[nameLength], valueData[valueLength]
                     param_ptr = (unsigned char*)data_ptr;
                     param_pos = contentLength;
                     while(param_pos > 0) {
                        nameLength = 0;
                        valueLength = 0;
                        if((*param_ptr >> 7) == 0) {
                           nameLength = *param_ptr;
                           param_ptr++;
                           param_pos -= 1;
                        } else {
                           nameLength += (*param_ptr & 0x7f) << 24; // nameLengthB3
                           param_ptr++;
                           nameLength += *param_ptr << 16; // nameLengthB2
                           param_ptr++;
                           nameLength += *param_ptr << 8; // nameLengthB1
                           param_ptr++;
                           nameLength += *param_ptr; // nameLengthB0
                           param_ptr++;
                           param_pos -= 4;
                        }
                        dbg("nameLength: %d", nameLength);

                        if((*param_ptr >> 7) == 0) {
                           valueLength = *param_ptr;            
                           param_ptr++;
                           param_pos -= 1;
                        } else {
                           valueLength += (*param_ptr & 0x7f) << 24; // valueLengthB3
                           param_ptr++;
                           valueLength += *param_ptr << 16; // valueLengthB2
                           param_ptr++;
                           valueLength += *param_ptr << 8; // valueLengthB1
                           param_ptr++;
                           valueLength += *param_ptr; // valueLengthB0
                           param_ptr++;
                           param_pos -= 4;
                        }
                        dbg("valueLength: %d", valueLength);

                        dbg("param_name:");
                        for(int i = 0; i < nameLength; i++) {
                           printf("%c", *param_ptr);
                           //dbg("%c", *param_ptr);
                           param_ptr++;
                        }
                        printf("\n", *param_ptr);
                        dbg("param_value:");
                        for(int i = 0; i < valueLength; i++) {
                           printf("%c", *param_ptr);
                           param_ptr++;
                        }
                        printf("\n", *param_ptr);
                        
                        param_pos -= nameLength + valueLength;
                     }
                     break;
               case FCGI_STDIN:
                     dbg("FCGI_STDIN");
                     break;
               case FCGI_STDOUT:
                     dbg("FCGI_STDOUT");
                     break;
               case FCGI_DATA:
                     dbg("FCGI_DATA");
                     break;
               case FCGI_GET_VALUES:
                     dbg("FCGI_GET_VALUES");
                     break;
               case FCGI_GET_VALUES_RESULT:
                     dbg("FCGI_GET_VALUES_RESULT");
                     break;
               case FCGI_UNKNOWN_TYPE:
                     dbg("FCGI_UNKNOWN_TYPE");
                     break;
               default:
                     break;
            }

            status -= FCGI_HEADER_LEN + contentLength + paddingLength;
            dbg("bytes consumed this record: %d", FCGI_HEADER_LEN + contentLength + paddingLength);
            dbg("bytes left to consume: %d", status);
            data_ptr += contentLength + paddingLength;

            assert(status >= 0);
         }

         // TODO: Send response packet
         char *sendContent = str_copy("HTTP/1.1 200 OK\n\n<html><h2>Does this work?</h1></html>");
         
         void *sendData = malloc(megabytes(1));
         FCGI_Header *sendH = (FCGI_Header*) sendData;
         sendH->version = (char)FCGI_VERSION_1;
         sendH->type = FCGI_STDOUT;
         sendH->requestIdB1 = 0;
         sendH->requestIdB0 = 1;
         sendH->contentLengthB1 = 0;
         sendH->contentLengthB0 = str_len(sendContent);
         sendH->paddingLength = 0;
         sendH->reserved = 0;

         unsigned char *sendPtr = ((unsigned char*) sendData);
         sendPtr += 8; //  + sizeof(FCGI_Header);
         int len = str_len(sendContent);
         for(int i = 0; i < len+1; i++) {
            *sendPtr = *sendContent;
            sendPtr++, sendContent++;
         }
         
         FCGI_Header *empSTDOUT = (FCGI_Header*) sendPtr;
         empSTDOUT->version = FCGI_VERSION_1;
         empSTDOUT->type = FCGI_STDOUT;
         empSTDOUT->requestIdB1 = 0;
         empSTDOUT->requestIdB0 = 1;
         empSTDOUT->contentLengthB1 = 0;
         empSTDOUT->contentLengthB0 = 0;
         empSTDOUT->paddingLength = 0;
         empSTDOUT->reserved = 0;

         FCGI_Header *empENDREQ = (FCGI_Header*) (sendPtr + 8); //sizeof(FCGI_Header));
         empENDREQ->version = FCGI_VERSION_1;
         empENDREQ->type = FCGI_END_REQUEST;
         empENDREQ->requestIdB1 = 0;
         empENDREQ->requestIdB0 = 1;
         empENDREQ->contentLengthB1 = 0;
         empENDREQ->contentLengthB0 = 0;
         empENDREQ->paddingLength = 0;
         empENDREQ->reserved = 0;

         u64 packLen = ((u64)sendPtr + 2*sizeof(FCGI_Header)) - (u64)sendData;
         dbg("sendContent len: %d", len+1);
         dbg("packLen: %d", packLen);

         //unsigned char *a = (unsigned char*) sendData;
         //*a = 1;

         status = send(newfd, sendData, packLen, 0);
         //memset(buffer, 0, sizeof(char) * 4096);
      }

      dbg("Connection closed!");

      //shutdown(sockfd, 2);
      close(sockfd);
   }
}
