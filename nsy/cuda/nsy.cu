#include <cmath>
#include <cuda_runtime.h>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

#define CUDA_CHECK(status)                                                     \
  if (status != cudaSuccess) {                                                 \
    fprintf(stderr, "error: '%s' at %s:%d\n", cudaGetErrorString(status),      \
            __FILE__, __LINE__);                                               \
    exit(0);                                                                   \
  }

__global__ void kernel(const wchar_t *in, wchar_t *out, const size_t dim,
                       const wchar_t mark) {
  const int idx = threadIdx.x * dim + threadIdx.y;
  out[idx * 2 + blockIdx.x] = blockIdx.x == 0 ? in[idx] : mark;
}

wchar_t get_mark(char *s) {
  const char a = s[0];
  const char b = s[1];
  const bool u = b != '\0' && b == 'u';
  if (a == 'q') {
    return u ? L'\x00BF' : L'\xFF1F';
  } else if (a == 'e' && u) {
    return L'\x00A1';
  }
  return L'\xFF01';
}

int main(int argc, char *argv[]) {
  std::setlocale(LC_ALL, "");
  std::vector<wchar_t> str;

  wchar_t c;
  while ((c = std::wcin.get()) != (wchar_t)WEOF) {
    if (c == ' ') {
      continue;
    }
    str.push_back(c);
  }

  wchar_t *input;
  wchar_t *output;
  const size_t length = str.size();
  const size_t dim = ceil(sqrt(double(length)));
  const size_t input_size = sizeof(wchar_t) * dim * dim;
  const size_t output_length = dim * 2 * dim;
  const size_t output_size = sizeof(wchar_t) * output_length;
  CUDA_CHECK(cudaMalloc((void **)&input, input_size));
  CUDA_CHECK(cudaMalloc((void **)&output, output_size));
  CUDA_CHECK(cudaMemcpy(input, str.data(), sizeof(wchar_t) * length,
                        cudaMemcpyHostToDevice));

  const wchar_t mark = argc < 2 ? L'\xFF01' : get_mark(argv[1]);
  kernel<<<2, dim3(dim, dim), 0, 0>>>(input, output, dim, mark);

  auto result = new wchar_t[output_length + 2]; // mark ... \x0000
  *result = mark;
  CUDA_CHECK(
      cudaMemcpy(result + 1, output, output_size, cudaMemcpyDeviceToHost));
  result[length * 2 + 1] = L'\0';

  CUDA_CHECK(cudaFree(input));
  CUDA_CHECK(cudaFree(output));

  std::wcout << result << L'\n';

  delete[] result;

  return 0;
}
