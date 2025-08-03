#include "datetime.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <functional>
#include <vector>

using namespace datetime;

// 简单的测试框架
class TestRunner {
private:
    int tests_run = 0;
    int tests_passed = 0;
    
public:
    void run_test(const std::string& name, std::function<void()> test) {
        tests_run++;
        try {
            test();
            tests_passed++;
            std::cout << "[PASS] " << name << std::endl;
        } catch (const std::exception& e) {
            std::cout << "[FAIL] " << name << " - " << e.what() << std::endl;
        } catch (...) {
            std::cout << "[FAIL] " << name << " - Unknown exception" << std::endl;
        }
    }
    
    void print_summary() {
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Tests run: " << tests_run << std::endl;
        std::cout << "Tests passed: " << tests_passed << std::endl;
        std::cout << "Tests failed: " << (tests_run - tests_passed) << std::endl;
        std::cout << "Success rate: " << (tests_passed * 100.0 / tests_run) << "%" << std::endl;
    }
    
    bool all_passed() const {
        return tests_run == tests_passed;
    }
};

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        std::ostringstream oss; \
        oss << "Expected " << (expected) << " but got " << (actual); \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        throw std::runtime_error("Condition was false"); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        throw std::runtime_error("Condition was true"); \
    }

#define ASSERT_THROWS(expression) \
    { \
        bool threw = false; \
        try { \
            expression; \
        } catch (...) { \
            threw = true; \
        } \
        if (!threw) { \
            throw std::runtime_error("Expected exception was not thrown"); \
        } \
    }

