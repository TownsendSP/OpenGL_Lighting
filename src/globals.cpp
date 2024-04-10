//
// Created by tgsp on 4/9/2024.
//

# include "globals.h"

#include <iostream>
#include <map>

std::streambuf::int_type GLStreamOut::overflow(std::streambuf::int_type c) {
    if (c != EOF) {
        // Append the character to the buffer
        buffer += c;
        // If the character is a newline, write the buffer to the console and clear it
        if (c == '\n') {
            glConsole.push_back(buffer);
            buffer.clear();
        }
    }
    return c;
}

std::streambuf::int_type GLStreamOut::sync() {
    if (!buffer.empty()) {
        glConsole.push_back(buffer);
        buffer.clear();
    }
    return 0;
}

const std::vector<std::string> & GLStreamOut::getConsole() const {
    return glConsole;
}

GLStreamOut out;
std::ostream glOut(&out);


std::streambuf::int_type GLInfo::overflow(std::streambuf::int_type c) {
    if (c != EOF) {
        // Append the character to the buffer
        buffer += c;

        // If the character is a newline, interpret buffer [0] and [1] as a 2-digit number. store that number to int[idx]
        if (c == '\n') {
            int idx = buffer[0] - '0'; // convert char to int
            idx *= 10;
            idx += buffer[1] - '0'; // convert char to int
            glInfoMap[idx] = buffer.substr(2);
            buffer.clear();
        }
    }
    return c;
}

std::streambuf::int_type GLInfo::sync() {
    if (!buffer.empty()) {
        int idx = buffer[0] - '0'; // convert char to int
        idx *= 10;
        idx += buffer[1] - '0'; // convert char to int
        glInfoMap[idx] = buffer.substr(2);
        buffer.clear();
    }
    return 0;
}

const std::map<int, std::string> & GLInfo::getInfo() const {
    return glInfoMap;
}

GLInfo glInfo;
std::ostream glInfoOut(&glInfo);
