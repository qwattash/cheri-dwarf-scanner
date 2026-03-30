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

struct bitfield_struct bf;
struct mixed_bitfield_struct mbf;
struct anon_bitfield_struct abf;

int main() { return 0; }
