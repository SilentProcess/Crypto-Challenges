#include <stdio.h>
#include <string.h>

static const char hex[] = "0123456789abcdef";
static const char key[] = "686974207468652062756c6c277320657965";

int char_to_int(char c) {
  const char* ptr = strchr(hex, c);
  if(ptr) {
    int index = ptr - hex;
    return index;
  }
  return 0;
}

void xor(char* text, char* result) {
  int index = 0;
  for(int i = 0; i < strlen(text); i++) {
    index = (char_to_int(text[i])) ^ (char_to_int(key[i]));
    result[i] = hex[index];
  }
}

int main() {
  int i;
  char text[1024];
  text[0] = '\0';
  printf("input hex string: ");
  fgets(text, 1024, stdin);
  text[strcspn(text, "\r\n")] = 0;
  char result[strlen(text)];
  result[0] = '\0';
  xor(text, result);
  printf("%s\n", result);
  return 0;
}
