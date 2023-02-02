#include <filesystem>
#include <iostream>

constexpr char kTmpPath[] = "/tmp";

void list_directory(const std::filesystem::path path) {
  for (auto const& entry : std::filesystem::directory_iterator(path)) {
    std::cout << entry << std::endl;
  }
}

int main(int argc, char* argv[]) {
  std::filesystem::path path(kTmpPath);

  std::filesystem::path full_path = path / "somefile.txt";

  std::cout << full_path << std::endl;

  list_directory(path);

  return 0;
}
