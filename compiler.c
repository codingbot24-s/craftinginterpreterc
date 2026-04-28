#include "compiler.h"
#include "scanner.h"
#include <stdio.h>
typedef struct Parser
{
    Token current;
    Token previous;
    bool  had_error;
    bool panic_mode;

}Parser;




Parser parser;   
Chunk* compiling_chunk;


static Chunk* current_chunk () 
{
    return compiling_chunk; 
}

static void error_at (Token* token, const char* message) 
{   
    if (parser.panic_mode)
    {
        true;
    }
    
    parser.panic_mode = true;
    fprintf(stderr, "[line %d] Error", token->line); 
    if (token->type ==  TOKEN_EOF)
    {
        fprintf(stderr, "at end");
    }else if (token->type == TOKEN_ERROR)
    {

    }else {
        fprintf(stderr, " at '%.*s'", token->len, token->start);
    }
    
   fprintf(stderr, ": %s\n", message); 
   parser.had_error = true;
}

static void error(const char* message) 
{
    error_at(&parser.previous,message);
}

static void erorr_at_current(const char* message)
{
    error_at(&parser.current, message);
}

static void advance()
{
    parser.previous = parser.current;
    for(;;) {
        parser.current = scan_token();
        if (parser.current.type != TOKEN_ERROR)
        {
            break;
        }

        erorr_at_current(parser.current.start);
    }
}

static void consume(TokenType tt, const char* message)
{
    if (parser.current.type == tt)
    {
        advance();
        return;
    }
 
    erorr_at_curren(message);
}

static void emit_byte(uint8_t byte)
{
    write_chunk(current_chunk(),byte,parser.previous.line);
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
}


static void expression ( ) 
{

}

bool compile(const char *source, Chunk* c)
{
    parser.had_error = false;
    parser.panic_mode = false;
    init_scanner(source);

    compiling_chunk = c;

    advance();
    expression();
    consume(TOKEN_EOF,"Expect End of expression. ");
    end_compiler();
    return !parser.had_error;    
}