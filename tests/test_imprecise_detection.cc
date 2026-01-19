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

#include <filesystem>

#include "fixture.hh"
#include "scraper.hh"

using namespace cheri;

TEST_F(TestStorage, SimpleImpreciseMember) {
  // Note: this selects the architecture
  std::filesystem::path src("assets/sample_imprecise_member");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  {
    auto q_imprecise =
        sm_->query("SELECT * FROM layout_member WHERE is_imprecise = 1");
    EXPECT_FALSE(q_imprecise.lastError().isValid());
    EXPECT_EQ(selectedRows(q_imprecise), 1);
    EXPECT_TRUE(q_imprecise.seek(0));
    EXPECT_EQ(q_imprecise.value("name").toString(), "foo::hash");
    EXPECT_EQ(q_imprecise.value("byte_offset").toULongLong(), 0x4002);
    EXPECT_EQ(q_imprecise.value("bit_offset").toULongLong(), 0);
    EXPECT_EQ(q_imprecise.value("byte_size").toULongLong(), 0x4000);
    EXPECT_EQ(q_imprecise.value("bit_size").toULongLong(), 0);
    EXPECT_EQ(q_imprecise.value("array_items").toULongLong(), 0x2000);
    EXPECT_EQ(q_imprecise.value("base").toULongLong(), 0x4000);
    EXPECT_EQ(q_imprecise.value("top").toULongLong(), 0x8020);
    EXPECT_EQ(q_imprecise.value("required_precision").toInt(), 14);
    EXPECT_TRUE(q_imprecise.value("max_vla_size").isNull());
  }
}
