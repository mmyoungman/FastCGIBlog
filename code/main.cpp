/*
    TODO:   - Memory leaks on reloading posts?
            - Post data currently only refreshes when restarting
            - Remove dependency on spawn-fcgi
            - Remove dependency on fcgi_studio.h
*/

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

blogPosts loadPosts(char* postDir) {

    blogPosts postsObj;

    // Read posts from posts/ dir
    int dirListCount = 0;
    char **dirList = (char**)malloc(sizeof(char*)*1024);

    DIR *d;
    struct dirent *dir;
    d = opendir(postDir);
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

    postsObj.num = dirListCount;
    postsObj.posts = (blogPost**)malloc( sizeof(blogPost*)*postsObj.num );

    // Transfer data to blogPosts struct
    for(int i = 0; i < dirListCount; i++) {
        postsObj.posts[i] = (blogPost*)malloc(sizeof(blogPost));
        postsObj.posts[i]->body = stringCopy("");
        FILE* fp;
        char* line = (char*)malloc(sizeof(char)*1024);

        char* path = stringCopy(postDir);
        stringConcat(path, dirList[i]);

        fp = fopen(path, "r");
        //if(fp == NULL) { return NULL; }

        // TODO: Clean up so no non-alphanumeric characters are imported at the end of each string, specifically with the uri
        while(fgets(line, 1024, fp) != 0) {
            if(stringBeginsWith(line, "title:")) {
                while(*line != ':') { line++; }
                line++;
                postsObj.posts[i]->title = stringCopy(line);
            }
            else if(stringBeginsWith(line, "uri:")) {
                while(*line != ':') { line++; }
                line++;
                postsObj.posts[i]->uri = stringCopy(line);
            }
            else if(stringBeginsWith(line, "author:")) {
                while(*line != ':') { line++; }
                line++;
                postsObj.posts[i]->author = stringCopy(line);
            }
            else if(stringBeginsWith(line, "date:")) {
                while(*line != ':') { line++; }
                line++;
                int listSize = 0;
                char** splitList = stringSplit(line, '-', &listSize);
                postsObj.posts[i]->dateDay = stringToInt(stringCopy(splitList[0]));
                postsObj.posts[i]->dateMonth = stringToInt(stringCopy(splitList[1]));
                postsObj.posts[i]->dateYear = stringToInt(stringCopy(splitList[2]));
            }
            else if(stringBeginsWith(line, "body:")) {
                while(fgets(line, 1024, fp) != 0) {
                    if(stringsAreEqual(postsObj.posts[i]->body, "")) {
                        postsObj.posts[i]->body = stringCopy(line);
                    }
                    else { 
                        stringConcat(postsObj.posts[i]->body, "\n");
                        stringConcat(postsObj.posts[i]->body, line);
                    }
                }
                break;
            }
        }
    }

    // Sort posts by date, newest to oldest
    for(int i = 0; i < postsObj.num; i++) {
        int newestIndex = i;
        int newestDay = -1;
        int newestMonth = -1;
        int newestYear = -1;
        for(int j = i; j < postsObj.num; j++) {
            if((newestYear < postsObj.posts[j]->dateYear) || 
               (newestYear == postsObj.posts[j]->dateYear && newestMonth < postsObj.posts[j]->dateMonth) ||
               (newestMonth == postsObj.posts[j]->dateMonth && newestDay < postsObj.posts[j]->dateDay)) {
                newestIndex = j;
                newestYear = postsObj.posts[j]->dateYear;
                newestMonth = postsObj.posts[j]->dateMonth;
                newestDay = postsObj.posts[j]->dateDay;
            }
        }
        blogPost* temp = postsObj.posts[i];
        postsObj.posts[i] = postsObj.posts[newestIndex];
        postsObj.posts[newestIndex] = temp;
    }
    return postsObj;
}

int main(int argc, char *argv[]) {
    blogPosts allPosts = loadPosts("../posts/");

    int requestCount = 0;
    while(FCGI_Accept() >= 0) {
        printPage(allPosts, getenv("REQUEST_URI"), &requestCount);
    }
}
