// Copyright (c) 2016, Yannick Tapsoba
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
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#include <cstdint>
#include <functional>

//
// Implements a random number generator based on a maximal-length
// linear-feedback shift register. A maximal-length LFSR of size N
// will cycle through all possible 2^N - 1 states within the shift
// register, except the state where all bits are zero.
//
// This class can be used to randomly generate unsigned integers
// from 1 to 2^N -1, without repeats.
//
// When the LFSR has cycled through all states, it is reseeded so that
// the next cycle produces a different sequence of numbers.
//
class MaxLFSR
{
public:

   //
   // This condition must be met: 2 <= size <= 64. If not, an out_of_range
   // exception is thrown.
   //
   MaxLFSR (std::uint8_t size = 64);
   
   std::uint8_t size () const;
   std::uint64_t next ();
   std::uint64_t cycleCount () const;

private:

   std::uint64_t numCycles;
   std::uint64_t seed;
   std::uint64_t state;
   std::function <std::uint64_t (std::uint64_t, std::uint64_t)> generator;
   std::uint8_t numBits;
   void reseed ();
};
