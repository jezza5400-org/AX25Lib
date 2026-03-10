#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace direwolf_fatal
{
inline void printColoredError(const std::string& title, const std::string& details)
{
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO originalInfo{};
    const bool hasConsole = consoleHandle != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(consoleHandle, &originalInfo) != 0;

    if (hasConsole)
    {
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    }
    std::cerr << "[Direwolf Error] ";
    if (hasConsole)
    {
        SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    std::cerr << title << '\n';
    if (hasConsole)
    {
        SetConsoleTextAttribute(consoleHandle, originalInfo.wAttributes);
    }
#else
    std::cerr << "\033[1;31m[Direwolf Error]\033[0m \033[1m" << title << "\033[0m\n";
#endif

    std::cerr << details << std::endl;
}

[[noreturn]] inline void fail(const std::string& title, const std::string& details)
{
    printColoredError(title, details);
    std::exit(EXIT_FAILURE);
}

[[noreturn]] inline void fail(const std::string& title, const std::ostringstream& details)
{
    fail(title, details.str());
}
}