#ifndef __kisslog__
#define __kisslog__

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdio>

#define LOG(TYPE, ...) { std::cout << __FUNCTION__ << ":" << TYPE << ":\n "; kisslog::instance().info(__VA_ARGS__); std::cout << std::endl; }
#define LOG_INFO(...) if(kisslog::instance().logLevel >= kisslog::INFO) LOG("info", __VA_ARGS__)
#define LOG_WARN(...) if(kisslog::instance().logLevel >= kisslog::WARN) LOG("warn", __VA_ARGS__)
#define LOG_ERROR(...) if(kisslog::instance().logLevel >= kisslog::ERRORS) LOG("error", __VA_ARGS__)

class kisslog
{
   kisslog() : logLevel(INFO) {}

public:
    enum Level
    {
        ERRORS,
        WARN,
        INFO
    };
    
    Level logLevel;
    
    static kisslog& instance()
    {
        static kisslog dumbLog;
        return dumbLog;
    }
    
    void info(){};
    template <typename Head, typename ...Tail>
    void info(Head head, Tail&&... tail)
    {
        std::cout << head << ' ';
        info(std::forward<Tail>(tail)...);
    }
};

#endif

