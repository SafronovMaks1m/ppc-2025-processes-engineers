#include <gtest/gtest.h>
#include <stb/stb_image.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "safronov_m_sum_values_matrix/common/include/common.hpp"
#include "safronov_m_sum_values_matrix/mpi/include/ops_mpi.hpp"
#include "safronov_m_sum_values_matrix/seq/include/ops_seq.hpp"
#include "util/include/func_test_util.hpp"
#include "util/include/util.hpp"

namespace safronov_m_sum_values_matrix {

class SafronovMSumValuesMatrixFuncTests : public ppc::util::BaseRunFuncTests<InType, OutType, TestType> {
 public:
  static std::string PrintTestParam(const TestType &test_param) {
    return std::get<0>(test_param);
  }

 protected:
  void SetUp() override {
    TestType params = std::get<static_cast<std::size_t>(ppc::util::GTestParamIndex::kTestParams)>(GetParam());
    input_data_ = std::get<1>(params);
    _res = std::get<2>(params);
  }

  bool CheckTestOutputData(OutType &output_data) final {
    return (_res == output_data);
  }

  InType GetTestInputData() final {
    return input_data_;
  }

 private:
  InType input_data_;
  OutType _res;
};

namespace {

TEST_P(SafronovMSumValuesMatrixFuncTests, SumColumnsMatrix) {
  ExecuteTest(GetParam());
}

const std::array<TestType, 5> kTestParam = {
    std::make_tuple("a", std::vector<std::vector<double>>{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},
                    std::vector<double>({12.0, 15.0, 18.0})),
    std::make_tuple("b", std::vector<std::vector<double>>{{1, 2, 3}}, std::vector<double>({1.0, 2.0, 3.0})),
    std::make_tuple("v", std::vector<std::vector<double>>{{1, 2, 3, 4}, {4, 5, 6, 7}, {7, 8, 9, 10}},
                    std::vector<double>({12.0, 15.0, 18.0, 21.0})),
    std::make_tuple("g", std::vector<std::vector<double>>{{1, 2, 3, 4}, {4, 5, 6, 7}, {7, 8, 9, 10}},
                    std::vector<double>({12.0, 15.0, 18.0, 21.0})),
    std::make_tuple("d", std::vector<std::vector<double>>(100, std::vector<double>(100, 1)),
                    std::vector<double>(100, 100.0))};

const auto kTestTasksList = std::tuple_cat(
    ppc::util::AddFuncTask<SafronovMSumValuesMatrixMPI, InType>(kTestParam, PPC_SETTINGS_safronov_m_sum_values_matrix),
    ppc::util::AddFuncTask<SafronovMSumValuesMatrixSEQ, InType>(kTestParam, PPC_SETTINGS_safronov_m_sum_values_matrix));

const auto kGtestValues = ppc::util::ExpandToValues(kTestTasksList);

const auto kPerfTestName = SafronovMSumValuesMatrixFuncTests::PrintFuncTestName<SafronovMSumValuesMatrixFuncTests>;

INSTANTIATE_TEST_SUITE_P(PicMatrixTests, SafronovMSumValuesMatrixFuncTests, kGtestValues, kPerfTestName);

}  // namespace

}  // namespace safronov_m_sum_values_matrix
