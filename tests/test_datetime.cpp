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

    return 0;
}