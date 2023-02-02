#include <iostream>
#include <variant>
#include <any>
#include <string>
#include <cassert>

enum class MathError {
  Undefined,
  DivByZero,
};

using MathResult = std::variant<int, MathError>;

MathResult divide(int divident, int divisor) {
  if (divisor == 0) return MathError::DivByZero;

  return divident / divisor;
}

int main() {
  std::any some_number = 4;
  std::cout << std::any_cast<int>(some_number) << std::endl;

  auto result = divide(25, 0);
  if (std::holds_alternative<MathError>(result)) {
    switch (std::get<MathError>(result)) {
      case MathError::Undefined:
        std::cout << "Undefined" << std::endl;
        break;
      case MathError::DivByZero:
        std::cout << "Divide By Zero" << std::endl;
        break;
    }
  } else {
    auto value = std::get<int>(result);
    std::cout << value << std::endl;
  }


  // Avoiding exceptions when doing an any cast.
  std::any some_string = std::string("Hello World!");
  int* v = std::any_cast<int>(&some_string);    // Note any_cast to `int` and not `int*`
  assert(v == nullptr);

  return 0;
}