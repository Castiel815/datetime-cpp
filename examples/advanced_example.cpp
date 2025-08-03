#include "datetime.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace datetime;

// 日程管理示例
class Event {
public:
    std::string name;
    DateTime start_time;
    TimeDelta duration;
    
    Event(const std::string& n, const DateTime& start, const TimeDelta& dur)
        : name(n), start_time(start), duration(dur) {}
    
    DateTime end_time() const {
        return start_time + duration;
    }
    
    bool overlaps_with(const Event& other) const {
        return (start_time < other.end_time()) && (end_time() > other.start_time);
    }
};

class Calendar {
private:
    std::vector<Event> events;
    
public:
    void add_event(const Event& event) {
        events.push_back(event);
        // 按开始时间排序
        std::sort(events.begin(), events.end(), 
                 [](const Event& a, const Event& b) {
                     return a.start_time < b.start_time;
                 });
    }
    
    std::vector<Event> get_events_on_date(const DateTime& date) const {
        std::vector<Event> result;
        DateTime day_start = date.replace(-1, -1, -1, 0, 0, 0);
        DateTime day_end = day_start.addDays(1);
        
        for (const auto& event : events) {
            if (event.start_time >= day_start && event.start_time < day_end) {
                result.push_back(event);
            }
        }
        return result;
    }
    
    bool has_conflicts() const {
        for (size_t i = 0; i < events.size(); ++i) {
            for (size_t j = i + 1; j < events.size(); ++j) {
                if (events[i].overlaps_with(events[j])) {
                    return true;
                }
            }
        }
        return false;
    }
    
    void print_schedule(const DateTime& date) const {
        auto events_today = get_events_on_date(date);
        
        std::cout << "Schedule for " << date.strftime("%A, %B %d, %Y") << ":\n";
        std::cout << std::string(50, '-') << "\n";
        
        if (events_today.empty()) {
            std::cout << "No events scheduled.\n";
            return;
        }
        
        for (const auto& event : events_today) {
            std::cout << event.start_time.strftime("%H:%M") << " - " 
                      << event.end_time().strftime("%H:%M") << " | "
                      << event.name << "\n";
        }
    }
};

// 工作时间统计示例
class TimeTracker {
private:
    std::vector<std::pair<DateTime, DateTime>> work_sessions;
    
public:
    void start_session(const DateTime& start) {
        work_sessions.push_back({start, DateTime()});
    }
    
    void end_session(const DateTime& end) {
        if (!work_sessions.empty() && work_sessions.back().second == DateTime()) {
            work_sessions.back().second = end;
        }
    }
    
    TimeDelta total_work_time() const {
        TimeDelta total(0);
        for (const auto& session : work_sessions) {
            if (session.second != DateTime()) {  // 会话已结束
                total = total + (session.second - session.first);
            }
        }
        return total;
    }
    
    TimeDelta work_time_on_date(const DateTime& date) const {
        DateTime day_start = date.replace(-1, -1, -1, 0, 0, 0);
        DateTime day_end = day_start.addDays(1);
        TimeDelta total(0);
        
        for (const auto& session : work_sessions) {
            if (session.second != DateTime() &&
                session.first >= day_start && session.first < day_end) {
                total = total + (session.second - session.first);
            }
        }
        return total;
    }
    
    void print_weekly_report(const DateTime& week_start) const {
        std::cout << "Weekly Work Report (starting " 
                  << week_start.strftime("%Y-%m-%d") << "):\n";
        std::cout << std::string(50, '=') << "\n";
        
        TimeDelta week_total(0);
        
        for (int i = 0; i < 7; ++i) {
            DateTime day = week_start.addDays(i);
            TimeDelta day_work = work_time_on_date(day);
            week_total = week_total + day_work;
            
            std::cout << day.strftime("%A (%m/%d)") << ": " 
                      << day_work.toString() << "\n";
        }
        
        std::cout << std::string(50, '-') << "\n";
        std::cout << "Total: " << week_total.toString() << "\n";
        std::cout << "Average per day: " 
                  << TimeDelta(week_total.totalSeconds() / 7).toString() << "\n";
    }
};

// 生日提醒示例
class BirthdayReminder {
private:
    struct Person {
        std::string name;
        DateTime birthday;
        
