# Examples CMakeLists.txt

# 基本示例程序
add_executable(example
        example.cpp
)

target_link_libraries(example datetime)

# 高级示例程序
add_executable(advanced_example
        advanced_example.cpp
)

target_link_libraries(advanced_example datetime)

# 性能测试示例
add_executable(performance_test
        performance_test.cpp
)

target_link_libraries(performance_test datetime)

# 格式化示例
add_executable(formatting_example
        formatting_example.cpp
)

target_link_libraries(formatting_example datetime)

# 时区示例（如果支持）
add_executable(timezone_example
        timezone_example.cpp
)

target_link_libraries(timezone_example datetime)

# 设置示例程序的输出目录
set_target_properties(
        example advanced_example performance_test formatting_example timezone_example
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples
)

# 安装示例程序（可选）
option(INSTALL_EXAMPLES "Install example programs" OFF)

if(INSTALL_EXAMPLES)
    install(TARGETS example advanced_example performance_test formatting_example timezone_example
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/examples
    )

    # 安装示例源代码
    install(FILES
            example.cpp
            advanced_example.cpp
            performance_test.cpp
            formatting_example.cpp
            timezone_example.cpp
            DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples
    )
endif()

message(STATUS "Examples will be built in: ${CMAKE_BINARY_DIR}/bin/examples")