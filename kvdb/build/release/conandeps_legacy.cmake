message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(asio)
find_package(xxHash)
find_package(GTest)

set(CONANDEPS_LEGACY  asio::asio  xxHash::xxhash  gtest::gtest )