//#include <fcgi_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#if DEBUG
#define assert(expression) if(!(expression)) { *(int*)0 = 0; }
#else
#define assert(expression)
#endif

struct blogPost {
    char* title;
    char* uri;
    char* author;
    int dateDay;
    int dateMonth;
    int dateYear;

    char *body;
};

struct blogPosts {
    blogPost** posts;
    int num;
};

int mathPower(int num, int pow) {
    int result = 1;
    for(int i = 0; i < pow; i++) {
        result *= num;
    }
    return result;
}

int stringLength(char* str) {
  char* strPtr = str;
  while(*strPtr != '\0') { strPtr++; }
  return strPtr - str;
}

int stringBeginsWith(char* str, char* start) {
  char* strPtr = str;
  while((*start != '\0') && (*start == *strPtr)) {
    strPtr++, start++;
  }
  return *start == '\0';
}

int stringEndsWith(char* str, char* end) {
  char* strPtr = str;
  int endLength = stringLength(end);
  while(*end != '\0') { end++; }
  while(*strPtr != '\0') { strPtr++; }

  while(*strPtr == *end && endLength > 0) {
    strPtr--, end--;
    endLength--;
  }
  return *strPtr == *end;
}

int stringToInt(char* str) {
    int result = 0;
    char* strPtr = str;

    // Strip non-numeric char from the end of the string
    while(*strPtr >= 48 && *strPtr <= 57) { strPtr++; }
    *strPtr = '\0';
    strPtr = str;

    int length = stringLength(str);
    while(length > 0) {
        length--;
        result += (*strPtr - 48) * mathPower(10, length);
        strPtr++;
    }
    return result;
}

char* stringCopy(char *s) {
  char* copy = (char*)malloc(sizeof(char)*(stringLength(s)+1));
  char* sPtr = s;
  char* copyPtr = copy;
  while(*sPtr != '\0') {
    *copyPtr = *sPtr;
    copyPtr++, sPtr++;
  }
  *copyPtr = '\0';
  return copy;
}

int stringsAreEqual(char *a, char *b) {
  while((*a != '\0') && (*a == *b))
    a++, b++;
  return ((*a == '\0') && (*b == '\0'));
}

char** stringSplit(char* str, char c, int* size) {
  int numStrs = 1;
  char* strPtr = str;
  while(*strPtr != '\0') {
    if(*strPtr == c) {
      *strPtr = '\0';
      numStrs++;
    }
    strPtr++;
  }
  char** result = (char**)malloc(sizeof(char*)*numStrs);
  char* strStart = str;
  strPtr = strStart;
  int i = 0;
  while(numStrs > 0) {
    if(*strPtr == '\0') {
      numStrs--;
      result[i] = strStart;
      strStart = strPtr + 1;
      i++;
    }
    strPtr++;
  }
  *size = i;
  return result;
}

void stringConcat(char* str, char* addition) {
  int newLength = stringLength(str) + stringLength(addition) + 1;
  str = (char*)realloc(str, sizeof(char) * newLength);
  char* strPtr = str;
  while(*strPtr != '\0') { strPtr++; }
  char* addPtr = addition;
  while(*addPtr != '\0') {
    *strPtr = *addPtr;
    strPtr++, addPtr++;
  }
  *strPtr = '\0';
}

