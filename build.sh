#!/bin/bash

# C++ DateTime Library Build Script

echo "Building C++ DateTime Library..."

# 创建目录结构
echo "Creating directory structure..."
mkdir -p src include obj lib

# 复制源文件到对应目录
echo "Setting up source files..."
cp datetime.h include/
cp datetime.cpp src/

# 编译静态库
echo "Compiling static library..."
g++ -std=c++11 -Wall -Wextra -O2 -fPIC -Iinclude -c src/datetime.cpp -o obj/datetime.o

if [ $? -eq 0 ]; then
    echo "Object file compiled successfully"
else
    echo "Compilation failed"
    exit 1
fi

# 创建静态库
echo "Creating static library..."
ar rcs lib/libdatetime.a obj/datetime.o

if [ $? -eq 0 ]; then
    echo "Static library created successfully: lib/libdatetime.a"
else
    echo "Library creation failed"
    exit 1
fi

# 编译示例程序
echo "Compiling example program..."
g++ -std=c++11 -Wall -Wextra -O2 -Iinclude example.cpp -Llib -ldatetime -o example

if [ $? -eq 0 ]; then
    echo "Example program compiled successfully"
    echo ""
    echo "Running example..."
    ./example
else
    echo "Example compilation failed"
fi

echo ""
echo "Build complete!"
echo ""
echo "Files created:"
echo "  - lib/libdatetime.a (static library)"
echo "  - include/datetime.h (header file)"
echo "  - example (example program)"
echo ""
echo "To use the library in your project:"
echo "  g++ -std=c++11 your_program.cpp -Iinclude -Llib -ldatetime -o your_program"