# Build for CPU

Requirements: clang compiler, [HIP-CPU](https://github.com/ROCm/HIP-CPU)

```
export CXX=clang++
make
```

# Build for NVIDIA GPUs

Requirements: NVIDIA Cuda compiler

```
export CXX=nvcc
make
```

# Build for AMDGPUs

Requirements: HIP C++ compiler, AMD clang compiler

```
export CXX=hipcc
make
```
