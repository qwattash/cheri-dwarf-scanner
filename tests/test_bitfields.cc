/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2023-2026 Alfredo Mazzinghi
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

#include "flat_layout_scraper.hh"
#include "storage.hh"

#include "fixture.hh"

using namespace cheri;

TEST_F(TestStorage, TestBitfieldStruct) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_bf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                         "'bitfield_struct::%' ORDER BY name");
  EXPECT_FALSE(q_bf.lastError().isValid());
  EXPECT_EQ(selectedRows(q_bf), 3);

  EXPECT_TRUE(q_bf.seek(0));
  EXPECT_EQ(q_bf.value("name").toString(), "bitfield_struct::a");
  EXPECT_EQ(q_bf.value("bit_size").toULongLong(), 3);
  EXPECT_EQ(q_bf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_bf.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_bf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_bf.value("base").toString(), "0");
  EXPECT_EQ(q_bf.value("top").toString(), "1");
  EXPECT_EQ(q_bf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_bf.seek(1));
  EXPECT_EQ(q_bf.value("name").toString(), "bitfield_struct::b");
  EXPECT_EQ(q_bf.value("bit_size").toULongLong(), 5);
  EXPECT_EQ(q_bf.value("bit_offset").toULongLong(), 3);
  EXPECT_EQ(q_bf.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_bf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_bf.value("base").toString(), "0");
  EXPECT_EQ(q_bf.value("top").toString(), "1");
  EXPECT_EQ(q_bf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_bf.seek(2));
  EXPECT_EQ(q_bf.value("name").toString(), "bitfield_struct::c");
  EXPECT_EQ(q_bf.value("bit_size").toULongLong(), 24);
  EXPECT_EQ(q_bf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_bf.value("byte_offset").toULongLong(), 1);
  EXPECT_EQ(q_bf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_bf.value("base").toString(), "1");
  EXPECT_EQ(q_bf.value("top").toString(), "4");
  EXPECT_EQ(q_bf.value("required_precision").toULongLong(), 2);
}

TEST_F(TestStorage, TestMixedBitfieldStruct) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_mbf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                          "'mixed_bitfield_struct::%' ORDER BY name");
  EXPECT_FALSE(q_mbf.lastError().isValid());
  EXPECT_EQ(selectedRows(q_mbf), 4);

  EXPECT_TRUE(q_mbf.seek(0));
  EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::w");
  EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 8);
  EXPECT_EQ(q_mbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("base").toString(), "8");
  EXPECT_EQ(q_mbf.value("top").toString(), "16");
  EXPECT_EQ(q_mbf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_mbf.seek(1));
  EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::x");
  EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("base").toString(), "0");
  EXPECT_EQ(q_mbf.value("top").toString(), "1");
  EXPECT_EQ(q_mbf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_mbf.seek(2));
  EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::y");
  EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 15);
  EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 1);
  EXPECT_EQ(q_mbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("base").toString(), "1");
  EXPECT_EQ(q_mbf.value("top").toString(), "3");
  EXPECT_EQ(q_mbf.value("required_precision").toULongLong(), 2);

  EXPECT_TRUE(q_mbf.seek(3));
  EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::z");
  EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 17);
  EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 4);
  EXPECT_EQ(q_mbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_mbf.value("base").toString(), "4");
  EXPECT_EQ(q_mbf.value("top").toString(), "7");
  EXPECT_EQ(q_mbf.value("required_precision").toULongLong(), 2);
}

TEST_F(TestStorage, TestAnonBitfieldStruct) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_abf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                          "'anon_bitfield_struct::%' ORDER BY name");
  EXPECT_FALSE(q_abf.lastError().isValid());
  EXPECT_EQ(selectedRows(q_abf), 2);

  EXPECT_TRUE(q_abf.seek(0));
  EXPECT_EQ(q_abf.value("name").toString(), "anon_bitfield_struct::a");
  EXPECT_EQ(q_abf.value("bit_size").toULongLong(), 3);
  EXPECT_EQ(q_abf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_abf.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_abf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_abf.value("base").toString(), "0");
  EXPECT_EQ(q_abf.value("top").toString(), "1");
  EXPECT_EQ(q_abf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_abf.seek(1));
  EXPECT_EQ(q_abf.value("name").toString(), "anon_bitfield_struct::b");
  EXPECT_EQ(q_abf.value("bit_size").toULongLong(), 4);
  EXPECT_EQ(q_abf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_abf.value("byte_offset").toULongLong(), 1);
  EXPECT_EQ(q_abf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_abf.value("base").toString(), "1");
  EXPECT_EQ(q_abf.value("top").toString(), "2");
  EXPECT_EQ(q_abf.value("required_precision").toULongLong(), 1);
}

