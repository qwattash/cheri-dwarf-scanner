
struct struct_with_vla {
  int value;
  int vla[];
} a;

struct struct_with_size0_vla {
  int value;
  int vla[0];
} b;

struct struct_with_size1_vla {
  int value;
  int vla[1];
} c;

int main() { return (0); }
