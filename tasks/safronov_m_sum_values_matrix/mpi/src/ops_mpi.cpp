#include "safronov_m_sum_values_matrix/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <numeric>
#include <vector>

#include "safronov_m_sum_values_matrix/common/include/common.hpp"
#include "util/include/util.hpp"

namespace safronov_m_sum_values_matrix {

SafronovMSumValuesMatrixMPI::SafronovMSumValuesMatrixMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  GetInput() = in;
  if (!GetOutput().empty()) {
    GetOutput().clear();
  }  //
}

bool SafronovMSumValuesMatrixMPI::ValidationImpl() {
  return !GetInput().empty() && !GetInput()[0].empty();
}

bool SafronovMSumValuesMatrixMPI::PreProcessingImpl() {
  return true;
}

std::vector<double> SafronovMSumValuesMatrixMPI::summ_values(const int start, const int end) {
  std::vector<double> vec;
  for (int i = start; i <= end; i++) {
    double summa = 0;
    for (size_t j = 0; j < GetInput().size(); j++) {
      summa += GetInput()[j][i];
    }
    vec.push_back(summa);
  }
  return vec;
}

std::vector<int> SafronovMSumValuesMatrixMPI::calculating_interval(const int &size_prcs, const int rank,
                                                                   const int &count_column) {
  std::vector<int> vec(2);
  int whole_part = count_column / size_prcs;
  int real_part = count_column % size_prcs;
  int start = rank * whole_part + (rank < real_part ? rank : real_part);
  int end = start + whole_part - 1;
  if (rank < real_part) {
    end += 1;
  }
  vec[0] = start;
  vec[1] = end;
  return vec;
}

bool SafronovMSumValuesMatrixMPI::RunImpl() {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    int count_column = GetInput()[0].size();
    GetOutput().resize(count_column);

    for (int i = 1; i < size; i++) {
      std::vector<int> interval = calculating_interval(size, i, count_column);
      MPI_Send(interval.data(), 2, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    std::vector<int> interval = calculating_interval(size, 0, count_column);
    std::vector<double> elems = summ_values(interval[0], interval[1]);
    for (int i = interval[0]; i <= interval[1]; i++) {
      GetOutput()[i] = elems[i - interval[0]];
    }

    MPI_Status status;
    for (int i = 1; i < size; i++) {
      int size_elems;
      MPI_Recv(&size_elems, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
      std::vector<double> buf(size_elems);
      MPI_Recv(buf.data(), size_elems, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
      interval = calculating_interval(size, i, count_column);
      for (int j = interval[0]; j <= interval[1]; j++) {
        GetOutput()[j] = buf[j - interval[0]];
      }
    }

  } else {
    MPI_Status status;
    std::vector<int> buf(2);
    MPI_Recv(buf.data(), 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    std::vector<double> elems = summ_values(buf[0], buf[1]);
    int size_elems = elems.size();
    MPI_Send(&size_elems, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    MPI_Send(elems.data(), size_elems, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  int total_size = 0;
  if (rank == 0) {
    total_size = GetOutput().size();
  }

  MPI_Bcast(&total_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    GetOutput().resize(total_size);
  }

  MPI_Bcast(GetOutput().data(), total_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  return true;
}

bool SafronovMSumValuesMatrixMPI::PostProcessingImpl() {
  return true;
}

}  // namespace safronov_m_sum_values_matrix
