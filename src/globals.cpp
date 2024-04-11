//
// Created by tgsp on 4/9/2024.
//

# include "globals.h"

#include <iostream>
#include <map>

#include "LeftVP.h"

// #define NOPREFIX 0x03
// #define PREFIX 0x05
// #define CLEARALL 0x04
// #define LINEFEED 0x06
// #define NEWLINE 0x07
// #define DEL 0x08
// #define GROW
// float conHeightPercent = 0.3;


bool GLStreamOut::parseControlChars(char in_char) {
    //cast to hex for comparison
    // unsigned char c = in_char;
    uint_fast16_t c = xx8(in_char);
    if (c == 0xFF || control == true) {
        // std::cout << "Control char: " << c << std::endl;
        switch (c) {
            case 0x00:
                control = 0;
                std::cout << "Control off" << std::endl;
                return false;
                break;
            case 0xFF:
                control = 1;
                std::cout << "Control on" << std::endl;
                return false;break;
            case 0x03:
                pref = 0;
                std::cout << "No prefix" << std::endl;
                return false;
            break;
            case 0x04: //clear buffer and console
                std::cout << "Clear all" << std::endl;
                buffer.clear();
                glConsoleVec.clear();
                pref = true;
                glConsoleVec.push_back("$>");
                buffer.clear();

                return false;break;
            case 0x05:
                std::cout << "Prefix On" << std::endl;
                pref = 1;
                return false;break;
            case 0x06:
                glConsoleVec.push_back(buffer);
                std::cout << "Linefeed" << std::endl;
                buffer.clear();
                return false;break;
            case 0x09:
                glConsoleVec.push_back(buffer);
                buffer.clear();
                buffer += "  ";
                std::cout << "Carriage return" << std::endl;
                return false;break;
            case 0x0A:
                if(buffer.size() > 2)
                    buffer.pop_back();
                else if(glConsoleVec.size() > 1)
                    glConsoleVec.pop_back();
                std::cout << "Delete" << std::endl;
                return false;break;
            case 0x7f:
                conHeightPercent += 0.1;
                if(conHeightPercent > 0.52)
                    conHeightPercent = 0.52;
                std::cout << "Grow" << std::endl;
                return false;break;
            case 0x80:
                conHeightPercent -= 0.1;
                if(conHeightPercent < 0.0)
                    conHeightPercent = 0.0;
                std::cout << "Shrink" << std::endl;
                return false;break;
            case 0x81:
                conHeightPercent =0.3;
                std::cout << "Default" << std::endl;
                return false;break;
            case 0x82:
                conHeightPercent = 0.52;
                std::cout << "Max" << std::endl;
                return false;break;
            default:
                return true;
        }
    }else {
        return true;
    }
}

std::streambuf::int_type GLStreamOut::overflow(std::streambuf::int_type c) {
    //print c as hex:
    bool result = parseControlChars(reinterpret_cast<std::streambuf::int_type &>(c));
    if(!result) {
        std::cout << c << ": " << std::hex << c;
        std::cout << " "  << result << std::endl;
    }
    // std::streambuf::int_type c = parseControlChars(c)?c:EOF;

    // c = c = parseControlChars(c);
    if (c != EOF ) {
        // the buffer should always start with $> to look like a cli
        if (buffer.empty() && pref) {
            buffer += "$>";
        }
        // Append the character to the buffer
        buffer += c;
        if (c == '\n') {
            glConsoleVec.push_back(buffer);
            buffer.clear();
        }
    }
    return c;
}

// std::streambuf::int_type GLStreamOut::sync() {
//     if (!buffer.empty()) {
//         glConsoleVec.push_back(buffer);
//         buffer.clear();
//     }
//     return 0;
// }
std::streambuf::int_type GLStreamOut::sync() {
    std::string parsedBuffer;
    std::cout << "before parsing: " << buffer << std::endl;
    for (char c : buffer) {
        // std::streambuf::int_type parsedChar = parseControlChars(c);
        if(parseControlChars(reinterpret_cast<std::streambuf::int_type &>(c)))
            parsedBuffer += c;
        // if (parsedChar != EOF) {
        //     parsedBuffer += parsedChar;
        // }
    }//std::cout << "after parsing: " << parsedBuffer << "before: " << buffer << std::endl;
    if (!parsedBuffer.empty()) {
        //add the shell prompt to parsedBuffer
        glConsoleVec.push_back(">$" + parsedBuffer);
        parsedBuffer.clear();
    }
    buffer.clear();
    return 0;
}

const std::vector<std::string> &GLStreamOut::getConsole() const {
    return glConsoleVec;
}

void GLStreamOut::rm(int n) {
    glConsoleVec.erase(glConsoleVec.begin() + n);
}

GLStreamOut glConsole;
std::ostream glout(&glConsole);


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

const std::map<int, std::string> &GLInfo::getInfo() const {
    return glInfoMap;
}

GLInfo glStatus;
std::ostream glInfoOut(&glStatus);


//I did write these, c++ was annoying me my not letting me have the hex
#ifndef FOLDING_REGION_WHY_CPLUSPLUS_WHY
int_fast8_t xx8(char input) {
    return static_cast<int_fast8_t>(input);
}
char xd8(int_fast8_t input) {
    return static_cast<char>(input);
}
int_fast16_t xx16(int input) {
    return static_cast<int_fast16_t>(input);
}
short xd16(int_fast16_t input) {
    return static_cast<short>(input);
}
int_fast32_t xx32(long input) {
    return static_cast<int_fast32_t>(input);
}
long xd32(int_fast32_t input) {
    return static_cast<long>(input);
}
int_fast64_t xx64(long input) {
    return static_cast<int_fast64_t>(input);
}
long xd64(int_fast64_t input) {
    return static_cast<long>(input);
}

std::string xs8(int_fast8_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(value);
    return ss.str();
}
std::string xs16(int_fast16_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(value);
    return ss.str();
}
std::string xs32(int_fast32_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << static_cast<int>(value);
    return ss.str();
}
std::string xs64(int_fast64_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << static_cast<int>(value);
    return ss.str();
}
#endif
