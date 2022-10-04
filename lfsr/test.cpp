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

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "maxlfsr.h"
#include <iostream>
#include <map>

void generateLfsrCycle (
   std::uint8_t size,
   std::map <std::uint64_t, std::uint64_t> & countMap
)
{
   countMap.clear ();
   MaxLFSR lfsr (size);
   while (0 == lfsr.cycleCount ())
   {
      ++countMap[lfsr.next ()];
   }
}

TEST_CASE ("Test maximal LFSR sizes 2 to 20", "")
{
   std::map <std::uint64_t, std::uint64_t> countMap;
   for (std::uint8_t size = 2; size < 21; ++size)
   {
      generateLfsrCycle (size, countMap);
      std::uint64_t limit = 1 << size;
      REQUIRE(countMap.size () == limit-1);
      for (std::uint64_t i = 1; i < limit; ++i)
      {
         // All numbers between 1 and (1 << size) -1 must be found exactly once
         REQUIRE(countMap.end () != countMap.find (i));
         REQUIRE(1 == countMap[i]);
      }
   }
}
