#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  int position;
  char currentChar;
  int inputSize;
  char *input;
} Lexer;

void readCharacter(Lexer *t) {
  if ((t->position) >= t->inputSize) {
    t->currentChar = '\0';
  } else {
    t->currentChar = t->input[t->position];
  }
  t->position++;
}

void readJsonString(Lexer *t, char **jsonString, int *stringLength) {
  readCharacter(t);
  if (!(t->currentChar == '"')) {
    (*stringLength)++;
    *jsonString = realloc(*jsonString, (sizeof(char) * (*stringLength + 1)));
    (*jsonString)[(*stringLength) - 1] = t->currentChar;
    (*jsonString)[(*stringLength)] = '\0';
    readJsonString(t, jsonString, stringLength);
  } else {
    *stringLength = 0;
  }
}

void readJsonInteger(Lexer *t, int *jsonInteger) {
  int isNegative = 0;
  if ((t->input[t->position - 1]) == '-') {
    isNegative = 1;
    readCharacter(t);
    printf("nc");
  }
  while (isdigit(t->currentChar)) {
    *jsonInteger = ((*jsonInteger) * 10) + (t->currentChar - '0');
    readCharacter(t);
  }

  if (isNegative) {
    *jsonInteger = -(*jsonInteger);
  }
}

void lex(Lexer *t) {
  char **tokenList = NULL;
  int tokenCount = 0;

  while ((t->currentChar) != '\0') {
    readCharacter(t);
    if (!(t->currentChar == '"')) {
      if (t->currentChar == ' ') {
        continue;
      }
      if (isdigit((t->currentChar))) {
        int jsonInteger = 0;
        readJsonInteger(t, &jsonInteger);
        tokenList = realloc(tokenList, (tokenCount + 1) * sizeof(char *));
        tokenList[tokenCount] = malloc(20 * sizeof(char));
        sprintf(tokenList[tokenCount], "%d", jsonInteger);
        tokenCount++;
      } else if (t->currentChar == 't') {
          tokenList = realloc(tokenList, (tokenCount + 1) * sizeof(char *));
          tokenList[tokenCount] = strdup("true");
          readCharacter(t);
          readCharacter(t);
          readCharacter(t);
          tokenCount++;
      }else if (t->currentChar == 'f') {
          tokenList = realloc(tokenList, (tokenCount + 1) * sizeof(char *));
          tokenList[tokenCount] = strdup("false");
          readCharacter(t);
          readCharacter(t);
          readCharacter(t);
          readCharacter(t);
          tokenCount++;
      } else if (!(t->currentChar == '\0')) {
          tokenList = realloc(tokenList, sizeof(char *) * (tokenCount + 1));
          tokenList[tokenCount] = malloc(2 * sizeof(char));
          tokenList[tokenCount][0] = t->currentChar;
          tokenList[tokenCount][1] = '\0';
          tokenCount++;
      }
    } else if (t->currentChar == '"') {
      char *jsonString = malloc(1 * sizeof(char));
      int strLength = 0;
      readJsonString(t, &jsonString, &strLength);
      tokenList = realloc(tokenList, (tokenCount + 1) * sizeof(char *));
      tokenList[tokenCount] = strdup(jsonString);
      free(jsonString);
      tokenCount++;
    } else if (t->currentChar == 't') {
      tokenList = realloc(tokenList, (tokenCount + 1) * sizeof(char *));
      tokenList[tokenCount] = strdup("true");
      readCharacter(t);
      readCharacter(t);
      readCharacter(t);
    }
  }
  for (int i = 0; i < tokenCount - 1; i++) {
    printf("'%s', ", tokenList[i]);
  }
}

int main() {
  char *json_data = "{"
    "\"id\": 101,"
    "\"name\": \"John Doe\","
    "\"email\": \"john.doe@example.com\","
    "\"active\": false,"
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

