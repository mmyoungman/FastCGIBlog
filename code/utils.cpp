#if DEBUG
#define assert(expression) if(!(expression)) { *(int*)0 = 0; }
#else
#define assert(expression)
#endif

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

int stringToInt(char* str) {
    int result = 0;
    char* strPtr = str;

    // Strip non-numeric char from the end of the string -- because I didn't clean up fgets in main.cpp when importing post data
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
    while((*a != '\0') && (*a == *b)) {
        a++, b++;
    }
    return ((*a == '\0') && (*b == '\0'));
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
