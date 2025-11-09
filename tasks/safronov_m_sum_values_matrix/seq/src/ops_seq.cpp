#include "safronov_m_sum_values_matrix/seq/include/ops_seq.hpp"

#include <numeric>
#include <vector>

#include "iostream"
#include "safronov_m_sum_values_matrix/common/include/common.hpp"
#include "util/include/util.hpp"

namespace safronov_m_sum_values_matrix {

SafronovMSumValuesMatrixSEQ::SafronovMSumValuesMatrixSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  GetOutput() = std::vector<double>();
}

bool SafronovMSumValuesMatrixSEQ::ValidationImpl() {
  return !GetInput().empty() && !GetInput()[0].empty();
}

bool SafronovMSumValuesMatrixSEQ::PreProcessingImpl() {
  return true;
}

bool SafronovMSumValuesMatrixSEQ::RunImpl() {
  std::vector<double> vector(GetInput()[0].size());
  for (size_t i = 0; i < GetInput()[0].size(); i++) {
    double summa = 0;
    for (size_t j = 0; j < GetInput().size(); j++) {
      summa += GetInput()[j][i];
    }
    vector[i] = summa;
  }
  GetOutput() = vector;
  return true;
}

bool SafronovMSumValuesMatrixSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace safronov_m_sum_values_matrix
