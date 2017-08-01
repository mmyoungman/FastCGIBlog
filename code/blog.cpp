#include <fcgi_stdio.h>
#include <stdlib.h>

#if DEBUG
#define assert(expression) if(!(expression)) { *(int*)0 = 0; }
#else
#define assert(expression)
#endif

int stringLength(char* str) {
  char* strPtr = str;
  while(*strPtr != '\0') { strPtr++; }
  return strPtr - str;
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

int printPage(blogPosts allPosts, char* type, int* requestCount) {
  *requestCount += *requestCount + 1;

  if(stringsAreEqual(type, "/")) {
        printf("Content-Type: text/html;\ncharset=UTF-8\n");
        printf("Status: 200 OK\n\n");
        printf("<!DOCTYPE html>\n");
        printf("<html>\n");
        printf("<head>\n");
        printf("<meta charset='utf-8'>\n");
        printf("<title>Blog title</title>\n");
        printf("\n");
        printf("<style type='text/css'>\n");
        printf("* {\n");
        printf("  font-family: 'Helvetica Neue',Helvetica,Arial,sans-serif;\n");
        printf("}\n");
        printf("\n");
        printf("div.container {\n");
        printf("    width: 100%;\n");
        printf("}\n");
        printf("\n");
        printf("header {\n");
        printf("    border-bottom: 0px solid black;\n");
        printf("    padding: 1em;\n");
        printf("    color: black;\n");
        printf("    background-color: white;\n");
        printf("    clear: left;\n");
        printf("    text-align: left;\n");
        printf("}\n");
        printf("\n");
        printf("footer {\n");
        printf("    border-top: 1px solid black;\n");
        printf("    padding: 1em;\n");
        printf("    color: black;\n");
        printf("    background-color: white;\n");
        printf("    clear: left;\n");
        printf("    text-align: center;\n");
        printf("}\n");
        printf("\n");
        printf("nav {\n");
        printf("    border-bottom: 1px solid black;\n");
        printf("    padding: 0;\n");
        printf("    width: 100%;\n");
        printf("    height: 50px;\n");
        printf("    text-align: justify;\n");
        printf("    padding-top: 1.5em;\n");
        printf("}\n");
        printf("\n");
        printf("nav > a {\n");
        printf("    display: inline-block;\n");
        printf("    padding-left: 50px;\n");
        printf("    padding-right: 50px;\n");
        printf("}\n");
        printf("\n");
        printf("nav:after {\n");
        printf("    content: ' ';\n");
        printf("    display: inline-block;\n");
        printf("    width: 100%;\n");
        printf("    height: 0;\n");
        printf("}\n");
        printf("\n");
        printf("aside {\n");
        printf("    float: right;\n");
        printf("    width: 25%;\n");
        printf("    margin: 0;\n");
        printf("    padding: 1em;\n");
        printf("    text-align: left\n");
        printf("}\n");
        printf("\n");
        printf("article {\n");
        printf("    width: 70%;\n");
        printf("    margin-right: 25%;\n");
        printf("    border-right: 0px solid black;\n");
        printf("    padding: 1em;\n");
        printf("    overflow: hidden;\n");
        printf("}\n");
        printf("\n");
        printf("fieldset {\n");
        printf("    border: 0px solid black;\n");
        printf("    background-color: white;\n");
        printf("}\n");
        printf("\n");
        printf("side-fieldset {\n");
        printf("    border: 0px solid black;\n");
        printf("    background-color: white;\n");
        printf("    margin-bottom: 1em;\n");
        printf("}\n");
        printf("\n");
        printf("legend {\n");
        printf("    background-color: #E8E8EE;\n");
        printf("    border: 1px black solid;\n");
        printf("    margin-left: 1em;\n");
        printf("    padding: 0.2em 0.8em;\n");
        printf("}\n");
        printf("\n");
        printf("</style>\n");
        printf("</head>\n");
        printf("<body>\n");
        printf("\n");
        printf("<div class='container'>\n");
        printf("\n");
        printf("<header>\n");
        printf("  <fieldset>\n");
        printf("  <h1>Bitwise Explorations</h1>\n");
        printf("  <p>Mark Youngman's Blog</p>\n");
        printf("  </fieldset>\n");
        printf("</header>\n");
        printf("\n");
        printf("<aside>\n");
        printf("  <fieldset>\n");
        printf("  <h3>About</h3>\n");
        printf("  <p>This blog covers testing and low-level computing.</p>\n");
        printf("  </fieldset>\n");
        printf("\n");
        printf("  <fieldset>\n");
        printf("  <h3>Menu</h3>\n");
        printf("  <ul>\n");
        printf("    <li><a href='#'>Search</a></li>\n");
        printf("    <li><a href='#'>Home</a></li>\n");
        printf("    <li><a href='#'>About</a></li>\n");
        printf("    <li><a href='#'>Contact</a></li>\n");
        printf("    <li><a href='#'>Categories</a></li>\n");
        printf("    <li><a href='#'>Archive</a></li>\n");
        printf("  </ul>\n");
        printf("  </fieldset>\n");
        printf("</aside>\n");
        printf("\n");
        
          /*
          char postHeading[1280];
          FILE *stream = fopen("posts.txt", "r");

          while( fgets(postHeading, 1280, stream) != NULL ) {
            printf("<article>\n");
            printf("  <fieldset>\n");
            printf("  <h3>");
            printf("%s", postHeading);
            printf("</h3>\n");
            printf("  <p>Lorem ipsum</p>\n");
            printf("  <p>Blog post date</p>\n");
            printf("  <p>Blog post categories</p>\n");
            printf("  <p>Blog post comments</p>\n");
            printf("  </fieldset>\n");
            printf("</article>\n");
            printf("\n");
          }
          */

        for(int i = 0; i < allPosts.num; i++) {
            printf("<article>\n");
            printf("  <fieldset>\n");
            printf("  <h3>");
            printf("%s", allPosts.posts[i]->title);
            printf("</h3>\n");
            printf("  <p>Author: %s</p>\n", allPosts.posts[i]->author);
            printf("  %s\n", allPosts.posts[i]->body);
            printf("  <p>Categories: %s</p>\n", allPosts.posts[i]->categories);
            printf("  <p>Blog post date: %d-%d-%d</p>\n", allPosts.posts[i]->dateDay,
                                                          allPosts.posts[i]->dateMonth,
                                                          allPosts.posts[i]->dateYear);
            printf("  <p>Blog post comments</p>\n");
            printf("  </fieldset>\n");
            printf("</article>\n");
            printf("\n");          
        }

        printf("<footer>Request %d</footer>\n", *requestCount);
        printf("\n");
        printf("</div>\n");
        printf("\n");
        printf("</body>\n");
        printf("</html>\n");
  }
  else if(stringsAreEqual(type, "/admin")) {
        printf("Content-Type: text/html;\ncharset=UTF-8\n");
        printf("Status: 200 OK\n\n");
        printf("You're in the secret admin area!\n");
  }
  else {
        printf("Content-Type: text/html;\ncharset=UTF-8\n");
        printf("Status: 200 OK\n\n");
        printf("Page not found! Or could eventually be a page particular to this blog post: %s\n", type);
    // Match type string to particular blog post
    // If no post is found, 
  }
}

int main(int argc, char *argv[]) {

    blogPosts allPosts;
    allPosts.num = 1;

    allPosts.posts = (blogPost**)malloc(sizeof(blogPost*));
    allPosts.posts[0] = (blogPost*)malloc(sizeof(blogPost));

    allPosts.posts[0]->title = stringCopy("A new blog post!");
    allPosts.posts[0]->author = stringCopy("Mark");
    allPosts.posts[0]->categories = stringCopy("blog,test,category,anothercategory");
    allPosts.posts[0]->dateDay = 1;
    allPosts.posts[0]->dateMonth = 8;
    allPosts.posts[0]->dateYear = 2017;

    allPosts.posts[0]->body = stringCopy("<p>This is the post body!!!</p>");

    // Read posts from posts/
    // Save each post in array of posts

    int requestCount = 0;
    //assert(1 == 0);
    while(FCGI_Accept() >= 0) {
      printPage(allPosts, getenv("REQUEST_URI"), &requestCount);
      /*
      printf("Content-Type: text/plain;\ncharset=UTF-8\n");
      printf("Status: 200 OK\n\n");
      printf("title: %s\n", allPosts.posts[0]->title);
      */
    }
}
