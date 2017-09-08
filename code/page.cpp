int printPage(blogPosts allPosts, char* type, int* requestCount) {
    *requestCount += 1;

    // TODO: Create listOfPages array? So contains "/", "/some-blog-post", /admin", etc.
    // Determine whether printing single post or front page
    int singlePost = -2;
    for(int i = 0; i < allPosts.num; i++) {
        if(stringBeginsWith(allPosts.posts[i]->uri, type)) {
            singlePost = i;
        }
    }
    if(stringsAreEqual(type, "/")) { singlePost = -1; }  // All URIs begin with '/'!


    if(!stringsAreEqual(type, "/") && singlePost == -2) {  // URI not "/", and no match found in allPosts
        printf("Content-Type: text/html;\ncharset=UTF-8\n");
        printf("Status: 200 OK\n\n");
        printf("Page not found! %s", type);
    } else if(stringsAreEqual(type, "/admin")) {
        printf("Content-Type: text/html;\ncharset=UTF-8\n");
        printf("Status: 200 OK\n\n");
        printf("You're in the secret admin area!\n");
    } else {
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
        //printf("    <li><a href='#'>Search</a></li>\n");
        printf("    <li><a href='/'>Home</a></li>\n");
        //printf("    <li><a href='#'>About</a></li>\n");
        //printf("    <li><a href='#'>Contact</a></li>\n");
        //printf("    <li><a href='#'>Categories</a></li>\n");
        printf("  </ul>\n");

        printf("  <h3>Archive</h3>\n");
        for(int i = 0; i < allPosts.num; i++) {
            printf("<a href='%s'>%s</a><br>", allPosts.posts[i]->uri, allPosts.posts[i]->title);
        }

        printf("  </fieldset>\n");
        printf("</aside>\n");
        printf("\n");

        if(singlePost == -1) {
            for(int i = 0; i < allPosts.num || i < 5; i++) {
                printf("<article>\n");
                printf("  <fieldset>\n");
                printf("  <h3>");
                printf("%s", allPosts.posts[i]->title);
                printf("</h3>\n");
                printf("  <p>Date: %02d-%02d-%04d</p>\n", allPosts.posts[i]->dateDay,
                       allPosts.posts[i]->dateMonth,
                       allPosts.posts[i]->dateYear);
                printf("  <p>Author: %s</p>\n", allPosts.posts[i]->author);
                printf("  %s\n", allPosts.posts[i]->body);
                printf("  </fieldset>\n");
                printf("</article>\n");
                printf("\n");          
            }
        } else {
            printf("<article>\n");
            printf("  <fieldset>\n");
            printf("  <h3>");
            printf("%s", allPosts.posts[singlePost]->title);
            printf("</h3>\n");
            printf("  <p>Date: %02d-%02d-%04d</p>\n", allPosts.posts[singlePost]->dateDay,
                   allPosts.posts[singlePost]->dateMonth,
                   allPosts.posts[singlePost]->dateYear);
            printf("  <p>Author: %s</p>\n", allPosts.posts[singlePost]->author);
            printf("  %s\n", allPosts.posts[singlePost]->body);
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
}
