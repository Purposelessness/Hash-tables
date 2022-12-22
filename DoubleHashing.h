#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#ifndef HASH_TABLES_DOUBLEHASHING_H_
#define HASH_TABLES_DOUBLEHASHING_H_

#include <iostream>
#include <string>
#include <vector>

#include "Hash.h"

template <typename T, THash<T> THash1 = H1, THash<T> THash2 = H2>
class DoubleHashing {
  static constexpr size_t kDefaultSize = 8;
  static constexpr double kRehashSize = 0.7;

 public:
  explicit DoubleHashing(size_t table_size = kDefaultSize)
      : _table_size(table_size), _data(std::vector<Bucket*>(_table_size)) {}

  ~DoubleHashing() {
    for (size_t i = 0; i < _table_size; ++i) {
      if (_data[i] != nullptr) {
        delete _data[i];
      }
    }
  }

  void resize() {
    size_t old_size = _table_size;
    _table_size *= 2;
    _size = 0;
    _all_size = 0;
    std::vector<Bucket*> t_data(_table_size);
    std::swap(_data, t_data);
    for (size_t i = 0; i < old_size; ++i) {
      if (t_data[i] != nullptr && t_data[i]->enabled) {
        insert(t_data[i]->value);
        delete t_data[i];
      }
    }
  }

  void rehash() {
    _all_size = 0;
    _size = 0;
    std::vector<Bucket*> t_data(_table_size);
    std::swap(_data, t_data);
    for (size_t i = 0; i < _table_size; ++i) {
      if (t_data[i] && t_data[i]->enabled) {
        insert(t_data[i]->value);
        delete t_data[i];
      }
    }
  }

  std::pair<bool, size_t> contains(const T& element) {
    size_t h1 = _t_hash_1(element, _table_size);
    size_t h2 = _t_hash_2(element, _table_size);
    size_t i = 0;
    for (; i < _table_size && _data[h1] != nullptr; ++i) {
      if (_data[h1]->enabled && _data[h1]->value == element) {
        std::pair<bool, size_t> out{true, i + 1};
        return out;
      }
      h1 = (h1 + h2) % _table_size;
    }
    std::pair<bool, size_t> out{false, i + 1};
    return out;
  }

  std::pair<bool, size_t> erase(const T& element) {
    size_t h1 = _t_hash_1(element, _table_size);
    size_t h2 = _t_hash_2(element, _table_size);
    size_t i = 0;
    for (; i < _table_size && _data[h1] != nullptr; ++i) {
      if (_data[h1]->enabled && _data[h1]->value == element) {
        _data[h1]->enabled = false;
        --_size;
        std::pair<bool, size_t> out{true, i + 1};
        return out;
      }
      h1 = (h1 + h2) % _table_size;
    }
    std::pair<bool, size_t> out{false, i + 1};
    return out;
  }

  std::pair<bool, size_t> insert(const T& element) {
    if (_size + 1 > static_cast<size_t>(kRehashSize * _table_size)) {
      resize();
    } else if (_all_size > 2 * _size) {
      rehash();
    }
    size_t h1 = _t_hash_1(element, _table_size);
    size_t h2 = _t_hash_2(element, _table_size);
    size_t i = 0;
    for (; i < _table_size && _data[h1] != nullptr; ++i) {
      if (_data[h1]->enabled && _data[h1]->value == element) {
        std::pair<bool, size_t> out{false, i + 1};
        return out;
      }
      if (!_data[h1]->enabled) {
        _data[h1]->value = element;
        _data[h1]->enabled = true;
        ++_size;
        std::pair<bool, size_t> out{true, i + 1};
        return out;
      }
      h1 = (h1 + h2) % _table_size;
    }
    _data[h1] = new Bucket(element);
    ++_all_size;
    ++_size;
    std::pair<bool, size_t> out{true, i + 1};
    return out;
  }

  void print() {
    std::cout << "---------------\n";
    printSize();
    for (size_t i = 0; i < _table_size; ++i) {
      if (_data[i] != nullptr) {
        std::cout << "Index: " << i << ", Value: " << _data[i]->value
                  << ", Enabled: " << _data[i]->enabled << '\n';
      }
    }
    std::cout << "---------------\n";
  }

  void printSize() {
    std::cout << "Size: " << _size << ", all size: " << _all_size
              << ", table size: " << _table_size << '\n';
  }

 private:
  struct Bucket {
    T value;
    bool enabled = true;

    explicit Bucket(const T& value) : value(value) {}
  };

  size_t _size = 0;
  size_t _all_size = 0;
  size_t _table_size = kDefaultSize;
  std::vector<Bucket*> _data;

  THash1 _t_hash_1;
  THash2 _t_hash_2;
};

#endif  // HASH_TABLES_DOUBLEHASHING_H_
#pragma clang diagnostic pop