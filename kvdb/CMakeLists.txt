cmake_minimum_required(VERSION 3.16)

project(kvdb
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(KVDB_BUILD_TESTS "Build kvdb tests" ON)
option(KVDB_BUILD_BENCHMARKS "Build kvdb benchmarks" ON)

# include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules) # future custom modules

add_library(kvdb
    src/kvdb.cpp
    src/db/shard.cpp
    src/db/hashtable.cpp
    src/db/storage_engine.cpp
    src/db/ttl_manager.cpp
    src/db/snapshot.cpp
    src/model/value.cpp
    src/model/entry.cpp
    src/net/server.cpp
    src/net/protocol.cpp
    src/commands/command_registry.cpp
    src/commands/set_command.cpp
    src/commands/get_command.cpp
    src/commands/del_command.cpp
    src/commands/expire_command.cpp
    src/utils/logger.cpp
    src/utils/hash.cpp
)

target_include_directories(kvdb
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(kvdb PUBLIC cxx_std_20)

add_executable(kvdb-server
    src/main.cpp
)

find_package(asio REQUIRED)
find_package(xxHash REQUIRED CONFIG)

target_link_libraries(kvdb
    PUBLIC asio::asio
    PRIVATE xxHash::xxhash
)

target_link_libraries(kvdb-server
    PRIVATE kvdb
)

if(KVDB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(KVDB_BUILD_BENCHMARKS)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/CMakeLists.txt)
        add_subdirectory(benchmark)
    endif()
endif()

