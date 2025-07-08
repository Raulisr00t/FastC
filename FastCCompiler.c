#include "FastCCompiler.h"

#define MAX_CODE_SIZE 16384
#define MAX_LINE_LEN 1024

static char headers[MAX_CODE_SIZE] = "#include <stdio.h>\n#include <stdlib.h>\n\n";
static int global_initialized = 0;

void tcc_error(void *opaque, const char *msg) {
    fprintf(stderr, "[TCC ERROR] %s\n", msg);
}

bool is_declaration(const char *line) {
    
    return (
        (strstr(line, "int ")   || strstr(line, "float ") || strstr(line, "char ") ||
         strstr(line, "double ")|| strstr(line, "void ")  || strstr(line, "struct ") ||
         strstr(line, "typedef ") || strstr(line, "#include")) &&
        strchr(line, '(') == NULL 
    );
}

int compile(const char *code) {
    TCCState *state = tcc_new();

    if (!state) {
        fprintf(stderr, "[!] Failed to create TCC state\n");
        return -1;
    }

    tcc_set_error_func(state, NULL, tcc_error);
    tcc_set_output_type(state, TCC_OUTPUT_MEMORY);

    tcc_add_symbol(state, "printf", printf);
    tcc_add_symbol(state, "exit", exit);
    tcc_add_symbol(state, "malloc", malloc);
    tcc_add_symbol(state, "free", free);

    if (tcc_compile_string(state, code) == -1) {
        fprintf(stderr, "[!] Compilation failed\n");
        tcc_delete(state);
        return -1;
    }

    int size = tcc_relocate(state, NULL);

    if (size <= 0) {
        fprintf(stderr, "[!] Relocation size error\n");
        tcc_delete(state);
        return -1;
    }

    void *memory = malloc(size);

    if (!memory) {
        fprintf(stderr, "[!] Out of memory\n");
        tcc_delete(state);
        return -1;
    }

    if (tcc_relocate(state, memory) < 0) {
        fprintf(stderr, "[!] Relocation failed\n");
        free(memory);
        tcc_delete(state);
        return -1;
    }

    void (*run)() = tcc_get_symbol(state, "run");
    
    if (!run) {
        fprintf(stderr, "[!] Couldn't find run symbol\n");
        free(memory);
        tcc_delete(state);
        return -1;
    }

    run();

    free(memory);
    tcc_delete(state);

    return 0;
}

void repl() {
    char line[MAX_LINE_LEN];

    printf("Welcome to FastC REPL (Ctrl+C to exit, :q to quit)\n");

    while (1) {
        printf("FastC>> ");
        fflush(stdout);

        if (!fgets(line, sizeof(line), stdin))
            break;

        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, ":q") == 0 || strcmp(line, ":quit") == 0)
            break;

        if (is_declaration(line)) {
            if (strlen(headers) + strlen(line) + 2 < MAX_CODE_SIZE) {
                strcat(headers, line);
                strcat(headers, "\n");
            } 

            else 
                fprintf(stderr, "[!] Global code buffer overflow\n");
            
            continue;
        }

        char actual_code[MAX_CODE_SIZE + MAX_LINE_LEN + 128];

        snprintf(actual_code, sizeof(actual_code),
                 "%s\nvoid run() {\n%s%s\n}\n",
                 headers, line, strchr(line, ';') ? "" : ";");

        if (compile(actual_code) != 0)
            fprintf(stderr, "[!] Error executing line\n");
            continue;
    }
}

int main(void) {
    repl();

    return 0;
}
