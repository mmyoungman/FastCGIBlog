#include <fcgi_stdio.h>
#include <stdlib.h>
#include <dirent.h>

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

#include "utils.cpp"
#include "page.cpp"

int main(int argc, char *argv[]) {

    blogPosts allPosts;

    // Read posts from posts/ dir
    int dirListCount = 0;
    char **dirList = (char**)malloc(sizeof(char*)*1024);

    DIR *d;
    struct dirent *dir;
    d = opendir("../posts/");
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

        char* path = stringCopy("../posts/");
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

    int requestCount = 0;
    while(FCGI_Accept() >= 0) {
        printPage(allPosts, getenv("REQUEST_URI"), &requestCount);
    }
}
