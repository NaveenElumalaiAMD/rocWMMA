# rocWMMA

AMD's C++ library for accelerating mixed precision matrix multiplication and accumulation (MFMA) operations leveraging specialized GPU matrix cores. rocWMMA provides a C++ API to facilitate breaking down matrix multiply-accumulate problems into fragments and using them in block-wise operations that are distributed in parallel across GPU wavefronts. The API is a header library of GPU device code meaning that matrix core acceleration may be compiled directly into your kernel device code. This can benefit from compiler optimization in the generation of kernel assembly, and does not incur additional overhead costs of linking to external runtime libraries or having to launch separate kernels.

Conceptually, the rocWMMA API is designed from a 'wave' perspective, such that block-wise API calls are processed by GPU threads coordinated together as a group, or a 'wave'. Importantly, individual threads may access only a portion of a fragment, and there is no guaranteed order or locality of this data. Thread access to fragment data is analogous to vector register access of individual threads. Fragments and block-wise operations therefore can be thought of in a 'wave' perspective.

Thread blocks that contain multiple waves have the added benefit of sharing resources and cooperating. For rocWMMA, this is especially useful for moving data. The header library contains a separate rocWMMA Coop API which facilitates the cooperation of multiple waves in loading and storing fragment data. Keeping this in mind, the order and locality of fragment data in a wave is not guaranteed, and can be thought of in a 'distributed wave' perspective.

Memory addresses are treated as 1D arrays and the rocWMMA API can opaquely handle both global and shared memory address locations. The library code also includes utilities for mapping 2D grid / matrix coordinates to 1D array coordinates, and supports either row major or column major data layouts. Block-wise dimensions (BlockM,N,K) familiar to block-wise GEMM matrix product algorithms are supported directly by availability of matrix core instructions for the target architecture. Likewise, mixed precision datatypes for input, output and accumulation fragments can be varied as listed below in currently supported configurations.

rocWMMA is released as a header library, but also includes test and sample projects to validate and illustrate example usages of the C++ API. GEMM matrix multiplication is used as primary validation given the heavy precedent for the library, however the usage portfolio is growing significantly and demonstrates different ways rocWMMA may be consumed.

## Minimum Requirements
* ROCm stack minimum version 4.3
* C++ 14
* CMake >=3.5
* OpenMP

Optional:
* rocblas >= 4.0 (if rocBLAS) https://github.com/ROCmSoftwarePlatform/rocBLAS/releases/tag/rocm-4.0.0
* doxygen (for building documentation)

## Currently supported configurations (ongoing)

- Matrix Layout <LayoutA, LayoutB, Layout C, LayoutD> (N = col major, T = row major)

    <N, N, N, N>, <N, N, T, T>

    <N, T, N, N>, <N, T, T, T>

    <T, N, N, N>, <T, N, T, T>

    <T, T, N, N>, <T, T, T, T>

- Thread Block Sizes <TBlockX, TBlockY> =
**Note: TBlockX must be a multiple of 64 **

    <64, 1>, <64, 2>, <64, 4>

    <128, 1>, <128, 2>, <128, 4>

    <256, 1>, <256, 2>, <256, 4>

- Data Types <Ti / To / Tc> = <Input type / Output Type / Compute Type>

    Input Type = Matrix A/B

    Output Type = Matrix C/D

    Compute Type = math / accumulation type

<table>
    <thead>
      <tr>
        <th>Ti / To / Tc</th>
        <th>BlockM</th>
        <th>BlockN</th>
        <th>BlockK</th>
        <th>Notes</th>
      </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan=2>i8 / i32 / i32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>i8 / i8 / i32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>f16 / f32 / f32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>f16 / f16 / f32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>f16 / f16 / f16*</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td rowspan=2>*= FMA is natively f32, downcasted to fp16</td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
        </tr>
        <tr>
            <td rowspan=2>__half / f32 / f32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>__half / __half / f32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>__half / __half / __half*</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 16, pow2</td>
            <td rowspan=2>*= FMA is natively f32, downcasted to __half</td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 8, pow2</td>
        </tr>
        <tr>
            <td rowspan=2>bf16 / f32 / f32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 4, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>bf16 / bf16 / f32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 8, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 4, pow2</td>
            <td></td>
        </tr>
        <tr>
            <td rowspan=2>bf16 / bf16 / bf16*</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 8, pow2</td>
            <td rowspan=2>*= FMA is natively f32, downcasted to bf16</td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 4, pow2</td>
        </tr>
        <tr>
            <td rowspan=2>f32 / f32 / f32</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 4, pow2</td>
            <td rowspan=2></td>
        </tr>
        <tr>
            <td>32</td>
            <td>32</td>
            <td>Min: 2, pow2</td>
        </tr>
        <tr>
            <td>f64 / f64 / f64</td>
            <td>16</td>
            <td>16</td>
            <td>Min: 4, pow2</td>
            <td></td>
        </tr>
    </tbody>
  </table>


## API Functions
### fill_fragment
Broadcast a desired value to all elements in the fragment.

