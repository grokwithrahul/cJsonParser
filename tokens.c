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
}
