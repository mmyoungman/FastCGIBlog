#include <fcgi_stdio.h>
#include <stdlib.h>

#include "pages.cpp"
#include "random.cpp"

struct blogPost {
  char* title;
  char* author;
  char* categories;
  int dateDay;
  int dateMonth;
  int dateYear;

  int bodySize;
  char *body;
};

struct blogPosts {
    blogPost** posts;
    int num;
};

blogPosts blogPosts;

int main(int argc, char *argv[]) {

    // Read posts from posts/
    // Save each post in array of posts

    int requestCount = 0;
    //assert(1 == 0);
    while(FCGI_Accept() >= 0) {
      printPage(&requestCount, getenv("REQUEST_URI"));
    }
}
