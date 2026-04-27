#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
typedef struct Scanner
{
    const char *start;
    const char *current;
    int line;
} Scanner;

Scanner scanner;

void print_scanner_state()
{
    printf("current is on %s\n", scanner.current);
    printf("start in on %s\n", scanner.start);
    printf("scanner line is    %d\n", scanner.line);
}

void init_scanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

Token makeToken(TokenType tt)
{
    Token token;
    token.type = tt;
    token.start = scanner.start;
    token.len = (int)(scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

Token error_token(const char *message)
{

    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.len = (int)strlen(message);
    token.line = scanner.line;

    return token;
}

static char
advance()
{
    scanner.current++;
    return scanner.current[-1];
}

static bool is_at_end()
{
    return *scanner.current == '\0';
}

static bool match(char expected)
{
    if (is_at_end)
    {
        return false;
    }

    if (*scanner.current == expected)
    {
        scanner.current++;

        return true;
    }

    return false;
}


static char peek() 
{
    return *scanner.current;   
}

static char peek_next()
{
    if (is_at_end())
    {
        return '\0';
    }
    return scanner.current[1];
}


static void skip_whitespace() 
{
    for(;;)
    {
        char c = peek();
        switch (c)
        {
        case ' ':
        case '\r':
        case '\t':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;
        case '/':
            if (peek_next() == '/')
            {
                while (peek() != '\n' && !is_at_end())
                {
                    advance();
                }
                
            }else
            {
                return;
            }

            
            break;
        default:
            return;
        }
    }
} 


Token scan_token()
{
    skip_whitespace();
    scanner.start = scanner.current;

    if (is_at_end())
    {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();

    switch (c)
    {
    case '(':
        return makeToken(TOKEN_LEFT_PAREN);
    case ')':
        return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
        return makeToken(TOKEN_LEFT_BRACE);
    case '}':
        return makeToken(TOKEN_RIGHT_BRACE);
    case ';':
        return makeToken(TOKEN_SEMICOLON);
    case ',':
        return makeToken(TOKEN_COMMA);
    case '.':
        return makeToken(TOKEN_DOT);
    case '-':
        return makeToken(TOKEN_MINUS);
    case '+':
        return makeToken(TOKEN_PLUS);
    case '/':
        return makeToken(TOKEN_SLASH);
    case '*':
        return makeToken(TOKEN_STAR);
    case '!':
        return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_EQUAL);
    case '=':
        return makeToken(
            match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
        return makeToken(
            match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
        return makeToken(
            match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    default:
        printf("dont know what is this %c\n", c);
        break;
    }

    return error_token("Unexpected character.");
}