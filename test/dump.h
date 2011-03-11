/* C++ RCE Library
 * http://code.google.com/p/cpp-rce-lib/
 * (c) 2010 Abyx. MIT License.
 */
#pragma once

#include <string>
#include <array>
#include <vector>

inline std::string dump(const unsigned char* first, const unsigned char* last)
{
    auto table = "0123456789ABCDEF";
    std::string s;
    for(auto it = first; it != last; ++it)
        s = s + table[*it >> 4] + table[*it & 0xF] + ' ';
    return s;
}

template<size_t N>
inline std::string dump(const unsigned char (&arr)[N])
{
    return dump(arr, arr + N);
}

template<size_t N>
inline std::string dump(const std::array<unsigned char, N>& arr)
{
    return dump(arr.begin(), arr.end());
}

inline std::string dump(const std::vector<unsigned char>& v)
{
    return dump(&*v.begin(), &*v.end());
}
