/*!
  Goblin Library

  @file  gob_debug.cpp
  @brief Debugging Support
  @remark Disable if NDEBUG defined.
*/
#ifndef NDEBUG
#include "gob_debug.hpp"
#include <cstdio>
#include <cctype>

namespace goblib { namespace debug {

void dump(const std::uint8_t* buf, const std::size_t sz)
{
    constexpr std::size_t perline = 16;

    if(sz == 0) { return ; }

    char strbuf[perline+1];
    auto len = sz;

    printf("ADDR:%p Length:%zu\n--------------------------------\n", buf, sz);
    do
    {
        auto s = strbuf;
        auto columns = len > perline ? perline : len;

        std::size_t i = 0;
        while(i < columns)
        {
            auto v = *buf++;
            printf("%02x ", v);
            *s++ = std::isprint(v) ? v : '.';
            ++i;
        }
        if(i < perline) { printf("%*c", (int)((perline - i)*3), ' '); }
    
        *s = '\0';
        printf("%s\n", strbuf);

        len -= columns;
    }while(len > 0);
}
//
}}
#endif
