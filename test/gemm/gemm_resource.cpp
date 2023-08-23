/*******************************************************************************
 *
 * MIT License
 *
 * Copyright 2021-2023 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/
 #include "gemm_resource_impl.hpp"

namespace rocwmma
{
    // All supported instantiations
    template struct GemmResource<int8_t, int32_t>;
    template struct GemmResource<bfloat16_t, float32_t>;
    template struct GemmResource<float16_t, float32_t>;
    template struct GemmResource<hfloat16_t, float32_t>;
    template struct GemmResource<float32_t, float32_t>;
    template struct GemmResource<float64_t, float64_t>;

#if defined(ROCWMMA_EXTENDED_TESTS)
    template struct GemmResource<int8_t, int8_t>;
    template struct GemmResource<bfloat16_t, bfloat16_t>;
    template struct GemmResource<float16_t, float16_t>;
    template struct GemmResource<hfloat16_t, hfloat16_t>;
#endif // ROCWMMA_EXTENDED_TESTS

}
