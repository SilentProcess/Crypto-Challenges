#include <stdio.h>
#include <string.h>
#define MAXBUFLEN 4096

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char hex[] = "0123456789abcdef";

int char_to_int(char c) {
  const char* ptr = strchr(hex, c);
  if(ptr) {
    int index = ptr - hex;
    return index;
  }
  return 0;
}

int hamming_distance(char* a, char* b) {
  int distance = 0;
  char k, c;
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

int hamming_distance_char(char a, char b) {
  int distance = 0;
  char k, c;
  for(int i = 7; i >= 0; i--) {
    k = a >> i;
    c = b >> i;
    if((k & 0x01) != (c & 0x01)) {
      distance++;
    }
  }
  return distance;
}

int hex_to_ascii(char c1, char c2) {
  int upper = char_to_int(c1) * 16;
  int lower = char_to_int(c2);
  return upper+lower;
}


int base64_index(char c) {
  const char* ptr = strchr(base64_table, c); 
    if(ptr) {
      int index = ptr - base64_table;
      return index;
    }
  return 0;
}

void base64_to_hex(char* b64_text, char* ascii_text) {
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

int calculate_key_size(char* hextext) {
  int KEYSIZE;
  int i;
  int a = 2;
  unsigned char x;
  unsigned char y;
  unsigned char temp1;
  unsigned char temp2;
  int index = 0;
  int score = 0;
  for (KEYSIZE = 2; KEYSIZE <= 40; KEYSIZE++) {
    for(int i = 0; i < 10; i++) {
      for (int h = 0; h < KEYSIZE; h++) {
        x = hextext[index];
        y = hextext[index+1];
        temp1 = hex_to_ascii(x, y);
        //printf("T(1)%02x ", temp1);
        x = hextext[index+KEYSIZE*2];
        y = hextext[index+KEYSIZE*2+1];
        temp2 = hex_to_ascii(x, y);
        //printf("T(2)%02x ", temp2);
        score += hamming_distance_char(temp1, temp2);
        index += 2;
      }
      //printf("\n");
      index = KEYSIZE*2 *(i+1);
    }
    index = 0;
    //printf("-----------");
    score = ((score * 1000) / 5) / KEYSIZE;
    printf("score: %d key: %d\n", score, KEYSIZE);
    score = 0;
  }
  return score;
}

int main(int argc, char* argv[]) {
  char a[] = "this is a test";
  char b[] = "wokka wokka!!!";

  printf("hamming distance test: %d\n", hamming_distance(a,b));

  int distance = 0;
  for(int x = 0; x < strlen(a); x++) {
    distance += hamming_distance_char(a[x], b[x]);
  }
  printf("char dist: %d\n", distance);


  if (argc < 2) {
    printf("Missing filename!\n");
    printf("Usage: %s <filename>\n", argv[0]);
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
  
  char hex_text[MAXBUFLEN*2];
  hex_text[0] = '\0';
  base64_to_hex(basetext, hex_text);
  //printf("%s", hex_text);
  printf("Best key size: %d\n", calculate_key_size(hex_text));
  return(0);
}
