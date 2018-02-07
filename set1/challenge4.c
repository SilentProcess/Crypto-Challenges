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


int decode(char* text, char* result, int best_score, int* finalkey) {
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
  if(top_score > best_score) {
    h = 0;
    *finalkey = key;
    for (int i = 0; i < strlen(text); i += 2) {
      c1 = text[i];
      c2 = text[i+1];
      result[h] = (hex_to_ascii(c1, c2)) ^ key;
      h++;
    }
  }
  return top_score;
}


int main(int argc, char* argv[]) {
  int currentscore = 0;
  int bestscore = 0;
  int key = 0;
  if (argc < 2) {
    printf("Missing filename!\n");
    printf("Usage: %s <filename>", argv[0]);
  }
  char const* const fileName = argv[1];
  FILE* file = fopen(fileName, "r");
  if (!file) {
    printf("couldn't open file %s", argv[1]);
    return 1;
  }

  char line[256];
  char result[256];
  result[0] = '\0';
  line[0] = '\0';
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\r\n")] = 0;
    currentscore = decode(line, result, bestscore, &key);
    if(currentscore > bestscore) {
      bestscore = currentscore;
    }
  }
  fclose(file);
  printf("The encrypted string is: %s", result);
  printf("The key is %c\n", key);
  return 0;
}
