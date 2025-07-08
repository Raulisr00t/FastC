#ifndef FASTC_COMPILER_H
#define FASTC_COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libtcc.h>

void tcc_error(void *opaque, const char *msg);
int compile(const char *full_code);
bool is_declaration(const char *line);
void repl();

#endif 