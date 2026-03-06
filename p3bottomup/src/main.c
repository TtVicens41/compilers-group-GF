/**
 * @file main.c
 * @brief Entry point for the bottom-up shift-reduce parser.
 * @author Davi Penna-Mattos Dias de Paiva, Marc Bosch Manzano.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser/parser.h"
#include "arguments/arguments.h"
#include "utils/string_utils.h"

int main(int argc, char *argv[])
{
    ParserContext ctx = { 0 };

    parse_arguments(argc, argv, &ctx);
    context_exit(&ctx);

    parser_run(&ctx);
    
    context_cleanup(&ctx);
    context_exit(&ctx);
    return 0;
}
