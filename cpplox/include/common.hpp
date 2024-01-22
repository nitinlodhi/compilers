#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <any>

using std::map;
using std::string;
using std::vector;
using std::vector;
using std::any;
using std::any_cast;

#define LOG_FUNCTION_ENTRY()                           \
    do                                                 \
    {                                                  \
        std::cout << "\n"                              \
                  << __PRETTY_FUNCTION__ << std::endl; \
    } while (0)

#define LOG_FUNCTION_EXIT()                            \
    do                                                 \
    {                                                  \
        std::cout << "\n"                              \
                  << __PRETTY_FUNCTION__ << std::endl; \
    } while (0)
