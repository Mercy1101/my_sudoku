#include "sudoku.hpp"

int main(){
  std::cout << "\ncreate sudoku: " << std::endl;
  lee::sudoku sudoku;
  sudoku.complete_sudoku();
  std::cout << "\nresolved sudoku: " << std::endl;
  sudoku.print();
  system("pause");
}
