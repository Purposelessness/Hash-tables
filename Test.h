#ifndef HASH_TABLES__TEST_H_
#define HASH_TABLES__TEST_H_

#include <concepts>
#include <iostream>
#include <vector>

// #define LOG

template <template <typename> class T, typename F>
concept THashTable = requires(T<F> t) {
                       {
                         t.insert(std::declval<F>())
                       } -> std::same_as<std::pair<bool, size_t>>;
                       {
                         t.erase(std::declval<F>())
                       } -> std::same_as<std::pair<bool, size_t>>;
                       {
                         t.contains(std::declval<F>())
                       } -> std::same_as<std::pair<bool, size_t>>;
                       t.printSize();
                     };

struct TestMethodOutput {
  clock_t time;
  size_t avg_iterations;
  size_t max_iterations;

  explicit operator std::string() const {
    return std::to_string(time) + "\t" + std::to_string(avg_iterations) + "\t" +
           std::to_string(max_iterations);
  }
};

template <typename T, typename F>
TestMethodOutput testMethod(const char* name, T& obj, F& func,
                            const std::vector<std::string>& test_data,
                            int step = 1);

struct TestOutput {
  TestMethodOutput insertion_test;
  TestMethodOutput erasing_test;
  TestMethodOutput contains_test;

  explicit operator std::string() const {
    return "Insertion:\t" + static_cast<std::string>(insertion_test) + "\t" +
           "Erasing:\t" + static_cast<std::string>(erasing_test) + "\t" +
           "Contains:\t" + static_cast<std::string>(contains_test);
  }
};

template <template <typename> class T, typename F>
  requires THashTable<T, F>
TestOutput testHashTable(const char* name,
                         const std::vector<std::string>& test_data,
                         size_t table_size = 8) {
  T<F> hash_table{table_size};
#ifdef LOG
  std::cout << "------------------\n";
  std::cout << name << '\n';
  std::cout << "------------------\n";
#endif

  auto insert_func = &T<F>::insert;
  auto erase_func = &T<F>::erase;
  auto contains_func = &T<F>::contains;
  auto insert_test =
      testMethod("Insertion", hash_table, insert_func, test_data);
  auto contains_test =
      testMethod("Contains", hash_table, contains_func, test_data, -1);
  auto erase_test =
      testMethod("Erasing", hash_table, erase_func, test_data, -1);

#ifdef LOG
  std::cout << "------------------\n\n";
#endif

  TestOutput out{insert_test, erase_test, contains_test};
  return out;
}

template <typename T, typename F>
TestMethodOutput testMethod(const char* name, T& obj, F& func,
                            const std::vector<std::string>& test_data,
                            int step) {
  size_t max_iter = 0;
  size_t avg_iter = 0;
  clock_t time;
  if (step == 0) {
    return {};
  }

  if (step > 0) {
    time = clock();
    for (size_t i = 0; i < test_data.size(); i += step) {
      auto p = (obj.*func)(test_data[i]);
      avg_iter += p.second;
      if (p.second > max_iter) {
        max_iter = p.second;
      }
    }
    time = clock() - time;
  } else {
    time = clock();
    for (size_t i = test_data.size(); i > 0; i += step) {
      auto p = (obj.*func)(test_data[i - 1]);
      avg_iter += p.second;
      if (p.second > max_iter) {
        max_iter = p.second;
      }
    }
    time = clock() - time;
  }
  avg_iter /= test_data.size();

#ifdef LOG
  std::cout << name << ": " << time << ' ' << avg_iter << ' ' << max_iter
            << '\n';
  obj.printSize();
#endif

  TestMethodOutput out{time, avg_iter, max_iter};
  return out;
}

#endif  // HASH_TABLES__TEST_H_