TEST_F(TestStorage, TestOffsetNotByteBoundary) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_onbb = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                           "'offset_not_byte_boundary::%' ORDER BY name");
  EXPECT_FALSE(q_onbb.lastError().isValid());
  EXPECT_EQ(selectedRows(q_onbb), 3);

  EXPECT_TRUE(q_onbb.seek(0));
  EXPECT_EQ(q_onbb.value("name").toString(), "offset_not_byte_boundary::a");
  EXPECT_EQ(q_onbb.value("bit_size").toULongLong(), 0);
  EXPECT_EQ(q_onbb.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_onbb.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_onbb.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_onbb.value("base").toString(), "0");
  EXPECT_EQ(q_onbb.value("top").toString(), "2");
  EXPECT_EQ(q_onbb.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_onbb.seek(1));
  EXPECT_EQ(q_onbb.value("name").toString(), "offset_not_byte_boundary::b");
  EXPECT_EQ(q_onbb.value("bit_size").toULongLong(), 3);
  EXPECT_EQ(q_onbb.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_onbb.value("byte_offset").toULongLong(), 2);
  EXPECT_EQ(q_onbb.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_onbb.value("base").toString(), "2");
  EXPECT_EQ(q_onbb.value("top").toString(), "3");
  EXPECT_EQ(q_onbb.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_onbb.seek(2));
  EXPECT_EQ(q_onbb.value("name").toString(), "offset_not_byte_boundary::c");
  EXPECT_EQ(q_onbb.value("bit_size").toULongLong(), 6);
  EXPECT_EQ(q_onbb.value("bit_offset").toULongLong(), 3);
  EXPECT_EQ(q_onbb.value("byte_offset").toULongLong(), 2);
  EXPECT_EQ(q_onbb.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_onbb.value("base").toString(), "2");
  EXPECT_EQ(q_onbb.value("top").toString(), "4");
  EXPECT_EQ(q_onbb.value("required_precision").toULongLong(), 1);
}

TEST_F(TestStorage, TestBoundaryBitfields) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_bb = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                         "'boundary_bitfields::%' ORDER BY name");
  EXPECT_FALSE(q_bb.lastError().isValid());
  EXPECT_EQ(selectedRows(q_bb), 3);

  EXPECT_TRUE(q_bb.seek(0));
  EXPECT_EQ(q_bb.value("name").toString(), "boundary_bitfields::a");
  EXPECT_EQ(q_bb.value("bit_size").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("base").toString(), "0");
  EXPECT_EQ(q_bb.value("top").toString(), "1");
  EXPECT_EQ(q_bb.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_bb.seek(1));
  EXPECT_EQ(q_bb.value("name").toString(), "boundary_bitfields::b");
  EXPECT_EQ(q_bb.value("bit_size").toULongLong(), 8);
  EXPECT_EQ(q_bb.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("byte_offset").toULongLong(), 1);
  EXPECT_EQ(q_bb.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("base").toString(), "1");
  EXPECT_EQ(q_bb.value("top").toString(), "2");
  EXPECT_EQ(q_bb.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_bb.seek(2));
  EXPECT_EQ(q_bb.value("name").toString(), "boundary_bitfields::c");
  EXPECT_EQ(q_bb.value("bit_size").toULongLong(), 16);
  EXPECT_EQ(q_bb.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("byte_offset").toULongLong(), 2);
  EXPECT_EQ(q_bb.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_bb.value("base").toString(), "2");
  EXPECT_EQ(q_bb.value("top").toString(), "4");
  EXPECT_EQ(q_bb.value("required_precision").toULongLong(), 1);
}

