#include <fcgi_stdio.h>
#include <stdlib.h>

struct blogPost {
  char* title;
  char* author;
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

#include "utils.cpp"
#include "page.cpp"

int main(int argc, char *argv[]) {

    blogPosts allPosts;
    allPosts.num = 1;

    allPosts.posts = (blogPost**)malloc(sizeof(blogPost*));
    allPosts.posts[0] = (blogPost*)malloc(sizeof(blogPost));

    allPosts.posts[0]->title = stringCopy("A new blog post!");
    allPosts.posts[0]->author = stringCopy("Mark");
    allPosts.posts[0]->dateDay = 1;
    allPosts.posts[0]->dateMonth = 8;
    allPosts.posts[0]->dateYear = 2017;

    allPosts.posts[0]->body = stringCopy("<p>This is the post body!!!</p>");

    // Read posts from posts/
    // Save each post in array of posts

    int requestCount = 0;
    while(FCGI_Accept() >= 0) {
      printPage(allPosts, getenv("REQUEST_URI"), &requestCount);
    }
}
