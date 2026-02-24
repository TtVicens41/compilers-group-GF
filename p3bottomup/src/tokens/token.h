/**
 * @file token.h
 * @brief Single token representation.
 */

#ifndef TOKEN_H
#define TOKEN_H

typedef struct Token {
    int type;           /* terminal symbol id from the Language */
    char lexeme[64];
    struct Token *next; /* intrusive list pointer */
} Token;

/** Allocate and initialise a token. Returns NULL on failure. */
Token *token_create(int type, const char *lexeme);

/** Free a single token (does NOT follow ->next). */
void token_destroy(Token *token);

#endif /* TOKEN_H */