int main() {
    TestRunner runner;
    
    std::cout << "Running DateTime Library Basic Tests\n";
    std::cout << "====================================\n\n";
    
    // 测试构造函数
    runner.run_test("DateTime Default Constructor", []() {
        DateTime dt;
        // 只要不抛异常就算通过
    });
    
    runner.run_test("DateTime Parameterized Constructor", []() {
        DateTime dt(2023, 5, 15, 14, 30, 45);
        ASSERT_EQ(2023, dt.year());
        ASSERT_EQ(5, dt.month());
        ASSERT_EQ(15, dt.day());
        ASSERT_EQ(14, dt.hour());
        ASSERT_EQ(30, dt.minute());
        ASSERT_EQ(45, dt.second());
    });
    
    runner.run_test("DateTime Invalid Date", []() {
        ASSERT_THROWS(DateTime(2023, 2, 30));  // February 30th doesn't exist
        ASSERT_THROWS(DateTime(2023, 13, 1));  // Month 13 doesn't exist
        ASSERT_THROWS(DateTime(2023, 1, 32));  // January 32nd doesn't exist
    });
    
    // 测试静态方法
    runner.run_test("DateTime::now()", []() {
        DateTime dt1 = DateTime::now();
        DateTime dt2 = DateTime::now();
        // 两次调用应该很接近，差异应该小于1秒
        TimeDelta diff = dt2 - dt1;
        ASSERT_TRUE(diff.totalSeconds() >= 0);
        ASSERT_TRUE(diff.totalSeconds() < 1);
    });
    
    runner.run_test("DateTime::fromTimestamp()", []() {
        time_t timestamp = 1609459200; // 2021-01-01 00:00:00 UTC
        DateTime dt = DateTime::fromTimestamp(timestamp);
        ASSERT_EQ(timestamp, dt.timestamp());
    });
    
    // 测试日期组件获取
    runner.run_test("Date Components", []() {
        DateTime dt(2023, 7, 15, 10, 30, 45);
        
        ASSERT_EQ(2023, dt.year());
        ASSERT_EQ(7, dt.month());
        ASSERT_EQ(15, dt.day());
        ASSERT_EQ(10, dt.hour());
        ASSERT_EQ(30, dt.minute());
        ASSERT_EQ(45, dt.second());
        
        // 2023-07-15 是星期六 (weekday 6)
        ASSERT_EQ(6, dt.weekday());
        
        // 7月15日是一年中的第196天
        ASSERT_EQ(196, dt.dayOfYear());
    });
    
    // 测试格式化
    runner.run_test("String Formatting", []() {
        DateTime dt(2023, 5, 15, 9, 30, 45);
        
        std::string formatted = dt.toString();
        ASSERT_TRUE(formatted.find("2023") != std::string::npos);
        ASSERT_TRUE(formatted.find("05") != std::string::npos);
        ASSERT_TRUE(formatted.find("15") != std::string::npos);
        
        std::string iso = dt.isoformat();
        ASSERT_EQ("2023-05-15T09:30:45", iso);
        
        std::string custom = dt.strftime("%Y-%m-%d");
        ASSERT_EQ("2023-05-15", custom);
    });
    
    // 测试日期运算
    runner.run_test("Date Arithmetic", []() {
        DateTime dt(2023, 5, 15, 10, 30, 0);
        
        DateTime next_day = dt.addDays(1);
        ASSERT_EQ(16, next_day.day());
        
        DateTime prev_day = dt.addDays(-1);
        ASSERT_EQ(14, prev_day.day());
        
        DateTime next_month = dt.addMonths(1);
        ASSERT_EQ(6, next_month.month());
        
        DateTime next_year = dt.addYears(1);
        ASSERT_EQ(2024, next_year.year());
        
        DateTime next_hour = dt.addHours(2);
        ASSERT_EQ(12, next_hour.hour());
        
        DateTime next_minute = dt.addMinutes(30);
        ASSERT_EQ(0, next_minute.minute());
        ASSERT_EQ(11, next_minute.hour());
    });
    
    // 测试日期替换
    runner.run_test("Date Replacement", []() {
        DateTime dt(2023, 5, 15, 10, 30, 45);
        
        DateTime new_year = dt.replace(2024);
        ASSERT_EQ(2024, new_year.year());
        ASSERT_EQ(5, new_year.month());  // 其他组件不变
        
        DateTime new_month = dt.replace(-1, 12);
        ASSERT_EQ(2023, new_month.year());
        ASSERT_EQ(12, new_month.month());
        
        DateTime noon = dt.replace(-1, -1, -1, 12, 0, 0);
        ASSERT_EQ(12, noon.hour());
        ASSERT_EQ(0, noon.minute());
        ASSERT_EQ(0, noon.second());
    });
    
    // 测试比较操作
    runner.run_test("Date Comparison", []() {
        DateTime dt1(2023, 5, 15, 10, 30, 0);
        DateTime dt2(2023, 5, 15, 10, 30, 0);
        DateTime dt3(2023, 5, 16, 10, 30, 0);
        
        ASSERT_TRUE(dt1 == dt2);
        ASSERT_FALSE(dt1 == dt3);
        ASSERT_TRUE(dt1 != dt3);
        ASSERT_TRUE(dt1 < dt3);
        ASSERT_TRUE(dt3 > dt1);
        ASSERT_TRUE(dt1 <= dt2);
        ASSERT_TRUE(dt1 >= dt2);
    });
    
    // 测试TimeDelta构造
    runner.run_test("TimeDelta Construction", []() {
        TimeDelta td1;
        ASSERT_EQ(0, td1.totalSeconds());
        
        TimeDelta td2(5, 2, 30, 15);  // 5 days, 2 hours, 30 minutes, 15 seconds
        long long expected = 5 * 24 * 3600 + 2 * 3600 + 30 * 60 + 15;
        ASSERT_EQ(expected, td2.totalSeconds());
        
        ASSERT_EQ(5, td2.days());
        ASSERT_EQ(2 * 3600 + 30 * 60 + 15, td2.seconds());
    });
    
    // 测试TimeDelta运算
    runner.run_test("TimeDelta Arithmetic", []() {
        TimeDelta td1(1, 0, 0, 0);  // 1 day
        TimeDelta td2(0, 12, 0, 0); // 12 hours
        
        TimeDelta sum = td1 + td2;
        ASSERT_EQ(1 * 24 * 3600 + 12 * 3600, sum.totalSeconds());
        
        TimeDelta diff = td1 - td2;
        ASSERT_EQ(1 * 24 * 3600 - 12 * 3600, diff.totalSeconds());
        
        TimeDelta mult = td2 * 2;
        ASSERT_EQ(24 * 3600, mult.totalSeconds());
        
        TimeDelta div = td1 / 2;
        ASSERT_EQ(12 * 3600, div.totalSeconds());
    });
    
    // 测试TimeDelta比较
    runner.run_test("TimeDelta Comparison", []() {
        TimeDelta td1(1, 0, 0, 0);  // 1 day
        TimeDelta td2(0, 24, 0, 0); // 24 hours
        TimeDelta td3(2, 0, 0, 0);  // 2 days
        
        ASSERT_TRUE(td1 == td2);  // 1 day == 24 hours
        ASSERT_FALSE(td1 == td3);
        ASSERT_TRUE(td1 != td3);
        ASSERT_TRUE(td1 < td3);
        ASSERT_TRUE(td3 > td1);
        ASSERT_TRUE(td1 <= td2);
        ASSERT_TRUE(td1 >= td2);
    });
    
    // 测试DateTime和TimeDelta的混合运算
    runner.run_test("DateTime TimeDelta Operations", []() {
        DateTime dt(2023, 5, 15, 12, 0, 0);
        TimeDelta td(1, 6, 0, 0);  // 1 day, 6 hours
        
        DateTime future = dt + td;
        ASSERT_EQ(16, future.day());
        ASSERT_EQ(18, future.hour());
        
        DateTime past = dt - td;
        ASSERT_EQ(14, past.day());
        ASSERT_EQ(6, past.hour());
        
        DateTime dt2(2023, 5, 20, 12, 0, 0);
        TimeDelta diff = dt2 - dt;
        ASSERT_EQ(5, diff.days());
        ASSERT_EQ(0, diff.seconds());
    });
    
    // 测试字符串解析
    runner.run_test("String Parsing", []() {
        DateTime dt = DateTime::fromString("2023-05-15 14:30:45");
        ASSERT_EQ(2023, dt.year());
        ASSERT_EQ(5, dt.month());
        ASSERT_EQ(15, dt.day());
        ASSERT_EQ(14, dt.hour());
        ASSERT_EQ(30, dt.minute());
        ASSERT_EQ(45, dt.second());
        
        // 测试自定义格式
        DateTime dt2 = DateTime::fromString("15/05/2023", "%d/%m/%Y");
        ASSERT_EQ(2023, dt2.year());
        ASSERT_EQ(5, dt2.month());
        ASSERT_EQ(15, dt2.day());
    });
    
    // 测试无效字符串解析
    runner.run_test("Invalid String Parsing", []() {
        ASSERT_THROWS(DateTime::fromString("invalid date"));
        ASSERT_THROWS(DateTime::fromString("2023-13-01"));  // Invalid month
        ASSERT_THROWS(DateTime::fromString("2023-02-30"));  // Invalid day
    });
    
    // 测试工具函数
    runner.run_test("Utility Functions", []() {
        ASSERT_TRUE(isLeapYear(2020));
        ASSERT_FALSE(isLeapYear(2021));
        ASSERT_TRUE(isLeapYear(2000));
        ASSERT_FALSE(isLeapYear(1900));
        
        ASSERT_EQ(31, daysInMonth(2023, 1));  // January
        ASSERT_EQ(28, daysInMonth(2023, 2));  // February (non-leap)
        ASSERT_EQ(29, daysInMonth(2020, 2));  // February (leap)
        ASSERT_EQ(30, daysInMonth(2023, 4));  // April
        ASSERT_EQ(31, daysInMonth(2023, 12)); // December
    });
    
    // 测试TimeDelta格式化
    runner.run_test("TimeDelta Formatting", []() {
        TimeDelta td1(0, 2, 30, 45);
        std::string formatted = td1.toString();
        ASSERT_TRUE(formatted.find("02:30:45") != std::string::npos);
        
        TimeDelta td2(3, 0, 0, 0);
        std::string formatted2 = td2.toString();
        ASSERT_TRUE(formatted2.find("3 days") != std::string::npos);
        
        TimeDelta td3(1, 0, 0, 0);
        std::string formatted3 = td3.toString();
        ASSERT_TRUE(formatted3.find("1 day") != std::string::npos);  // singular
    });
    
    // 测试边界情况
    runner.run_test("Boundary Cases", []() {
        // 测试闰年2月29日
        DateTime leap_day(2020, 2, 29);
        ASSERT_EQ(29, leap_day.day());
        ASSERT_EQ(2, leap_day.month());
        
        // 测试月份边界
        DateTime end_of_month(2023, 1, 31);
        DateTime next_month = end_of_month.addMonths(1);
        ASSERT_EQ(2, next_month.month());
        // 注意：1月31日 + 1个月 = 2月28日或3月3日，具体取决于实现
        
        // 测试年份边界
        DateTime end_of_year(2023, 12, 31, 23, 59, 59);
        DateTime next_second = end_of_year.addSeconds(1);
        ASSERT_EQ(2024, next_second.year());
        ASSERT_EQ(1, next_second.month());
        ASSERT_EQ(1, next_second.day());
        ASSERT_EQ(0, next_second.hour());
        ASSERT_EQ(0, next_second.minute());
        ASSERT_EQ(0, next_second.second());
    });
    
    // 性能测试（简单版本）
    runner.run_test("Basic Performance", []() {
        auto start = std::chrono::high_resolution_clock::now();
        
        // 执行1000次操作
        for (int i = 0; i < 1000; ++i) {
            DateTime dt(2023, 5, 15, 10, 30, i % 60);
            DateTime future = dt.addDays(i % 100);
            std::string formatted = future.toString();
            TimeDelta diff = future - dt;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // 应该在合理时间内完成（比如100ms）
        ASSERT_TRUE(duration.count() < 1000);  // 不超过1秒
        
        std::cout << "    Performance: 1000 operations in " 
                  << duration.count() << "ms" << std::endl;
    });
    
    // 测试内存管理（确保没有内存泄漏的基本检查）
    runner.run_test("Memory Management", []() {
        // 创建和销毁大量对象
        std::vector<DateTime> dates;
        std::vector<TimeDelta> deltas;
        
        for (int i = 0; i < 1000; ++i) {
            dates.emplace_back(2020 + i % 10, (i % 12) + 1, (i % 28) + 1);
            deltas.emplace_back(i % 100, i % 24, i % 60, i % 60);
        }
        
        // 执行一些操作
        for (size_t i = 0; i < dates.size(); ++i) {
            DateTime result = dates[i] + deltas[i];
            std::string formatted = result.toString();
        }
        
        // 清理
        dates.clear();
        deltas.clear();
        
        // 如果到这里没有崩溃，就认为通过了
    });
    
    runner.print_summary();
    
    if (runner.all_passed()) {
        std::cout << "\n🎉 All tests PASSED! 🎉\n" << std::endl;
        return 0;
    } else {
        std::cout << "\n❌ Some tests FAILED! ❌\n" << std::endl;
        return 1;
    }
}