#include "datetime.h"
#include <iostream>

using namespace datetime;

int main() {
    std::cout << "=== C++ DateTime Library Example ===" << std::endl;
    
    // 1. 创建DateTime对象
    std::cout << "\n1. Creating DateTime objects:" << std::endl;
    DateTime now = DateTime::now();
    DateTime birthday(1990, 5, 15, 14, 30, 0);
    DateTime epoch = DateTime::fromTimestamp(0);
    
    std::cout << "Current time: " << now.toString() << std::endl;
    std::cout << "Birthday: " << birthday.toString() << std::endl;
    std::cout << "Unix epoch: " << epoch.toString() << std::endl;
    
    // 2. 获取日期时间组件
    std::cout << "\n2. Date/time components:" << std::endl;
    std::cout << "Year: " << now.year() << std::endl;
    std::cout << "Month: " << now.month() << std::endl;
    std::cout << "Day: " << now.day() << std::endl;
    std::cout << "Hour: " << now.hour() << std::endl;
    std::cout << "Minute: " << now.minute() << std::endl;
    std::cout << "Second: " << now.second() << std::endl;
    std::cout << "Weekday: " << now.weekday() << " (0=Sunday)" << std::endl;
    std::cout << "Day of year: " << now.dayOfYear() << std::endl;
    
    // 3. 格式化输出
    std::cout << "\n3. Formatting:" << std::endl;
    std::cout << "Default format: " << now.toString() << std::endl;
    std::cout << "ISO format: " << now.isoformat() << std::endl;
    std::cout << "Custom format: " << now.strftime("%A, %B %d, %Y at %I:%M %p") << std::endl;
    std::cout << "Date only: " << now.strftime("%Y-%m-%d") << std::endl;
    std::cout << "Time only: " << now.strftime("%H:%M:%S") << std::endl;
    
    // 4. 时间运算
    std::cout << "\n4. Date arithmetic:" << std::endl;
    DateTime tomorrow = now.addDays(1);
    DateTime nextWeek = now.addDays(7);
    DateTime nextMonth = now.addMonths(1);
    DateTime nextYear = now.addYears(1);
    
    std::cout << "Tomorrow: " << tomorrow.toString() << std::endl;
    std::cout << "Next week: " << nextWeek.toString() << std::endl;
    std::cout << "Next month: " << nextMonth.toString() << std::endl;
    std::cout << "Next year: " << nextYear.toString() << std::endl;
    
    // 5. 时间替换
    std::cout << "\n5. Date replacement:" << std::endl;
    DateTime newYear = now.replace(2025, 1, 1, 0, 0, 0);
    DateTime noon = now.replace(-1, -1, -1, 12, 0, 0);
    
    std::cout << "New Year 2025: " << newYear.toString() << std::endl;
    std::cout << "Today at noon: " << noon.toString() << std::endl;
    
    // 6. 时间差计算
    std::cout << "\n6. Time differences:" << std::endl;
    TimeDelta age = now - birthday;
    TimeDelta until_new_year = newYear - now;
    
    std::cout << "Age: " << age.toString() << std::endl;
    std::cout << "Days lived: " << age.days() << std::endl;
    std::cout << "Until New Year: " << until_new_year.toString() << std::endl;
    
    // 7. TimeDelta操作
    std::cout << "\n7. TimeDelta operations:" << std::endl;
    TimeDelta one_week(7, 0, 0, 0);
    TimeDelta two_hours(0, 2, 0, 0);
    TimeDelta combined = one_week + two_hours;
    
    std::cout << "One week: " << one_week.toString() << std::endl;
    std::cout << "Two hours: " << two_hours.toString() << std::endl;
    std::cout << "Combined: " << combined.toString() << std::endl;
    
    // 8. 比较操作
    std::cout << "\n8. Comparisons:" << std::endl;
    std::cout << "Birthday < Now: " << (birthday < now ? "true" : "false") << std::endl;
    std::cout << "Tomorrow > Now: " << (tomorrow > now ? "true" : "false") << std::endl;
    std::cout << "Now == Now: " << (now == now ? "true" : "false") << std::endl;
    
    // 9. 字符串解析
    std::cout << "\n9. String parsing:" << std::endl;
    try {
        DateTime parsed = DateTime::fromString("2023-12-25 15:30:45");
        std::cout << "Parsed date: " << parsed.toString() << std::endl;
        
        DateTime parsed2 = DateTime::fromString("25/12/2023", "%d/%m/%Y");
        std::cout << "Parsed date (custom format): " << parsed2.toString() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Parse error: " << e.what() << std::endl;
    }
    
    // 10. 工具函数
    std::cout << "\n10. Utility functions:" << std::endl;
    std::cout << "2024 is leap year: " << (isLeapYear(2024) ? "true" : "false") << std::endl;
    std::cout << "2023 is leap year: " << (isLeapYear(2023) ? "true" : "false") << std::endl;
    std::cout << "Days in February 2024: " << daysInMonth(2024, 2) << std::endl;
    std::cout << "Days in February 2023: " << daysInMonth(2023, 2) << std::endl;
    
    return 0;
}