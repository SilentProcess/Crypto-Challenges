#include <stdio.h>
#include <string.h>
#include <ctype.h>

static const char hex[] = "0123456789abcdef";

int char_to_int(char c) {
  const char* ptr = strchr(hex, c);
  if(ptr) {
    int index = ptr - hex;
    return index;
  }
  return 0;
}

int hex_to_ascii(char c1, char c2) {
  int upper = char_to_int(c1) * 16;
  int lower = char_to_int(c2);
  return upper + lower;
}

int evaluate(char c) {
  switch(tolower(c)) {
    case 'e':
      return 1;
      break;
    case 't':
      return 1;
      break;
    case 'a':
      return 1;
      break;
    case 'o':
      return 1;
      break;
    case 'i':
      return 1;
      break;
    case 'n':
      return 1;
      break;
    case ' ':
      return 1;
      break;
    case 's':
      return 1;
      break;
    case 'h':
      return 1;
      break;
    case 'r':
      return 1;
      break;
    case 'd':
      return 1;
      break;
    case 'l':
      return 1;
      break;
    case 'u':
      return 1;
      break;
    default:
      return 0;
  }
}

void decode(char* text, char* result) {
  char c1, c2, ascii;
  int score = 0; 
  int top_score = 0;
  int h = 0;
  int key = 0;
  for(int j = 0; j < 255; j++) {
    h = 0;
    score = 0;
    for (int i = 0; i < strlen(text); i += 2) {
      c1 = text[i];
      c2 = text[i+1];
      ascii = (hex_to_ascii(c1, c2)) ^ j;
      score += evaluate(ascii);
      h++;
    }
    if(score > top_score) {
      top_score = score;
      key = j;
    }
  }
  h = 0;
  for (int i = 0; i < strlen(text); i += 2) {
    c1 = text[i];
    c2 = text[i+1];
    result[h] = (hex_to_ascii(c1, c2)) ^ key;
    h++;
  }
  printf("Resulting string: ");
  printf("%s\n", result);
  printf("key: %c\n", key);
}

int main() {
  int i;
  char text[1024];
  text[0] = '\0';
  printf("input hex string: ");
  fgets(text, 1024, stdin);
  text[strcspn(text, "\r\n")] = 0;
  char result[strlen(text)/2];
  result[0] = '\0';
  printf("Decoding...\n");
  decode(text, result);
}
