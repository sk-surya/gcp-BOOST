#ifndef _CPPX_DECIMAL_FROM_H
#define _CPPX_DECIMAL_FROM_H

#include <limits>       // std::numeric_limits
#include <algorithm>    // std::reverse
 
namespace cppx {
    using std::numeric_limits;
    using std::reverse;
 
    typedef numeric_limits<ulong>    uLong_info;
    int const ulong_digits   = uLong_info::digits10;
    int const ulong_bufsize  = ulong_digits + 2;

    typedef numeric_limits<long>    Long_info;
    int const long_digits   = Long_info::digits10;
    int const long_bufsize  = long_digits + 2;
 
    inline void unsigned_to_decimal( unsigned long number, char* buffer )
    {
        if( number == 0 )
        {
            *buffer++ = '0';
        }
        else
        {
            char* p_first = buffer;
            while( number != 0 )
            {
                *buffer++ = '0' + number % 10;
                number /= 10;
            }
            reverse( p_first, buffer );
        }
        *buffer = '\0';
    }
 
    inline auto decimal_from_unsigned( unsigned long number, char* buffer )
        -> char const*
    {
        unsigned_to_decimal( number, buffer );
        return buffer;
    }
 
    inline void to_decimal( long number, char* buffer )
    {
        if( number < 0 )
        {
            buffer[0] = '-';
            unsigned_to_decimal( -number, buffer + 1 );
        }
        else
        {
            unsigned_to_decimal( number, buffer );
        }
    }
 
    inline auto decimal_from( long number, char* buffer )
        -> char const*
    {
        to_decimal( number, buffer );
        return buffer;
    }
}  // namespace cppx

/* #include <iostream>
using namespace std;
 
auto main() -> int
{
    using cppx::decimal_from;
    using cppx::decimal_from_unsigned;
    using cppx::ulong_bufsize;
    using cppx::uLong_info;
    using cppx::Long_info;
    
    char spec[ulong_bufsize];
 
    cout << decimal_from( uLong_info::min(), spec ) << endl;
    cout << decimal_from( uLong_info::min() - 1, spec ) << endl;
    cout << decimal_from( 0, spec ) << endl;
    cout << decimal_from( Long_info::max(), spec ) << endl;

 
    
    cout << decimal_from_unsigned( uLong_info::max(), spec ) << endl;
} */

#endif