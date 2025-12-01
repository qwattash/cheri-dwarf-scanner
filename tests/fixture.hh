#include <QCoreApplication>
#include <filesystem>
#include <gtest/gtest.h>
#include <memory>

namespace cheri {

// Common in-memory storage initialization
class TestStorage : public ::testing::Test {
protected:
  std::unique_ptr<DwarfScraper> setupScraper(std::filesystem::path src) {
    auto source = std::make_unique<DwarfSource>(src);
    return std::make_unique<FlatLayoutScraper>(*sm_, std::move(source));
  }

  ScraperResult execScraper(DwarfScraper *scraper) {
    std::stop_source dummy_stop_src;
    scraper->initSchema();
    scraper->run(dummy_stop_src.get_token());
    return scraper->result();
  }

  ssize_t selectedRows(QSqlQuery &q) {
    ssize_t count = -1;
    if (q.last()) {
      count = q.at() + 1;
      q.first();
      q.previous();
    }
    return count;
  }

  void SetUp() override {
    int dummy_argc = 0;
    app_ = std::make_unique<QCoreApplication>(dummy_argc, nullptr);
    QCoreApplication::setApplicationName("dwarf-scanner-test");
    QCoreApplication::setApplicationVersion("1.0");

    std::filesystem::path dummy(":memory:");
    sm_ = std::make_unique<StorageManager>(dummy);
  }

  std::unique_ptr<QCoreApplication> app_;
  std::unique_ptr<StorageManager> sm_;
};

} // namespace cheri