main(int argc, char *argv[]) {

    blogPosts allPosts;

    // Read posts from posts/ dir
    int dirListCount = 0;
    char **dirList = (char**)malloc(sizeof(char*)*1024);

    DIR *d;
    struct dirent *dir;
    d = opendir("/home/mark/Desktop/MarkBackup20170829/Programming 20170829/fastcgi-blog/posts/");
    if(d) {
        while((dir = readdir(d)) != NULL) {
            if(stringsAreEqual(dir->d_name, ".") || stringsAreEqual(dir->d_name, "..") || !stringEndsWith(dir->d_name, ".txt")) {
                continue;
            }
            dirList[dirListCount] = stringCopy(dir->d_name);
            dirListCount++;
        }
        closedir(d);
    }

    allPosts.num = dirListCount;
    allPosts.posts = (blogPost**)malloc( sizeof(blogPost*)*allPosts.num );

    // Transfer data to blogPosts struct
    for(int i = 0; i < dirListCount; i++) {
        allPosts.posts[i] = (blogPost*)malloc(sizeof(blogPost));
        allPosts.posts[i]->body = stringCopy("");
        FILE* fp;
        char* line = (char*)malloc(sizeof(char)*1024);

        char* path = stringCopy("/home/mark/Desktop/MarkBackup20170829/Programming 20170829/fastcgi-blog/posts/");
        stringConcat(path, dirList[i]);

        fp = fopen(path, "r");
        if(fp == NULL) { return -1; }

        while(fgets(line, 1024, fp) != 0) {
            if(stringBeginsWith(line, "title:")) {
                while(*line != ':') { line++; }
                line++;
                allPosts.posts[i]->title = stringCopy(line);
            }
            else if(stringBeginsWith(line, "uri:")) {
                while(*line != ':') { line++; }
                line++;
                allPosts.posts[i]->uri = stringCopy(line);
            }
            else if(stringBeginsWith(line, "author:")) {
                while(*line != ':') { line++; }
                line++;
                allPosts.posts[i]->author = stringCopy(line);
            }
            else if(stringBeginsWith(line, "date:")) {
                while(*line != ':') { line++; }
                line++;
                int listSize = 0;
                char** splitList = stringSplit(line, '-', &listSize);
                allPosts.posts[i]->dateDay = stringToInt(stringCopy(splitList[0]));
                allPosts.posts[i]->dateMonth = stringToInt(stringCopy(splitList[1]));
                allPosts.posts[i]->dateYear = stringToInt(stringCopy(splitList[2]));
            }
            else if(stringBeginsWith(line, "body:")) {
                while(fgets(line, 1024, fp) != 0) {
                    if(stringsAreEqual(allPosts.posts[i]->body, "")) {
                        allPosts.posts[i]->body = stringCopy(line);
                    }
                    else { 
                        stringConcat(allPosts.posts[i]->body, "\n");
                        stringConcat(allPosts.posts[i]->body, line);
                    }
                }
                break;
            }
        }
    }

    // Sort posts by date, newest to oldest
    ///*
    for(int i = 0; i < allPosts.num; i++) {
        int newestIndex = i;
        int newestDay = -1;
        int newestMonth = -1;
        int newestYear = -1;
        int j = i;
        for(j = i; j < allPosts.num; j++) {
            if((newestYear < allPosts.posts[j]->dateYear) || 
               (newestYear == allPosts.posts[j]->dateYear && newestMonth < allPosts.posts[j]->dateMonth) ||
               (newestMonth == allPosts.posts[j]->dateMonth && newestDay < allPosts.posts[j]->dateDay)) {
                newestIndex = j;
                newestYear = allPosts.posts[j]->dateYear;
                newestMonth = allPosts.posts[j]->dateMonth;
                newestDay = allPosts.posts[j]->dateDay;
            }
        }
        blogPost* temp = allPosts.posts[i];
        allPosts.posts[i] = allPosts.posts[newestIndex];
        allPosts.posts[newestIndex] = temp;
    }
    //*/


  /*
  char* str = stringCopy("2017");
  int year = stringToInt(stringCopy(str));
  printf("Year: %04d\n", year);

  str = stringCopy("25-07-2017");
  int listLength = 0;
  char** list = stringSplit(str, '-', &listLength);
  for(int i = 0; i < listLength; i++) {
    printf("%s\n", list[i]);
  }

  str = stringCopy("Test");
  stringConcat(str, "");
  printf("%s\n", str);


  //List files in posts/ dir, stick them into an array
  int dirListSize = sizeof(char)*1024;
  int dirListCount = 0;
  char **dirList = (char**)malloc(sizeof(char)*1024);

  DIR *d;
  struct dirent *dir;
  d = opendir("posts/");
  if(d) {
    while((dir = readdir(d)) != NULL) {
      if(stringsAreEqual(dir->d_name, ".") || stringsAreEqual(dir->d_name, "..")) {
        printf("Skipping . and ..\n");
        continue;
      }

      if(dirListSize < (sizeof(char)*1024) * dirListCount) {
        dirList = (char**)realloc(dirList, (sizeof(char)*1024) * dirListCount);
        dirListSize += sizeof(char)*1024;
      }

      assert(stringLength(dir->d_name) < 1024);
      dirList[dirListCount] = dir->d_name;
      dirListCount++;
    }
    closedir(d);
  }

  for(int i = 0; i < dirListCount; i++) {
    printf("%s\n", dirList[i]);
  }

  //Loop through files, adding them to array of posts
  //while grabbing title,author,categories,etc.
  //char line[1280];
  //FILE *stream = fopen("posts.txt", "r");
  //if(fgets(line, 1280, stream) != 0) {
  //}

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
    */
}