        Person(const std::string& n, const DateTime& bd) : name(n), birthday(bd) {}
        
        int age_on_date(const DateTime& date) const {
            int years = date.year() - birthday.year();
            DateTime this_year_birthday = birthday.replace(date.year());
            if (date < this_year_birthday) {
                years--;
            }
            return years;
        }
        
        DateTime next_birthday(const DateTime& from_date) const {
            DateTime this_year = birthday.replace(from_date.year());
            if (this_year >= from_date) {
                return this_year;
            } else {
                return birthday.replace(from_date.year() + 1);
            }
        }
    };
    
    std::vector<Person> people;
    
public:
    void add_person(const std::string& name, const DateTime& birthday) {
        people.emplace_back(name, birthday);
    }
    
    void check_upcoming_birthdays(const DateTime& from_date, int days_ahead = 30) const {
        std::cout << "Upcoming birthdays (next " << days_ahead << " days):\n";
        std::cout << std::string(60, '=') << "\n";
        
        DateTime end_date = from_date.addDays(days_ahead);
        std::vector<std::pair<DateTime, const Person*>> upcoming;
        
        for (const auto& person : people) {
            DateTime next_bd = person.next_birthday(from_date);
            if (next_bd <= end_date) {
                upcoming.push_back({next_bd, &person});
            }
        }
        
        // 按日期排序
        std::sort(upcoming.begin(), upcoming.end());
        
        if (upcoming.empty()) {
            std::cout << "No upcoming birthdays.\n";
            return;
        }
        
        for (const auto& item : upcoming) {
            const DateTime& birthday = item.first;
            const Person* person = item.second;
            
            TimeDelta days_until = birthday - from_date;
            int age = person->age_on_date(birthday);
            
            std::cout << birthday.strftime("%B %d (%A)") << " - " 
                      << person->name << " turns " << age;
            
            if (days_until.days() == 0) {
                std::cout << " (TODAY!)";
            } else if (days_until.days() == 1) {
                std::cout << " (tomorrow)";
            } else {
                std::cout << " (in " << days_until.days() << " days)";
            }
            std::cout << "\n";
        }
    }
};

// 项目里程碑跟踪
class ProjectTracker {
private:
    struct Milestone {
        std::string name;
        DateTime deadline;
        bool completed;
        
        Milestone(const std::string& n, const DateTime& d) 
            : name(n), deadline(d), completed(false) {}
    };
    
    std::string project_name;
    std::vector<Milestone> milestones;
    
public:
    ProjectTracker(const std::string& name) : project_name(name) {}
    
    void add_milestone(const std::string& name, const DateTime& deadline) {
        milestones.emplace_back(name, deadline);
        // 按截止日期排序
        std::sort(milestones.begin(), milestones.end(),
                 [](const Milestone& a, const Milestone& b) {
                     return a.deadline < b.deadline;
                 });
    }
    
    void complete_milestone(const std::string& name) {
        for (auto& milestone : milestones) {
            if (milestone.name == name) {
                milestone.completed = true;
                break;
            }
        }
    }
    
    void print_status(const DateTime& current_date) const {
        std::cout << "Project: " << project_name << "\n";
        std::cout << "Status as of " << current_date.strftime("%Y-%m-%d") << "\n";
        std::cout << std::string(60, '=') << "\n";
        
        int total = milestones.size();
        int completed = 0;
        int overdue = 0;
        
        for (const auto& milestone : milestones) {
            std::string status;
            if (milestone.completed) {
                status = "[COMPLETED]";
                completed++;
            } else if (milestone.deadline < current_date) {
                status = "[OVERDUE]";
                overdue++;
            } else {
                TimeDelta time_left = milestone.deadline - current_date;
                status = "[" + std::to_string(time_left.days()) + " days left]";
            }
            
            std::cout << milestone.deadline.strftime("%Y-%m-%d") << " " 
                      << std::setw(15) << status << " " 
                      << milestone.name << "\n";
        }
        
        std::cout << std::string(60, '-') << "\n";
        std::cout << "Progress: " << completed << "/" << total 
                  << " completed (" << (completed * 100 / total) << "%)\n";
        if (overdue > 0) {
            std::cout << "Warning: " << overdue << " milestone(s) overdue!\n";
        }
    }
};

