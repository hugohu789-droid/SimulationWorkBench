cmake_minimum_required(VERSION 3.19)

# Use FetchContent to automatically download GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Define the test executable file
add_executable(tests
    tst_core_logic.cpp
)

# Link Dependencies
target_link_libraries(tests
    PRIVATE
        core           # Linking core libraries
        GTest::gtest_main # GTest comes with a main function.
)

# Register for testing with CTest
include(GoogleTest)
gtest_discover_tests(tests)
