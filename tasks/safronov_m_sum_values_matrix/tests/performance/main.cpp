#include <gtest/gtest.h>

#include "safronov_m_sum_values_matrix/common/include/common.hpp"
#include "safronov_m_sum_values_matrix/mpi/include/ops_mpi.hpp"
#include "safronov_m_sum_values_matrix/seq/include/ops_seq.hpp"
#include "util/include/perf_test_util.hpp"

namespace safronov_m_sum_values_matrix {

class SafronovMSumValuesMatrixPerfTest : public ppc::util::BaseRunPerfTests<InType, OutType> {
  const int kCount_ = 4000000;
  InType input_data_{};
  OutType _res;

  void SetUp() override {
    input_data_ = std::vector<std::vector<double>>(2000, std::vector<double>(2000, 2));
    _res = std::vector<double>(2000, 4000.0);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return _res == output_data;
  }

  InType GetTestInputData() final {
    return input_data_;
  }
};

TEST_P(SafronovMSumValuesMatrixPerfTest, RunPerfModes) {
  ExecuteTest(GetParam());
}

const auto kAllPerfTasks =
    ppc::util::MakeAllPerfTasks<InType, SafronovMSumValuesMatrixMPI, SafronovMSumValuesMatrixSEQ>(
        PPC_SETTINGS_safronov_m_sum_values_matrix);

const auto kGtestValues = ppc::util::TupleToGTestValues(kAllPerfTasks);

const auto kPerfTestName = SafronovMSumValuesMatrixPerfTest::CustomPerfTestName;

INSTANTIATE_TEST_SUITE_P(RunModeTests, SafronovMSumValuesMatrixPerfTest, kGtestValues, kPerfTestName);

}  // namespace safronov_m_sum_values_matrix
