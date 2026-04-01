struct bitfield_struct {
  int a : 3;
  int b : 5;
  int c : 24;
};

struct mixed_bitfield_struct {
  char x;
  int y : 15;
  int z : 17;
  long w;
};

struct anon_bitfield_struct {
  int a : 3;
  int : 5;
  int b : 4;
};

struct offset_not_byte_boundary {
  short a;
  int b : 3;
  int c : 6;
};

struct boundary_bitfields {
  char a;
  int b : 8;
  int c : 16;
};

struct bitfield_struct bf;
struct mixed_bitfield_struct mbf;
struct anon_bitfield_struct abf;
struct offset_not_byte_boundary onbb;
struct boundary_bitfields bb;

int main() { return 0; }

struct large_bitfield_struct {
  int a : 32;
  int b : 16;
};

struct large_bitfield_struct lbf;
