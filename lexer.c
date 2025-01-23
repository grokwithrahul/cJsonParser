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

void readJsonString(Lexer *t, char **jsonString, int *stringLength) {
  readCharacter(t);
  if(!(t->currentChar=='"')) {
    (*stringLength)++;
    *jsonString = realloc(*jsonString, (sizeof(char)*(*stringLength+1)));
    (*jsonString)[(*stringLength)-1] = t->currentChar;
    (*jsonString)[(*stringLength)] = '\0';
    readJsonString(t, jsonString, stringLength);
  } else {
    *stringLength = 0;
  }
}

void lex(Lexer *t) {
  char **tokenList = NULL;
  int tokenCount = 0;
  
  while((t->currentChar)!='\0') {
    readCharacter(t);
    if (!(t->currentChar=='"')) {  
      if(!(t->currentChar=='\0')) {
        tokenList = realloc(tokenList, sizeof(char*)*(tokenCount+1));
        tokenList[tokenCount] = malloc(2*sizeof(char));
        tokenList[tokenCount][0] = t->currentChar;
        tokenList[tokenCount][1] = '\0';
        tokenCount++;
      }
    } else {
      char *jsonString = malloc(1 * sizeof(char));
      int strLength = 0;
      readJsonString(t, &jsonString, &strLength);
      tokenList = realloc(tokenList, (tokenCount+1)*sizeof(char*));
      tokenList[tokenCount] = strdup(jsonString);
      tokenCount++;
    }
  }
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
