#pragma once

#include <string>
#include <tuple>

#include "task/include/task.hpp"

namespace safronov_m_sum_values_matrix {

using InType = int;
using OutType = int;
using TestType = std::tuple<int, std::string>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace safronov_m_sum_values_matrix
