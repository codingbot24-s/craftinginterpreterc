
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>


static void run_repl()
{
    char buf[1024];
    for (;;)
    {
        printf("> ");
        if (!fgets(buf, sizeof(buf), stdin))
        {
            printf("\n");
            break;
        }

        interpret(buf);
    }
}

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "rw");
    if (file == NULL)
    {
        fprintf(stderr, "Cant open the file for read");
        exit(65);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *buff = (char *)malloc(file_size + 1);
    if (buff == NULL)
    {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(65);
    }

    size_t bytes_read = fread(buff, sizeof(char), file_size, file);
    if (bytes_read < file_size)
    {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        free(buff);
        exit(74);
    }

    buff[bytes_read] = '\0';

    fclose(file);
    return buff;
}

static void run_file(const char *path)
{
    char *source = read_file(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(70);
}

int main(int argc, char **argv)
{
    init_vm();

    if (argc == 1)
    {
        run_repl();
    }
    else if (argc == 2)
    {
        run_file(argv[1]);
    }else {
            fprintf(stderr, "Usage: clox [path]\n");
        exit(64);

    }

    free_vm();

    return 0;
}
