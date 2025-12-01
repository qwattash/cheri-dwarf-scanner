
#include "flat_layout_scraper.hh"
#include "storage.hh"

#include "fixture.hh"

using namespace cheri;

TEST_F(TestStorage, TestExtractStructVLA) {
  std::filesystem::path src("assets/sample_struct_vla");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  {
    auto q_vla = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'struct_with_vla::%' ORDER BY name");
    EXPECT_FALSE(q_vla.lastError().isValid());
    EXPECT_EQ(selectedRows(q_vla), 2);
    EXPECT_TRUE(q_vla.seek(0));
    EXPECT_EQ(q_vla.value("name").toString(), "struct_with_vla::value");
    EXPECT_FALSE(q_vla.value("is_vla").toBool());
    EXPECT_TRUE(q_vla.seek(1));
    EXPECT_EQ(q_vla.value("name").toString(), "struct_with_vla::vla");
    EXPECT_TRUE(q_vla.value("is_vla").toBool());
  }

  {
    auto q_vla = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'struct_with_size0_vla::%' ORDER BY name");
    EXPECT_FALSE(q_vla.lastError().isValid());
    EXPECT_EQ(selectedRows(q_vla), 2);
    EXPECT_TRUE(q_vla.seek(0));
    EXPECT_EQ(q_vla.value("name").toString(), "struct_with_size0_vla::value");
    EXPECT_FALSE(q_vla.value("is_vla").toBool());
    EXPECT_TRUE(q_vla.seek(1));
    EXPECT_EQ(q_vla.value("name").toString(), "struct_with_size0_vla::vla");
    EXPECT_TRUE(q_vla.value("is_vla").toBool());
  }

  {
    auto q_vla = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'struct_with_size1_vla::%' ORDER BY name");
    EXPECT_FALSE(q_vla.lastError().isValid());
    EXPECT_EQ(selectedRows(q_vla), 2);
    EXPECT_TRUE(q_vla.seek(0));
    EXPECT_EQ(q_vla.value("name").toString(), "struct_with_size1_vla::value");
    EXPECT_FALSE(q_vla.value("is_vla").toBool());
    EXPECT_TRUE(q_vla.seek(1));
    EXPECT_EQ(q_vla.value("name").toString(), "struct_with_size1_vla::vla");
    EXPECT_TRUE(q_vla.value("is_vla").toBool());
  }
}

TEST_F(TestStorage, TestExtractNestedVLA) {
  std::filesystem::path src("assets/sample_nested_struct_vla");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  {
    auto q_vla = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'nested_with_vla::inner::%' ORDER BY name");
    EXPECT_FALSE(q_vla.lastError().isValid());
    EXPECT_EQ(selectedRows(q_vla), 2);
    EXPECT_TRUE(q_vla.seek(0));
    EXPECT_EQ(q_vla.value("name").toString(), "nested_with_vla::inner::value");
    EXPECT_FALSE(q_vla.value("is_vla").toBool());
    EXPECT_TRUE(q_vla.seek(1));
    EXPECT_EQ(q_vla.value("name").toString(), "nested_with_vla::inner::vla");
    EXPECT_TRUE(q_vla.value("is_vla").toBool());
  }

  {
    auto q_vla = sm_->query(
        "SELECT * FROM layout_member WHERE name = 'nested_with_vla::inner'");
    EXPECT_FALSE(q_vla.lastError().isValid());
    EXPECT_EQ(selectedRows(q_vla), 1);
    EXPECT_TRUE(q_vla.seek(0));
    EXPECT_FALSE(q_vla.value("is_vla").toBool());
  }

  {
    auto q_info =
        sm_->query("SELECT * FROM type_layout WHERE name = 'nested_with_vla'");
    EXPECT_FALSE(q_info.lastError().isValid());
    EXPECT_EQ(selectedRows(q_info), 1);
    EXPECT_TRUE(q_info.seek(0));
    EXPECT_TRUE(q_info.value("has_vla").toBool());
  }

  {
    auto q_info =
        sm_->query("SELECT * FROM type_layout WHERE name = 'inner_with_vla'");
    EXPECT_FALSE(q_info.lastError().isValid());
    EXPECT_EQ(selectedRows(q_info), 1);
    EXPECT_TRUE(q_info.seek(0));
    EXPECT_TRUE(q_info.value("has_vla").toBool());
  }
}

TEST_F(TestStorage, TestExtractUnionVLA) {
  std::filesystem::path src("assets/sample_union_vla");
  auto scraper = setupScraper(src);

  auto result = execScraper(scraper.get());
  EXPECT_EQ(result.errors.size(), 0);

  {
    auto q_vla = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'union_with_vla::%' ORDER BY name");
    EXPECT_FALSE(q_vla.lastError().isValid());
    EXPECT_EQ(selectedRows(q_vla), 2);
    EXPECT_TRUE(q_vla.seek(0));
    EXPECT_EQ(q_vla.value("name").toString(), "union_with_vla::value");
    EXPECT_FALSE(q_vla.value("is_vla").toBool());
    EXPECT_TRUE(q_vla.seek(1));
    EXPECT_EQ(q_vla.value("name").toString(), "union_with_vla::vla");
    EXPECT_TRUE(q_vla.value("is_vla").toBool());
  }

  {
    auto q_vla = sm_->query("SELECT * FROM layout_member WHERE name LIKE "
                            "'union_with_vla_mix::%' ORDER BY name");
    EXPECT_FALSE(q_vla.lastError().isValid());
    EXPECT_EQ(selectedRows(q_vla), 2);
    EXPECT_TRUE(q_vla.seek(0));
    EXPECT_EQ(q_vla.value("name").toString(), "union_with_vla_mix::value");
    EXPECT_FALSE(q_vla.value("is_vla").toBool());
    EXPECT_TRUE(q_vla.seek(1));
    EXPECT_EQ(q_vla.value("name").toString(), "union_with_vla_mix::vla");
    EXPECT_TRUE(q_vla.value("is_vla").toBool());
  }

  {
    auto q_info =
        sm_->query("SELECT * FROM type_layout WHERE name = 'union_with_vla'");
    EXPECT_FALSE(q_info.lastError().isValid());
    EXPECT_EQ(selectedRows(q_info), 1);
    EXPECT_TRUE(q_info.seek(0));
    EXPECT_TRUE(q_info.value("has_vla").toBool());
  }

  {
    auto q_info = sm_->query(
        "SELECT * FROM type_layout WHERE name = 'union_with_vla_mix'");
    EXPECT_FALSE(q_info.lastError().isValid());
    EXPECT_EQ(selectedRows(q_info), 1);
    EXPECT_TRUE(q_info.seek(0));
    EXPECT_TRUE(q_info.value("has_vla").toBool());
  }
}
