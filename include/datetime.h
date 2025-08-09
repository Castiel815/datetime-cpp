#ifndef DATETIME_H
#define DATETIME_H

#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace datetime {

class DateTime {
private:
    std::chrono::system_clock::time_point time_point_;

public:
    // 构造函数 
    DateTime();
    DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0);
    DateTime(const std::chrono::system_clock::time_point& tp);
    DateTime(time_t timestamp);

    // 静态工厂方法 
    static DateTime now();
    static DateTime fromString(const std::string& dateStr, const std::string& format = "%Y-%m-%d %H:%M:%S");
    static DateTime fromTimestamp(time_t timestamp);

    // 获取日期时间组件 
    int year() const;
    int month() const;
    int day() const;
    int hour() const;
    int minute() const;
    int second() const;
    int weekday() const;  // 0=Sunday, 1=Monday, ..., 6=Saturday
    int dayOfYear() const;

    // 格式化输出 
    std::string toString(const std::string& format = "%Y-%m-%d %H:%M:%S") const;
    std::string isoformat() const;
    std::string strftime(const std::string& format) const;

    // 时间戳 
    time_t timestamp() const;
    long long milliseconds() const;

    // 日期时间运算 
    DateTime addYears(int years) const;
    DateTime addMonths(int months) const;
    DateTime addDays(int days) const;
    DateTime addHours(int hours) const;
    DateTime addMinutes(int minutes) const;
    DateTime addSeconds(int seconds) const;

    // 日期替换 
    DateTime replace(int year = -1, int month = -1, int day = -1,
                    int hour = -1, int minute = -1, int second = -1) const;

    // 比较操作符 
    bool operator==(const DateTime& other) const;
    bool operator!=(const DateTime& other) const;
    bool operator<(const DateTime& other) const;
    bool operator<=(const DateTime& other) const;
    bool operator>(const DateTime& other) const;
    bool operator>=(const DateTime& other) const;

    // 获取内部时间点 
    std::chrono::system_clock::time_point getTimePoint() const;
};

// 时间差类 
class TimeDelta {
private:
    std::chrono::seconds duration_;

public:
    TimeDelta();
    TimeDelta(int days, int hours = 0, int minutes = 0, int seconds = 0);
    TimeDelta(const std::chrono::seconds& duration);

    // 获取组件 
    long long totalSeconds() const;
    int days() const;
    int seconds() const;  // 不包括天数的秒数部分 

    // 运算符重载 
    TimeDelta operator+(const TimeDelta& other) const;
    TimeDelta operator-(const TimeDelta& other) const;
    TimeDelta operator*(int multiplier) const;
    TimeDelta operator/(int divisor) const;

    bool operator==(const TimeDelta& other) const;
    bool operator!=(const TimeDelta& other) const;
    bool operator<(const TimeDelta& other) const;
    bool operator<=(const TimeDelta& other) const;
    bool operator>(const TimeDelta& other) const;
    bool operator>=(const TimeDelta& other) const;

    std::string toString() const;
};

// DateTime 和 TimeDelta 之间的运算 
DateTime operator+(const DateTime& dt, const TimeDelta& td);
DateTime operator-(const DateTime& dt, const TimeDelta& td);
TimeDelta operator-(const DateTime& dt1, const DateTime& dt2);

// 工具函数 
bool isLeapYear(int year);
int daysInMonth(int year, int month);
std::string formatDuration(const TimeDelta& td);

} // namespace datetime

#endif // DATETIME_H
