# Tests CMakeLists.txt

# 启用测试支持
enable_testing()

# 基本功能测试
add_executable(test_basic
        test_basic.cpp
)

target_link_libraries(test_basic datetime)

# DateTime类测试
add_executable(test_datetime
        test_datetime.cpp
)

target_link_libraries(test_datetime datetime)

# TimeDelta类测试
add_executable(test_timedelta
        test_timedelta.cpp
)

target_link_libraries(test_timedelta datetime)

# 格式化功能测试
add_executable(test_formatting
        test_formatting.cpp
)

target_link_libraries(test_formatting datetime)

# 解析功能测试
add_executable(test_parsing
        test_parsing.cpp
)

target_link_libraries(test_parsing datetime)

# 运算功能测试
add_executable(test_arithmetic
        test_arithmetic.cpp
)

target_link_libraries(test_arithmetic datetime)

# 边界情况测试
add_executable(test_edge_cases
        test_edge_cases.cpp
)

target_link_libraries(test_edge_cases datetime)

# 设置测试程序的输出目录
set_target_properties(
        test_basic test_datetime test_timedelta test_formatting
        test_parsing test_arithmetic test_edge_cases
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
)

# 添加测试到CTest
add_test(NAME BasicFunctionality COMMAND test_basic)
add_test(NAME DateTimeClass COMMAND test_datetime)
add_test(NAME TimeDeltaClass COMMAND test_timedelta)
add_test(NAME FormattingFeatures COMMAND test_formatting)
add_test(NAME ParsingFeatures COMMAND test_parsing)
add_test(NAME ArithmeticOperations COMMAND test_arithmetic)
add_test(NAME EdgeCases COMMAND test_edge_cases)

# 设置测试属性
set_tests_properties(
        BasicFunctionality DateTimeClass TimeDeltaClass FormattingFeatures
        ParsingFeatures ArithmeticOperations EdgeCases
        PROPERTIES
        TIMEOUT 30
)

# 如果需要，可以添加内存检查
find_program(VALGRIND_EXECUTABLE valgrind)
if(VALGRIND_EXECUTABLE)
    option(USE_VALGRIND "Run tests with valgrind" OFF)

    if(USE_VALGRIND)
        add_test(NAME BasicFunctionality_Valgrind
                COMMAND ${VALGRIND_EXECUTABLE} --error-exitcode=1 --leak-check=full
                $<TARGET_FILE:test_basic>)
        add_test(NAME DateTimeClass_Valgrind
                COMMAND ${VALGRIND_EXECUTABLE} --error-exitcode=1 --leak-check=full
                $<TARGET_FILE:test_datetime>)
        # 可以为其他测试添加类似的valgrind测试
    endif()
endif()

# 添加覆盖率支持（GCC/Clang）
option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(test_basic PRIVATE --coverage)
    target_link_libraries(test_basic --coverage)

    target_compile_options(test_datetime PRIVATE --coverage)
    target_link_libraries(test_datetime --coverage)

    target_compile_options(test_timedelta PRIVATE --coverage)
    target_link_libraries(test_timedelta --coverage)

    target_compile_options(test_formatting PRIVATE --coverage)
    target_link_libraries(test_formatting --coverage)

    target_compile_options(test_parsing PRIVATE --coverage)
    target_link_libraries(test_parsing --coverage)

    target_compile_options(test_arithmetic PRIVATE --coverage)
    target_link_libraries(test_arithmetic --coverage)

    target_compile_options(test_edge_cases PRIVATE --coverage)
    target_link_libraries(test_edge_cases --coverage)

    # 添加覆盖率报告目标
    find_program(GCOV_EXECUTABLE gcov)
    find_program(LCOV_EXECUTABLE lcov)
    find_program(GENHTML_EXECUTABLE genhtml)

    if(GCOV_EXECUTABLE AND LCOV_EXECUTABLE AND GENHTML_EXECUTABLE)
        add_custom_target(coverage
                COMMAND ${LCOV_EXECUTABLE} --directory . --capture --output-file coverage.info
                COMMAND ${LCOV_EXECUTABLE} --remove coverage.info '/usr/*' --output-file coverage.info
                COMMAND ${LCOV_EXECUTABLE} --list coverage.info
                COMMAND ${GENHTML_EXECUTABLE} -o coverage_report coverage.info
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Generating code coverage report"
        )
    endif()
endif()

message(STATUS "Tests will be built in: ${CMAKE_BINARY_DIR}/bin/tests")
message(STATUS "Run 'make test' or 'ctest' to execute tests")