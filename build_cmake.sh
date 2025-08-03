#!/bin/bash
# build_cmake.sh - CMake构建脚本

set -e  # 遇到错误立即退出

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印彩色信息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 显示帮助信息
show_help() {
    echo "DateTime Library CMake Build Script"
    echo ""
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -h, --help           Show this help message"
    echo "  -c, --clean          Clean build directory before building"
    echo "  -r, --release        Build in Release mode (default: Debug)"
    echo "  -s, --shared         Build shared libraries"
    echo "  -t, --tests          Build and run tests"
    echo "  -e, --examples       Build examples"
    echo "  -i, --install        Install after building"
    echo "  -j, --jobs N         Use N parallel jobs for building (default: auto)"
    echo "  --coverage           Enable code coverage (requires GCC/Clang)"
    echo "  --valgrind           Run tests with valgrind"
    echo "  --prefix PATH        Set installation prefix"
    echo ""
    echo "Examples:"
    echo "  $0                   # Basic debug build"
    echo "  $0 -r -s -i          # Release build with shared libs and install"
    echo "  $0 -t --coverage     # Debug build with tests and coverage"
    echo "  $0 -c -r -j8         # Clean release build with 8 parallel jobs"
}

# 默认参数
BUILD_TYPE="Debug"
SHARED_LIBS="OFF"
BUILD_TESTS="ON"
BUILD_EXAMPLES="ON"
CLEAN_BUILD=false
RUN_INSTALL=false
ENABLE_COVERAGE="OFF"
USE_VALGRIND="OFF"
PARALLEL_JOBS=""
INSTALL_PREFIX=""

# 解析命令行参数
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -s|--shared)
            SHARED_LIBS="ON"
            shift
            ;;
        -t|--tests)
            BUILD_TESTS="ON"
            shift
            ;;
        -e|--examples)
            BUILD_EXAMPLES="ON"
            shift
            ;;
        -i|--install)
            RUN_INSTALL=true
            shift
            ;;
        -j|--jobs)
            PARALLEL_JOBS="$2"
            shift 2
            ;;
        --coverage)
            ENABLE_COVERAGE="ON"
            BUILD_TYPE="Debug"  # 覆盖率测试需要Debug模式
            shift
            ;;
        --valgrind)
            USE_VALGRIND="ON"
            shift
            ;;
        --prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        *)
            print_error "Unknown option: $1"
            show_help
            exit 1
            ;;
    esac
done

# 自动检测并行作业数
if [[ -z "$PARALLEL_JOBS" ]]; then
    if command -v nproc >/dev/null 2>&1; then
        PARALLEL_JOBS=$(nproc)
    elif command -v sysctl >/dev/null 2>&1; then
        PARALLEL_JOBS=$(sysctl -n hw.ncpu)
    else
        PARALLEL_JOBS=4
    fi
fi

# 显示构建配置
print_info "DateTime Library Build Configuration:"
echo "  Build Type: $BUILD_TYPE"
echo "  Shared Libraries: $SHARED_LIBS"
echo "  Build Tests: $BUILD_TESTS"
echo "  Build Examples: $BUILD_EXAMPLES"
echo "  Enable Coverage: $ENABLE_COVERAGE"
echo "  Use Valgrind: $USE_VALGRIND"
echo "  Parallel Jobs: $PARALLEL_JOBS"
echo "  Clean Build: $CLEAN_BUILD"
echo "  Install: $RUN_INSTALL"
if [[ -n "$INSTALL_PREFIX" ]]; then
    echo "  Install Prefix: $INSTALL_PREFIX"
fi
echo ""

# 创建构建目录
BUILD_DIR="build"

if [[ "$CLEAN_BUILD" == true ]]; then
    print_info "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 准备CMake参数
CMAKE_ARGS=(
    "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
    "-DBUILD_SHARED_LIBS=$SHARED_LIBS"
    "-DBUILD_TESTS=$BUILD_TESTS"
    "-DBUILD_EXAMPLES=$BUILD_EXAMPLES"
    "-DENABLE_COVERAGE=$ENABLE_COVERAGE"
    "-DUSE_VALGRIND=$USE_VALGRIND"
)

if [[ -n "$INSTALL_PREFIX" ]]; then
    CMAKE_ARGS+=("-DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX")
fi

# 配置项目
print_info "Configuring project with CMake..."
cmake "${CMAKE_ARGS[@]}" ..

if [[ $? -ne 0 ]]; then
    print_error "CMake configuration failed!"
    exit 1
fi

# 构建项目
print_info "Building project..."
cmake --build . --config "$BUILD_TYPE" -j "$PARALLEL_JOBS"

if [[ $? -ne 0 ]]; then
    print_error "Build failed!"
    exit 1
fi

print_success "Build completed successfully!"

# 运行测试
if [[ "$BUILD_TESTS" == "ON" ]]; then
    print_info "Running tests..."
    ctest --output-on-failure -j "$PARALLEL_JOBS"

    if [[ $? -ne 0 ]]; then
        print_warning "Some tests failed, but continuing..."
    else
        print_success "All tests passed!"
    fi

    # 生成覆盖率报告
    if [[ "$ENABLE_COVERAGE" == "ON" ]]; then
        print_info "Generating coverage report..."
        make coverage 2>/dev/null || print_warning "Coverage report generation failed"
    fi
fi

# 运行示例程序
if [[ "$BUILD_EXAMPLES" == "ON" ]]; then
    print_info "Running example program..."
    if [[ -f "bin/examples/example" ]]; then
        ./bin/examples/example
    elif [[ -f "bin/example" ]]; then
        ./bin/example
    else
        print_warning "Example program not found"
    fi
fi

# 安装
if [[ "$RUN_INSTALL" == true ]]; then
    print_info "Installing library..."

    if [[ "$EUID" -ne 0 ]] && [[ -z "$INSTALL_PREFIX" || "$INSTALL_PREFIX" == "/usr/local"* ]]; then
        print_info "Installing to system directory, using sudo..."
        sudo cmake --install . --config "$BUILD_TYPE"
    else
        cmake --install . --config "$BUILD_TYPE"
    fi

    if [[ $? -eq 0 ]]; then
        print_success "Installation completed!"

        # 更新库缓存（Linux）
        if command -v ldconfig >/dev/null 2>&1; then
            sudo ldconfig 2>/dev/null || true
        fi
    else
        print_error "Installation failed!"
        exit 1
    fi
fi

# 显示构建结果
echo ""
print_success "Build Summary:"
echo "  Library: $(find . -name "libdatetime.*" | head -1)"
echo "  Header: ../include/datetime.h"

if [[ "$BUILD_EXAMPLES" == "ON" ]]; then
    echo "  Examples: bin/examples/"
fi

if [[ "$BUILD_TESTS" == "ON" ]]; then
    echo "  Tests: bin/tests/"
fi

if [[ "$ENABLE_COVERAGE" == "ON" ]]; then
    echo "  Coverage Report: coverage_report/index.html"
fi

echo ""
print_info "Build completed successfully!"

# 使用说明
echo ""
print_info "Usage in your project:"
echo "  # Method 1: CMake find_package"
echo "  find_package(DateTime REQUIRED)"
echo "  target_link_libraries(your_target DateTime::datetime)"
echo ""
echo "  # Method 2: pkg-config"
echo "  pkg-config --cflags --libs datetime"
echo ""
echo "  # Method 3: Manual"
echo "  g++ -std=c++11 -I../include your_file.cpp -L./lib -ldatetime"