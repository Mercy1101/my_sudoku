#include <algorithm>
#include <functional>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>
#include <vector>

void print_sudoku(const std::vector<std::vector<int>>& board) {
  std::cout << " ---------------------------" << std::endl;
  for (int i = 0; i < 9; ++i) {
    for (int j = 0; j < 9; ++j) {
      if (j == 1 || j == 2 || j == 4 || j == 5 || j == 7 || j == 8) {
        std::cout << ' ';
      } else {
        std::cout << '|';
      }
      if (board.at(i).at(j) == -1) {
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

inline std::vector<int> create_random_number() {
  static std::random_device rd;
  static std::mt19937 g(rd());
  std::vector<int> res(9, 0);
  std::iota(res.begin(), res.end(), 1);
  std::shuffle(res.begin(), res.end(), g);
  return res;
}

void completeSudoku(std::vector<std::vector<int>>& board) {
  std::vector<int> temp;
  std::vector<std::vector<int>> row(9, temp);
  std::vector<std::vector<int>> column(9, temp);
  std::vector<std::vector<int>> matrix(9, temp);
  std::vector<std::pair<int, int>> pos;
  for (size_t x = 0; x < board.size(); ++x) {
    for (size_t y = 0; y < board.size(); ++y) {
      if (board.at(x).at(y) != 0) {
        int num = board.at(x).at(y);
        row.at(y).push_back(num);
        column.at(x).push_back(num);
        matrix.at((x / 3 + (y / 3) * 3)).push_back(num);
      } else {
        pos.push_back(std::make_pair(x, y));
      }
    }
  }

  bool res = false;
  std::function<void(int)> func = [&](int p) {
    if (res) return;
    if (p == pos.size()) {
      res = true;
      return;
    }
    auto [x, y] = pos[p];
    auto num =  create_random_number();
    for (int i = 1; i < 10 && !res; ++i) {
      if (std::find(row.at(y).begin(), row.at(y).end(), num.at(i - 1)) !=
          row.at(y).end()) {
        continue;
      }
      if (std::find(column.at(x).begin(), column.at(x).end(), num.at(i - 1)) !=
          column.at(x).end()) {
        continue;
      }
      auto matrix_index = (x / 3 + (y / 3) * 3);
      if (std::find(matrix.at(matrix_index).begin(),
                    matrix.at(matrix_index).end(),
                    num.at(i - 1)) != matrix.at(matrix_index).end()) {
        continue;
      }
      board.at(x).at(y) = num.at(i - 1);
      row.at(y).push_back(num.at(i - 1));
      column.at(x).push_back(num.at(i - 1));
      matrix.at(matrix_index).push_back(num.at(i - 1));
      func(p + 1);
      /// board.at(x).at(y) = 0;
      row.at(y).pop_back();
      column.at(x).pop_back();
      matrix.at(matrix_index).pop_back();
    }
  };
  func(0);
  return;
}

void remove_number(std::vector<std::vector<int>>& board, double persent) {
  auto get_random = []() {
    static std::once_flag InitFlag;
  auto current_time_s = std::chrono::duration_cast<std::chrono::milliseconds>(
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

  int num = persent*100;
  for (int i = 0; i < board.size(); ++i) {
    for (int j = 0; j < board.at(0).size(); ++j) {
      bool is_remove = get_range_random(0, 100) > num;
      if (is_remove) {
        board.at(i).at(j) = 0;
      }
    }
  }
}

int main() {
  std::vector<int> temp(9, 0);
  std::vector<std::vector<int>> board(9, temp);
  /// 生成完整数独
  completeSudoku(board);
  /// 去除部分数字
  remove_number(board, 50.0/81);
  /// 打印数独
  print_sudoku(board);
  std::cout << std::endl;
  /// 解数独
  completeSudoku(board);
  /// 打印数独
  print_sudoku(board);
  system("pause");
}
