/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.

/// @file   sudoku.hpp
/// @brief  数独的生成与解
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-09-19 20:57:45

#ifndef INCLUDE_SUDOKU_HPP_
#define INCLUDE_SUDOKU_HPP_

#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

namespace lee {
class sudoku {
 public:
  sudoku() {
    /// 创建一个数独
    board = create();
    /// 打印出来
    print();
  }

  /// @name     print
  /// @brief    打印当前数独
  /// 
  /// @param    NONE
  /// 
  /// @return   NONE
  /// 
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-09-19 21:35:31
  /// @warning  线程不安全
  void print() {
    std::cout << std::endl;
    std::cout << " ---------------------------" << std::endl;
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        if (j == 1 || j == 2 || j == 4 || j == 5 || j == 7 || j == 8) {
          std::cout << ' ';
        } else {
          std::cout << '|';
        }
        if (board.at(i).at(j) == 0) {
          std::cout << "  ";
        } else {
          std::cout << ' ' << board.at(i).at(j);
        }
        if (j != 0 && j % 8 == 0) {
          std::cout << '|';
        }
      }
      std::cout << std::endl;
      if (i == 2 || i == 5 || i == 8) {
        std::cout << " ---------------------------" << std::endl;
      } else {
        std::cout << "  -  -  -  -  -  -  -  -  -  " << std::endl;
      }
    }
  }

  /// @name     complete_sudoku
  /// @brief    解决数独
  /// 
  /// @param    NONE
  /// 
  /// @return   NONE
  /// 
  /// @author   Lijiancong, pipinstall@163.com
  /// @date     2020-09-19 21:36:29
  /// @warning  线程不安全
  void complete_sudoku() {
    std::vector<int> temp;
    std::vector<std::vector<int>> row(9, temp); ///< 保存每行出现过的数字
    std::vector<std::vector<int>> column(9, temp);  ///< 保存没列出现过的数字
    std::vector<std::vector<int>> matrix(9, temp);  ///< 保存每个九宫格出现的数字
    std::vector<std::pair<int, int>> pos; ///< 保存九宫格中需要填入数字的位置
    for (size_t x = 0; x < board.size(); ++x) {
      for (size_t y = 0; y < board.size(); ++y) {
        if (board.at(x).at(y) != 0) {
          /// 已经出现过的数字记录下来
          int num = board.at(x).at(y);
          row.at(y).push_back(num);
          column.at(x).push_back(num);
          matrix.at((x / 3 + (y / 3) * 3)).push_back(num);
        } else {
          /// 记录需要填入数字的位置
          pos.push_back(std::make_pair(x, y));
        }
      }
    }

    bool res = false;
    /// 创建一个回溯的函数对象
    std::function<void(int)> backtracking = [&](int p) {
      if (res) return;
      if (p == pos.size()) {
        res = true;
        return;
      }
      auto [x, y] = pos[p];
      /// 创建一个随机的数组，通过遍历index来获取可以填入的随机数字
      auto num = create_random();
      for (size_t i = 1; i < 10 && !res; ++i) {
        if (std::find(row.at(y).begin(), row.at(y).end(), num.at(i - 1)) !=
            row.at(y).end()) {
          /// 找寻该数字在这行有没有出现过，出现过就下一个循环
          continue;
        }
        if (std::find(column.at(x).begin(), column.at(x).end(),
                      num.at(i - 1)) != column.at(x).end()) {
          /// 找寻该数字在这列有没有出现过，出现过就下一个循环
          continue;
        }
        auto matrix_index = (x / 3 + (y / 3) * 3);
        if (std::find(matrix.at(matrix_index).begin(),
                      matrix.at(matrix_index).end(),
                      num.at(i - 1)) != matrix.at(matrix_index).end()) {
          /// 找寻该数字在这个九宫格有没有出现过，出现过就下一个循环
          continue;
        }
        board.at(x).at(y) = num.at(i - 1);
        row.at(y).push_back(num.at(i - 1));
        column.at(x).push_back(num.at(i - 1));
        matrix.at(matrix_index).push_back(num.at(i - 1));
        backtracking(p + 1);
        /// board.at(x).at(y) = 0;
        row.at(y).pop_back();
        column.at(x).pop_back();
        matrix.at(matrix_index).pop_back();
      }
    };
    backtracking(0);
    return;
  }

 private:
  std::vector<std::vector<int>> board;
  std::vector<std::vector<int>> create() {
    std::vector<int> temp(9, 0);
    std::vector<std::vector<int>> xx(9, temp);
    board = xx;
    /// 生成完整数独
    complete_sudoku();
    /// 去除部分数字
    remove_number(50.0 / 81);
    return board;
  }

  std::vector<int> create_random() {
    static std::random_device rd;
    static std::mt19937 g(rd());
    std::vector<int> res(9, 0);
    std::iota(res.begin(), res.end(), 1);
    std::shuffle(res.begin(), res.end(), g);
    return res;
  }

  void remove_number(double persent) {
    auto get_random = []() {
      static std::once_flag InitFlag;
      auto current_time_s =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch())
              .count();
      std::call_once(InitFlag, [&]() {
        srand(static_cast<unsigned long long>(current_time_s));
      });
      return rand();
    };
    auto get_range_random = [&](int x, int y) {
      if (x > y) std::swap(x, y);
      return (get_random() % (y - x + 1)) + x;
    };

    int num = persent * 100;
    for (int i = 0; i < board.size(); ++i) {
      for (int j = 0; j < board.at(0).size(); ++j) {
        bool is_remove = get_range_random(0, 100) > num;
        if (is_remove) {
          board.at(i).at(j) = 0;
        }
      }
    }
  }
};
}  // namespace lee

#endif
