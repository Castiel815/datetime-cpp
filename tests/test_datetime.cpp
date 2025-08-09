#include "datetime.h"
#include <iostream>


int main() {
    using namespace datetime;

    std::cout << "\n1. Creating DateTime objects:" << std::endl;
    DateTime now = DateTime::now();
    DateTime birthday(1990, 5, 15, 14, 30, 0);
    DateTime epoch = DateTime::fromTimestamp(0);

    std::cout << "Current time: " << now.toString() << std::endl;
    std::cout << "Birthday: " << birthday.toString() << std::endl;
    std::cout << "Unix epoch: " << epoch.toString() << std::endl;

    try
    {
        DateTime test = DateTime(2023, 2, 30);
        std::cout << test.toString() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    try
    {
        DateTime test2 = DateTime::fromString("2023-02-30");
        std::cout << test2.toString() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    DateTime end_of_month(2023, 1, 31);
    DateTime next_month = end_of_month.addMonths(1);
    std::cout << next_month.toString() << std::endl;

    return 0;
}