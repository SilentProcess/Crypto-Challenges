#include <stdio.h>
#include <stdlib.h>
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
        }
      }
    }
  }
}

void bin_to_base64(char* bin_text, char* res) {
  int i, j, h, k, len, index;
  char bina[7];
  h = 0;
  index = 0;
  len = strlen(bin_text);
  printf("Entering loop!\n");
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
}

int main() {
  int i;
  char text[128] = "49276d206b696c6c696e6720796f757220627261696e2i06c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  size_t len = strlen(text);
  printf("%d\n", len);
  char bin_text[len*4+4];
  char hex_text[len+1];
  char base64_text[3000];
  bin_text[0] = '\0';
  hex_text[0] = '\0';
  base64_text[0] = '\0';

  hex_to_bin(text, bin_text, len);
  printf("%s\n", bin_text);

  bin_to_hex(bin_text, hex_text);
  printf("%s\n", hex_text);

  bin_to_base64(bin_text, base64_text);
  printf("%s\n", base64_text);
  return 0;
}