int main() {
    std::cout << "=== Advanced DateTime Library Examples ===\n\n";
    
    DateTime now = DateTime::now();
    
    // 1. 日程管理示例
    std::cout << "1. Calendar Management Example\n";
    std::cout << std::string(40, '-') << "\n";
    
    Calendar calendar;
    
    // 添加一些事件
    DateTime today = now.replace(-1, -1, -1, 0, 0, 0);
    calendar.add_event(Event("Team Meeting", today.replace(-1, -1, -1, 9, 0, 0), TimeDelta(0, 1, 30, 0)));
    calendar.add_event(Event("Code Review", today.replace(-1, -1, -1, 14, 0, 0), TimeDelta(0, 1, 0, 0)));
    calendar.add_event(Event("Project Planning", today.replace(-1, -1, -1, 16, 0, 0), TimeDelta(0, 2, 0, 0)));
    
    calendar.print_schedule(today);
    std::cout << "Has conflicts: " << (calendar.has_conflicts() ? "Yes" : "No") << "\n\n";
    
    // 2. 工作时间统计示例
    std::cout << "2. Time Tracking Example\n";
    std::cout << std::string(40, '-') << "\n";
    
    TimeTracker tracker;
    
    // 模拟一周的工作记录
    DateTime week_start = today.addDays(-today.weekday());  // 本周一
    for (int day = 0; day < 5; ++day) {  // 工作日
        DateTime work_day = week_start.addDays(day);
        tracker.start_session(work_day.replace(-1, -1, -1, 9, 0, 0));
        tracker.end_session(work_day.replace(-1, -1, -1, 17, 30, 0));
    }
    
    tracker.print_weekly_report(week_start);
    std::cout << "\n";
    
    // 3. 生日提醒示例
    std::cout << "3. Birthday Reminder Example\n";
    std::cout << std::string(40, '-') << "\n";
    
    BirthdayReminder reminder;
    reminder.add_person("Alice", DateTime(1990, 8, 15));
    reminder.add_person("Bob", DateTime(1985, 8, 5));
    reminder.add_person("Charlie", DateTime(1992, 9, 10));
    
    reminder.check_upcoming_birthdays(now, 60);
    std::cout << "\n";
    
    // 4. 项目管理示例
    std::cout << "4. Project Tracking Example\n";
    std::cout << std::string(40, '-') << "\n";
    
    ProjectTracker project("DateTime Library Development");
    project.add_milestone("Core Implementation", now.addDays(-10));
    project.add_milestone("Unit Tests", now.addDays(-5));
    project.add_milestone("Documentation", now.addDays(5));
    project.add_milestone("Release v1.0", now.addDays(15));
    
    project.complete_milestone("Core Implementation");
    project.complete_milestone("Unit Tests");
    
    project.print_status(now);
    std::cout << "\n";
    
    // 5. 日期范围和间隔示例
    std::cout << "5. Date Range and Interval Examples\n";
    std::cout << std::string(40, '-') << "\n";
    
    // 计算年龄精确到天
    DateTime birth_date(1990, 3, 15, 14, 30, 0);
    TimeDelta age = now - birth_date;
    std::cout << "Born: " << birth_date.toString() << "\n";
    std::cout << "Current age: " << age.days() << " days (" 
              << (age.days() / 365) << " years)\n";
    
    // 下一个生日倒计时
    DateTime next_birthday = birth_date.replace(now.year());
    if (next_birthday < now) {
        next_birthday = birth_date.replace(now.year() + 1);
    }
    TimeDelta days_to_birthday = next_birthday - now;
    std::cout << "Next birthday in: " << days_to_birthday.days() << " days\n";
    
    // 工作日计算（简单版本，不考虑节假日）
    auto count_weekdays = [](DateTime start, DateTime end) -> int {
        int count = 0;
        DateTime current = start;
        while (current < end) {
            int weekday = current.weekday();
            if (weekday >= 1 && weekday <= 5) {  // Monday to Friday
                count++;
            }
            current = current.addDays(1);
        }
        return count;
    };
    
    DateTime project_start = now;
    DateTime project_end = now.addDays(30);
    int working_days = count_weekdays(project_start, project_end);
    std::cout << "Working days in next 30 days: " << working_days << "\n";
    
    std::cout << "\n=== Advanced Examples Complete ===\n";
    
    return 0;
}