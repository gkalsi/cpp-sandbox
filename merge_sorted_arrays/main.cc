#include <iostream>
#include <optional>
#include <vector>

template <typename T>
std::vector<T> merge_lists(const std::vector<std::vector<T>>& vecs) {
  std::vector<size_t> fronts(vecs.size(), 0);
  std::vector<T> result;

  while (true) {
    std::optional<size_t> max_candidate_idx = std::nullopt;
    for (size_t i = 0; i < fronts.size(); i++) {
      if (fronts[i] == vecs[i].size()) continue;

      if (max_candidate_idx == std::nullopt) {
        max_candidate_idx = i;
      } else {
        if (vecs[i][fronts[i]] <
            vecs[*max_candidate_idx][fronts[*max_candidate_idx]]) {
          max_candidate_idx = i;
        }
      }
    }

    if (max_candidate_idx == std::nullopt) {
      return result;
    }

    result.push_back(vecs[*max_candidate_idx][fronts[*max_candidate_idx]]);
    fronts[*max_candidate_idx]++;
  }
}

int main(int argc, char* argv[]) {
  std::vector<int> a = {1, 3, 5, 7};
  std::vector<int> b = {
      0,
      2,
  };
  std::vector<int> c = {0, 3, 4, 5, 12, 13};

  std::vector<std::vector<int>> in = {a, b, c};

  auto sorted = merge_lists<int>(in);

  for (int i : sorted) {
    std::cout << i << " ";
  }
  std::cout << std::endl;
}