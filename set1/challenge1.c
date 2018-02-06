#include <stdio.h>
#include <string.h>

static const char bin[16][5] = {"0000", "0001", "0010", "0011", "0100",
  "0101", "0110", "0111", "1000", "1001", "1010", "1011",
  "1100", "1101", "1110", "1111"};
static const char hex[] = "0123456789abcdef";
static const char base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void hex_to_bin(char* hex_text, char* res, int len) {
  int i;
  for (i = 0; i < len; i++) {
    const char *ptr = strchr(hex, hex_text[i]);
    if (ptr) {
      int index = ptr - hex;
      strncat(res, bin[index], 4);
    }
  }
}

void bin_to_hex(char* bin_text, char* res) {
  int i, j, h, len;
  h = 0;
  char hexa[5];
  len = strlen(bin_text);
  for (i = 0; i < len; i++) {
    hexa[i%4] = bin_text[i];
    if (i%4 == 3) {
      hexa[5] = '\0';
      for (j = 0; j < 16; j++) {
        if (strcmp(hexa, bin[j]) == 0) {
          res[h] = hex[j];
          h++;
          res[h] = '\0';
        }
      }
    }
  }
}

void bin_to_base64(char* bin_text, char* res) {
  int i, j, h, k, p, len, index;
  char bina[7];
  char pad[17] = "0000000000000000";
  h = 0;
  index = 0;
  p = 0;
  len = strlen(bin_text);
  if (len%6 == 2) {
    strncat(bin_text, pad, 16);
    p = 16;
  }
  if (len%6 == 4) {
    strncat(bin_text, pad, 8);
    p = 8;
  }
  len += p;
  printf("%d\n", len);
  printf("%s\n", bin_text);
  for (i = 0; i < len; i++) {
    bina[i%6] = bin_text[i];
    if (i%6 == 5) {
      k = 0;
      bina[7] = '\0';
      for (j = (strlen(bina)-1); j >= 0; j--) {
        if (bina[j] == '1') {
          index += (1 << k);
        }
        k++;
      }
      res[h] = base64_table[index];
      h++;
      index = 0;
    }
  }
  
  if (p != 0) {
    if (p == 16) {
      if (res[h-1] == 'A') {
        res[h-1] = '=';
      }
      if (res[h-2] == 'A') {
        res[h-2] = '=';
      }
    }
    if (p == 8) {
      if (res[h-1] == 'A') {
        res[h-1] = '=';
      }
    }
  }
}

int main() {
  int i;
  char text[1024];
  text[0] = '\0';
  fgets(text, 1024, stdin);
  text[strcspn(text, "\n")] = 0;
  if (strlen(text)%2 != 0) {
    printf("The hex string is odd\n");
    printf("%d", strlen(text));
    return 1;
  }
  size_t len = strlen(text);
  printf("lenght of initial text %d\n", len);
  char bin_text[len*4+4];
  char hex_text[len+1];
  char base64_text[4096];
  bin_text[0] = '\0';
  hex_text[0] = '\0';
  base64_text[0] = '\0';

  hex_to_bin(text, bin_text, len);
  printf("hex to bin: %s\n", bin_text);

  bin_to_hex(bin_text, hex_text);
  printf("bin to hex: %s\n", hex_text);

  bin_to_base64(bin_text, base64_text);
  printf("bin to base64: %s\n", base64_text);
  return 0;
}
