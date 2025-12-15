#include "safronov_m_quicksort_with_batcher_even_odd_merge/seq/include/ops_seq.hpp"

#include <utility>
#include <vector>

#include "safronov_m_quicksort_with_batcher_even_odd_merge/common/include/common.hpp"

namespace safronov_m_quicksort_with_batcher_even_odd_merge {

SafronovMQuicksortWithBatcherEvenOddMergeSEQ::SafronovMQuicksortWithBatcherEvenOddMergeSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
}

bool SafronovMQuicksortWithBatcherEvenOddMergeSEQ::ValidationImpl() {
  return GetOutput().empty();
}

bool SafronovMQuicksortWithBatcherEvenOddMergeSEQ::PreProcessingImpl() {
  GetOutput().clear();
  return true;
}

bool SafronovMQuicksortWithBatcherEvenOddMergeSEQ::RunImpl() {
  std::vector<int> array = GetInput();
  if (array.empty()) {
    return true;
  }

  std::vector<std::pair<int, int>> stack;
  int size = static_cast<int>(array.size());
  std::pair<int, int> range(0, size - 1);
  stack.push_back(range);

  while (!stack.empty()) {
    int left = stack.back().first;
    int right = stack.back().second;
    stack.pop_back();
    if (left >= right) {
      continue;
    }
    int i = left;
    int j = right;
    int pivot = array[left + (right - left) / 2];
    while (i <= j) {
      while (array[i] < pivot) {
        i++;
      }
      while (array[j] > pivot) {
        j--;
      }
      if (i <= j) {
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
        i++;
        j--;
      }
    }
    if (left < j) {
      std::pair<int, int> left_range(left, j);
      stack.push_back(left_range);
    }
    if (i < right) {
      std::pair<int, int> right_range(i, right);
      stack.push_back(right_range);
    }
  }
  GetOutput().swap(array);
  return true;
}

bool SafronovMQuicksortWithBatcherEvenOddMergeSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace safronov_m_quicksort_with_batcher_even_odd_merge
