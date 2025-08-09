#include "datetime.h"
#include <stdexcept>

namespace datetime {

// DateTime 实现 
DateTime::DateTime() : time_point_(std::chrono::system_clock::now()) {}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second) {
    std::tm tm = {};
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    time_t time = std::mktime(&tm);
    if (time == -1) {
        throw std::invalid_argument("Invalid date/time");
    }
    time_point_ = std::chrono::system_clock::from_time_t(time);
}

DateTime::DateTime(const std::chrono::system_clock::time_point& tp) : time_point_(tp) {}

DateTime::DateTime(time_t timestamp) : time_point_(std::chrono::system_clock::from_time_t(timestamp)) {}

DateTime DateTime::now() {
    return DateTime(std::chrono::system_clock::now());
}

DateTime DateTime::fromString(const std::string& dateStr, const std::string& format) {
    std::tm tm = {};
    std::istringstream ss(dateStr);
    ss >> std::get_time(&tm, format.c_str());

    if (ss.fail()) {
        throw std::invalid_argument("Failed to parse date string");
    }

    tm.tm_isdst = -1;
    time_t time = std::mktime(&tm);
    if (time == -1) {
        throw std::invalid_argument("Invalid date/time");
    }

    return DateTime(std::chrono::system_clock::from_time_t(time));
}

DateTime DateTime::fromTimestamp(time_t timestamp) {
    return DateTime(timestamp);
}

int DateTime::year() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_year + 1900;
}

int DateTime::month() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_mon + 1;
}

int DateTime::day() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_mday;
}

int DateTime::hour() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_hour;
}

int DateTime::minute() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_min;
}

int DateTime::second() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_sec;
}

int DateTime::weekday() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_wday;
}

int DateTime::dayOfYear() const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);
    return tm->tm_yday + 1;
}

std::string DateTime::toString(const std::string& format) const {
    return strftime(format);
}

std::string DateTime::isoformat() const {
    return strftime("%Y-%m-%dT%H:%M:%S");
}

std::string DateTime::strftime(const std::string& format) const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm* tm = std::localtime(&time);

    char buffer[256];
    std::strftime(buffer, sizeof(buffer), format.c_str(), tm);
    return std::string(buffer);
}

time_t DateTime::timestamp() const {
    return std::chrono::system_clock::to_time_t(time_point_);
}

