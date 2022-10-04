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

#include "maxlfsr.h"
#include <array>
#include <cmath>
#include <exception>
#include <random>

// Lookup table for two-tap LFSRs (size 2 to 4)
// Source: http://courses.cse.tamu.edu/csce680/walker/lfsr_table.pdf
std::array <std::array <unsigned, 2>, 3> kTwoTapLUT = {{
   { 2, 1 }, // Size = 2
   { 3, 2 }, // Size = 3
   { 4, 3 }  // Size = 4
}};

// Lookup Table for four tap LFSRs (size 5 to 64)
// Source: http://courses.cse.tamu.edu/csce680/walker/lfsr_table.pdf
std::array <std::array <unsigned, 4>, 60> kFourTapLUT = {{
   {  5,  4,  3,  2 }, // Size = 5
   {  6,  5,  3,  2 }, // Size = 6
   {  7,  6,  5,  4 }, // Size = 7
   {  8,  6,  5,  4 }, // Size = 8
   {  9,  8,  6,  5 }, // Size = 9
   { 10,  9,  7,  6 }, // Size = 10
   { 11, 10,  9,  7 }, // Size = 11
   { 12, 11,  8,  6 }, // Size = 12
   { 13, 12, 10,  9 }, // Size = 13
   { 14, 13, 11,  9 }, // Size = 14
   { 15, 14, 13, 11 }, // Size = 15
   { 16, 14, 13, 11 }, // Size = 16
   { 17, 16, 15, 14 }, // Size = 17
   { 18, 17, 16, 13 }, // Size = 18
   { 19, 18, 17, 14 }, // Size = 19
   { 20, 19, 16, 14 }, // Size = 20
   { 21, 20, 19, 16 }, // Size = 21
   { 22, 19, 18, 17 }, // Size = 22
   { 23, 22, 20, 18 }, // Size = 23
   { 24, 23, 21, 20 }, // Size = 24
   { 25, 24, 23, 22 }, // Size = 25
   { 26, 25, 24, 20 }, // Size = 26
   { 27, 26, 25, 22 }, // Size = 27
   { 28, 27, 24, 22 }, // Size = 28
   { 29, 28, 27, 25 }, // Size = 29
   { 30, 29, 26, 24 }, // Size = 30
   { 31, 30, 29, 28 }, // Size = 31
   { 32, 30, 26, 25 }, // Size = 32
   { 33, 32, 29, 27 }, // Size = 33
   { 34, 31, 30, 26 }, // Size = 34
   { 35, 34, 28, 27 }, // Size = 35
   { 36, 35, 29, 28 }, // Size = 36
   { 37, 36, 33, 31 }, // Size = 37
   { 38, 37, 33, 32 }, // Size = 38
   { 39, 38, 35, 32 }, // Size = 39
   { 40, 37, 36, 35 }, // Size = 40
   { 41, 40, 39, 38 }, // Size = 41
   { 42, 40, 37, 35 }, // Size = 42
   { 43, 42, 38, 37 }, // Size = 43
   { 44, 42, 39, 38 }, // Size = 44
   { 45, 44, 42, 41 }, // Size = 45
   { 46, 40, 39, 38 }, // Size = 46
   { 47, 46, 43, 42 }, // Size = 47
   { 48, 44, 41, 39 }, // Size = 48
   { 49, 45, 44, 43 }, // Size = 49
   { 50, 48, 47, 46 }, // Size = 50
   { 51, 50, 48, 45 }, // Size = 51
   { 52, 51, 49, 46 }, // Size = 52
   { 53, 52, 51, 47 }, // Size = 53
   { 54, 51, 48, 46 }, // Size = 54
   { 55, 54, 53, 49 }, // Size = 55
   { 56, 54, 52, 49 }, // Size = 56
   { 57, 55, 54, 52 }, // Size = 57
   { 58, 57, 53, 52 }, // Size = 58
   { 59, 57, 55, 52 }, // Size = 59
   { 60, 58, 56, 55 }, // Size = 60
   { 61, 60, 59, 56 }, // Size = 61
   { 62, 59, 57, 56 }, // Size = 62
   { 63, 62, 59, 58 }, // Size = 63
   { 64, 63, 61, 60 }  // Size = 64
}};

std::uint64_t twoTapPolynomial (std::uint8_t size, std::uint64_t state)
{
   std::uint64_t bit = 0;
   auto index = size - 2;
   bit = (  (state >> (size - kTwoTapLUT[index][0]))
          ^ (state >> (size - kTwoTapLUT[index][1]))
         ) & 1;
   return (state >> 1) | (bit << (size - 1));
}

std::uint64_t fourTapPolynomial (std::uint8_t size, std::uint64_t state)
{
   std::uint64_t bit = 0;
   auto index = size - 5;
   bit = (  (state >> (size - kFourTapLUT[index][0]))
          ^ (state >> (size - kFourTapLUT[index][1]))
          ^ (state >> (size - kFourTapLUT[index][2]))
          ^ (state >> (size - kFourTapLUT[index][3]))
         ) & 1;
   return (state >> 1) | (bit << (size - 1));
}

MaxLFSR::MaxLFSR (std::uint8_t size)
   : numCycles(0), seed(0), state(0), numBits(size)
{
   if (numBits < 2 || numBits > 64)
   {
      throw std::out_of_range ("The register size must be >= 2 and <= 64.\n");
   }
   generator = size < 5 ? twoTapPolynomial
                        : fourTapPolynomial;
   reseed();
}

void MaxLFSR::reseed ()
{
   std::random_device rd;
   std::mt19937_64 rng (rd ());
   do
   {
      seed = rng () % (1 << numBits);
   } while (0 == seed); // The start state has to be non-zero
   state = seed;
}

std::uint8_t MaxLFSR::size () const
{
   return numBits;
}

std::uint64_t MaxLFSR::next ()
{
   state = generator (numBits, state);
   std::uint64_t result = state;
   if (state == seed)
   {
      ++numCycles;
      reseed();
   }
   return result;
}

std::uint64_t MaxLFSR::cycleCount () const
{
   return numCycles;
}
