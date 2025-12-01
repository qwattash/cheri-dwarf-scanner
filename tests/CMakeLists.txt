
find_package(GTest REQUIRED)
include(GoogleTest)

# add_executable(test_subobject "test_subobject.cc")
# target_link_libraries(test_subobject dwarf_scraper_lib GTest::gtest_main)
# gtest_discover_tests(test_subobject PROPERTIES
#   ENVIRONMENT "ASSET_DIR=${PROJECT_SOURCE_DIR}/../tests/assets")

add_executable(test_vla "test_vla.cc")
target_link_libraries(test_vla dwarf_scraper_lib GTest::gtest_main)
gtest_discover_tests(test_vla
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/tests")
