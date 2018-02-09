#include <stdio.h>
#include <string.h>

static const char key[] = "ICE";

int main(int argc, char* argv[]) {
  int c;
  int i = 0;
  if (argc < 2) {
    printf("Missing filename!\n");
    printf("Usage: %s <filename>", argv[0]);
    return 1;
  }
  char const* const fileName = argv[1];
  FILE* file = fopen(fileName, "r");
  if (!file) {
    printf("couldn't open file %s", argv[1]);
    return 1;
  }

  while((c = fgetc(file)) != EOF) {
    if(i%3 == 0) {
      i = 0;
    }
    printf("%02x", c ^ key[i]);
    i++;
  }
  return(0);
}
