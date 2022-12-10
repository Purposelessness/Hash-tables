#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#ifndef HASH_TABLES_DOUBLEHASHING_H_
#define HASH_TABLES_DOUBLEHASHING_H_

#define DEBUG

#include <iostream>
#include <string>
#include <vector>

#include "Hash.h"

template <typename T, typename F>
concept THash = requires(T t) {
                  {
                    t(std::declval<F>(), std::declval<size_t>())
                  } -> std::same_as<size_t>;
                };

template <typename T, THash<T> THash1 = H1, THash<T> THash2 = H2>
class DoubleHashing {
 public:
  explicit DoubleHashing() : _data(std::vector<Bucket*>(_table_size)) {}

  void resize() {
    size_t old_size = _table_size;
    _table_size *= 2;
    _size = 0;
    _all_size = 0;
    std::vector<Bucket*> new_data(_table_size);
    std::swap(_data, new_data);
    for (size_t i = 0; i < old_size; ++i) {
      if (new_data[i] != nullptr && new_data[i]->enabled) {
        insert(new_data[i]->value);
        delete new_data[i];
      }
    }
  }

  void rehash() {
    _all_size = 0;
    _size = 0;
    std::vector<Bucket*> new_data(_table_size);
    std::swap(_data, new_data);
    for (size_t i = 0; i < _table_size; ++i) {
      if (new_data[i] && new_data[i]->enabled) {
        insert(new_data[i]->value);
        delete new_data[i];
      }
    }
  }

  bool contains(const T& element) {
    size_t h1 = _t_hash_1(element, _table_size);
    size_t h2 = _t_hash_2(element, _table_size);
#ifdef DEBUG
    std::cout << "Contains '" << element << "'\n\th1: " << h1 << ", h2: " << h2
              << '\n';
#endif
    for (size_t i = 0; i < _table_size && _data[h1] != nullptr; ++i) {
      if (_data[h1]->enabled && _data[h1]->value == element) {
        return true;
      }
      h1 = (h1 + h2) % _table_size;
    }
    return false;
  }

  bool erase(const T& element) {
    size_t h1 = _t_hash_1(element, _table_size);
    size_t h2 = _t_hash_2(element, _table_size);
    for (size_t i = 0; i < _table_size && _data[h1] != nullptr; ++i) {
      if (_data[h1]->enabled && _data[h1]->value == element) {
        _data[h1]->enabled = false;
        --_size;
        return true;
      }
      h1 = (h1 + h2) % _table_size;
    }
  }

  bool insert(const T& element) {
    if (_size + 1 > static_cast<size_t>(kRehashSize * _table_size)) {
      resize();
    } else if (_all_size > 2 * _size) {
      rehash();
    }
    size_t h1 = _t_hash_1(element, _table_size);
    size_t h2 = _t_hash_2(element, _table_size);
#ifdef DEBUG
    std::cout << "Insert '" << element << "'\n\th1: " << h1 << ", h2: " << h2
              << '\n';
#endif
    for (size_t i = 0; i < _table_size && _data[h1] != nullptr; ++i) {
      if (_data[h1]->enabled && _data[h1]->value == element) {
        return false;
      }
      if (!_data[h1]->enabled) {
        _data[h1]->value = element;
        _data[h1]->enabled = true;
        ++_size;
        return true;
      }
      h1 = (h1 + h2) % _table_size;
    }
    _data[h1] = new Bucket(element);
    ++_all_size;
    ++_size;
    return true;
  }

  void print() {
    std::cout << "---------------\n";
    std::cout << "Size: " << _size << ", all size: " << _all_size
              << ", table size: " << _table_size << '\n';
    for (size_t i = 0; i < _table_size; ++i) {
      if (_data[i] != nullptr) {
        std::cout << "Index: " << i << ", Value: " << _data[i]->value
                  << ", Enabled: " << _data[i]->enabled << '\n';
      }
    }
    std::cout << "---------------\n";
  }

 private:
  struct Bucket {
    T value;
    bool enabled = true;

    explicit Bucket(const T& value) : value(value) {}
  };

  static constexpr size_t kDefaultSize = 8;
  static constexpr double kRehashSize = 0.75;

  size_t _size = 0;
  size_t _all_size = 0;
  size_t _table_size = kDefaultSize;
  std::vector<Bucket*> _data;

  THash1 _t_hash_1;
  THash2 _t_hash_2;
};

#endif  // HASH_TABLES_DOUBLEHASHING_H_
#pragma clang diagnostic pop