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

#include "fixture.hh"
#include "flat_layout_scraper.hh"
#include "storage.hh"

using namespace cheri;

TEST_F(TestStorage, TestPaddedStruct) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q = sm_->query("SELECT * FROM type_layout WHERE name = 'padded_struct'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 12);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 6);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 3);
  EXPECT_EQ(q.value("holes").toULongLong(), 1);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
  EXPECT_EQ(q.value("has_extra_padding").toULongLong(), 0);
}

TEST_F(TestStorage, TestPaddedUnion) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q = sm_->query("SELECT * FROM type_layout WHERE name = 'padded_union'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 8);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 3);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 3);
  EXPECT_EQ(q.value("holes").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
  EXPECT_EQ(q.value("has_extra_padding").toULongLong(), 0);
}

TEST_F(TestStorage, TestNoPaddingStruct) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q =
      sm_->query("SELECT * FROM type_layout WHERE name = 'no_padding_struct'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 8);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("holes").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
  EXPECT_EQ(q.value("has_extra_padding").toULongLong(), 0);
}

TEST_F(TestStorage, TestNoPaddingUnion) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q =
      sm_->query("SELECT * FROM type_layout WHERE name = 'no_padding_union'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 4);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("holes").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
  EXPECT_EQ(q.value("has_extra_padding").toULongLong(), 0);
}

TEST_F(TestStorage, TestPointerPadding) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q =
      sm_->query("SELECT * FROM type_layout WHERE name = 'pointer_padding'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 48);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 30);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("holes").toULongLong(), 1);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
}

TEST_F(TestStorage, TestAlignInner) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q = sm_->query("SELECT * FROM type_layout WHERE name = 'align_inner'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 32);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("holes").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
}

TEST_F(TestStorage, TestParentPadding) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q =
      sm_->query("SELECT * FROM type_layout WHERE name = 'parent_padding'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 64);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 30);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("holes").toULongLong(), 1);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
}

TEST_F(TestStorage, TestArrayOfNested) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q =
      sm_->query("SELECT * FROM type_layout WHERE name = 'array_of_nested'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 96);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 30);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("holes").toULongLong(), 1);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 30);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
}

TEST_F(TestStorage, TestArrayPadding) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q = sm_->query("SELECT * FROM type_layout WHERE name = 'array_padding'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 20);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 6);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 3);
  EXPECT_EQ(q.value("holes").toULongLong(), 1);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
}

TEST_F(TestStorage, TestPointerUnion) {
  std::filesystem::path src("assets/sample_padding");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  auto q = sm_->query("SELECT * FROM type_layout WHERE name = 'pointer_union'");
  EXPECT_FALSE(q.lastError().isValid());
  EXPECT_EQ(selectedRows(q), 1);
  EXPECT_TRUE(q.seek(0));
  EXPECT_EQ(q.value("size").toULongLong(), 32);
  EXPECT_EQ(q.value("total_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("tail_padding").toULongLong(), 15);
  EXPECT_EQ(q.value("holes").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_padding").toULongLong(), 0);
  EXPECT_EQ(q.value("nested_holes").toULongLong(), 0);
}
