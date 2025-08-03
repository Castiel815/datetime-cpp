# Makefile for DateTime Static Library

# 编译器设置
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2 -fPIC
AR = ar
ARFLAGS = rcs

# 目录设置
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
LIB_DIR = lib

# 文件设置
SOURCES = $(SRC_DIR)/datetime.cpp
OBJECTS = $(OBJ_DIR)/datetime.o
HEADERS = $(INC_DIR)/datetime.h
LIBRARY = $(LIB_DIR)/libdatetime.a

# 目标设置
.PHONY: all clean install test example

all: $(LIBRARY)

# 创建目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# 编译对象文件
$(OBJ_DIR)/datetime.o: $(SRC_DIR)/datetime.cpp $(INC_DIR)/datetime.h | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# 创建静态库
$(LIBRARY): $(OBJECTS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Static library created: $@"

# 编译示例程序
example: $(LIBRARY) example.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) example.cpp -L$(LIB_DIR) -ldatetime -o example
	@echo "Example compiled successfully"

# 编译测试程序
test: $(LIBRARY) test.cpp
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) test.cpp -L$(LIB_DIR) -ldatetime -o test
	@echo "Test compiled successfully"

# 安装库文件到系统目录（可选）
install: $(LIBRARY)
	@echo "Installing library..."
	sudo cp $(LIBRARY) /usr/local/lib/
	sudo cp $(INC_DIR)/datetime.h /usr/local/include/
	sudo ldconfig
	@echo "Installation complete"

# 清理编译文件
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f example test
	@echo "Clean complete"

# 显示帮助信息
help:
	@echo "Available targets:"
	@echo "  all      - Build the static library"
	@echo "  example  - Build example program"
	@echo "  test     - Build test program"
	@echo "  install  - Install library to system (requires sudo)"
	@echo "  clean    - Remove all build files"
	@echo "  help     - Show this help message"