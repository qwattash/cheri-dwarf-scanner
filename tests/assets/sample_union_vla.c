
union union_with_vla_mix {
  int vla[0];
  float value;
} a;

union union_with_vla {
  float value;
  int vla[0];
} b;

int main() { return (0); }
