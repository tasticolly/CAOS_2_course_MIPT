#include <string>
#include <iostream>

int main() {
  std::string str;
  std::cin >> str;
  int i = 0;
  int counter = 1;
  char current = str[0];
  for (size_t j = 1; j <= str.size(); ++j) {
    if (j == str.size() || current != str[j]) {
      str[i++] = current;
      if (counter > 1) {
        for (char& symb : std::to_string(counter)) {
          str[i++] = symb;
        }
      }
      if (j != str.size()) {
        counter = 1;
        current = str[j];
      }
    } else {
      ++counter;
    }
  }
  std::cout << str.substr(0, i);
}
