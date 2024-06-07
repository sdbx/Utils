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

__global__ void kernel(const wchar_t *in, wchar_t *out) {
  const int x = hipThreadIdx_x + hipBlockDim_x * hipBlockIdx_x;
  const int y = hipThreadIdx_y + hipBlockDim_y * hipBlockIdx_y;
  if (x) {
    out[y * 2 + 1] = L'\xFF01';
  } else {
    out[y * 2] = in[y];
  }
}

int main() {
  std::setlocale(LC_ALL, "");
  std::vector<wchar_t> str;

  wchar_t c;
  while ((c = std::wcin.get()) != WEOF) {
    if (c == ' ') {
      continue;
    }
    str.push_back(c);
  }

  wchar_t *input;
  wchar_t *output;
  const size_t length = str.size();
  const size_t input_size = sizeof(wchar_t) * (length + 1);
  const size_t output_size = sizeof(wchar_t) * (length * 2 + 1);
  HIP_CHECK(hipMalloc((void **)&input, input_size));
  HIP_CHECK(hipMalloc((void **)&output, output_size));
  HIP_CHECK(hipMemcpy(input, str.data(), input_size, hipMemcpyHostToDevice));

  kernel<<<dim3(1), dim3(2, length), 0, 0>>>(input, output);

  wchar_t *result = new wchar_t[output_size];
  HIP_CHECK(hipMemcpy(result, output, output_size, hipMemcpyDeviceToHost));

  HIP_CHECK(hipFree(input));
  HIP_CHECK(hipFree(output));

  result[length * 2 - 1] = 0;
  std::wcout << result << L'\n';

  delete[] result;

  return 0;
}
