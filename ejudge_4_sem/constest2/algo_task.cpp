#include <iostream>
#include <unordered_map>
#include <string>
#include <cmath>
#include <list>
#include <stack>
#include <queue>
#include <set>
class LRUCache {
 public:
  explicit LRUCache(size_t n) : size(n) {}

  void put(int key, int value) {
    if (values.find(key) != values.end()) {
      keys.erase(values[key].first);
    }
    keys.push_front(key);
    values.insert({key, std::make_pair(keys.begin(), value)});
    if (values.size() > size) {
      values.erase(keys.back());
      keys.pop_back();
    }
  }

  int get(int key) {
    if (values.contains(key)) {
      keys.erase(values[key].first);
      keys.push_front(key);
      return values[key].second;
    }
    return -1;
  }

 private:
  std::unordered_map<int, std::pair<std::list<int>::iterator, int>> values;
  std::list<int> keys;
  const size_t size;
};

int main() {
  size_t n;
  std::cin >> n;
  LRUCache obj(n);
  char i;
  int key;
  int value;
  int query;
  std::cin >> query;
  while (query--) {
    std::cin >> i;
    if (i == 'p') {
      std::cin >> key >> value;
      obj.put(key, value);
    } else {
      std::cin >> key;
      std::cout << obj.get(key);
    }
    using value = std::pair<int, int>;
    std::priority_queue<value, std::vector<value>, std::greater<value>> min_heap;
    min_heap.emplace(5,5);
  }
  std::set<int> a;
}



//3 10
//p 1 1
//p 2 2
//p 3 3
//p 4 4
//g 1
//-1g 3
//3p 5 5
//p 6 6
//g 4
