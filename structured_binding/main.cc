#include <tuple>
#include <iostream>

std::tuple<int, int> get_tuple() {
  return {1, 2};
}

int main(int argc, char* argv[]) {
  auto [first, second] = get_tuple();

  std::cout << "first = " << first << " "
            << "second = " << second << std::endl;

  return 0;
}