long long DateTime::milliseconds() const {
    auto duration = time_point_.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

DateTime DateTime::addYears(int years) const {
    std::tm tm = {};
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    tm = *std::localtime(&time);
    tm.tm_year += years;
    tm.tm_isdst = -1;

    time_t new_time = std::mktime(&tm);
    return DateTime(std::chrono::system_clock::from_time_t(new_time));
}

DateTime DateTime::addMonths(int months) const {
    std::tm tm = {};
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    tm = *std::localtime(&time);

    tm.tm_mon += months;
    tm.tm_isdst = -1;

    time_t new_time = std::mktime(&tm);
    return DateTime(std::chrono::system_clock::from_time_t(new_time));
}

DateTime DateTime::addDays(int days) const {
    auto new_time = time_point_ + std::chrono::hours(24 * days);
    return DateTime(new_time);
}

DateTime DateTime::addHours(int hours) const {
    auto new_time = time_point_ + std::chrono::hours(hours);
    return DateTime(new_time);
}

DateTime DateTime::addMinutes(int minutes) const {
    auto new_time = time_point_ + std::chrono::minutes(minutes);
    return DateTime(new_time);
}

DateTime DateTime::addSeconds(int seconds) const {
    auto new_time = time_point_ + std::chrono::seconds(seconds);
    return DateTime(new_time);
}

DateTime DateTime::replace(int year, int month, int day, int hour, int minute, int second) const {
    time_t time = std::chrono::system_clock::to_time_t(time_point_);
    std::tm tm = *std::localtime(&time);

    if (year != -1) tm.tm_year = year - 1900;
    if (month != -1) tm.tm_mon = month - 1;
    if (day != -1) tm.tm_mday = day;
    if (hour != -1) tm.tm_hour = hour;
    if (minute != -1) tm.tm_min = minute;
    if (second != -1) tm.tm_sec = second;

    tm.tm_isdst = -1;
    time_t new_time = std::mktime(&tm);
    return DateTime(std::chrono::system_clock::from_time_t(new_time));
}

bool DateTime::operator==(const DateTime& other) const {
    return time_point_ == other.time_point_;
}

bool DateTime::operator!=(const DateTime& other) const {
    return !(*this == other);
}

bool DateTime::operator<(const DateTime& other) const {
    return time_point_ < other.time_point_;
}

bool DateTime::operator<=(const DateTime& other) const {
    return time_point_ <= other.time_point_;
}

bool DateTime::operator>(const DateTime& other) const {
    return time_point_ > other.time_point_;
}

bool DateTime::operator>=(const DateTime& other) const {
    return time_point_ >= other.time_point_;
}

std::chrono::system_clock::time_point DateTime::getTimePoint() const {
    return time_point_;
}

// TimeDelta 实现 
TimeDelta::TimeDelta() : duration_(0) {}

TimeDelta::TimeDelta(int days, int hours, int minutes, int seconds) {
    duration_ = std::chrono::seconds(days * 24 * 3600 + hours * 3600 + minutes * 60 + seconds);
}

TimeDelta::TimeDelta(const std::chrono::seconds& duration) : duration_(duration) {}

long long TimeDelta::totalSeconds() const {
    return duration_.count();
}

int TimeDelta::days() const {
    return static_cast<int>(duration_.count() / (24 * 3600));
}

int TimeDelta::seconds() const {
    return static_cast<int>(duration_.count() % (24 * 3600));
}

TimeDelta TimeDelta::operator+(const TimeDelta& other) const {
    return TimeDelta(duration_ + other.duration_);
}

TimeDelta TimeDelta::operator-(const TimeDelta& other) const {
    return TimeDelta(duration_ - other.duration_);
}

TimeDelta TimeDelta::operator*(int multiplier) const {
    return TimeDelta(duration_ * multiplier);
}

TimeDelta TimeDelta::operator/(int divisor) const {
    return TimeDelta(duration_ / divisor);
}

bool TimeDelta::operator==(const TimeDelta& other) const {
    return duration_ == other.duration_;
}

bool TimeDelta::operator!=(const TimeDelta& other) const {
    return !(*this == other);
}

bool TimeDelta::operator<(const TimeDelta& other) const {
    return duration_ < other.duration_;
}

bool TimeDelta::operator<=(const TimeDelta& other) const {
    return duration_ <= other.duration_;
}

bool TimeDelta::operator>(const TimeDelta& other) const {
    return duration_ > other.duration_;
}

bool TimeDelta::operator>=(const TimeDelta& other) const {
    return duration_ >= other.duration_;
}

std::string TimeDelta::toString() const {
    long long total_seconds = duration_.count();
    int days = static_cast<int>(total_seconds / (24 * 3600));
    int hours = static_cast<int>((total_seconds % (24 * 3600)) / 3600);
    int minutes = static_cast<int>((total_seconds % 3600) / 60);
    int seconds = static_cast<int>(total_seconds % 60);

    std::ostringstream oss;
    if (days != 0) {
        oss << days << " day" << (days != 1 ? "s" : "") << ", ";
    }

    oss << std::setfill('0') << std::setw(2) << hours << ":"
        << std::setw(2) << minutes << ":" << std::setw(2) << seconds;

    return oss.str();
}

// DateTime 和 TimeDelta 之间的运算 
DateTime operator+(const DateTime& dt, const TimeDelta& td) {
    auto new_time = dt.getTimePoint() + std::chrono::seconds(td.totalSeconds());
    return DateTime(new_time);
}

DateTime operator-(const DateTime& dt, const TimeDelta& td) {
    auto new_time = dt.getTimePoint() - std::chrono::seconds(td.totalSeconds());
    return DateTime(new_time);
}

TimeDelta operator-(const DateTime& dt1, const DateTime& dt2) {
    auto diff = dt1.getTimePoint() - dt2.getTimePoint();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(diff);
    return TimeDelta(seconds);
}

// 工具函数 
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int year, int month) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}

std::string formatDuration(const TimeDelta& td) {
    return td.toString();
}

} // namespace datetime