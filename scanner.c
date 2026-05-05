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
    for (;;)
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
            }
            else
            {
                return;
            }

            break;
        default:
            return;
        }
    }
}

static Token string()
{
    while (peek() != '"' && !is_at_end())
    {
        if (peek() == '\n')
        {
            scanner.line++;
        }

        advance();
    }

    if (is_at_end())
    {
        return error_token("unterminated string. ");
    }

    advance();

    return makeToken(TOKEN_STRING);
}

static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

static Token number()
{
    while (is_digit(peek()))
    {
        advance();
    }

    if (peek() == '.' && is_digit(peek_next()))
    {
        advance();

        while (is_digit(peek()))
        {
            advance();
        }
    }
    return makeToken(TOKEN_NUMBER);
}

static bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

static TokenType checkKeyword(int start, int len, const char *rest, TokenType type)
{
    if (scanner.current - scanner.start == start + len &&
        memcmp(scanner.start + start, rest, len) == 0)
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type()
{
    switch (scanner.start[0])
    {
    case 'a':
        return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c':
        return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e':
        return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'i':
        return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n':
        return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o':
        return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p':
        return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r':
        return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's':
        return checkKeyword(1, 4, "uper", TOKEN_SUPER);
    case 'v':
        return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w':
        return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    case 'f':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'a':
                return checkKeyword(2, 3, "lse", TOKEN_FALSE);
            case 'o':
                return checkKeyword(2, 1, "r", TOKEN_FOR);
            case 'u':
                return checkKeyword(2, 1, "n", TOKEN_FUN);
            }
        }
        break;
    case 't':
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
            case 'h':
                return checkKeyword(2, 2, "is", TOKEN_THIS);
            case 'r':
                return checkKeyword(2, 2, "ue", TOKEN_TRUE);
            }
        }
        break;
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier()
{
    while (is_alpha(peek()) || is_digit(peek()))
    {
        advance();
    }

    return makeToken(identifier_type());
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
    if (is_digit(c))
        return number();
    if (is_alpha(c))
    {
        return identifier();
    }

    switch (c)
    {
    case '"':
        return string();
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
        return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
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