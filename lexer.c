#include "token.h"

typedef struct {
  char input[],
  int position,
  char currentChar,
} Lexer;

void readCharacter(Token *t) {
  if((*t.position+1) >= sizeof(*t.input)) {
    t->currentChar=0;
  } else {
    t->currentChar=input[*t.position+1];
  }
  t->position++;
}
