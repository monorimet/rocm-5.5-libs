// Copyright (c) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef ROCRAND_VERSION_H_
#define ROCRAND_VERSION_H_

/// \brief rocRAND library version
///
/// ROCRAND_VERSION % 100 is the patch level,
/// ROCRAND_VERSION / 100 % 1000 is the minor version,
/// ROCRAND_VERSION / 100000 is the major version.
#define ROCRAND_VERSION 201016

// Enables inline asm optimisations
#if !defined(ROCRAND_ENABLE_INLINE_ASM) && !defined(ROCRAND_DISABLE_INLINE_ASM)
    #define ROCRAND_ENABLE_INLINE_ASM
#endif

#endif // ROCRAND_VERSION_H_
