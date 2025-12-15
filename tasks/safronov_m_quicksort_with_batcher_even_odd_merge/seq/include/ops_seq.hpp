#pragma once

#include "safronov_m_quicksort_with_batcher_even_odd_merge/common/include/common.hpp"
#include "task/include/task.hpp"

namespace safronov_m_quicksort_with_batcher_even_odd_merge {

class SafronovMQuicksortWithBatcherEvenOddMergeSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit SafronovMQuicksortWithBatcherEvenOddMergeSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace safronov_m_quicksort_with_batcher_even_odd_merge
