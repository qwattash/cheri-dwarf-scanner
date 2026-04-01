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

// 1. Pointer padding
struct pointer_padding {
  char a;
  // 15 bytes padding to align the pointer
  void *p;
  char b;
  // 15 bytes padding at the end to align the struct size to 16
};

// 2. Nested structure with tail padding and parent padding
struct align_inner {
  void *p; // size 16, align 16
  char b;  // size 1, align 1
  // 15 bytes tail padding to make size 32
};

struct parent_padding {
  char c; // size 1, align 1
  // 15 bytes padding to align the inner struct to 16
  struct align_inner inner; // size 32, align 16
  char d;                   // size 1, align 1
  // 15 bytes tail padding to make size 64
};

// 3. Array of nested structs
struct array_of_nested {
  char a; // size 1, align 1
  // 15 bytes padding to align the array to 16
  struct align_inner arr[2]; // size 64 (2 * 32), align 16
  char b;                    // size 1, align 1
  // 15 bytes tail padding to make size 96
};

// 4. Array padding (simple)
struct array_padding {
  char a;
  // 3 bytes padding
  int arr[3];
  char b;
  // 3 bytes padding at the end
};

// 5. Union with pointers
union pointer_union {
  void *p;    // size 16, align 16
  char a[17]; // size 17, align 1
  // The union size will be rounded up to a multiple of 16 (so 32).
  // The largest member is 17 bytes, so there will be 15 bytes of padding at the
  // end.
};

int main() {
  struct padded_struct ps;
  union padded_union pu;
  struct no_padding_struct nps;
  union no_padding_union npu;
  struct pointer_padding pp;
  struct parent_padding p_pad;
  struct array_of_nested aon;
  struct array_padding ap;
  union pointer_union p_u;
  return 0;
}
