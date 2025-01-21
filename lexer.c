#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int position;
  char currentChar;
  int inputSize;
  char *input;
} Lexer;

void readCharacter(Lexer *t) {
  if((t->position+1) >= t->inputSize) {
    t->currentChar=0;
  } else {
    t->currentChar=t->input[t->position];
  }
  t->position++;
}

void lex(Lexer *t) {
  char **tokenList = NULL;
  int tokenCount = 0;
  printf("got here");
  while(t->currentChar!=0) {
    readCharacter(t);
    //if (currentCharacter == TokenType.String) {
      
    //}
    //else {
    tokenList = (char **)realloc(tokenList, sizeof(char)*(tokenCount+1));
    tokenList[t->position] = (char *)malloc(2*sizeof(char));
    tokenList[t->position][0] = t->currentChar;
    tokenList[t->position][1] = '\0';
    tokenCount++;
    printf("%d\n", tokenCount);  
  }
    //}
  printf("done lexing"); 
  for (int i = 0; i < tokenCount; i++) {
    printf("%s", tokenList[i]);
  }
 // printf("%ld", sizeof(tokenCount));
}

int main(){
  const char *json_data = "{"
    "\"id\": 101,"
    "\"name\": \"John Doe\","
    "\"email\": \"john.doe@example.com\","
    "\"active\": true,"
    "\"address\": {"
    "\"street\": \"123 Main St\","
    "\"city\": \"Anytown\","
    "\"state\": \"CA\","
    "\"zip\": \"12345\""
    "},"
    "\"phone_numbers\": ["
    "\"+1-555-1234\","
    "\"+1-555-5678\""
    "],"
    "\"tags\": ["
    "\"developer\","
    "\"coder\","
    "\"python\","
    "\"json\""
    "]"
    "}";
  Lexer t;
  t.currentChar = 0;
  t.position = 0;
  t.inputSize = strlen(json_data);
  t.input = (char *)malloc(t.inputSize + 1);
  Lexer *t_ptr = &t;
  lex(t_ptr);
}
