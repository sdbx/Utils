#ifdef _MSC_VER
#include "msvc_defines.h"
#endif
#include <hip/hip_runtime.h>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

#define HIP_CHECK(status)                                                      \
  if (status != hipSuccess) {                                                  \
    fprintf(stderr, "error: '%s' at %s:%d\n", hipGetErrorString(status),       \
            __FILE__, __LINE__);                                               \
    exit(0);                                                                   \
  }

__global__ void kernel(const wchar_t *in, wchar_t *out, const size_t dim,
                       const wchar_t mark) {
  const int idx = hipThreadIdx_x * dim + hipThreadIdx_y;
  if (hipBlockIdx_x) {
    out[idx * 2 + 1] = mark;
  } else {
    out[idx * 2] = in[idx];
  }
}

int main(int argc, char *argv[]) {
  std::setlocale(LC_ALL, "");
  std::vector<wchar_t> str;

  wchar_t c;
  while ((c = std::wcin.get()) != WEOF) {
    if (c == ' ') {
      continue;
    }
    str.push_back(c);
  }

  wchar_t mark = L'\xFF01';
  if (argc > 1) {
    const char *arg = argv[1];
    const char a = arg[0];
    const char b = arg[1];
    const bool u = b != '\0' && b == 'u';
    if (a == 'q') {
      mark = u ? L'\x00BF' : L'\xFF1F';
    } else if (a == 'e' && u) {
      mark = L'\x00A1';
    }
  }

  wchar_t *input;
  wchar_t *output;
  const size_t length = str.size();
  const size_t dim = ceil(sqrt(double(length)));
  const size_t input_size = sizeof(wchar_t) * dim * dim;
  const size_t output_length = dim * 2 * dim;
  const size_t output_size = sizeof(wchar_t) * output_length;
  HIP_CHECK(hipMalloc((void **)&input, input_size));
  HIP_CHECK(hipMalloc((void **)&output, output_size));
  HIP_CHECK(hipMemcpy(input, str.data(), sizeof(wchar_t) * length,
                      hipMemcpyHostToDevice));

  kernel<<<2, dim3(dim, dim), 0, 0>>>(input, output, dim, mark);

  auto result = new wchar_t[output_length + 2]; // mark ... \x0000
  *result = mark;
  HIP_CHECK(hipMemcpy(result + 1, output, output_size, hipMemcpyDeviceToHost));
  result[length * 2 + 1] = 0;

  HIP_CHECK(hipFree(input));
  HIP_CHECK(hipFree(output));

  std::wcout << result << L'\n';

  delete[] result;

  return 0;
}
