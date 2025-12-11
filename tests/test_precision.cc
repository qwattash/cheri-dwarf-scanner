/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2023-2025 Alfredo Mazzinghi
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <filesystem>

#include "fixture.hh"
#include "scraper.hh"

using namespace cheri;

TEST(CompressedCap, RequiredPrecision) {
  // Note: this selects the architecture
  std::filesystem::path src("assets/sample_struct_vla");
  cheri::DwarfSource dwsrc(src);

  auto Check = [&dwsrc](uint64_t base, uint64_t top) {
    return dwsrc.findRequiredPrecision(base, top - base);
  };

  ASSERT_EQ(Check(0x00000000, 0x00100000), 1);
  ASSERT_EQ(Check(0x00000004, 0x00001004), 11);
  ASSERT_EQ(Check(0x0FFFFFFF, 0x10000000), 1);
  ASSERT_EQ(Check(0x00000FFF, 0x00002001), 13);
}

TEST(CompressedCap, MaxRepresentableLength) {
  // Note: this selects the architecture
  std::filesystem::path src("assets/sample_struct_vla");
  cheri::DwarfSource dwsrc(src);
  uint64_t max_len;

  max_len = dwsrc.findMaxRepresentableLength(0xf1);
  ASSERT_EQ(max_len, 0xfff);
  max_len = dwsrc.findMaxRepresentableLength(0xf2);
  ASSERT_EQ(max_len, 0xfff);
  max_len = dwsrc.findMaxRepresentableLength(0xf4);
  ASSERT_EQ(max_len, 0xfff);
  max_len = dwsrc.findMaxRepresentableLength(0xf8);
  ASSERT_EQ(max_len, 0x1ff8);
  max_len = dwsrc.findMaxRepresentableLength(0xf0);
  ASSERT_EQ(max_len, 0x3ff0);
}
