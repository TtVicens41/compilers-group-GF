/**
 * @file main.c
 * @brief Entry point for the bottom-up shift/reduce parser.
 *
 * Usage:
 *     ./parser <language_file> <parse_table_file> <token_file> [output_file]
 *
 * If output_file is omitted the debug trace is written to
 *     <token_file_base>_p3dbg.txt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser/parser.h"

/* ── usage ──────────────────────────────────────────────────────────── */

static void print_usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s <language_file> <parse_table_file> "
        "<token_file> [output_file]\n"
        "\n"
        "  language_file    Path to the language definition  (language.txt)\n"
        "  parse_table_file Path to the parse table          (parse_table.txt)\n"
        "  token_file       Path to the scanned token stream (.cscn)\n"
        "  output_file      (optional) Path for the debug trace\n",
        prog);
}

/* ── main ───────────────────────────────────────────────────────────── */

int main(int argc, char *argv[])
{
    if (argc < 4) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *lang_file  = argv[1];
    const char *table_file = argv[2];
    const char *token_file = argv[3];
    const char *out_file   = (argc > 4) ? argv[4] : NULL;

    int rc = parser_run(lang_file, table_file, token_file, out_file);
    return (rc == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
