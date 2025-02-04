#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum { 
  JSON_OBJECT, JSON_ARRAY, JSON_STRING, JSON_NUMBER, JSON_BOOL 
} JsonType;

typedef struct JsonNode {
  JsonType type;
  union {
    struct {
      char **keys;
      struct JsonNode **values;
      size_t size;
    } object;
    struct {
      struct JsonNode **elements;
      size_t size;
    } array;
    char *string;
    double number;
    bool boolean;
  } value;
} JsonNode;

void json_free(JsonNode *node);
JsonNode* json_parse_value(const char **tokens, size_t *index);
JsonNode* json_parse_object(const char **tokens, size_t *index);
JsonNode* json_parse_array(const char **tokens, size_t *index);
JsonNode* json_parse_string(const char **tokens, size_t *index);
JsonNode* json_parse_number(const char **tokens, size_t *index);
JsonNode* json_parse_bool(const char **tokens, size_t *index);

void skip_whitespace(const char **tokens) {
  while (isspace(**tokens)) {
    (*tokens)++;
  }
}

JsonNode* create_string_node(const char *str) {
  JsonNode *node = malloc(sizeof(JsonNode));
  node->type = JSON_STRING;
  node->value.string = strdup(str);
  return node;
}

JsonNode* create_number_node(double number) {
  JsonNode *node = malloc(sizeof(JsonNode));
  node->type = JSON_NUMBER;
  node->value.number = number;
  return node;
}

JsonNode* create_bool_node(bool boolean) {
  JsonNode *node = malloc(sizeof(JsonNode));
  node->type = JSON_BOOL;
  node->value.boolean = boolean;
  return node;
}

JsonNode* json_parse_value(const char **tokens, size_t *index) {
  skip_whitespace(tokens);
  if (**tokens == '{') {
    return json_parse_object(tokens, index);
  } else if (**tokens == '[') {
    return json_parse_array(tokens, index);
  } else if (**tokens == '"') {
    return json_parse_string(tokens, index);
  } else if (isdigit(**tokens) || **tokens == '-') {
    return json_parse_number(tokens, index);
  } else if (**tokens == 't' || **tokens == 'f') {
    return json_parse_bool(tokens, index);
  }
  return NULL;
}

JsonNode* json_parse_object(const char **tokens, size_t *index) {
  JsonNode *node = malloc(sizeof(JsonNode));
  node->type = JSON_OBJECT;
  node->value.object.size = 0;
  node->value.object.keys = NULL;
  node->value.object.values = NULL;

  (*tokens)++;
  skip_whitespace(tokens);

  while (**tokens != '}') {
    if (**tokens == '"') {
      JsonNode *key_node = json_parse_string(tokens, index);
      skip_whitespace(tokens);
      if (**tokens != ':') {
        return NULL;
      }
      (*tokens)++;
      skip_whitespace(tokens);

      JsonNode *value_node = json_parse_value(tokens, index);

      node->value.object.size++;
      node->value.object.keys = realloc(node->value.object.keys, node->value.object.size * sizeof(char *));
      node->value.object.values = realloc(node->value.object.values, node->value.object.size * sizeof(JsonNode *));
      node->value.object.keys[node->value.object.size - 1] = key_node->value.string;
      node->value.object.values[node->value.object.size - 1] = value_node;

      skip_whitespace(tokens);

      if (**tokens == ',') {
        (*tokens)++;
        skip_whitespace(tokens);
      }
    } else {
      return NULL;
    }
  }
  (*tokens)++;
  return node;
}

JsonNode* json_parse_array(const char **tokens, size_t *index) {
  JsonNode *node = malloc(sizeof(JsonNode));
  node->type = JSON_ARRAY;
  node->value.array.size = 0;
  node->value.array.elements = NULL;

  (*tokens)++;
  skip_whitespace(tokens);

  while (**tokens != ']') {
    JsonNode *value_node = json_parse_value(tokens, index);

    node->value.array.size++;
    node->value.array.elements = realloc(node->value.array.elements, node->value.array.size * sizeof(JsonNode *));
    node->value.array.elements[node->value.array.size - 1] = value_node;

    skip_whitespace(tokens);

    if (**tokens == ',') {
      (*tokens)++;
      skip_whitespace(tokens);
    }
  }
  (*tokens)++;
  return node;
}

JsonNode* json_parse_string(const char **tokens, size_t *index) {
  (*tokens)++;
  const char *start = *tokens;

  while (**tokens != '"') {
    (*tokens)++;
  }

  size_t length = *tokens - start;
  char *str = malloc(length + 1);
  strncpy(str, start, length);
  str[length] = '\0';

  (*tokens)++;
  return create_string_node(str);
}

JsonNode* json_parse_number(const char **tokens, size_t *index) {
  char *end;
  double num = strtod(*tokens, &end);
  *tokens = end;
  return create_number_node(num);
}

JsonNode* json_parse_bool(const char **tokens, size_t *index) {
  if (strncmp(*tokens, "true", 4) == 0) {
    *tokens += 4;
    return create_bool_node(true);
  } else if (strncmp(*tokens, "false", 5) == 0) {
    *tokens += 5;
    return create_bool_node(false);
  }
  return NULL;
}

void json_free(JsonNode *node) {
  if (!node) return;
  if (node->type == JSON_OBJECT) {
    for (size_t i = 0; i < node->value.object.size; i++) {
      free(node->value.object.keys[i]);
      json_free(node->value.object.values[i]);
    }
    free(node->value.object.keys);
    free(node->value.object.values);
  } else if (node->type == JSON_ARRAY) {
    for (size_t i = 0; i < node->value.array.size; i++) {
      json_free(node->value.array.elements[i]);
    }
    free(node->value.array.elements);
  } else if (node->type == JSON_STRING) {
    free(node->value.string);
  }
  free(node);
}

int main() {
  const char *json_tokens[] = {
    "{", "\"id\"", ":", "\"123\"", "}", NULL
  };
  size_t index = 0;

  JsonNode *root = json_parse_object(&json_tokens[index], &index);

  if (root) {
    printf("Parsed JSON object successfully!\n");
    json_free(root);
  } else {
    printf("Failed to parse JSON.\n");
  }

  return 0;
}
