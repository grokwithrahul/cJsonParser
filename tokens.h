#ifndef TOKENS_H
#define TOKENS_H
#endif

typedef enum {
  ERROR,
  ILLEGAL,
  COMMA,
  COLON,
  LCURLY,
  RCURLY,
  STRING,
  LSQUARE,
  RSQUARE,
} TokenType;

typedef struct {
  TokenType type;
  char literal[256];
} Token;
