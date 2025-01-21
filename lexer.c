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
  if((t->position) >= t->inputSize) {
    t->currentChar='\0';
  } else {
    t->currentChar=t->input[t->position];
  }
  t->position++;
}

void lex(Lexer *t) {
  char **tokenList = NULL;
  int tokenCount = 0;
  
  while((t->currentChar)!='\0') {
    readCharacter(t);
  //if (currentCharacter == TokenType.String) {
      
    //}
    //else {
    if(!(t->currentChar=='\0')) {
      tokenList = realloc(tokenList, sizeof(char*)*(tokenCount+1));
      tokenList[tokenCount] = malloc(2*sizeof(char));
      tokenList[tokenCount][0] = t->currentChar;
      tokenList[tokenCount][1] = '\0';
      tokenCount=t->position;
    }
  }
    //}
  for (int i = 0; i <= t->inputSize-1; i++) {
    printf("'%s', ", tokenList[i]);
  }
}

int main(){
  char *json_data = "{"
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
  t.currentChar = '0';
  t.position = 0;
  t.inputSize = strlen(json_data);
  t.input = (char *)malloc(t.inputSize + 1);
  t.input = json_data;
  Lexer *t_ptr = &t;
  lex(t_ptr);
  return 0;
}
