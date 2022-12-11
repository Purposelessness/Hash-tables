#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#ifndef HASH_TABLES__CHAINMETHOD_H_
#define HASH_TABLES__CHAINMETHOD_H_

#include <iostream>
#include <vector>

#include "Hash.h"

template <typename T, THash<T> THash = H>
class ChainMethod {
 public:
  ChainMethod() : _data(std::vector<Bucket*>(_table_size)) {}

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
      ++_bucket_size;
      for (auto* bucket = t_data[i]; bucket != nullptr;
           ++_size, bucket = bucket->next) {
        insert(bucket->value);
      }
    }
  }

  bool contains(const T& element) {
    size_t h = _t_hash(element, _table_size);
    if (_data[h] == nullptr) {
      return false;
    }
    for (auto* bucket = _data[h]; bucket != nullptr; bucket = bucket->next) {
      if (bucket->value == element) {
        return true;
      }
    }
    return false;
  }

  bool erase(const T& element) {
    size_t h = _t_hash(element, _table_size);
    if (_data[h] == nullptr) {
      return false;
    }
    auto* bucket = _data[h];
    if (bucket->value == element) {
      if (bucket->next == nullptr) {
        --_bucket_size;
      }
      --_size;
      _data[h] = bucket->next;
      delete bucket;
      return true;
    }
    for (; bucket->next != nullptr; bucket = bucket->next) {
      if (bucket->next->value == element) {
        --_size;
        auto* tmp = bucket->next;
        bucket->next = bucket->next->next;
        delete tmp;
        return true;
      }
    }
    return false;
  }

  bool insert(const T& element) {
    size_t h = _t_hash(element, _table_size);
    if (_data[h] == nullptr) {
      _data[h] = new Bucket(element);
      ++_bucket_size;
      ++_size;
      return true;
    }
    auto* bucket = _data[h];
    for (;; bucket = bucket->next) {
      if (bucket->value == element) {
        return false;
      }
      if (bucket->next == nullptr) {
        break;
      }
    }
    bucket->next = new Bucket(element);
    ++_size;
    return true;
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

  static constexpr size_t kDefaultSize = 8;
  static constexpr double kRehashSize = 0.75;

  size_t _bucket_size = 0;
  size_t _size = 0;
  size_t _table_size = kDefaultSize;
  std::vector<Bucket*> _data;

  THash _t_hash;
};

#endif  // HASH_TABLES__CHAINMETHOD_H_
#pragma clang diagnostic pop