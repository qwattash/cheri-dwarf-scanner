
struct inner_with_vla {
  int value;
  int vla[];
};

struct nested_with_vla {
  int value;
  struct inner_with_vla inner;
} a;

int main() { return (0); }
