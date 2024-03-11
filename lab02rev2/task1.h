#ifndef TASK1_H
#define TASK1_H


#include <iostream>
#include <iomanip>
#include <string>

class Time {
private:
    int hours;
    int minutes;
    int seconds;

public:
    // Constructor taking time in seconds
    Time(int total_seconds = 0) {
        hours = total_seconds / 3600;
        minutes = (total_seconds % 3600) / 60;
        seconds = total_seconds % 60;
    }

    // Display time in user-readable format
    void display() const {
        std::cout << std::setw(2) << std::setfill('0') << hours << "h:"
                  << std::setw(2) << std::setfill('0') << minutes << "m:"
                  << std::setw(2) << std::setfill('0') << seconds << "s";
    }

    // Overload output stream operator for displaying time
    friend std::ostream& operator<<(std::ostream& os, const Time& time) {
        time.display();
        return os;
    }

    // Overload input stream operator for reading time from user
    friend std::istream& operator>>(std::istream& input, Time& time) {
        std::string temp;
        input >> temp;

        size_t hPos = temp.find('h');
        size_t mPos = temp.find('m');
        size_t sPos = temp.find('s');

        if (hPos != std::string::npos)
            time.hours = stoi(temp.substr(0, hPos));
        if (mPos != std::string::npos)
            time.minutes = stoi(temp.substr(hPos + 2, mPos - hPos - 1));
        if (sPos != std::string::npos)
            time.seconds = stoi(temp.substr(mPos + 2, sPos - mPos - 1));
    }

    // Addition operator overloading
    Time operator+(const Time& other) const {
        int total_seconds = hours * 3600 + minutes * 60 + seconds;
        total_seconds += other.hours * 3600 + other.minutes * 60 + other.seconds;
        return Time(total_seconds);
    }

    // Subtraction operator overloading
    Time operator-(const Time& other) const {
        int total_seconds = hours * 3600 + minutes * 60 + seconds;
        total_seconds -= other.hours * 3600 + other.minutes * 60 + other.seconds;
        return Time(total_seconds);
    }

    // Scalar multiplication operator overloading
    Time operator*(int scalar) const {
        int total_seconds = (hours * 3600 + minutes * 60 + seconds) * scalar;
        return Time(total_seconds);
    }

    // Conversion operator for converting time to seconds
    operator int() const {
        return hours * 3600 + minutes * 60 + seconds;
    }
};


#endif // TASK1_H