### load_matrix_sync / store_matrix_sync
Loads data from memory according to Matrix Layout.
- Matrix A layout loads / stores matrix columns in the K direction (Matrix A = M x K, fragA = BlockM x BlockK)
- Matrix B layout loads / stores matrix rows in the K direction (Matrix B = K x N, fragB = BlockK x BlockN)
- Matrix C layout loads / stores matrix rows in vector width of 4 (Matrix C = M x N, fragAcc = BlockM x BlockN)

Fragments are stored in packed registers in optimal load / store patterns. In-register elements have no guaranteed order, which have been optimized for loading / storing efficiency.

### mma_sync
MFMA accumulation is performed with fragment data. Fragment A elements are multiplied with Fragment B elements and added to the accumulator fragment.

### synchronize_workgroup
Performs synchronization across multiple wavefronts in a workgroup. It also ensures the synchronization of shared/global memory accesses across wavefronts.

## Cooperative API Functions

### load_matrix_coop_sync / store_matrix_coop_sync
Loads data from memory according to Matrix Layout. Splits operation amongst wave members of a workgroup.
- Matrix A layout loads / stores matrix columns in the K direction (Matrix A = M x K, fragA = BlockM x BlockK)
- Matrix B layout loads / stores matrix rows in the K direction (Matrix B = K x N, fragB = BlockK x BlockN)
- Matrix C layout loads / stores matrix rows in vector width of 4 (Matrix C = M x N, fragAcc = BlockM x BlockN)

Each contributing wave handles partial operation data, therefore split parameters should be consistent across subsequent operations. E.g. Moving data from global to shared memory cooperatively should use the same split parameters for the global load and subsequent local store.

## Contributing to the code
1. Create and track a rocWMMA fork.
2. Clone your fork:
```
git clone -b develop https://github.com/<your_fork>/rocWMMA.git .
.githooks/install
git checkout -b <new_branch>
...
git add <new_work>
git commit -m "What was changed"
git push origin <new_branch>
...
```
3. Create a pull request to ROCmSoftwarePlatform/rocWMMA develop branch.
4. Await CI and approval feedback.
5. Once approved, merge!

**Please don't forget to install the githooks** as there are triggers for clang formatting in commits.


## Build with CMake

### Project options
|Option|Description|Default Value|
|---|---|---|
|AMDGPU_TARGETS|Build code for specific GPU target(s)|gfx908:xnack-;gfx90a:xnack-;gfx90a:xnack+|
|ROCWMMA_BUILD_TESTS|Build Tests|ON|
|ROCWMMA_BUILD_SAMPLES|Build Samples|ON|
|ROCWMMA_BUILD_DOCS|Build doxygen documentation from code|OFF|
|ROCWMMA_BUILD_ASSEMBLY|Generate assembly files|OFF|
|ROCWMMA_BUILD_VALIDATION_TESTS|Build validation tests |ON (requires ROCWMMA_BUILD_TESTS=ON)|
|ROCWMMA_BUILD_BENCHMARK_TESTS|Build benchmark tests |ON (requires ROCWMMA_BUILD_TESTS=ON)|
|ROCWMMA_BUILD_EXTENDED_TESTS|Build extended testing coverage |OFF (requires ROCWMMA_BUILD_TESTS=ON)|
|ROCWMMA_VALIDATE_WITH_ROCBLAS|Use rocBLAS for validation tests| ON (requires ROCWMMA_BUILD_VALIDATION_TESTS=ON)|
|ROCWMMA_BENCHMARK_WITH_ROCBLAS|Include rocBLAS benchmarking data| OFF (requires ROCWMMA_BUILD_BENCHMARK_TESTS=ON)|

### Example configurations
By default, the project is configured as Release mode, and is linked against rocBLAS for validating results.
Here are some of the examples for the configuration:
|Configuration|Command|
|---|---|
|Basic|`CC=hipcc CXX=hipcc cmake -B<build_dir> .`|
|Targeting MI100|`CC=hipcc CXX=hipcc cmake -B<build_dir> . -DAMDGPU_TARGETS=gfx908:xnack-` |
|Debug build|`CC=hipcc CXX=hipcc cmake -B<build_dir> . -DCMAKE_BUILD_TYPE=Debug` |
|Build without rocBLAS (default on)|`CC=hipcc CXX=hipcc cmake -B<build_dir> . -DROCWMMA_VALIDATE_WITH_ROCBLAS=OFF -DROCWMMA_BENCHMARK_WITH_ROCBLAS=OFF` |

After configuration, build with `cmake --build <build_dir> -- -j`

**Warning: Build time for all projects can take several minutes**

### Tips to reduce tests compile time:
- Target a specific GPU (default = MI100, MI200+/-)
- Use lots of threads (e.g. -j64)
- Select ROCWMMA_BUILD_ASSEMBLY=OFF
- Select ROCWMMA_BUILD_DOCS=OFF
- Select ROCWMMA_BUILD_EXTENDED_TESTS=OFF
- Select either ROCWMMA_BUILD_VALIDATION_TESTS or ROCWMMA_BUILD_BENCHMARK_TESTS as ON, and the other as OFF.
- Manually build specific tests:
```
cd <build_dir>
make <target_name> -j64
```
- Manually reduce test(s) and/or test case coverage

