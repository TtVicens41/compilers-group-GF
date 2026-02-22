/**
 * @file scanner.h
 * @brief Lexical Analyzer Module: Scanner
 * @author Marc Bosch Manzano
 * @since 2026-02-08
 */

#ifndef SCANNER_H
#define SCANNER_H

#include "./token_lines.h"

TokenLines *scan_string(NFA *automaton, const char *string);

#endif /* SCANNER_H */