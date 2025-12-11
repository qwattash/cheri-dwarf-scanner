
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test_vla "test_vla.cc")
target_link_libraries(test_vla dwarf_scraper_lib GTest::gtest_main)
gtest_discover_tests(test_vla
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests")

add_executable(test_precision "test_precision.cc")
target_link_libraries(test_precision dwarf_scraper_lib GTest::gtest_main)
gtest_discover_tests(test_precision
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests")
