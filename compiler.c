#include "compiler.h"
#include "chunk.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "debug.h"
#include "value.h"

typedef enum
{
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
} Prescedence;

typedef void (*Parsefn)();

typedef struct parse_rule
{
    Parsefn prefix;
    Parsefn infix;
    Prescedence p;
} parse_rule;


static  parse_rule *get_rule(TokenType tt);


typedef struct Parser
{
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;

} Parser;

Parser parser;
Chunk *compiling_chunk;

static Chunk *current_chunk()
{
    return compiling_chunk;
}

static void error_at(Token *token, const char *message)
{
    if (parser.panic_mode)
    {
        true;
    }

    parser.panic_mode = true;
    fprintf(stderr, "[line %d] Error", token->line);
    if (token->type == TOKEN_EOF)
    {
        fprintf(stderr, "at end");
    }
    else if (token->type == TOKEN_ERROR)
    {
    }
    else
    {
        fprintf(stderr, " at '%.*s'", token->len, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.had_error = true;
}

static void error(const char *message)
{
    error_at(&parser.previous, message);
}

static void erorr_at_current(const char *message)
{
    error_at(&parser.current, message);
}

static void advance()
{
    parser.previous = parser.current;
    for (;;)
    {
        parser.current = scan_token();
        if (parser.current.type != TOKEN_ERROR)
        {
            break;
        }

        erorr_at_current(parser.current.start);
    }
}

static void consume(TokenType tt, const char *message)
{
    if (parser.current.type == tt)
    {
        advance();
        return;
    }

    erorr_at_current(message);
}

static void emit_byte(uint8_t byte)
{
    write_chunk(current_chunk(), byte, parser.previous.line);
}

static void emit_bytes(uint8_t byte1, uint8_t byte2)
{
    emit_byte(byte1);
    emit_byte(byte2);
}

static void emit_return()
{
    emit_byte(OP_RETURN);
}

static void end_compiler()
{
    emit_return();
    #ifdef DEBUG_PRINT_CODE
    if (!parser.had_error)
    {
        disassembleChunk(current_chunk(),"code");
    }
    
    #endif
}

static uint8_t make_constant(Value value)
{
    int c_index = add_constant(current_chunk(), value);
    if (c_index > UINT8_MAX)
    {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)c_index;
}

static void emit_constant(Value value)
{
    emit_bytes(OP_CONSTANT, make_constant(value));
}

static void number()
{
    double value = strtod(parser.previous.start, NULL);
    emit_constant(NUMBER_VAL(value));
}



static void parse_precedence(Prescedence p)
{
    advance();
    Parsefn prefix_rule = get_rule(parser.previous.type)->prefix;

    if (prefix_rule == NULL)
    {
        error("Expect expression");
        return;
    }

    prefix_rule();

    while (p <  get_rule(parser.current.type)->p)
    {
        advance();

        Parsefn infix_rule = get_rule(parser.previous.type)->infix;
        infix_rule();   
    }
    
    
}

static void expression()
{
    parse_precedence(PREC_ASSIGNMENT);
}

static void grouping()
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}
static void unary()
{
    parse_precedence(PREC_UNARY);
    TokenType operator_type = parser.previous.type;
    expression();

    switch (operator_type)
    {
    case TOKEN_MINUS:
        emit_byte(OP_NEGATE);
        break;
    default:
        break;
    }
}

static void binary()
{
    TokenType operator_type = parser.previous.type;
    parse_rule *rule = get_rule(operator_type);
    parse_precedence((Prescedence)(rule->p + 1));

    switch (operator_type)
    {
    case TOKEN_PLUS:
        emit_byte(OP_ADD);
        break;
    case TOKEN_MINUS:
        emit_byte(OP_SUB);
        break;
    case TOKEN_STAR:
        emit_byte(OP_MUL);
        break;
    case TOKEN_SLASH:
        emit_byte(OP_DIV);
        break;
    default:
        return;
    }
}

static void literal()
{

    switch (parser.previous.type) {
        case TOKEN_FALSE:
            emit_byte(OP_FALSE);
            break;
        case TOKEN_TRUE:
            emit_byte(OP_TRUE);
            break;
        case TOKEN_NIL:
            emit_byte(OP_NIL);
            break;
        default:
            return;
    }
}
parse_rule rules[] =
    {
        [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
        [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
        [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
        [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
        [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
        [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
        [TOKEN_MINUS] = {unary, binary, PREC_TERM},
        [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
        [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
        [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
        [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
        [TOKEN_BANG] = {NULL, NULL, PREC_NONE},
        [TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
        [TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_LESS] = {NULL, NULL, PREC_NONE},
        [TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
        [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
        [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
        [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
        [TOKEN_AND] = {NULL, NULL, PREC_NONE},
        [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
        [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
        [TOKEN_FALSE] = {literal, NULL, PREC_NONE},
        [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
        [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
        [TOKEN_IF] = {NULL, NULL, PREC_NONE},
        [TOKEN_NIL] = {literal, NULL, PREC_NONE},
        [TOKEN_OR] = {NULL, NULL, PREC_NONE},
        [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
        [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
        [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
        [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
        [TOKEN_TRUE] = {literal, NULL, PREC_NONE},
        [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
        [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
        [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
        [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};

static parse_rule *get_rule(TokenType tt)
{
    return &rules[tt];
}

bool compile(const char *source, Chunk *c)
{
    parser.had_error = false;
    parser.panic_mode = false;
    init_scanner(source);

    compiling_chunk = c;

    advance();
    // expression();
    consume(TOKEN_EOF, "Expect End of expression. ");
    end_compiler();
    return !parser.had_error;
}
