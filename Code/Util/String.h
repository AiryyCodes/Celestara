#pragma once

#include <sstream>
#include <string>

template <typename T>
std::string ToString(T value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << value;
    return std::move(out).str();
}