TEST_F(TestStorage, TestLargeBitfieldStruct) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_lbf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                          "'large_bitfield_struct::%' ORDER BY name");
  EXPECT_FALSE(q_lbf.lastError().isValid());
  EXPECT_EQ(selectedRows(q_lbf), 2);

  EXPECT_TRUE(q_lbf.seek(0));
  EXPECT_EQ(q_lbf.value("name").toString(), "large_bitfield_struct::a");
  EXPECT_EQ(q_lbf.value("bit_size").toULongLong(), 32);
  EXPECT_EQ(q_lbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_lbf.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_lbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_lbf.value("base").toString(), "0");
  EXPECT_EQ(q_lbf.value("top").toString(), "4");
  EXPECT_EQ(q_lbf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_lbf.seek(1));
  EXPECT_EQ(q_lbf.value("name").toString(), "large_bitfield_struct::b");
  EXPECT_EQ(q_lbf.value("bit_size").toULongLong(), 16);
  EXPECT_EQ(q_lbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_lbf.value("byte_offset").toULongLong(), 4);
  EXPECT_EQ(q_lbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_lbf.value("base").toString(), "4");
  EXPECT_EQ(q_lbf.value("top").toString(), "6");
  EXPECT_EQ(q_lbf.value("required_precision").toULongLong(), 1);
}

TEST_F(TestStorage, TestLargeBitfieldPadding) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_lbf = sm_->query(
      "SELECT * FROM type_layout WHERE name = 'large_bitfield_struct'");
  EXPECT_FALSE(q_lbf.lastError().isValid());
  EXPECT_EQ(selectedRows(q_lbf), 1);

  EXPECT_TRUE(q_lbf.seek(0));
  EXPECT_EQ(q_lbf.value("size").toULongLong(), 8);
  EXPECT_EQ(q_lbf.value("total_padding").toULongLong(), 2);
  EXPECT_EQ(q_lbf.value("tail_padding").toULongLong(), 2);
  EXPECT_EQ(q_lbf.value("holes").toULongLong(), 0);
}

TEST_F(TestStorage, NestedBitfield) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q_nbf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                          "'nested_bitfield_struct::%' ORDER BY name");
  EXPECT_FALSE(q_nbf.lastError().isValid());
  EXPECT_EQ(selectedRows(q_nbf), 5);

  EXPECT_TRUE(q_nbf.seek(0));
  EXPECT_EQ(q_nbf.value("name").toString(), "nested_bitfield_struct::a");
  EXPECT_EQ(q_nbf.value("bit_size").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("byte_size").toULongLong(), 1);
  EXPECT_EQ(q_nbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("byte_offset").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("base").toString().toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("top").toString().toULongLong(), 1);
  EXPECT_EQ(q_nbf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_nbf.seek(1));
  EXPECT_EQ(q_nbf.value("name").toString(), "nested_bitfield_struct::b");
  EXPECT_EQ(q_nbf.value("bit_size").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("byte_size").toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("byte_offset").toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("base").toString().toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("top").toString().toULongLong(), 8);
  EXPECT_EQ(q_nbf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_nbf.seek(2));
  EXPECT_EQ(q_nbf.value("name").toString(), "nested_bitfield_struct::b::a");
  EXPECT_EQ(q_nbf.value("bit_size").toULongLong(), 3);
  EXPECT_EQ(q_nbf.value("byte_size").toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("byte_offset").toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("base").toString().toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("top").toString().toULongLong(), 5);
  EXPECT_EQ(q_nbf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_nbf.seek(3));
  EXPECT_EQ(q_nbf.value("name").toString(), "nested_bitfield_struct::b::b");
  EXPECT_EQ(q_nbf.value("bit_size").toULongLong(), 5);
  EXPECT_EQ(q_nbf.value("byte_size").toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("bit_offset").toULongLong(), 3);
  EXPECT_EQ(q_nbf.value("byte_offset").toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("base").toString().toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("top").toString().toULongLong(), 5);
  EXPECT_EQ(q_nbf.value("required_precision").toULongLong(), 1);

  EXPECT_TRUE(q_nbf.seek(4));
  EXPECT_EQ(q_nbf.value("name").toString(), "nested_bitfield_struct::b::c");
  EXPECT_EQ(q_nbf.value("bit_size").toULongLong(), 24);
  EXPECT_EQ(q_nbf.value("byte_size").toULongLong(), 4);
  EXPECT_EQ(q_nbf.value("bit_offset").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("byte_offset").toULongLong(), 5);
  EXPECT_EQ(q_nbf.value("is_imprecise").toULongLong(), 0);
  EXPECT_EQ(q_nbf.value("base").toString().toULongLong(), 5);
  EXPECT_EQ(q_nbf.value("top").toString().toULongLong(), 8);
  EXPECT_EQ(q_nbf.value("required_precision").toULongLong(), 2);
}
