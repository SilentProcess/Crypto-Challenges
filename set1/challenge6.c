#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
      // mask against 0xff because sprintf promotes char to int 
      sprintf(ascii_text+(h*2), "%02x", (((a << 2) | (b >> 4)) &0xff ));
      sprintf(ascii_text+(++h*2), "%02x", (((b << 4) | (c >> 2)) &0xff ));
      sprintf(ascii_text+(++h*2), "%02x", (((c << 6) | d) &0xff));
      h++;
      i++;
      counter = 1;
      continue;
    }
    counter++;
    i++;
  }
}

int calculate_key_size(char* hextext) {
  int KEYSIZE, i, keysize;
  unsigned char x, y, temp1, temp2;
  int index = 0;
  int score = 0;
  int best_score = 100000;
  for (KEYSIZE = 2; KEYSIZE <= 40; KEYSIZE++) {
    for(int i = 0; i < 10; i++) {
      for (int h = 0; h < KEYSIZE; h++) {
        x = hextext[index];
        y = hextext[index+1];
        temp1 = hex_to_ascii(x, y);
        x = hextext[index+KEYSIZE*2];
        y = hextext[index+KEYSIZE*2+1];
        temp2 = hex_to_ascii(x, y);
        score += hamming_distance_char(temp1, temp2);
        index += 2;
      }
      index = KEYSIZE * 2 *(i+1);
    }
    index = 0;
    score = ((score * 1000) / 4) / KEYSIZE;
    if(score < best_score) {
      best_score = score;
      keysize = KEYSIZE;
    }
    score = 0;
  }
  return keysize;
}

void transpose_and_bruteforce(int keysize, int* keyarr, char* hextext) {
  unsigned char temp1, temp2;
  int key;
  int score = 0;
  int best_score = 0;
  int index = 0;
  int j = 0;
  for (int i = 0; i < keysize * 2; i += 2) {
    for (int h = 0; h < 255; h++) {
      index = 0;
      score = 0;
      while(index < strlen(hextext)) {
        temp1 = hextext[index+i];
        temp2 = hextext[index+i+1];
        score += evaluate(hex_to_ascii(temp1, temp2) ^ h);
        index += keysize * 2;
      }
      if (score > best_score) {
        best_score = score;
        key = h;
      }
    }
    best_score = 0;
    score = 0;
    index = 0;
    keyarr[j] = key;
    j++;
  }
}

void decode(int* keyarr, char* hextext, char* result, int keysize) {
  int h = 0;
  char c1, c2;
  for (int i = 0; i < strlen(hextext); i +=2) {
    c1 = hextext[i];
    c2 = hextext[i+1];
    result[h] = (hex_to_ascii(c1, c2)) ^ (keyarr[h%keysize]);
    h++;
  }
}

int main(int argc, char* argv[]) {
  int keysize;
  char a[] = "this is a test";
  char b[] = "wokka wokka!!!";

  int distance = 0;
  for(int x = 0; x < strlen(a); x++) {
    distance += hamming_distance_char(a[x], b[x]);
  }
  printf("hamming distance test: %d\n", distance);

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
  char result[MAXBUFLEN];
  int keyarr[29];
  hex_text[0] = '\0';
  result[0] = '\0';

  base64_to_hex(basetext, hex_text);
  printf("\n");
  keysize = calculate_key_size(hex_text);
  transpose_and_bruteforce(keysize, keyarr, hex_text);

  decode(keyarr, hex_text, result, keysize);
  printf("%s", result);
  return(0);
}
