#include "scanner.h"
#include <stdio.h>

typedef struct Scanner
{
    const char* start;
    const char* current;
    int     line;
}Scanner;


Scanner scanner;

void print_scanner_state()
{
    printf("current is on %s\n",scanner.current);
    printf("start in on %s\n",scanner.start);
    printf("%d\n",scanner.line);
    
}


void init_scanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}




void error_token(const char *message)
{
}

Token scan_token() 
{   
}