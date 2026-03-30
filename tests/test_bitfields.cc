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

#include "flat_layout_scraper.hh"
#include "storage.hh"

#include "fixture.hh"

using namespace cheri;

TEST_F(TestStorage, TestExtractBitfields) {
  std::filesystem::path src("assets/sample_bitfields");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  {
    auto q_bf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                           "'bitfield_struct::%' ORDER BY name");
    EXPECT_FALSE(q_bf.lastError().isValid());
    EXPECT_EQ(selectedRows(q_bf), 3);

    EXPECT_TRUE(q_bf.seek(0));
    EXPECT_EQ(q_bf.value("name").toString(), "bitfield_struct::a");
    EXPECT_EQ(q_bf.value("bit_size").toULongLong(), 3);
    EXPECT_EQ(q_bf.value("bit_offset").toULongLong(), 0);
    EXPECT_EQ(q_bf.value("byte_offset").toULongLong(), 0);

    EXPECT_TRUE(q_bf.seek(1));
    EXPECT_EQ(q_bf.value("name").toString(), "bitfield_struct::b");
    EXPECT_EQ(q_bf.value("bit_size").toULongLong(), 5);
    EXPECT_EQ(q_bf.value("bit_offset").toULongLong(), 3);
    EXPECT_EQ(q_bf.value("byte_offset").toULongLong(), 0);

    EXPECT_TRUE(q_bf.seek(2));
    EXPECT_EQ(q_bf.value("name").toString(), "bitfield_struct::c");
    EXPECT_EQ(q_bf.value("bit_size").toULongLong(), 24);
    EXPECT_EQ(q_bf.value("bit_offset").toULongLong(), 8);
    EXPECT_EQ(q_bf.value("byte_offset").toULongLong(), 0);
  }

  {
    auto q_mbf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'mixed_bitfield_struct::%' ORDER BY name");
    EXPECT_FALSE(q_mbf.lastError().isValid());
    EXPECT_EQ(selectedRows(q_mbf), 4);

    EXPECT_TRUE(q_mbf.seek(0));
    EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::w");
    EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 0);
    EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 0);
    EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 8);

    EXPECT_TRUE(q_mbf.seek(1));
    EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::x");
    EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 0);
    EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 0);
    EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 0);

    EXPECT_TRUE(q_mbf.seek(2));
    EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::y");
    EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 15);
    EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 8);
    EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 0);

    EXPECT_TRUE(q_mbf.seek(3));
    EXPECT_EQ(q_mbf.value("name").toString(), "mixed_bitfield_struct::z");
    EXPECT_EQ(q_mbf.value("bit_size").toULongLong(), 17);
    EXPECT_EQ(q_mbf.value("bit_offset").toULongLong(), 32);
    EXPECT_EQ(q_mbf.value("byte_offset").toULongLong(), 0);
  }

  {
    auto q_abf = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'anon_bitfield_struct::%' ORDER BY name");
    EXPECT_FALSE(q_abf.lastError().isValid());
    EXPECT_EQ(selectedRows(q_abf), 2);

    EXPECT_TRUE(q_abf.seek(0));
    EXPECT_EQ(q_abf.value("name").toString(), "anon_bitfield_struct::a");
    EXPECT_EQ(q_abf.value("bit_size").toULongLong(), 3);
    EXPECT_EQ(q_abf.value("bit_offset").toULongLong(), 0);
    EXPECT_EQ(q_abf.value("byte_offset").toULongLong(), 0);

    EXPECT_TRUE(q_abf.seek(1));
    EXPECT_EQ(q_abf.value("name").toString(), "anon_bitfield_struct::b");
    EXPECT_EQ(q_abf.value("bit_size").toULongLong(), 4);
    EXPECT_EQ(q_abf.value("bit_offset").toULongLong(), 8);
    EXPECT_EQ(q_abf.value("byte_offset").toULongLong(), 0);
  }
}
