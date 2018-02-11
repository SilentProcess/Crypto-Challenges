#include <stdio.h>
#include <string.h>
#define MAXBUFLEN 4096

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int hamming_distance(char* a, char* b) {
  int distance = 0;
  int k, c;
  for (int j = 0; j < strlen(a); j++) {
    for(int i = sizeof(a)-1;i >= 0; i--) {
      k = a[j] >> i;
      c = b[j] >> i;
      if((k&1) != (c&1)) {
        distance++;
      }
    }
  }
  return distance;
}

int base64_index(char c) {
  const char* ptr = strchr(base64_table, c); 
    if(ptr) {
      int index = ptr - base64_table;
      return index;
    }
  return 0;
}

void base64_to_ascii(char* b64_text, char* ascii_text) {
  char a,b,c,d;
  int counter = 1;
  int h = 0;
  int i = 0;
  while(b64_text[i] != '\0') {
    switch (counter) {
      case 1:
       a = base64_index(b64_text[i]);
       break;
      case 2:
       b = base64_index(b64_text[i]);
       break;
      case 3:
       c = base64_index(b64_text[i]);
       break;
      case 4:
       d = base64_index(b64_text[i]);
       break;
    }
    if (counter%4 == 0) {
      // valued needs to be masked against 0xff because sprintf promotes
      // char to int and it can turn some higher bits to 1 so we zero them
      // out to get the byte we need
      sprintf(ascii_text+(h*2), "%02x", (((a << 2) | (b >> 4)) &0xff ));
      sprintf(ascii_text+(++h*2), "%02x", (((b << 4) | (c >> 2)) &0xff ));
      sprintf(ascii_text+(++h*2), "%02x", (((c << 6) | d) &0xff));
      h++;
      counter = 1;
      i++;
      continue;
    }
    counter++;
    i++;
  }
}

int main(int argc, char* argv[]) {
  char a[] = "this is a test";
  char b[] = "wokka wokka!!!";
  printf("hamming distance test: %d\n", hamming_distance(a,b));

  if (argc < 2) {
    printf("Missing filename!\n");
    printf("Usage: %s <filename>", argv[0]);
    return 1;
  }

  char basetext[MAXBUFLEN];

  char const* const fileName = argv[1];
  FILE* file = fopen(fileName, "r");
  if (!file) {
    printf("couldn't open file %s", argv[1]);
    return 1;
  } else {
    size_t fileLen = fread(basetext, sizeof(char), MAXBUFLEN, file);
    if (ferror (file) != 0) {
      printf("Error while reading file!\n");
    } else {
      basetext[fileLen++] = '\0';
    }
    fclose(file);
  }
  
  char asciitext[MAXBUFLEN*2];
  asciitext[0] = '\0';
  base64_to_ascii(basetext, asciitext);
  printf("%s", asciitext);
  printf("\n");
  return(0);
}
