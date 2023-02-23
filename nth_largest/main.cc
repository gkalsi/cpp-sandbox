#include <iostream>
#include <vector>
#include <optional>
#include <algorithm>

template <typename T>
std::optional<T> nth_largest(const std::vector<T>& v, size_t n) {
  std::vector<T> results;
  for (const T& e : v) {
    auto insertion_point = std::upper_bound(results.begin(), results.end(), e,
    [](const T& lhs, const T& rhs) {
      return lhs > rhs;
    });
    results.insert(insertion_point, e);

    if (results.size() > n) {
      results.pop_back();
    }
  }

  if (results.size() == n) {
    return results.at(n - 1);
  }

  return std::nullopt;
}

int main(int argc, char* argv[]) {
  std::vector<int> test = {
    8, 2, 4, 12, 9, 0, 5
  };

  std::optional<int> result = nth_largest(test, 2);

  if (!result) {
    std::cerr << "Did not return a value" << std::endl;
  } else {
    std::cout << "Nth largest value is " << *result << std::endl;
  }

  return 0;
}