#include <stdint.h>

struct padded_struct {
  char a;
  // 3 bytes padding
  int b;
  char c;
  // 3 bytes padding
};

union padded_union {
  int a;
  char b[5];
  // size is 8, max member size is 5, so 3 bytes padding at the end
};

struct no_padding_struct {
  int a;
  int b;
};

union no_padding_union {
  int a;
  char b[4];
};

int main() {
  struct padded_struct ps;
  union padded_union pu;
  struct no_padding_struct nps;
  union no_padding_union npu;
  return 0;
}