## Running Unit Tests
rocWMMA library features are showcased, validated and benchmarked if applicable in GTest applications.

### Fill Fragment Test
Tests the rocwmma::fill_fragment API function for all supported configurations. Tests broadcasting of a desired value to all elements in the fragment.
Run validation:
```
<build_dir>/test/fill_fragment_test
```

### Load / Store Matrix Sync Test
Tests the rocwmma::load_matrix_sync and rocwmma::store_matrix_sync API functions for all supported configurations. Tests proper emplacement of data during loads and stores.
Run validation:
```
<build_dir>/test/load_store_matrix_sync_test
<build_dir>/test/load_store_matrix_coop_sync_test
```

### Contamination Test
Unit tests for loading and storing API to verify data boundaries are not crossed and pristine data remains untouched.
Run validation:
```
<build_dir>/test/contamination_test
```

### Layout Test
Unit tests for the internal collect / scatter matrix element to register mapping transforms.
Run validation:
```
<build_dir>/test/layout_test
```

### Mapping Util Test
Unit tests for the utility class used to calculate transforms and offsets between grid, matrix and data coordinate systems.
Run validation:
```
<build_dir>/test/mapping_util_test
```

### Vector Iterator Test
Unit tests for internal vector iteration and navigation during access and storage.
Run validation:
```
<build_dir>/test/vector_iterator_test
```

### GEMM tests
Implements a GEMM blocking algorithm using rocWMMA for all supported configurations. Validates on CPU algorithm or rocBLAS if available. Validation runs are performed on a reduced subset of matrix sizes. Benchmark runs are on a larger set of matrix sizes. There are currently 3 variants of blocked GEMM kernels. 

**Mma Sync Test** is the simplest blocked GEMM example which targets one output block of matrix multiplication per wave. 

**Mma Sync Multi Test** implements blocked GEMM where each wave is responsible for a BlocksX x BlocksY grid of output blocks, scaling the outer workgroup macro tile size.

**Mma Sync Multi Lds Test** implements the blocked multi-GEMM leveraging shared memory to implement data prefetching and movement pipelining to improve performance. 

**Barrier Test** is a simple blocked GEMM example, using a wave barrier to showcase benefits of synchronizing waves for performance and synchronization.

**Ad Hoc Test** is an executable that focuses on a specific set of kernel parameters. This is used as a quick mock-up of situational investigation of a particular GEMM kernel.

Run validation tests:
```
<build_dir>/test/gemm/mma_sync_test-validate
<build_dir>/test/gemm/mma_sync_multi_test-validate
<build_dir>/test/gemm/mma_sync_multi_lds_test-validate
<build_dir>/test/gemm/barrier_test-validate
```

Run benchmark only: **Benchmark runs can take several hours to complete**
```
<build_dir>/test/gemm/mma_sync_test-bench
<build_dir>/test/gemm/mma_sync_multi_test-bench
<build_dir>/test/gemm/mma_sync_multi_lds_test-bench
<build_dir>/test/gemm/barrier_test-bench
```

Run ad-hoc test:
```
<build_dir>/test/gemm/mma_sync_ad_hoc_test
```

### Tips to reduce run time:
- Use gtest filters, target specific test names:
```
<test_exe> --gtest_filter=*name_filter*
```
- Manually adjust the test cases coverage.
- Use ad-hoc tests to focus in on specific parameters.
- Select ROCWMMA_BUILD_EXTENDED_TESTS=OFF

### Samples
These are stand-alone real-world use-cases of the rocWMMA API. They have minimal dependencies and represent a targeted application with a fixed set of parameters.

**SGEMM-V**

Simple matrix multiply-accumulate with a vector demonstration, without LDS and no transpose. Calculates Y = alpha * (A) * X + beta * Y with mixed precision fp16 inputs and fp32 output. Includes simple CPU validation and benchmark.

 A = Matrix of size m * k (row-major)
 
 X = Vector of size k * 1 (col-major)
 
 Y = accumulator of size m * 1 (row-major)
   
Run sgemmv sample:
```
<build_dir>/samples/sgemmv
```

**Simple DLRM**

Simple Deep Learning Recommendation Model (DLRM) for machine learning. Implements both forward and backward passes on fp16 inputs and outputs. Includes simple CPU validation and benchmark.

Run simple_dlrm sample:
```
<build_dir>/samples/simple_dlrm
```

**Simple GEMM**

Simple GEMM algorithm demonstration without LDS memory usage and no transpose. Calculates D = Alpha * A x B + Beta * C with mixed precision fp16 inputs and fp32 output. Includes simple CPU validation and benchmark.

Run simple_gemm sample:
```
<build_dir>/samples/simple_gemm
```
