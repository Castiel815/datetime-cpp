cmake_minimum_required(VERSION 3.10)
project(DateTime VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
endif()

# 设置输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 创建静态库
add_library(datetime STATIC
        src/datetime.cpp
)

# 设置库的包含目录
target_include_directories(datetime PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 设置库的编译特性
target_compile_features(datetime PUBLIC cxx_std_11)

# 选项控制是否构建示例和测试
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_TESTS "Build test programs" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# 如果选择构建共享库
if(BUILD_SHARED_LIBS)
    add_library(datetime_shared SHARED
            src/datetime.cpp
    )

    target_include_directories(datetime_shared PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )

    target_compile_features(datetime_shared PUBLIC cxx_std_11)

    # 设置共享库版本
    set_target_properties(datetime_shared PROPERTIES
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
            OUTPUT_NAME datetime
    )

    # Windows特定设置
    if(WIN32)
        target_compile_definitions(datetime_shared PRIVATE DATETIME_EXPORTS)
        target_compile_definitions(datetime_shared INTERFACE DATETIME_IMPORTS)
    endif()
endif()

# 构建示例程序
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 构建测试程序
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# 安装配置
include(GNUInstallDirs)

# 安装库文件
install(TARGETS datetime
        EXPORT DateTimeTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 如果构建了共享库，也要安装
if(BUILD_SHARED_LIBS)
    install(TARGETS datetime_shared
            EXPORT DateTimeTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()

# 安装头文件
install(FILES include/datetime.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 导出配置
install(EXPORT DateTimeTargets
        FILE DateTimeTargets.cmake
        NAMESPACE DateTime::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DateTime
)

# 创建配置文件
include(CMakePackageConfigHelpers)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DateTimeConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/DateTimeConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DateTime
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/DateTimeConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/DateTimeConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/DateTimeConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DateTime
)

# 创建pkg-config文件
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/datetime.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/datetime.pc
        @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/datetime.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# 显示配置信息
message(STATUS "DateTime Library Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
message(STATUS "  Build Shared Libraries: ${BUILD_SHARED_LIBS}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")

# 添加uninstall目标
if(NOT TARGET uninstall)
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
            IMMEDIATE @ONLY
    )

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()