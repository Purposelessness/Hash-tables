#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#ifndef HASH_TABLES__CHAINMETHOD_H_
#define HASH_TABLES__CHAINMETHOD_H_

#include <iostream>
#include <vector>

#include "Hash.h"

template <typename T, THash<T> THash = H1>
class ChainMethod {
  static constexpr size_t kDefaultSize = 8;
  static constexpr double kRehashSize = 0.7;

 public:
  explicit ChainMethod(size_t table_size = kDefaultSize)
      : _table_size(table_size), _data(std::vector<Bucket*>(_table_size)) {}

  ~ChainMethod() {
    for (size_t i = 0; i < _table_size; ++i) {
      if (_data[i] == nullptr) {
        continue;
      }
      auto* prev = _data[i];
      for (auto* bucket = _data[i]->next;;
           prev = bucket, bucket = bucket->next) {
        delete prev;
        if (bucket == nullptr) {
          break;
        }
      }
    }
  }

  void resize() {
    size_t old_size = _table_size;
    _table_size *= 2;
    _size = 0;
    _bucket_size = 0;
    std::vector<Bucket*> t_data(_table_size);
    std::swap(_data, t_data);
    for (size_t i = 0; i < old_size; ++i) {
      if (t_data[i] == nullptr) {
        continue;
      }
      for (auto* bucket = t_data[i]; bucket != nullptr; bucket = bucket->next) {
        insert(bucket->value);
      }
    }
  }

  std::pair<bool, size_t> contains(const T& element) {
    size_t h = _t_hash(element, _table_size);
    if (_data[h] == nullptr) {
      std::pair<bool, size_t> out{false, 1};
      return out;
    }
    size_t i = 0;
    for (auto* bucket = _data[h]; bucket != nullptr;
         bucket = bucket->next, ++i) {
      if (bucket->value == element) {
        std::pair<bool, size_t> out{true, i + 1};
        return out;
      }
    }
    std::pair<bool, size_t> out{false, i + 1};
    return out;
  }

  std::pair<bool, size_t> erase(const T& element) {
    size_t h = _t_hash(element, _table_size);
    if (_data[h] == nullptr) {
      std::pair<bool, size_t> out{false, 1};
      return out;
    }
    auto* bucket = _data[h];
    if (bucket->value == element) {
      if (bucket->next == nullptr) {
        --_bucket_size;
      }
      --_size;
      _data[h] = bucket->next;
      delete bucket;
      std::pair<bool, size_t> out{true, 1};
      return out;
    }
    size_t i = 0;
    for (; bucket->next != nullptr; bucket = bucket->next, ++i) {
      if (bucket->next->value == element) {
        --_size;
        auto* tmp = bucket->next;
        bucket->next = bucket->next->next;
        delete tmp;
        std::pair<bool, size_t> out{true, i + 1};
        return out;
      }
    }
    std::pair<bool, size_t> out{false, i + 1};
    return out;
  }

  std::pair<bool, size_t> insert(const T& element) {
    if (_size + 1 > static_cast<size_t>(kRehashSize * _table_size)) {
      resize();
    }
    size_t h = _t_hash(element, _table_size);
    if (_data[h] == nullptr) {
      _data[h] = new Bucket(element);
      ++_bucket_size;
      ++_size;
      std::pair<bool, size_t> out{true, 1};
      return out;
    }
    auto* bucket = _data[h];
    size_t i = 0;
    for (;; bucket = bucket->next, ++i) {
      if (bucket->value == element) {
        std::pair<bool, size_t> out{false, i + 1};
        return out;
      }
      if (bucket->next == nullptr) {
        break;
      }
    }
    bucket->next = new Bucket(element);
    ++_size;
    std::pair<bool, size_t> out{true, i + 1};
    return out;
  }

  void print() {
    std::cout << "--------------\n";
    printSize();
    for (size_t i = 0; i < _table_size; ++i) {
      if (_data[i] == nullptr) {
        continue;
      }
      std::cout << i << ": ";
      for (auto* bucket = _data[i]; bucket != nullptr; bucket = bucket->next) {
        std::cout << bucket->value << " -> ";
      }
      std::cout << "null\n";
    }
    std::cout << "--------------\n";
  }

  void printSize() {
    std::cout << "Size: " << _size << ", Bucket size: " << _bucket_size
              << ", Table size: " << _table_size << '\n';
  }

 private:
  struct Bucket {
    T value;
    Bucket* next = nullptr;

    explicit Bucket(const T& value) : value(value) {}
  };

  size_t _bucket_size = 0;
  size_t _size = 0;
  size_t _table_size = kDefaultSize;
  std::vector<Bucket*> _data;

  THash _t_hash;
};

#endif  // HASH_TABLES__CHAINMETHOD_H_
#pragma clang diagnostic pop